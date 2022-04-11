// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/kelo_drive.h>

#include <string.h>
#include <assert.h>

#include <robif2b/types/control_mode.h>


void robif2b_kelo_drive_encoder_update(struct robif2b_kelo_drive_encoder *b)
{
    assert(b);
    assert(b->msr_pdo);
    assert(b->num_drives);

    for (int i = 0; i < *b->num_drives; i++) {
        if (b->wheel_pos_msr) {
            b->wheel_pos_msr[i * 2 + 0] = b->msr_pdo[i].encoder_1;
            b->wheel_pos_msr[i * 2 + 1] = b->msr_pdo[i].encoder_2;
        }
        if (b->wheel_vel_msr) {
            b->wheel_vel_msr[i * 2 + 0] = b->msr_pdo[i].velocity_1;
            b->wheel_vel_msr[i * 2 + 1] = b->msr_pdo[i].velocity_2;
        }
        if (b->pivot_pos_msr) {
            b->pivot_pos_msr[i] = b->msr_pdo[i].encoder_pivot;
            if (b->pivot_pos_off) b->pivot_pos_msr[i] -= b->pivot_pos_off[i];
        }
        if (b->pivot_vel_msr) {
            b->pivot_vel_msr[i] = b->msr_pdo[i].velocity_pivot;
        }
    }
}


void robif2b_kelo_drive_imu_update(struct robif2b_kelo_drive_imu *b)
{
    assert(b);
    assert(b->msr_pdo);
    assert(b->num_drives);

    for (int i = 0; i < *b->num_drives; i++) {
        if (b->imu_ang_vel_msr) {
            b->imu_ang_vel_msr[i * 3 + 0] = b->msr_pdo[i].gyro_x;
            b->imu_ang_vel_msr[i * 3 + 1] = b->msr_pdo[i].gyro_y;
            b->imu_ang_vel_msr[i * 3 + 2] = b->msr_pdo[i].gyro_z;
        }
        if (b->imu_lin_acc_msr) {
            b->imu_lin_acc_msr[i * 3 + 0] = b->msr_pdo[i].accel_x;
            b->imu_lin_acc_msr[i * 3 + 1] = b->msr_pdo[i].accel_y;
            b->imu_lin_acc_msr[i * 3 + 2] = b->msr_pdo[i].accel_z;
        }
    }
}


void robif2b_kelo_drive_actuator_update(struct robif2b_kelo_drive_actuator *b)
{
    assert(b);
    assert(b->cmd_pdo);
    assert(b->ctrl_mode);
    assert(b->num_drives);

    for (int i = 0; i < *b->num_drives; i++) {
        switch (b->ctrl_mode[i]) {
            case ROBIF2B_CTRL_MODE_VELOCITY:
                assert(b->act_vel_cmd);
                b->cmd_pdo[i].command1 = COM1_ENABLE1 | COM1_ENABLE2
                                       | COM1_MODE_VELOCITY;

                b->cmd_pdo[i].setpoint1 =  b->act_vel_cmd[i * 2 + 0];
                b->cmd_pdo[i].setpoint2 =  b->act_vel_cmd[i * 2 + 1];
                b->cmd_pdo[i].limit1_p  =  b->act_cur_cmd[i * 2 + 0];  // TODO: Compute based on commanded power limit?
                b->cmd_pdo[i].limit1_n  = -b->act_cur_cmd[i * 2 + 0];
                b->cmd_pdo[i].limit2_p  =  b->act_cur_cmd[i * 2 + 1];
                b->cmd_pdo[i].limit2_n  = -b->act_cur_cmd[i * 2 + 1];
            break;

            case ROBIF2B_CTRL_MODE_FORCE:
                assert(b->act_trq_cmd);
                assert(b->trq_const);

                // Note: the mode is called torque mode, but it is actually
                //       current control!
                b->cmd_pdo[i].command1 = COM1_ENABLE1 | COM1_ENABLE2
                                       | COM1_MODE_TORQUE;

                b->cmd_pdo[i].setpoint1 = b->act_trq_cmd[i * 2 + 0]
                                          / b->trq_const[i * 2 + 0];
                b->cmd_pdo[i].setpoint2 = b->act_trq_cmd[i * 2 + 1]
                                          / b->trq_const[i * 2 + 1];
                b->cmd_pdo[i].limit1_p  = 0.0; // Controller does not seem to
                b->cmd_pdo[i].limit1_n  = 0.0; // use those limits in torque
                b->cmd_pdo[i].limit2_p  = 0.0; // control mode
                b->cmd_pdo[i].limit2_n  = 0.0;
            break;

            case ROBIF2B_CTRL_MODE_CURRENT:
                assert(b->act_cur_cmd);

                // Note: the mode is called torque mode, but it is actually
                //       current control!
                b->cmd_pdo->command1 = COM1_ENABLE1 | COM1_ENABLE2
                                     | COM1_MODE_TORQUE;

                b->cmd_pdo[i].setpoint1 =  b->act_cur_cmd[i * 2 + 0];
                b->cmd_pdo[i].setpoint2 =  b->act_cur_cmd[i * 2 + 1];
                b->cmd_pdo[i].limit1_p  =  0.0; // Controller does not seem to
                b->cmd_pdo[i].limit1_n  =  0.0; // use those limits in torque
                b->cmd_pdo[i].limit2_p  =  0.0; // control mode
                b->cmd_pdo[i].limit2_n  =  0.0;
            break;

            default:
                robif2b_kelo_drive_actuator_stop(b);
                return;
            break;
        }
    }
}


void robif2b_kelo_drive_actuator_stop(struct robif2b_kelo_drive_actuator *b)
{
    assert(b);
    assert(b->cmd_pdo);
    assert(b->max_current);
    assert(b->num_drives);

    for (int i = 0; i < *b->num_drives; i++) {
        b->cmd_pdo[i].command1 = COM1_ENABLE1 | COM1_ENABLE2
                               | COM1_MODE_VELOCITY;

        b->cmd_pdo[i].setpoint1 =  0.0;
        b->cmd_pdo[i].setpoint2 =  0.0;
        b->cmd_pdo[i].limit1_p  =  b->max_current[i * 2 + 0];
        b->cmd_pdo[i].limit1_n  = -b->max_current[i * 2 + 0];
        b->cmd_pdo[i].limit2_p  =  b->max_current[i * 2 + 1];
        b->cmd_pdo[i].limit2_n  = -b->max_current[i * 2 + 1];
    }
}
