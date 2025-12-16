#ifndef APDU_DISPATCHER_H
#define APDU_DISPATCHER_H

#include <stdint.h>
#include <stdbool.h>

#include "parser.h"

/**
 * @enum command_e
 * @brief APDU INS instructions supported by the application.
 */
typedef enum {
    /** Verify an address.
     * See @ref doc_verify_address "doc/verify_address.md"
     */
    INS_VERIFY_ADDRESS = 0x00,

    /** Start the public-key flow: @ref get_public_key "doc/ins_get_public_key.md"
     */
    INS_GET_PUBLIC_KEY = 0x01,

    /** Start the transfer signing flow: @ref doc_sign_transfer "doc/ins_doc_sign_transfer"*/
    INS_SIGN_TRANSFER = 0x02,

    /** Start the scheduled transfer signing flow    : @ref doc_sign_transfer_with_schedule
       "doc/doc_sign_transfer_with_schedule"*/
    INS_SIGN_TRANSFER_WITH_SCHEDULE = 0x03,

    /** Start the credential deployment signing flow FLOW IS NOT DOCUMENTED*/
    INS_CREDENTIAL_DEPLOYMENT = 0x04,

    /** @ref doc_export_private_key "doc/doc_export_private_key" */
    INS_EXPORT_PRIVATE_KEY_LEGACY = 0x05,

    /** FLOW IS NOT DOCUMENTED*/
    INS_DEPLOY_MODULE = 0x06,

    /** FLOW IS NOT DOCUMENTED*/
    INS_INIT_CONTRACT = 0x07,

    /** FLOW IS NOT DOCUMENTED*/
    INS_UPDATE_CONTRACT = 0x08,

    /** @ref doc_transfer_to_public "doc/doc_transfer_to_public" */
    INS_TRANSFER_TO_PUBLIC = 0x12,

    /** @ref doc_configure_delegation "doc/doc_configure_delegation" */
    INS_CONFIGURE_DELEGATION = 0x17,

    /**@ref doc_configure_baker "doc/doc_configure_baker" */
    INS_CONFIGURE_BAKER = 0x18,
    /** @ref doc_public_info "doc/doc_public_info" */
    INS_PUBLIC_INFO_FOR_IP = 0x20,

    /** FLOW IS NOT DOCUMENTED*/
    INS_SIGN_UPDATE_CREDENTIAL = 0x31,

    /** FLOW IS NOT DOCUMENTED*/
    INS_SIGN_TRANSFER_WITH_MEMO = 0x32,

    /** FLOW IS NOT DOCUMENTED*/
    INS_SIGN_TRANSFER_WITH_SCHEDULE_AND_MEMO = 0x34,

    /** @ref doc_register_data "doc/doc_register_data" */
    INS_REGISTER_DATA = 0x35,

    /** FLOW IS NOT DOCUMENTED*/
    INS_APP_NAME = 0x36,

    /** FLOW IS NOT DOCUMENTED*/
    INS_GET_APP_NAME = 0x21,

    /** @ref doc_export_private_key "doc/doc_export_private_key" */
    INS_EXPORT_PRIVATE_KEY_NEW = 0x37,

} command_e;

int apdu_dispatcher(const command_t *cmd, volatile unsigned int *flags, bool isInitialCall);

#endif  // APDU_DISPATCHER_H