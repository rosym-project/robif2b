// SPDX-License-Identifier: LGPL-3.0
#include "ubx_2dof.h"
#include <robif2b/functions/2dof.h>

struct ubx_2dof_info
{
    double pos_msr[ROBIF2B_2DOF_NR_JOINTS];
    double vel_msr[ROBIF2B_2DOF_NR_JOINTS];
    double vel_cmd[ROBIF2B_2DOF_NR_JOINTS];
    struct robif2b_2dof_nbx rob;

    /* this is to have fast access to ports for reading and writing, without
     * needing a hash table lookup */
    struct ubx_2dof_port_cache ports;
};


int ubx_2dof_init(ubx_block_t *b)
{
    int ret = -1;
    struct ubx_2dof_info *inf;

    /* allocate memory for the block local state */
    if ((inf = calloc(1, sizeof(struct ubx_2dof_info)))==NULL) {
        ubx_err(b, "ubx_2dof: failed to alloc memory");
        ret=EOUTOFMEM;
        goto out;
    }
    b->private_data=inf;
    update_port_cache(b, &inf->ports);

    struct robif2b_2dof_nbx *nb = &inf->rob;

    // Connections
    nb->jnt_pos_msr = inf->pos_msr;
    nb->jnt_vel_msr = inf->vel_msr;
    nb->jnt_vel_cmd = inf->vel_cmd;


    ret = 0;
out:
    return ret;
}


void ubx_2dof_cleanup(ubx_block_t *b)
{
    ubx_info(b, "%s", __func__);

    free(b->private_data);
}


void ubx_2dof_step(ubx_block_t *b)
{
    struct ubx_2dof_info *inf = (struct ubx_2dof_info*) b->private_data;

    ubx_info(b, "%s", __func__);


    read_double_array(inf->ports.vel_cmd, inf->vel_cmd, ROBIF2B_2DOF_NR_JOINTS);

    robif2b_2dof_update(&inf->rob);

    write_double_array(inf->ports.pos_msr, inf->pos_msr, ROBIF2B_2DOF_NR_JOINTS);
    write_double_array(inf->ports.vel_msr, inf->vel_msr, ROBIF2B_2DOF_NR_JOINTS);
}

