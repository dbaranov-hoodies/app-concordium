#ifndef G_INSTRUCTION_CONTEXT_H
#define G_INSTRUCTION_CONTEXT_H

#include "exportPrivateKey.h"
#include "getPublicKey.h"
#include "verifyAddress.h"
#include "signPublicInformationForIp.h"
#include "signCredentialDeployment.h"
#include "signTransferToPublic.h"
#include "signConfigureBaker.h"
#include "signConfigureDelegation.h"
#include "deployModule.h"
#include "initContract.h"
#include "updateContract.h"
#include "signTransfer.h"
#include "signTransferWithSchedule.h"
#include "signRegisterData.h"

typedef struct {
    uint32_t cborLength;
    uint32_t displayUsed;
    uint8_t  display[255];
    uint8_t  majorType;
} cborContext_t;

typedef struct {
    union {
        signTransferContext_t             signTransferContext;
        signTransferWithScheduleContext_t signTransferWithScheduleContext;
        signRegisterData_t                signRegisterData;
    };
    cborContext_t cborContext;

} transactionWithDataBlob_t;

/**
 * As the memory we have available is very limited, the context for each instruction is stored
 * in a shared global union, so that we do not use more memory than that of the most memory
 * consuming instruction context.
 */
typedef union {
    exportPrivateKeyContext_t exportPrivateKeyContext;
    exportPublicKeyContext_t  exportPublicKeyContext;
    verifyAddressContext_t    verifyAddressContext;

    signPublicInformationForIp_t      signPublicInformationForIp;
    signCredentialDeploymentContext_t signCredentialDeploymentContext;

    signTransferToPublic_t           signTransferToPublic;
    signConfigureBaker_t             signConfigureBaker;
    signConfigureDelegationContext_t signConfigureDelegation;
    deployModule_t                   deployModule;
    initContract_t                   initContract;
    updateContract_t                 updateContract;
    transactionWithDataBlob_t        withDataBlob;
} instructionContext_t;

extern instructionContext_t g_instructionContext;

#endif  // G_INSTRUCTION_CONTEXT_H