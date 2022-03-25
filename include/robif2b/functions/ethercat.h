// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_FUNCTIONS_ETHERCAT_H
#define ROBIF2B_FUNCTIONS_ETHERCAT_H

#include <robif2b/types/ethercat.h>


#ifdef __cplusplus
extern "C" {
#endif

void robif2b_ethercat_configure(struct robif2b_ethercat *e);

void robif2b_ethercat_start(struct robif2b_ethercat *e);

void robif2b_ethercat_update(struct robif2b_ethercat *e);

void robif2b_ethercat_stop(struct robif2b_ethercat *e);

void robif2b_ethercat_shutdown(struct robif2b_ethercat *e);

#ifdef __cplusplus
}
#endif

#endif
