#include "globals.h"

#include "apdu/dispatcher.h"

#include <os.h>
#include <cx.h>
#include <io.h>
#include <parser.h>
#include <status_words.h>

#include "deploy_module.h"
#include "export_private_key_legacy_path.h"
#include "export_private_key_new_path.h"
#include "get_app_name.h"
#include "get_app_version.h"
#include "get_challenge.h"
#include "get_public_key.h"
#include "init_contract.h"
#include "set_trusted_name.h"
#include "sign_configure_baker.h"
#include "sign_configure_delegation.h"
#include "sign_credential_deployment.h"
#include "sign_public_information_for_ip.h"
#include "sign_register_data.h"
#include "sign_transfer.h"
#include "sign_transfer_to_public.h"
#include "sign_transfer_with_memo.h"
#include "sign_transfer_with_schedule.h"
#include "sign_transfer_with_schedule_and_memo.h"
#include "sign_update_credential.h"
#include "update_contract.h"
#include "verify_address.h"

/**
 * Central APDU dispatcher for the Concordium app (CLA is checked in app_main).
 * Validates cdata presence (and P1/P2 for some instructions), then dispatches on `cmd->ins`
 * to the instruction handler. Multi-step signing flows use `isInitialCall` on the first chunk.
 *
 * @param cmd            Parsed APDU (`command_t`: ins, p1, p2, lc, data).
 * @param flags          BOLOS/UI flags for asynchronous signing and navigation.
 * @param isInitialCall  True on the first invocation of this instruction for the current
 * transaction.
 *
 * @return 0 after a handler runs to completion; validation failures return the value from
 *         `io_send_sw(...)`. Unknown `INS` throws `SWO_INVALID_INS`.
 */
int apdu_dispatcher(const command_t *cmd, volatile unsigned int *flags, bool isInitialCall) {
    switch (cmd->ins) {
        case INS_GET_PUBLIC_KEY:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_get_public_key(cmd, flags);
            break;
        case INS_VERIFY_ADDRESS:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_verify_address(cmd, flags);
            break;
        case INS_SIGN_TRANSFER:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_transfer(cmd, flags);
            break;
        case INS_SIGN_TRANSFER_WITH_MEMO:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_transfer_with_memo(cmd, flags, isInitialCall);
            break;
        case INS_SIGN_TRANSFER_WITH_SCHEDULE:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_transfer_with_schedule(cmd, flags, isInitialCall);
            break;
        case INS_SIGN_TRANSFER_WITH_SCHEDULE_AND_MEMO:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_transfer_with_schedule_and_memo(cmd, flags, isInitialCall);
            break;
        case INS_CREDENTIAL_DEPLOYMENT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_credential_deployment(cmd, flags, isInitialCall);
            break;
        case INS_EXPORT_PRIVATE_KEY_LEGACY:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_export_private_key_legacy_path(cmd, flags);
            break;
        case INS_EXPORT_PRIVATE_KEY_NEW:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_export_private_key_new_path(cmd, flags);
            break;
        case INS_TRANSFER_TO_PUBLIC:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_transfer_to_public(cmd, flags, isInitialCall);
            break;
        case INS_REGISTER_DATA:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_register_data(cmd, flags, isInitialCall);
            break;
        case INS_PUBLIC_INFO_FOR_IP:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_public_information_for_ip(cmd, flags, isInitialCall);
            break;
        case INS_CONFIGURE_BAKER:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_configure_baker(cmd, flags, isInitialCall);
            break;
        case INS_CONFIGURE_DELEGATION:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_configure_delegation(cmd, flags);
            break;
        case INS_SIGN_UPDATE_CREDENTIAL:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_sign_update_credential(cmd, flags, isInitialCall);
            break;
        case INS_GET_APP_NAME:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }
            handle_get_app_name();
            break;
        case INS_SET_TRUSTED_NAME:
            handle_set_trusted_name(cmd);
            break;
        case INS_GET_CHALLENGE:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }
            handle_get_challenge();
            break;
        case INS_DEPLOY_MODULE:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_deploy_module(cmd);
            break;
        case INS_INIT_CONTRACT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_init_contract(cmd);
            break;
        case INS_UPDATE_CONTRACT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handle_update_contract(cmd);
            break;
        case INS_APP_VERSION:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            handle_get_app_version();
            break;
        default:
            THROW(SWO_INVALID_INS);
            break;
    }
    return 0;
}
