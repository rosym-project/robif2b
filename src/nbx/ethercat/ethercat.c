// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/ethercat.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

// SOEM
#include <soem/ethercattype.h>
#include <soem/nicdrv.h>
#include <soem/ethercatbase.h>
#include <soem/ethercatmain.h>
#include <soem/ethercatconfig.h>
#include <soem/ethercatcoe.h>
#include <soem/ethercatdc.h>
#include <soem/ethercatprint.h>


static const int GROUP = 0;

struct robif2b_ethercat_comm
{
    ec_slavet      slave[EC_MAXSLAVE];
    ec_groupt      group[EC_MAXGROUP];
    uint8          esibuf[EC_MAXEEPBUF];
    uint32         esimap[EC_MAXEEPBITMAP];
    ec_eringt      elist;
    ec_idxstackT   idxstack;
    ec_SMcommtypet sm_commtype;
    ec_PDOassignt  pdo_assign;
    ec_PDOdesct    pdo_desc;
    ec_eepromSMt   eep_sm;
    ec_eepromFMMUt eep_fmmu;
    boolean        error;
    int64          dc_time;
    ecx_portt      port;
    ecx_contextt   context;

    uint8          io_map[4096];
};


static void print_slaves_not_in_state(struct robif2b_ethercat *e, ec_state state)
{
    assert(e);
    assert(e->comm);
    assert(e->num_initial_slaves);

    struct robif2b_ethercat_comm *soem = e->comm;

    // Query/refresh state of all slaves
    // (ecx_statecheck only refreshes one slave's state)
    ecx_readstate(&soem->context);

    // Find and print slaves that are not in the requested state
    for (int i = 1; i <= *e->num_initial_slaves; i++) {
        if (soem->slave[i].state != state) {
            printf("Slave %i State=%i StatusCode=%i : %s\n",
                    i, soem->slave[i].state,
                    soem->slave[i].ALstatuscode,
                    ec_ALstatuscode2string(soem->slave[i].ALstatuscode));
        }
    }
}


void robif2b_ethercat_configure(struct robif2b_ethercat *e)
{
    assert(e);
    assert(e->error_code);
    assert(e->ethernet_if);
    assert(e->num_exposed_slaves);
    assert(e->slave_idx);
    assert(e->name);
    assert(e->input_size);
    assert(e->output_size);
    assert(e->num_initial_slaves);
    assert(e->num_current_slaves);
    assert(e->input);
    assert(e->output);

    struct robif2b_ethercat_comm *soem = calloc(1, sizeof(*soem));
    if (!soem) {
        fprintf(stderr, "Failed to allocate memory for SOEM state\n");
        *e->error_code = -1;
        goto out;
    }
    e->comm = soem;

    soem->error = false;

    soem->context.port       = &soem->port;
    soem->context.slavelist  = &soem->slave[0];
    soem->context.slavecount = e->num_current_slaves;
    soem->context.maxslave   = EC_MAXSLAVE;
    soem->context.grouplist  = &soem->group[0];
    soem->context.maxgroup   = EC_MAXGROUP;
    soem->context.esibuf     = &soem->esibuf[0];
    soem->context.esimap     = &soem->esimap[0];
    soem->context.esislave   = 0;
    soem->context.elist      = &soem->elist;
    soem->context.idxstack   = &soem->idxstack;
    soem->context.ecaterror  = &soem->error;
    soem->context.DCtime     = &soem->dc_time;
    soem->context.SMcommtype = &soem->sm_commtype;
    soem->context.PDOassign  = &soem->pdo_assign;
    soem->context.PDOdesc    = &soem->pdo_desc;
    soem->context.eepSM      = &soem->eep_sm;
    soem->context.eepFMMU    = &soem->eep_fmmu;

    // initially all slaves are disconnected
    for (int i = 0; i < *e->num_exposed_slaves; i++) {
        if (e->is_connected) e->is_connected[i] = false;
    }

    // open socket
    if (ecx_init(&soem->context, e->ethernet_if) <= 0) {
        fprintf(stderr, "Failed to initialize EtherCAT device\n");
        *e->error_code = -2;
        goto out_err_free;
    }

    // ecx_config_init transitions to PRE_OP state (if not configured otherwise)
    *e->num_initial_slaves = ecx_config_init(&soem->context, true);
    if (*e->num_initial_slaves <= 0) {
        fprintf(stderr, "Failed to discover or configure slaves\n");
        *e->error_code = -3;
        goto out_err_init;
    }

    if (*e->num_initial_slaves < *e->num_exposed_slaves) {
        fprintf(stderr, "Found less slaves than expected\n");
        *e->error_code = -4;
        goto out_err_init;
    }

    // ecx_config_map_group transitions to SAFE_OP state (if not configured
    // otherwise)
    size_t usedmem = ecx_config_map_group(&soem->context, soem->io_map, GROUP);
    if (usedmem > sizeof(soem->io_map)) {
        fprintf(stderr, "I/O map is too small\n");
        *e->error_code = -5;
        goto out_err_init;
    }

    for (int i = 0; i < *e->num_exposed_slaves; i++) {
        int idx = e->slave_idx[i];

        if (idx > *e->num_initial_slaves) {  // slaves start index 1
            fprintf(stderr, "Found slave with an invalid index\n");
            *e->error_code = -6;
            goto out_err_init;
        }

        if (strcmp(soem->slave[idx].name, e->name[i]) != 0) {
            fprintf(stderr, "Found slave with incorrect name\n");
            *e->error_code = -7;
            goto out_err_init;
        }

        if (soem->slave[idx].eep_id != e->product_code[i]) {
            fprintf(stderr, "Found slave with incorrect EEPROM ID\n");
            *e->error_code = -8;
            goto out_err_init;
        }

        if (soem->slave[idx].Ibytes != e->input_size[i]) {
            fprintf(stderr, "Found slave with incorrect input size\n");
            *e->error_code = -9;
            goto out_err_init;
        }

        if (soem->slave[idx].Obytes != e->output_size[i]) {
            fprintf(stderr, "Found slave with incorrect output size\n");
            *e->error_code = -10;
            goto out_err_init;
        }

        // only byte-aligned data is supported for now
        assert(soem->slave[idx].Istartbit % 8 == 0);
        assert(soem->slave[idx].Ostartbit % 8 == 0);
        assert(soem->slave[idx].Ibits % 8 == 0);
        assert(soem->slave[idx].Obits % 8 == 0);
    }

    // wait for all slaves to reach "safe operational" state
    ecx_statecheck(&soem->context, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
    if (soem->slave[0].state != EC_STATE_SAFE_OP) {
        print_slaves_not_in_state(e, EC_STATE_SAFE_OP);

        fprintf(stderr, "Not all EtherCAT slaves reached Safe-Operational "
                        "state\n");
        *e->error_code = -11;
        goto out_err_init;
    }

    // send one valid process data to make outputs in slaves happy
    ecx_send_processdata(&soem->context);
    ecx_receive_processdata(&soem->context, EC_TIMEOUTRET);

    // after the configuration all slaves are connected
    for (int i = 0; i < *e->num_exposed_slaves; i++) {
        if (e->is_connected) e->is_connected[i] = true;
    }

    *e->error_code = 0;
    goto out;

out_err_init:
    // request "init" state for all slaves
    soem->slave[0].state = EC_STATE_INIT;
    ecx_writestate(&soem->context, 0);

    // close socket
    ecx_close(&soem->context);
out_err_free:
    free(soem);
out:
    return;
}


void robif2b_ethercat_start(struct robif2b_ethercat *e)
{
    assert(e);
    assert(e->comm);
    assert(e->error_code);

    struct robif2b_ethercat_comm *soem = e->comm;

    // request "operational" state for all slaves
    soem->slave[0].state = EC_STATE_OPERATIONAL;
    ecx_writestate(&soem->context, 0);

    // wait for all slaves to reach "operational" state
    ecx_statecheck(&soem->context, 0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
    if (soem->slave[0].state != EC_STATE_OPERATIONAL) {
        print_slaves_not_in_state(e, EC_STATE_OPERATIONAL);

        fprintf(stderr, "Not all EtherCAT slaves reached Operational state\n");
        *e->error_code = -12;
        return;
    }

    *e->error_code = 0;
}


void robif2b_ethercat_update(struct robif2b_ethercat *e)
{
    assert(e);
    assert(e->comm);
    assert(e->error_code);
    assert(e->num_exposed_slaves);
    assert(e->num_current_slaves);
    assert(e->input_size);
    assert(e->output_size);
    assert(e->input);
    assert(e->output);

    struct robif2b_ethercat_comm *soem = e->comm;

    *e->num_current_slaves = ecx_receive_processdata(&soem->context,
                                                     EC_TIMEOUTRET);

    // Read/write data from/to data-flow ports
    for (int i = 0; i < *e->num_exposed_slaves; i++) {
        int idx = e->slave_idx[i];
        memcpy(e->input[i], soem->slave[idx].inputs, e->input_size[i]);
        memcpy(soem->slave[idx].outputs, e->output[i], e->output_size[i]);

        if (e->is_connected) e->is_connected[i] = !soem->slave[idx].islost;
    }

    ecx_send_processdata(&soem->context);

    if (ecx_iserror(&soem->context)) {
        fprintf(stderr, "General SOEM error\n");
        *e->error_code = -13;
        return;
    }

    *e->error_code = 0;
}


void robif2b_ethercat_stop(struct robif2b_ethercat *e)
{
    assert(e);
    assert(e->comm);
    assert(e->error_code);

    struct robif2b_ethercat_comm *soem = e->comm;

    // request "safe-operational" state for all slaves
    soem->slave[0].state = EC_STATE_SAFE_OP;
    ecx_writestate(&soem->context, 0);

    // wait for all slaves to reach "safe-operational" state
    ecx_statecheck(&soem->context, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);
    if (soem->slave[0].state != EC_STATE_SAFE_OP) {
        print_slaves_not_in_state(e, EC_STATE_SAFE_OP);

        fprintf(stderr, "Not all EtherCAT slaves reached Safe-Operational "
                        "state\n");
        *e->error_code = -14;
        return;
    }

    *e->error_code = 0;
}


void robif2b_ethercat_shutdown(struct robif2b_ethercat *e)
{
    assert(e);
    assert(e->comm);
    assert(e->error_code);

    struct robif2b_ethercat_comm *soem = e->comm;

    // request "init" state for all slaves
    soem->slave[0].state = EC_STATE_INIT;
    ecx_writestate(&soem->context, 0);

    // close socket
    ecx_close(&soem->context);

    // free SOEM state memory
    free(soem);

    *e->error_code = 0;
}

