// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_KELO_DRIVE_API_H
#define ROBIF2B_TYPES_KELO_DRIVE_API_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define STAT1_ENABLED1          0x0001
#define STAT1_ENABLED2          0x0002
#define STAT1_ENC_1_OK          0x0004
#define STAT1_ENC_2_OK          0x0008
#define STAT1_ENC_PIVOT_OK      0x0010
#define STAT1_UNDERVOLTAGE      0x0020
#define STAT1_OVERVOLTAGE       0x0040
#define STAT1_OVERCURRENT_1     0x0080
#define STAT1_OVERCURRENT_2     0x0100
#define STAT1_OVERTEMP_1        0x0200
#define STAT1_OVERTEMP_2        0x0400
#define STAT1_ENABLED_GRIP      0x0800
#define STAT1_INPOS_GRIP        0x1000
#define STAT1_OVERLOAD_GRIP     0x2000
#define STAT1_DETECT            0x4000

#define STAT2_UNUSED            0x0000

struct robif2b_kelo_drive_api_msr_pdo {
  uint16_t  status1;            // Status bits as defined in STAT1_
  uint16_t  status2;            // Status bits as defined in STAT2_
  uint64_t  sensor_ts;          // EtherCAT timestamp (ns) on sensor acquisition
  uint64_t  setpoint_ts;        // EtherCAT timestamp (ns) of last setpoint data
  float     encoder_1;          // encoder 1 value in rad (no wrapping at 2PI)
  float     velocity_1;         // encoder 1 velocity in rad/s
  float     current_1_d;        // motor 1 current direct in amp
  float     current_1_q;        // motor 1 current quadrature in amp
  float     current_1_u;        // motor 1 current phase U in amp
  float     current_1_v;        // motor 1 current phase V in amp
  float     current_1_w;        // motor 1 current phase W in amp
  float     voltage_1;          // motor 1 voltage from pwm in volts
  float     voltage_1_u;        // motor 1 voltage from phase U in volts
  float     voltage_1_v;        // motor 1 voltage from phase V in volts
  float     voltage_1_w;        // motor 1 voltage from phase W in volts
  float     temperature_1;      // motor 1 estimated temperature in K
  float     encoder_2;          // encoder 2 value in rad (no wrapping at 2PI)
  float     velocity_2;         // encoder 2 velocity in rad/s
  float     current_2_d;        // motor 2 current direct in amp
  float     current_2_q;        // motor 2 current quadrature in amp
  float     current_2_u;        // motor 2 current phase U in amp
  float     current_2_v;        // motor 2 current phase V in amp
  float     current_2_w;        // motor 2 current phase W in amp
  float     voltage_2;          // motor 2 voltage from pwm in volts
  float     voltage_2_u;        // motor 2 voltage from phase U in volts
  float     voltage_2_v;        // motor 2 voltage from phase V in volts
  float     voltage_2_w;        // motor 2 voltage from phase W in volts
  float     temperature_2;      // motor 2 estimated temperature in K
  float     encoder_pivot;      // encoder pivot value in rad (wrapping at -PI and +PI)
  float     velocity_pivot;     // encoder pivot velocity in rad/s
  float     voltage_bus;        // bus voltage in volts
  uint64_t  imu_ts;             // EtherCAT timestamp (ns) of IMU sensor acquisition
  float     accel_x;            // IMU accelerometer X-axis in m/s2
  float     accel_y;            // IMU accelerometer Y-axis in m/s2
  float     accel_z;            // IMU accelerometer Z-axis in m/s2
  float     gyro_x;             // IMU gyro X-axis in rad/s
  float     gyro_y;             // IMU gyro Y-axis in rad/s
  float     gyro_z;             // IMU gyro Z-axis in rad/s
  float     temperature_imu;    // IMU temperature in K
  float     pressure;           // barometric pressure in Pa absolute
  float     current_in;         // current input
} __attribute__ ((__packed__));


/* SMARTWHEEL SETPOINT MODES
 * 
 * Mode TORQUE
 *   Setpoint 1     = Current in Amp for motor 1
 *   Setpoint 2     = Current in Amp for motor 2
 *   Upper limit 1  = Most positive velocity (rad/s) allowed for motor 1
 *   Lower limit 1  = Most negative velocity (rad/s) allowed for motor 1
 *   Upper limit 2  = Most positive velocity (rad/s) allowed for motor 2
 *   Lower limit 2  = Most negative velocity (rad/s) allowed for motor 2
 *
 * Mode DTORQUE
 *   Setpoint 1     = Common current in Amp
 *   Setpoint 2     = Differential current in Amp
 *   Upper limit 1  = Most positive velocity (rad/s) allowed for linear motion
 *   Lower limit 1  = Most negative velocity (rad/s) allowed for linear motion
 *   Upper limit 2  = Most positive velocity (rad/s) allowed for pivot motion
 *   Lower limit 2  = Most negative velocity (rad/s) allowed for pivot motion
 *
 * Mode VELOCITY
 *   Setpoint 1     = Velocity in rad/s for motor 1
 *   Setpoint 2     = Velocity in rad/s for motor 2
 *   Upper limit 1  = Most positive current (amp) allowed for motor 1
 *   Lower limit 1  = Most negative current (amp) allowed for motor 1
 *   Upper limit 2  = Most positive current (amp) allowed for motor 2
 *   Lower limit 2  = Most negative current (amp) allowed for motor 2
 *
 * Mode DVELOCITY
 *   Setpoint 1     = Common velocity in rad/s
 *   Setpoint 2     = Differential velocity in rad/s
 *   Upper limit 1  = Most positive current (amp) allowed for linear motion
 *   Lower limit 1  = Most negative current (amp) allowed for linear motion
 *   Upper limit 2  = Most positive current (amp) allowed for pivot motion
 *   Lower limit 2  = Most negative current (amp) allowed for pivot motion
 *
 */

#define COM1_ENABLE1        0x0001
#define COM1_ENABLE2        0x0002
#define COM1_MODE_TORQUE    (0x0 << 2)
#define COM1_MODE_DTORQUE   (0x1 << 2)
#define COM1_MODE_VELOCITY  (0x2 << 2)
#define COM1_MODE_DVELOCITY (0x3 << 2)
#define COM1_EMERGENCY1     0x0010
#define COM1_EMERGENCY2     0x0020
#define COM1_ENABLESERVO    0x0400
#define COM1_SERVOCLOSE     0x0800
#define COM1_USE_TS         0x8000

#define COM2_UNUSED         0x0000

struct robif2b_kelo_drive_api_cmd_pdo {
  uint16_t  command1;   // Command bits as defined in COM1_
  uint16_t  command2;   // Command bits as defined in COM2_
  float     setpoint1;  // Setpoint 1
  float     setpoint2;  // Setpoint 2
  float     limit1_p;   // Upper limit 1
  float     limit1_n;   // Lower limit 1
  float     limit2_p;   // Upper limit 2
  float     limit2_n;   // Lower limit 2
  uint64_t  timestamp;  // EtherCAT timestamp (ns) setpoint execution
} __attribute__ ((__packed__));

#ifdef __cplusplus
}
#endif

#endif
