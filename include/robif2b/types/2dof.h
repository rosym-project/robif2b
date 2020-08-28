// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_2DOF_H
#define ROBIF2B_TYPES_2DOF_H

#ifdef __cplusplus
extern "C" {
#endif

#define ROBIF2B_2DOF_NR_JOINTS 2


struct robif2b_2dof_nbx
{
    // Ports
    double *jnt_pos_msr;        // [rad] or [m]
    double *jnt_vel_msr;        // [rad] or [m]
    const double *jnt_vel_cmd;  // [rad] or [m]
};

#ifdef __cplusplus
}
#endif

#endif
