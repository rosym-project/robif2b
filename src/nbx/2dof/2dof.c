// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/2dof.h>

#include <assert.h>


void robif2b_2dof_update(struct robif2b_2dof_nbx *b)
{
    assert(b);
    assert(b->jnt_pos_msr);
    assert(b->jnt_vel_msr);
    assert(b->jnt_vel_cmd);

    for (int i = 0; i < ROBIF2B_2DOF_NR_JOINTS; i++) {
        b->jnt_pos_msr[i] += b->jnt_vel_cmd[i];
        b->jnt_vel_msr[i]  = b->jnt_vel_cmd[i];
    }
}
