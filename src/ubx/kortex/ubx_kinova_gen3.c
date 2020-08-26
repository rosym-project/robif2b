// SPDX-License-Identifier: LGPL-3.0
#include "ubx_kinova_gen3.h"
#include <robif2b/functions/kinova_gen3.h>

enum CTRL_MODE {
    POS = 0,
    VEL,
    EFF,
    CUR,
    __CTRL_MODE_LAST__
};


struct ubx_kinova_gen3_info
{
    bool success;
    enum robif2b_ctrl_mode ctrl_mode;
    double pos_msr[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double vel_msr[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double eff_msr[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double cur_msr[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double pos_cmd[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double vel_cmd[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double eff_cmd[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double cur_cmd[ROBIF2B_KINOVA_GEN3_NR_JOINTS];
    double imu_ang_vel_msr[3];
    double imu_lin_acc_msr[3];
    struct robif2b_kinova_gen3_nbx rob;

    /* this is to have fast access to ports for reading and writing, without
     * needing a hash table lookup */
    struct ubx_kinova_gen3_port_cache ports;
};


int ubx_kinova_gen3_init(ubx_block_t *b)
{
    struct ubx_kinova_gen3_info *inf;

    /* allocate memory for the block local state */
    if ((inf = calloc(1, sizeof(struct ubx_kinova_gen3_info))) == NULL) {
        ubx_err(b, "ubx_kinova_gen3: failed to alloc memory");
        return EOUTOFMEM;
    }
    b->private_data = inf;
    update_port_cache(b, &inf->ports);


    struct robif2b_kinova_gen3_nbx *nb = &inf->rob;
    struct robif2b_kinova_gen3_config *conf = &nb->conf;
    const int *port, *port_real_time, *session_timeout, *connection_timeout;

    // Configuration
    if (cfg_getptr_char(b, "user", &conf->user) < 0) conf->user = "admin";
    if (cfg_getptr_char(b, "password", &conf->password) < 0) conf->password = "admin";
    if (cfg_getptr_char(b, "ip_address", &conf->ip_address) < 0) conf->ip_address = "192.168.1.10";
    conf->port = (cfg_getptr_int(b, "port", &port) < 0) ? 10000 : *port;
    conf->port_real_time = (cfg_getptr_int(b, "port_real_time", &port_real_time) < 0) ? 10001 : *port_real_time;
    conf->session_timeout = (cfg_getptr_int(b, "session_timeout", &session_timeout) < 0) ? 60000 : *session_timeout;
    conf->connection_timeout = (cfg_getptr_int(b, "connection_timeout", &connection_timeout) < 0) ? 2000 : *connection_timeout;

    // Connections
    nb->ctrl_mode = &inf->ctrl_mode;
    nb->jnt_pos_msr = inf->pos_msr;
    nb->jnt_vel_msr = inf->vel_msr;
    nb->jnt_trq_msr = inf->eff_msr;
    nb->act_cur_msr = inf->cur_msr;
    nb->jnt_pos_cmd = inf->pos_cmd;
    nb->jnt_vel_cmd = inf->vel_cmd;
    nb->jnt_trq_cmd = inf->eff_cmd;
    nb->act_cur_cmd = inf->cur_cmd;
    nb->imu_ang_vel_msr = inf->imu_ang_vel_msr;
    nb->imu_lin_acc_msr = inf->imu_lin_acc_msr;
    nb->success = &inf->success;


    for (int i = 0; i < ROBIF2B_KINOVA_GEN3_NR_JOINTS; i++) {
        inf->pos_cmd[i] = 0.0;
        inf->vel_cmd[i] = 0.0;
        inf->eff_cmd[i] = 0.0;
        inf->cur_cmd[i] = 0.0;
    }

    inf->success = false;
    inf->ctrl_mode = ROBIF2B_CTRL_MODE_FORCE;

    robif2b_kinova_gen3_configure(&inf->rob);

    return inf->success ? 0 : 1;
}



int ubx_kinova_gen3_start(ubx_block_t *b)
{
    struct ubx_kinova_gen3_info *inf = (struct ubx_kinova_gen3_info *)b->private_data;

    ubx_info(b, "%s", __func__);

    robif2b_kinova_gen3_recover(&inf->rob);
    robif2b_kinova_gen3_start(&inf->rob);

    return inf->success ? 0 : 1;
}



void ubx_kinova_gen3_stop(ubx_block_t *b)
{
    struct ubx_kinova_gen3_info *inf = (struct ubx_kinova_gen3_info *)b->private_data;

    robif2b_kinova_gen3_stop(&inf->rob);

    ubx_info(b, "%s", __func__);
}



void ubx_kinova_gen3_cleanup(ubx_block_t *b)
{
    struct ubx_kinova_gen3_info *inf = (struct ubx_kinova_gen3_info *)b->private_data;

    robif2b_kinova_gen3_shutdown(&inf->rob);

    ubx_info(b, "%s", __func__);
    free(b->private_data);
}


void ubx_kinova_gen3_step(ubx_block_t *b)
{
    struct ubx_kinova_gen3_info *inf = (struct ubx_kinova_gen3_info *)b->private_data;

    int ctrl_mode;
    long len;
    len = read_int(inf->ports.ctrl_mode, &ctrl_mode);

    if (len > 0) {
        switch (ctrl_mode) {
            case POS: inf->ctrl_mode = ROBIF2B_CTRL_MODE_POSITION; break;
            case VEL: inf->ctrl_mode = ROBIF2B_CTRL_MODE_VELOCITY; break;
            case EFF: inf->ctrl_mode = ROBIF2B_CTRL_MODE_FORCE; break;
            case CUR: inf->ctrl_mode = ROBIF2B_CTRL_MODE_CURRENT; break;
            default:
                ubx_err(b, "invalid ctrl_mode %i requested", ctrl_mode);
        }
    }


    read_double_array(inf->ports.pos_cmd, inf->pos_cmd, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    read_double_array(inf->ports.vel_cmd, inf->vel_cmd, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    read_double_array(inf->ports.eff_cmd, inf->eff_cmd, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    read_double_array(inf->ports.cur_cmd, inf->cur_cmd, ROBIF2B_KINOVA_GEN3_NR_JOINTS);

    robif2b_kinova_gen3_update(&inf->rob);

    write_double_array(inf->ports.pos_msr, inf->pos_msr, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    write_double_array(inf->ports.vel_msr, inf->vel_msr, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    write_double_array(inf->ports.eff_msr, inf->eff_msr, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
    write_double_array(inf->ports.cur_msr, inf->cur_msr, ROBIF2B_KINOVA_GEN3_NR_JOINTS);
}

