#include <mbed.h>
#include <rtos.h>
#include <Arduino.h>

#include <Arduino_LSM9DS1.h>
#include <Arduino_BMI270_BMM150.h>
#include <ArduinoBLE.h>

#include <chrono>
#include <thread>



struct SensorData {
  float accel[3];
  float gyro[3];
  float mag[3];
} sensorData;

struct FlightData {
  float roll;
  float pitch;
  float yaw;
} flightData;


rtos::Mutex sensorMutex;
rtos::Mutex flightDataMutex;

// REFERENECES
//mbed::CircularBuffer<uint8_t, 10> input_buffer;
//mbed::Timer timer;
//timer.start();
//rtos::Mutex mutex;
//rtos::MemoryPool<uint8_t, 10> pool;
//rtos::Queue<uint8_t, 10> queue;
//rtos::EventFlags event_flags;




rtos::Thread sensorThread;
rtos::Thread fusionThread;
rtos::Thread flightControlThread;
rtos::Thread radioThread;
rtos::Thread telemetryThread;

//mbed::BlockDevice* bd = mbed::BlockDevice::get_default_instance();
//rtos::Waiter waiter(rtos::ThisThread::get_id());



// Thread functions
void sensorReadingTask() {
  while (true) {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
      sensorMutex.lock();
      IMU.readAcceleration(sensorData.accel[0], sensorData.accel[1], sensorData.accel[2]);
      IMU.readGyroscope(sensorData.gyro[0], sensorData.gyro[1], sensorData.gyro[2]);
      IMU.readMagneticField(sensorData.mag[0], sensorData.mag[1], sensorData.mag[2]);
      sensorMutex.unlock();
    }
    rtos::ThisThread::sleep_for(10);
  }
}

void sensorFusionTask() {
  while (true) {
    rtos::ThisThread::sleep_for(20);
  }
}

void flightControlTask() {
  while (true) {
    rtos::ThisThread::sleep_for(20);
  }
}

void radioCommTask() {
  while (true) {
    rtos::ThisThread::sleep_for(20);
  }
}

void telemetryTask() {
  while (true) {
    rtos::ThisThread::sleep_for(20);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Start threads
  sensorThread.start(sensorReadingTask);
  fusionThread.start(sensorFusionTask);
  flightControlThread.start(flightControlTask);
  radioThread.start(radioCommTask);
  telemetryThread.start(telemetryTask);
}

void loop() {
  delay(1000);
}


