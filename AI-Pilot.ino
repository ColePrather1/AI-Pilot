#include <SPI.h>
#include <RF24.h>

#include "include/rf_com.h"

//#include "nano_rf_test.h"


RF24 radio(8, 10); // CE pin 8, CSN pin 10 for Arduino Nano


const uint64_t pipes[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };


void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1);
  }
  
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  //radio.setChannel(76);
  radio.openReadingPipe(0, pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.openReadingPipe(2, pipes[2]);
  radio.openReadingPipe(3, pipes[3]);
  
  radio.startListening();
  
  Serial.println("NRF24L01 receiver initialized successfully");
}

void loop() {

  Packet* pkt;
  uint8_t pipe;
  int pkt_size;

  PacketType pkt_type;
  char buffer[32];

  if (radio.available(&pipe)) {
    delay(10);
    //pkt_size = radio.getDynamicPayloadSize();

    Serial.print("Pipe: "); Serial.println(pipe);

    //radio.read(buffer, pkt_size);
    radio.read(&buffer, sizeof(buffer));

    //Serial.print("Packet size: ");
    //Serial.println(static_cast<uint8_t>(buffer[2]));

    //pkt_type = reinterpret_cast<const Packet*>(buffer)->type;
    pkt_type = static_cast<PacketType>(buffer[0]);
    Serial.print("Packet type: "); Serial.println(static_cast<uint8_t>(pkt_type));

    switch (pkt_type) {
      case PacketType::BASE:
      {
        Serial.println("BASE packet received.");
        break;
      }
      case PacketType::MANUAL:
      {
        Serial.println("MANUAL :");
        const ManualPacket* mPkt = reinterpret_cast<const ManualPacket*>(buffer);

        uint8_t throttle = mPkt->throttle;
        uint8_t elevator = mPkt->elevator;
        uint8_t rudder = mPkt->rudder;
        uint8_t left_aileron = mPkt->left_aileron;
        uint8_t right_aileron = mPkt->right_aileron;
        Serial.println("\n\n\n Throttle :");
        Serial.print(throttle);
        Serial.println(" Elevator :");
        Serial.print(elevator);
        Serial.println(" Rudder :");
        Serial.print(rudder);
        Serial.println(" Left Aileron :");
        Serial.print(left_aileron);
        Serial.println(" Right Aileron :");
        Serial.print(right_aileron);
        break;
      }
      default:
        Serial.println("Error: Unknown packet type.");
        break;
    }

  }
  delay(10);
}

