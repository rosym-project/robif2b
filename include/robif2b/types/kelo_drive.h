// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_KELO_DRIVE_H
#define ROBIF2B_TYPES_KELO_DRIVE_H

#include <stdlib.h>

#include <robif2b/types/control_mode.h>
#include <robif2b/types/kelo_drive_api.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Note that the two wheels on a drive are mounted in "opposite" directions.
 * Assuming the right-hand rule for curve orientation, the positive direction of
 * rotation for each wheel is pointing away from the central point between the
 * two wheels.
 *
 * The following diagram shows a top-down view of a drive and the direction of
 * rotation for each wheel.
 *
 * <pre>
 *             front
 *               o  (pivot joint)
 *               |
 *         l  _  |  _  r
 * +z      e | | | | | i      +z
 * <----|  f | |-+-| | g  |---->
 *         t |_|   |_| h
 *                     t
 * </pre>
 */
struct robif2b_kelo_drive_encoder
{
    /**
     * The number of drives to be processed.
     */
    const int *num_drives;

    /**
     * Process data objects that represent the measured robot state.
     * Pointer to an array of size @ref num_drives.
     */
    const struct robif2b_kelo_drive_api_msr_pdo *msr_pdo;

    /**
     * Wheel position measurements in [rad].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    double *wheel_pos_msr;

    /**
     * Wheel velocity measurements in [rad/s].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    double *wheel_vel_msr;

    /**
     * Calibration offset of the pivot joint in [rad]. This calibration offset
     * will be subtracted from the sensor measurements (i.e. sens - off).
     * Pointer to an array of size @ref num_drives.
     */
    const double *pivot_pos_off;

    /**
     * Pivot position measurements in [rad].
     * Pointer to an array of size @ref num_drives.
     */
    double *pivot_pos_msr;

    /**
     * Pivot velocity measurements in [rad/s].
     * Pointer to an array of size @ref num_drives.
     */
    double *pivot_vel_msr;
};


/**
 * The IMU's coordinate system is aligned with the drive so that the (i) the
 * positive x-axis points to the left; (ii) the positive y-axis points to the
 * rear; and (iii) the positive z-axis points upwards.
 *
 * The following diagram shows a top-down view of a drive and the IMU's
 * coordinate system.
 *
 * <pre>
 *      front
 *        o  (pivot joint)
 *        |          +x
 *  l  _  |  _  r     <----o [imu]
 *  e | | | | | i          |
 *  f | |-+-| | g          |
 *  t |_|   |_| h          v +y
 *              t
 * </pre>
 */
struct robif2b_kelo_drive_imu
{
    /**
     * The number of drives to be processed.
     */
    const int *num_drives;

    /**
     * Process data objects that represent the measured robot state.
     * Pointer to an array of size @ref num_drives.
     */
    const struct robif2b_kelo_drive_api_msr_pdo *msr_pdo;

    /**
     * Angular velocity as measured by the IMU in [rad/s].
     * Pointer to an array of size 3 * @ref num_drives. The entries per drive
     * are order as measurements about the X, Y and Z axis, respectively.
     */
    double *imu_ang_vel_msr;

    /**
     * Linear acceleration as measured by the IMU in [m/s^s].
     * Pointer to an array of size 3 * @ref num_drives. The entries per drive
     * are order as measurements along the X, Y and Z axis, respectively.
     */
    double *imu_lin_acc_msr;
};


/**
 * Note that the two wheels on a drive are mounted in "opposite" directions.
 * Assuming the right-hand rule for curve orientation, the positive direction of
 * rotation for each wheel is pointing away from the central point between the
 * two wheels.
 *
 * The following diagram shows a top-down view of a drive and the direction of
 * rotation for each wheel.
 *
 * <pre>
 *             front
 *               o  (pivot joint)
 *               |
 *         l  _  |  _  r
 * +z      e | | | | | i      +z
 * <----|  f | |-+-| | g  |---->
 *         t |_|   |_| h
 *                     t
 * </pre>
 */
struct robif2b_kelo_drive_actuator
{
    /**
     * The number of drives to be commanded.
     */
    const int *num_drives;

    /**
     * Process data objects that represent the drive commands.
     * Pointer to an array of size @ref num_drives.
     */
    struct robif2b_kelo_drive_api_cmd_pdo *cmd_pdo;

    /**
     * The desired control mode for each drive.
     * Pointer to an array of size @ref num_drives.
     */
    const enum robif2b_ctrl_mode *ctrl_mode;

    /**
     * Actuator velocity commands represented in [rad/s].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    const double *act_vel_cmd;

    /**
     * Actuator torque commands represented in [Nm].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    const double *act_trq_cmd;

    /**
     * Actuator current commands represented in [A].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    const double *act_cur_cmd;

    /**
     * The current limit for each actuator represented in [A].
     * Pointer to an array of size 2 * @ref num_drives.
     */
    const double *max_current;

    /**
     * The motor torque constant of each actuator represented in [Nm/A].
     * Pointer to an array of size 2 * @ref num_drives.
     *
     * The Kelo drives expose
     * (i)  the Flux linkage (\Phi, measured in [Wb]); and
     * (ii) number of pole pairs (N/2; half because of the _pairs_)
     * via the motor parameters PDO. Given those quantities, the motor torque
     * constant (K_T) follows as K_T = N \Phi.
     */
    const double *trq_const;
};

#ifdef __cplusplus
}
#endif

#endif
