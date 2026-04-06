#pragma once

#include <stdint.h>

#include <os.h>

#include "instruction_context.h"
#include "tx_state.h"

/** Application version triplet length (GET_VERSION / Makefile). */
#define APPVERSION_LEN 3
#ifndef MAJOR_VERSION
#error "Major version not set"
#endif
#ifndef MINOR_VERSION
#error "Minor version not set"
#endif
#ifndef PATCH_VERSION
#error "Patch version not set"
#endif

/**
 * Instruction class of the Concordium application.
 */
#define CLA 0xE0

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN 64

/**
 * P2 value for more data
 */
#define P2_MORE 0x80

typedef enum {
    DEPLOY_MODULE = 0,
    INIT_CONTRACT = 1,
    UPDATE_CONTRACT = 2,
    TRANSFER = 3,
    UPDATE_CREDENTIAL_KEYS = 13,
    TRANSFER_TO_PUBLIC = 18,
    TRANSFER_WITH_SCHEDULE = 19,
    UPDATE_CREDENTIALS = 20,
    REGISTER_DATA = 21,
    TRANSFER_WITH_MEMO = 22,
    TRANSFER_WITH_SCHEDULE_WITH_MEMO = 24,
    CONFIGURE_BAKER = 25,
    CONFIGURE_DELEGATION = 26,
} transactionKind_e;

/** Credential deployment UI continuation; implementation in handler/sign_credential_deployment.c */
void processNextVerificationKey(void);

typedef struct internal_storage_t {
    uint8_t dummy1_allowed;
    uint8_t dummy2_allowed;
    uint8_t initialized;
} internal_storage_t;

#define STORAGE_INITIALIZED 0x01
#define STORAGE_DEFAULT     0x00

/** Sentinel for no active instruction (before first command) */
#define INSTRUCTION_NONE -1

extern const internal_storage_t N_storage_real;

#define N_storage (*(volatile internal_storage_t *) PIC(&N_storage_real))

/*
 * Concordium-specific status words (0x6B01–0x6B0B, 0x530C). ISO7816 reserves 6Bxx for
 * proprietary use; Ledger's status_words.h only defines SWO_WRONG_P1_P2 (0x6B00) in that range.
 * These values are part of the public host↔app contract — they are not aliases of other SWO_*.
 */
#define ERROR_INVALID_STATE         0x6B01
#define ERROR_INVALID_PATH          0x6B02
#define ERROR_INVALID_PARAM         0x6B03
#define ERROR_INVALID_TRANSACTION   0x6B04
#define ERROR_UNSUPPORTED_CBOR      0x6B05
#define ERROR_BUFFER_OVERFLOW       0x6B06
#define ERROR_FAILED_CX_OPERATION   0x6B07
#define ERROR_INVALID_SOURCE_LENGTH 0x6B08
#define ERROR_INVALID_NAME_LENGTH   0x6B0A
#define ERROR_INVALID_PARAMS_LENGTH 0x6B0B
#define ERROR_INVALID_MODULE_REF    0x6B09

#define ERROR_DEVICE_LOCKED 0x530C
