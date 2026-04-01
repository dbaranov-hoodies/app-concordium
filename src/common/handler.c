#include "globals.h"
#include "challenge.h"
#include "getAppName.h"
#include "get_app_version.h"

int handler(const command_t *cmd, volatile unsigned int *flags, bool isInitialCall) {
    switch (cmd->ins) {
        case INS_GET_PUBLIC_KEY:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleGetPublicKey(cmd, flags);
            break;
        case INS_VERIFY_ADDRESS:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleVerifyAddress(cmd, flags);
            break;
        case INS_SIGN_TRANSFER:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignTransfer(cmd, flags);
            break;
        case INS_SIGN_TRANSFER_WITH_MEMO:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignTransferWithMemo(cmd, flags, isInitialCall);
            break;
        case INS_SIGN_TRANSFER_WITH_SCHEDULE:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignTransferWithSchedule(cmd, flags, isInitialCall);
            break;
        case INS_SIGN_TRANSFER_WITH_SCHEDULE_AND_MEMO:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignTransferWithScheduleAndMemo(cmd, flags, isInitialCall);
            break;
        case INS_CREDENTIAL_DEPLOYMENT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignCredentialDeployment(cmd, flags, isInitialCall);
            break;
        case INS_EXPORT_PRIVATE_KEY_LEGACY:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleExportPrivateKeyLegacyPath(cmd, flags);
            break;
        case INS_EXPORT_PRIVATE_KEY_NEW:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleExportPrivateKeyNewPath(cmd, flags);
            break;
        case INS_TRANSFER_TO_PUBLIC:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignTransferToPublic(cmd, flags, isInitialCall);
            break;
        case INS_REGISTER_DATA:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignRegisterData(cmd, flags, isInitialCall);
            break;
        case INS_PUBLIC_INFO_FOR_IP:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignPublicInformationForIp(cmd, flags, isInitialCall);
            break;
        case INS_CONFIGURE_BAKER:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignConfigureBaker(cmd, flags, isInitialCall);
            break;
        case INS_CONFIGURE_DELEGATION:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignConfigureDelegation(cmd, flags);
            break;
        case INS_SIGN_UPDATE_CREDENTIAL:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleSignUpdateCredential(cmd, flags, isInitialCall);
            break;
        case INS_GET_APP_NAME:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }
            handleGetAppName();
            break;
        case INS_SET_TRUSTED_NAME:
            handleSetTrustedName(cmd);
            break;
        case INS_GET_CHALLENGE:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }
            handleGetChallenge();
            break;
        case INS_DEPLOY_MODULE:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleDeployModule(cmd);
            break;
        case INS_INIT_CONTRACT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleInitContract(cmd);
            break;
        case INS_UPDATE_CONTRACT:
            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            handleUpdateContract(cmd);
            break;
        case INS_APP_VERSION:
            if (cmd->data != NULL) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }
            if (cmd->p1 != P1_DEFAULT || cmd->p2 != P2_DEFAULT) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            handler_get_version();
            break;
        default:
            THROW(SWO_INVALID_INS);
            break;
    }
    return 0;
}
