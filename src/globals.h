#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <os.h>
#include <cx.h>
#include <ux.h>
#include <io.h>
#include <format.h>
#include <os_io_seproxyhal.h>
#include <lcx_hash.h>
#include <parser.h>
#include <base58.h>
#include <glyphs.h>
#include <limits.h>
#include <format.h>
#include <lcx_hmac.h>

#ifdef HAVE_NBGL
#include <nbgl_use_case.h>
#endif

#include "display.h"

#include "menu.h"
#include "util.h"
#include "sign.h"
#include "numberHelpers.h"
#include "base58check.h"
#include "time.h"

#include "exportPrivateKey.h"
#include "verifyAddress.h"
#include "getPublicKey.h"
#include "signConfigureBaker.h"
#include "signConfigureDelegation.h"
#include "signCredentialDeployment.h"
#include "signPublicInformationForIp.h"
#include "signTransfer.h"
#include "signTransferToPublic.h"
#include "signTransferWithSchedule.h"
#include "signRegisterData.h"
#include "deployModule.h"
#include "initContract.h"
#include "updateContract.h"

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of MAJOR_VERSION || MINOR_VERSION || PATCH_VERSION.
 */
#define APPVERSION_LEN 3

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN 64

typedef struct internal_storage_t {
    uint8_t dummy1_allowed;
    uint8_t dummy2_allowed;
    uint8_t initialized;
} internal_storage_t;

extern const internal_storage_t N_storage_real;

#define N_storage (*(volatile internal_storage_t *)PIC(&N_storage_real))

enum {
    // Successful codes
    SUCCESS = 0x9000,

    // Error codes
    ERROR_NO_APDU_RECEIVED = 0x6982,
    ERROR_REJECTED_BY_USER = 0x6985,

    ERROR_INVALID_STATE = 0x6B01,
    ERROR_INVALID_PATH = 0x6B02,
    ERROR_INVALID_PARAM = 0x6B03,
    ERROR_INVALID_TRANSACTION = 0x6B04,
    ERROR_UNSUPPORTED_CBOR = 0x6B05,
    ERROR_BUFFER_OVERFLOW = 0x6B06,
    ERROR_FAILED_CX_OPERATION = 0x6B07,
    ERROR_INVALID_INSTRUCTION = 0x6D00,
    ERROR_INVALID_SOURCE_LENGTH = 0x6B08,
    ERROR_INVALID_NAME_LENGTH = 0x6B0A,
    ERROR_INVALID_PARAMS_LENGTH = 0x6B0B,
    ERROR_INVALID_MODULE_REF = 0x6B09,
    // Error codes from the Ledger firmware
    ERROR_DEVICE_LOCKED = 0x530C,
    SW_WRONG_DATA_LENGTH = 0x6A87
};
