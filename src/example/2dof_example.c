// SPDX-License-Identifier: LGPL-3.0
#include <stdio.h>

#include <robif2b/functions/2dof.h>


int main(int argc, char **argv)
{
    double pos_msr[ROBIF2B_2DOF_NR_JOINTS] = { 0.0, 0.0 };
    double vel_msr[ROBIF2B_2DOF_NR_JOINTS] = { 0.0, 0.0 };
    double vel_cmd[ROBIF2B_2DOF_NR_JOINTS] = { 0.1, 0.2 };

    struct robif2b_2dof_nbx rob = {
        // Connections
        .jnt_pos_msr = pos_msr,
        .jnt_vel_msr = vel_msr,
        .jnt_vel_cmd = vel_cmd,
    };

    for (int i = 0; i < 10; i++) {
        robif2b_2dof_update(&rob);

        printf("%f - %f\n", pos_msr[0], pos_msr[1]);
    }

    return 0;
}
