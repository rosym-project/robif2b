// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_KELO_POWER_BOARD_API_H
#define ROBIF2B_TYPES_KELO_POWER_BOARD_API_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

struct robif2b_kelo_power_board_api_msr_pdo {
    uint64_t timestamp;         // EtherCAT timestamp ms
    uint16_t status;            // Status bits
    uint16_t error;             // Error bits: bit0:LowBAT, bit1:EMPTYFRAM, bit2:WrongFRAM, bit3:WrongCHARGER, bit4:OverloadCHG, bit5:OverCurrentError, bit6:Watchdog
    uint16_t warning;           // Warning bits: bit0:OverCurrentWarning, bit1:Shutdown
    float    output_current;    // Total current consumption
    float    output_voltage;    // System Voltage
    float    output_power;      // Total power consumption of the system
    float    aux_port_current;  // Current consumption at Auxiliary port
    float    generic_data_1;    // Generic data, might be used for different purposes
    uint32_t generic_data_2;    // Generic data, might be used for different purposes

    uint16_t bmsm_power_device_id;  // Battery Management System Module
    uint16_t bmsm_status;           // Battery Management System Module
    float    bmsm_voltage;          // Battery Management System Module
    float    bmsm_current;          // Battery Management System Module
    float    bmsm_temperature;      // Battery Management System Module
    uint16_t bmsm_soc;              // Battery Management System Module
    uint32_t bmsm_sn;               // Battery Management System Module
    uint32_t bmsm_generic_data_1;   // Battery Management System Module
    float    bmsm_generic_data_2;   // Battery Management System Module
} __attribute__ ((__packed__));

#define COMMAND_DISABLE 0x00

struct robif2b_kelo_power_board_api_cmd_pdo {
    uint32_t generic_command;
    uint32_t charge_command;
    uint16_t shutdown_command;
    uint16_t power_device_id;
} __attribute__ ((__packed__));

#ifdef __cplusplus
}
#endif

#endif
