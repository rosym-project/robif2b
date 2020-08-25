// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_FUNCTIONS_KINOVA_GEN3_H
#define ROBIF2B_FUNCTIONS_KINOVA_GEN3_H

#include <robif2b/types/kinova_gen3.h>


#ifdef __cplusplus
extern "C" {
#endif

void robif2b_kinova_gen3_configure(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_shutdown(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_start(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_stop(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_recover(struct robif2b_kinova_gen3_nbx *b);

void robif2b_kinova_gen3_update(struct robif2b_kinova_gen3_nbx *b);

#ifdef __cplusplus
}
#endif

#endif
