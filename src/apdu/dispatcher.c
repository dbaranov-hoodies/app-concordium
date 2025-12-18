#include "dispatcher.h"

#include <io.h>
#include <ledger_assert.h>
#include <status_words.h>

#include "deployModule.h"
#include "exportPrivateKey.h"
#include "getAppName.h"
#include "getPublicKey.h"
#include "initContract.h"
#include "signConfigureBaker.h"
#include "signConfigureDelegation.h"
#include "signCredentialDeployment.h"
#include "signPublicInformationForIp.h"
#include "signRegisterData.h"
#include "signTransfer.h"
#include "signTransferToPublic.h"
#include "signTransferWithSchedule.h"
#include "updateContract.h"
#include "verifyAddress.h"

#define LEDGER_ASSERT_NULL_CDATA LEDGER_ASSERT(cmd->data != NULL, "NULL cdata")

int apdu_dispatcher(const command_t *cmd, volatile unsigned int *flags, bool isInitialCall)
{
    if (cmd->cla != CLA_CONCORDIUM) {
        io_send_sw(SWO_INVALID_CLA);
    }

    PRINTF("=> CLA=%02X | INS=%02X | P1=%02X | P2=%02X | Lc=%02X | CData=%.*H\n",
           cmd->cla,
           cmd->ins,
           cmd->p1,
           cmd->p2,
           cmd->lc,
           cmd->lc,
           cmd->data);

    switch (cmd->ins) {
        case INS_VERIFY_ADDRESS:
            LEDGER_ASSERT_NULL_CDATA;
            handleVerifyAddress(cmd->data, cmd->p1, cmd->lc, flags);
            break;

        case INS_GET_PUBLIC_KEY:
            LEDGER_ASSERT_NULL_CDATA;
            handleGetPublicKey(cmd->data, cmd->p1, cmd->p2, cmd->lc, flags);
            break;

        case INS_SIGN_TRANSFER:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignTransfer(cmd->data, cmd->lc, flags);
            break;

        case INS_SIGN_TRANSFER_WITH_MEMO:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignTransferWithMemo(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_SIGN_TRANSFER_WITH_SCHEDULE:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignTransferWithSchedule(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_SIGN_TRANSFER_WITH_SCHEDULE_AND_MEMO:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignTransferWithScheduleAndMemo(
                cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_CREDENTIAL_DEPLOYMENT:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignCredentialDeployment(
                cmd->data, cmd->p1, cmd->p2, cmd->lc, flags, isInitialCall);
            break;

        case INS_EXPORT_PRIVATE_KEY_LEGACY:
            LEDGER_ASSERT_NULL_CDATA;
            handleExportPrivateKey(cmd->data, cmd->p1, cmd->p2, cmd->lc, true, flags);
            break;

        case INS_EXPORT_PRIVATE_KEY_NEW:
            LEDGER_ASSERT_NULL_CDATA;
            handleExportPrivateKey(cmd->data, cmd->p1, cmd->p2, cmd->lc, false, flags);
            break;

        case INS_TRANSFER_TO_PUBLIC:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignTransferToPublic(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_REGISTER_DATA:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignRegisterData(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_PUBLIC_INFO_FOR_IP:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignPublicInformationForIp(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_CONFIGURE_BAKER:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignConfigureBaker(cmd->data, cmd->p1, cmd->lc, flags, isInitialCall);
            break;

        case INS_CONFIGURE_DELEGATION:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignConfigureDelegation(cmd->data, cmd->lc, flags);
            break;

        case INS_SIGN_UPDATE_CREDENTIAL:
            LEDGER_ASSERT_NULL_CDATA;
            handleSignUpdateCredential(cmd->data, cmd->p1, cmd->p2, cmd->lc, flags, isInitialCall);
            break;

        case INS_GET_APP_NAME:
            return handleGetAppName();
            break;

        case INS_DEPLOY_MODULE:
            LEDGER_ASSERT_NULL_CDATA;
            handleDeployModule(cmd->data, cmd->p1, cmd->lc);
            break;

        case INS_INIT_CONTRACT:
            LEDGER_ASSERT_NULL_CDATA;
            handleInitContract(cmd->data, cmd->p1, cmd->lc);
            break;

        case INS_UPDATE_CONTRACT:
            LEDGER_ASSERT_NULL_CDATA;
            handleUpdateContract(cmd->data, cmd->p1, cmd->lc);
            break;

        default:
            THROW(SWO_INVALID_INS);
            break;
    }
    return 0;
}
