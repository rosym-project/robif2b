// SPDX-License-Identifier: LGPL-3.0
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <robif2b/functions/kinova_gen3.h>


int main(int argc, char **argv)
{
    bool success = false;
    double cycle_time = 0.001;
    enum robif2b_ctrl_mode ctrl_mode = ROBIF2B_CTRL_MODE_FORCE;
    double pos_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double vel_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double eff_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cur_msr[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double pos_cmd[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double vel_cmd[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double eff_cmd[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cur_cmd[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double imu_ang_vel_msr[] = { 0.0, 0.0, 0.0 };
    double imu_lin_acc_msr[] = { 0.0, 0.0, 0.0 };

    struct robif2b_kinova_gen3_nbx rob = {
        // Configuration
        .conf.ip_address         = "192.168.1.10",
        .conf.port               = 10000,
        .conf.port_real_time     = 10001,
        .conf.user               = "admin",
        .conf.password           = "admin",
        .conf.session_timeout    = 60000,
        .conf.connection_timeout = 2000,

        // Connections
        .cycle_time = &cycle_time,
        .ctrl_mode = &ctrl_mode,
        .jnt_pos_msr = pos_msr,
        .jnt_vel_msr = vel_msr,
        .jnt_trq_msr = eff_msr,
        .act_cur_msr = cur_msr,
        .jnt_pos_cmd = pos_cmd,
        .jnt_vel_cmd = vel_cmd,
        .jnt_trq_cmd = eff_cmd,
        .act_cur_cmd = cur_cmd,
        .imu_ang_vel_msr = imu_ang_vel_msr,
        .imu_lin_acc_msr = imu_lin_acc_msr,
        .success = &success
    };


    robif2b_kinova_gen3_configure(&rob);
    if (!success) {
        printf("Error during gen3_configure\n");
        goto shutdown;
    }

    robif2b_kinova_gen3_recover(&rob);
    if (!success) {
        printf("Error during gen3_recover\n");
        goto shutdown;
    }

    printf("Starting\n");
    robif2b_kinova_gen3_start(&rob);
    if (!success) {
        printf("Error during gen3_start\n");
        goto stop;
    }


    for (int i = 0; i < 3000; i++) {
        robif2b_kinova_gen3_update(&rob);
        if (!success) {
            printf("Error during gen3_update\n");
            goto stop;
        }

        usleep(1000);
    }

stop:
    robif2b_kinova_gen3_stop(&rob);
    printf("Stopped\n");

shutdown:
    robif2b_kinova_gen3_shutdown(&rob);
    if (!success) {
        printf("Error during gen3_shutdown\n");
        return 1;
    }

    return 0;
}
