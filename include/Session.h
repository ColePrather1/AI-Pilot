#ifndef SESSION_H
#define SESSION_H
#pragma once

#include <mbed.h>
#include <array>
#include <cstdint>
#include "GPS_Coordinate.h"


enum class TypeLevel {};
enum class RollLevel { };
enum class FlightPhase { };
enum class ControlMode { PAIRING };

const int NUM_FEATS = 6; // Adjust as needed

namespace Session {
    
volatile bool quit_flag = false;

volatile bool sql_finalized = false, sql_closed = false;

// General Parameters

// Service Active
volatile bool atc_active = false;
volatile bool ctlr_active = false;
volatile bool logger_active = false;
volatile bool rf_rx_active = false;
volatile bool rf_tx_active = false;
volatile bool process_active = false;



// Locations
GPS_Coordinate atc_gps, plane_gps;

volatile uint16_t atc_gps_alt;
volatile double atc_gps_lat, atc_gps_lon;
double calc_atc_gps_lat, calc_atc_gps_lon;
volatile uint16_t plane_gps_alt;
volatile double plane_gps_lat, plane_gps_lon;

// Plane Parameters
volatile uint8_t rudder_val, aileron_left_val, aileron_right_val, elevator_val, throttle_val, throttle_lock_val;
std::array<uint8_t, NUM_FEATS> servo_defaults;
std::array<volatile uint8_t*, NUM_FEATS> feat_vals;

// Airplane Status
volatile TypeLevel plane_altitude_level, plane_pitch_level, plane_air_speed_level;
volatile RollLevel plane_roll_level;
volatile FlightPhase airplane_active_flight_phase;
volatile ControlMode control_mode = ControlMode::PAIRING;
volatile bool throttle_lock = false,
              within_range = false,
              approaching_boundary = false,
              returning_home = false,
              isWaypointSet = false,
              isEnrouteToWaypoint = false,
              isCircleWaypoint = false,
              imu_active = false,
              imu_fail = false,
              isFlying = false,
              airplane_gps_active = false,
              airplane_gps_fail = false,
              atc_gps_active = false,
              atc_gps_fail = false,
              engine_active = false,
              engine_fail = false,
              engine_stall = false,
              isMotorSpinning = false;

volatile uint16_t airplane_current_gps_heading = 0,
                  airplane_current_gps_speed = 0;

// Flight Parameters
volatile uint32_t flight_time_start = 0, flight_time_end = 0;
volatile uint16_t fixed_plane_heading = 0, fixed_plane_speed = 0, fixed_plane_altitude = 0;
volatile uint32_t waypoint_lat = 0, waypoint_lon = 0, waypoint_alt = 0;

volatile uint16_t atc_pilot_distance_meters = 0;
volatile uint32_t atc_pilot_distance_meters_squared = 0;

// User HUD Parameters
volatile bool paired = false, ctlr_paired = false, plane_connected = false;

// Radio Parameters
volatile int rf_tx_active_pipe = 5;

// Functions
void initialize();


inline bool getQuitFlag() {
    return core_util_atomic_load_bool(&quit_flag);
}

inline void setQuitFlag(bool value) {
    core_util_atomic_store_bool(&quit_flag, value);
}

inline uint16_t getAirplaneCurrentGPSHeading() {
    return core_util_atomic_load_u16(&airplane_current_gps_heading);
}

inline void setAirplaneCurrentGPSHeading(uint16_t value) {
    core_util_atomic_store_u16(&airplane_current_gps_heading, value);
}

} // namespace Session



#endif