// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_CONTROL_MODE_H
#define ROBIF2B_TYPES_CONTROL_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

enum robif2b_ctrl_mode
{
    ROBIF2B_CTRL_MODE_POSITION = 0,
    ROBIF2B_CTRL_MODE_VELOCITY = 1,
    ROBIF2B_CTRL_MODE_FORCE    = 2,
    ROBIF2B_CTRL_MODE_CURRENT  = 3
};

#ifdef __cplusplus
}
#endif

#endif
