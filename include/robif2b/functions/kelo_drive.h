// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_FUNCTIONS_KELO_DRIVE_H
#define ROBIF2B_FUNCTIONS_KELO_DRIVE_H

#include <robif2b/types/kelo_drive.h>


#ifdef __cplusplus
extern "C" {
#endif

void robif2b_kelo_drive_encoder_update(struct robif2b_kelo_drive_encoder *b);

void robif2b_kelo_drive_imu_update(struct robif2b_kelo_drive_imu *b);

void robif2b_kelo_drive_actuator_update(struct robif2b_kelo_drive_actuator *b);

void robif2b_kelo_drive_actuator_stop(struct robif2b_kelo_drive_actuator *b);

#ifdef __cplusplus
}
#endif

#endif
