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

/**
 * @brief Dispatch and handle a single Concordium APDU command.
 *
 * This function is the central APDU instruction dispatcher for the
 * Concordium Ledger application. It validates the APDU class byte,
 * logs the received command, and routes execution to the instruction-
 * specific handler based on the INS value.
 *
 * Each instruction handler is responsible for:
 *  - Validating P1 / P2 semantics
 *  - Parsing command data (CData)
 *  - Managing multi-step UI flows where applicable
 *  - Producing the appropriate APDU response or status word
 *
 * Multi-call flows:
 *  Some instructions require multiple APDU calls to complete a user-
 *  confirmed operation. The @p isInitialCall flag indicates whether
 *  this invocation starts a new flow or resumes an ongoing one.
 *
 * @param[in]  cmd            Parsed APDU command (CLA, INS, P1, P2, Lc, CData)
 * @param[out] flags          I/O flags controlling APDU exchange behavior
 * @param[in]  isInitialCall  True if this is the first call of a command flow
 *
 * @return 0 on successful dispatch. Some instructions may return
 *         directly after sending a response.
 *
 * @throws SWO_INVALID_CLA if the CLA byte does not match CLA_CONCORDIUM
 * @throws SWO_INVALID_INS if the INS value is not supported
 */
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
