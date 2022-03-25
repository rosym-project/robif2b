// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_TYPES_ETHERCAT_H
#define ROBIF2B_TYPES_ETHERCAT_H

#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

struct robif2b_ethercat_comm;

struct robif2b_ethercat
{
    /**
     * The name of the Ethernet interface via which to connect to EtherCAT bus.
     * Pointer to a null-terminated string. Should not be changed after the
     * EtherCAT master has been configured (cf.
     * \ref robif2b_ethercat_configure).
     */
    const char *ethernet_if;

    /**
     * The number of EtherCAT slaves that are exposed or exported (i.e. actively
     * communicating with the application). Should not be changed after the
     * EtherCAT master has been configured (cf.
     * \ref robif2b_ethercat_configure).
     */
    const int *num_exposed_slaves;

    /**
     * The index or address of each exposed slave on the EtherCAT bus. Pointer
     * to an array with @ref num_exposed_slaves entries. Should not be changed
     * after the EtherCAT master has been configured (cf.
     * \ref robif2b_ethercat_configure).
     */
    const int *slave_idx;

    /**
     * The expected name of each exposed slave. Pointer to an array with
     * @ref num_exposed_slaves entries. Each entry must be a pointer to a
     * null-terminated string. Should not be changed after the EtherCAT master
     * has been configured (cf. \ref robif2b_ethercat_configure).
     */
    const char **name;

    /**
     * The expected EEPROM ID/produce code of each exposed slave. Pointer to an
     * array with @ref num_exposed_slaves entries. Should not be changed after the
     * EtherCAT master has been configured (cf.
     * \ref robif2b_ethercat_configure).
     */
    const unsigned int *product_code;

    /**
     * For each exposed slave the expected size (in bytes) of messages (process
     * data objects) as received by the master. Pointer to an array with
     * @ref num_exposed_slaves entries. Should not be changed after the EtherCAT
     * master has been configured (cf. \ref robif2b_ethercat_configure).
     */
    const size_t *input_size;

    /**
     * For each exposed slave the expected size (in bytes) of messages (process
     * data objects) as sent by the master. Pointer to an array with
     * @ref num_exposed_slaves entries. Should not be changed after the EtherCAT
     * master has been configured (cf. \ref robif2b_ethercat_configure).
     */
    const size_t *output_size;



    /**
     * An error code associated with the last called function. Hence, an
     * application should check this value after each EtherCAT-related function
     * call. A negative value indicates a failure.
     */
    int *error_code;

    /**
     * The number of EtherCAT slaves that have been found during the
     * configuration of the EtherCAT bus. The value will only be changed during
     * the EtherCAT configuration (cf. \ref robif2b_ethercat_configure).
     */
    int *num_initial_slaves;

    /**
     * The number of EtherCAT slaves that were still actuve during the last
     * communication cycle. This number should always be less than or equal to
     * the initially discovered slaves. The value can change during each
     * EtherCAT communication cycle (cf. \ref robif2b_ethercat_update).
     */
    int *num_current_slaves;

    /**
     * Buffers to store the data (process data objects) that the EtherCAT master
     * receives from each worker slave. Pointer to an array with
     * @ref num_exposed_slaves entries. The i-th entry in that array must be a
     * pointer to a buffer that has the size (in bytes) as specified in the i-th
     * entry of the @ref input_size array. The value can change during each
     * EtherCAT communication cycle (cf. \ref robif2b_ethercat_update).
     */
    void **input;

    /**
     * Buffers to store the data (process data objects) that the EtherCAT master
     * sends to each worker slave. Pointer to an array with
     * @ref num_exposed_slaves entries. The i-th entry in that array must be a
     * pointer to a buffer that has the size (in bytes) as specified in the i-th
     * entry of the @ref output_size array. The value can change during each
     * EtherCAT communication cycle (cf. \ref robif2b_ethercat_update).
     */
    const void **output;

    /**
     * Indicate which worker slave is currently still connected to the EtherCAT
     * bus. Pointer to an array with @ref num_exposed_slaves entries. The value
     * can change during each EtherCAT communication cycle (cf. \ref
     * robif2b_ethercat_update).
     */
    bool *is_connected;

    /**
     * Internal state.
     */
    struct robif2b_ethercat_comm *comm;
};

#ifdef __cplusplus
}
#endif

#endif

