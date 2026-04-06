#pragma once

#include <cx.h>

#include "buffer.h"
#include "set_trusted_name.h"
#include "tlv_library.h"

typedef struct trustedNameMultiHashCtx_s {
    cx_sha256_t sha256;
    cx_sha3_t sha3_256;
    cx_sha3_t keccak_256;
    cx_ripemd160_t ripemd160;
    cx_sha512_t sha512;
} trustedNameMultiHashCtx_t;

typedef struct trustedNameTlvExtracted_s {
    TLV_reception_t received_tags;

    uint8_t structure_type;
    uint8_t version;
    uint8_t trusted_name_type;
    uint8_t trusted_name_source;
    char name[TRUSTED_NAME_MAX_LEN + 1];
    buffer_t address;
    uint64_t chain_id;
    uint64_t challenge;
    uint16_t signer_key_id;
    uint8_t signer_algo;
    buffer_t signature;
} trustedNameTlvExtracted_t;

typedef struct trustedNamePkiContext_s {
    /**
     * Last GET_CHALLENGE value. Must survive SET_TRUSTED_NAME TLV/hash partial clears
     * (see handler/set_trusted_name.c clear_trusted_name_pki_working_state).
     */
    uint64_t stored_challenge;
    trustedNameMultiHashCtx_t hash_ctx;
    trustedNameTlvExtracted_t tlv;
} trustedNamePkiContext_t;
