#pragma once

/**
 * Shared APDU instruction union (`global`) and the types of its members only.
 * Prefer including this (plus `tx_state.h` / `globals.h` as needed) instead of growing
 * `globals.h` with unrelated headers.
 */

#include "deploy_module.h"
#include "export_private_key.h"
#include "getPublicKey.h"
#include "init_contract.h"
#include "sign_configure_baker.h"
#include "sign_configure_delegation.h"
#include "sign_credential_deployment_context.h"
#include "sign_public_information_for_ip.h"
#include "sign_transfer_to_public.h"
#include "transaction_with_data_blob.h"
#include "trusted_name_pki_context.h"
#include "update_contract.h"
#include "verify_address.h"

typedef union {
    exportPrivateKeyContext_t exportPrivateKeyContext;
    exportPublicKeyContext_t exportPublicKeyContext;
    verifyAddressContext_t verifyAddressContext;

    signPublicInformationForIp_t signPublicInformationForIp;
    signCredentialDeploymentContext_t signCredentialDeploymentContext;

    signTransferToPublic_t signTransferToPublic;
    signConfigureBaker_t signConfigureBaker;
    signConfigureDelegationContext_t signConfigureDelegation;
    deployModule_t deployModule;
    initContract_t initContract;
    updateContract_t updateContract;
    transactionWithDataBlob_t withDataBlob;
    trustedNamePkiContext_t trustedNamePki;
} instructionContext;

extern instructionContext global;
