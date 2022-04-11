// SPDX-License-Identifier: LGPL-3.0
#include <robif2b/functions/kelo_power_board.h>

#include <string.h>
#include <assert.h>


void robif2b_kelo_power_board_update(struct robif2b_kelo_power_board *b)
{
    assert(b);
    assert(b->msr_pdo);
    assert(b->cmd_pdo);

    if (b->soc_msr)     *b->soc_msr     = b->msr_pdo->bmsm_soc;
    if (b->voltage_msr) *b->voltage_msr = b->msr_pdo->bmsm_voltage;
    if (b->current_msr) *b->current_msr = b->msr_pdo->bmsm_current;
    if (b->power_msr)   *b->power_msr   = b->msr_pdo->output_power;

    // Commands API currently not used
    b->cmd_pdo->generic_command  = COMMAND_DISABLE;
    b->cmd_pdo->charge_command   = COMMAND_DISABLE;
    b->cmd_pdo->shutdown_command = COMMAND_DISABLE;
}
