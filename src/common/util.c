

#include "util.h"

#include <exceptions.h>
#include <lcx_eddsa.h>
#include <lcx_hash.h>
#include <lcx_math.h>
#include <os_io.h>
#include <os_io_legacy.h>
#include <os_seed.h>
#include <cx_errors.h>
#include <ox_ec.h>
#include <ox_bn.h>

#include <stdint.h>
#include "account_sender.h"
#include "base58check.h"
#include "global_defines.h"
#include "key_derivation_path.h"
#include "menu.h"
#include "numberHelpers.h"
#include "status.h"
#include "tx_state.h"

extern accountSender_t global_account_sender;

static tx_state_t          *tx_state        = &g_tx_state;
static keyDerivationPath_t *keyPath         = &g_path;
static accountSender_t     *accountSender   = &global_account_sender;
static const uint32_t       HARDENED_OFFSET = 0x80000000;

int parseKeyDerivationPath(uint8_t *cdata, uint8_t dataLength)
{
    if (dataLength < 1) {
        THROW(SWO_INVALID_PATH);
    }
    keyPath->pathLength = cdata[0];

    // Concordium does not use key paths with a length greater than 8,
    // so if that was received, then throw an error.
    if (keyPath->pathLength > 8) {
        THROW(SWO_INVALID_PATH);
    }

    if (dataLength < 1 + (4 * keyPath->pathLength)) {
        THROW(SWO_INVALID_PATH);
    }

    // Each part of a key path is a uint32, parse through each part of the
    // derivation path. All paths are hardened, but we save a non-hardened
    // version that can be displayed if needed.
    for (int i = 0; i < keyPath->pathLength; ++i) {
        uint32_t node                    = U4BE(cdata, 1 + (i * 4));
        keyPath->rawKeyDerivationPath[i] = node;
        keyPath->keyDerivationPath[i]    = node | HARDENED_OFFSET;
    }

    return 1 + (4 * keyPath->pathLength);
}

/**
 * Generic method for hashing and validating header and type for a transaction.
 * Use hashAccountTransactionHeaderAndKind or hashUpdateHeaderAndType
 * instead of using this method directly.
 */
int hashHeaderAndType(uint8_t *cdata, uint8_t dataLength, uint8_t headerLength, uint8_t validType)
{
    if (dataLength < headerLength + 1) {
        THROW(SWO_INVALID_TRANSACTION);
    }
    updateHash((cx_hash_t *) &tx_state->hash, cdata, headerLength);
    cdata += headerLength;

    uint8_t type = cdata[0];
    if (type != validType) {
        THROW(SWO_INVALID_TRANSACTION);
    }
    updateHash((cx_hash_t *) &tx_state->hash, cdata, 1);

    return headerLength + 1;
}

/**
 * Adds the account transaction header and the transaction kind to the hash. The
 * transaction kind is verified to have the supplied value to prevent processing
 * invalid transactions.
 *
 * A side effect of this method is that the sender address from the transaction
 * header is parsed and saved in a global variable, so that it is available to
 * be displayed for all account transactions.
 */
int hashAccountTransactionHeaderAndKind(uint8_t *cdata,
                                        uint8_t  dataLength,
                                        uint8_t  validTransactionKind)
{
    // Parse the account sender address from the transaction header, so it can
    // be shown.
    size_t outputSize = sizeof(accountSender->sender);
    if (base58check_encode(cdata, 32, accountSender->sender, &outputSize) == -1) {
        // The received address bytes are not a valid base58 encoding.
        THROW(SWO_INVALID_TRANSACTION);
    }
    accountSender->sender[55] = '\0';

    return hashHeaderAndType(
        cdata, dataLength, ACCOUNT_TRANSACTION_HEADER_LENGTH, validTransactionKind);
}

/**
 * Adds the update header and the update type to the hash. The update
 * type is verified to have the supplied value to prevent processing
 * invalid transactions.
 */
int hashUpdateHeaderAndType(uint8_t *cdata, uint8_t dataLength, uint8_t validUpdateType)
{
    return hashHeaderAndType(cdata, dataLength, UPDATE_HEADER_LENGTH, validUpdateType);
}

int handleHeaderAndToAddress(uint8_t *cdata,
                             uint8_t  dataLength,
                             uint8_t  kind,
                             uint8_t *recipientDst,
                             size_t   recipientSize)
{
    // Parse the key derivation path, which should always be the first thing
    // received in a command to the Ledger application.
    int keyPathLength = parseKeyDerivationPath(cdata, dataLength);
    cdata += keyPathLength;
    uint8_t remainingDataLength = dataLength - keyPathLength;

    // Initialize the hash that will be the hash of the whole transaction, which
    // is what will be signed if the user approves.
    if (cx_sha256_init(&tx_state->hash) != CX_SHA256) {
        THROW(SWO_FAILED_CX_OPERATION);
    }
    int headerLength = hashAccountTransactionHeaderAndKind(cdata, remainingDataLength, kind);
    cdata += headerLength;
    remainingDataLength -= headerLength;

    // Extract the recipient address and add to the hash.
    uint8_t toAddress[32];
    if (remainingDataLength < 32) {
        THROW(SWO_INVALID_TRANSACTION);
    }
    memmove(toAddress, cdata, 32);
    updateHash((cx_hash_t *) &tx_state->hash, toAddress, 32);

    // The recipient address is in a base58 format, so we need to encode it to
    // be able to display in a human-readable way.
    if (base58check_encode(toAddress, sizeof(toAddress), recipientDst, &recipientSize) == -1) {
        // The received address bytes are not a valid base58 encoding.
        THROW(SWO_INVALID_TRANSACTION);
    }
    recipientDst[55] = '\0';
    return keyPathLength + headerLength + 32;
}

void sendUserRejection()
{
    sendUserRejectionNoIdle();
    ui_menu_main();
}

void sendUserRejectionNoIdle()
{
    G_io_apdu_buffer[0] = SWO_CONDITIONS_NOT_SATISFIED >> 8;
    G_io_apdu_buffer[1] = SWO_CONDITIONS_NOT_SATISFIED & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
}

void sendSuccess(uint8_t tx)
{
    G_io_apdu_buffer[tx++] = SWO_SUCCESS >> 8;
    G_io_apdu_buffer[tx++] = SWO_SUCCESS & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
    ui_menu_main();
}

void sendSuccessNoIdle()
{
    sendSuccessResultNoIdle(0);
}

void sendSuccessResultNoIdle(uint8_t tx)
{
    G_io_apdu_buffer[tx++] = SWO_SUCCESS >> 8;
    G_io_apdu_buffer[tx++] = SWO_SUCCESS & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
}

void getIdentityAccountDisplay(uint8_t *dst,
                               size_t   dstLength,
                               uint32_t identityIndex,
                               uint32_t accountIndex)
{
    int offset = numberToText(dst, dstLength, identityIndex);
    memmove(dst + offset, "/", 1);
    offset += 1;
    bin2dec(dst + offset, dstLength - offset, accountIndex);
}

void getIdentityAccountDisplayNewPath(uint8_t *dst,
                                      size_t   dstLength,
                                      uint32_t identityProviderIndex,
                                      uint32_t identityIndex,
                                      uint32_t accountIndex)
{
    // Convert identityProviderIndex to text and store it in dst
    int offset = numberToText(dst, dstLength, identityProviderIndex);
    memmove(dst + offset, "/", 1);
    offset += 1;

    // Convert identityIndex to text and append it to dst
    offset += numberToText(dst + offset, dstLength - offset, identityIndex);
    memmove(dst + offset, "/", 1);
    offset += 1;

    // Convert accountIndex to text and append it to dst
    bin2dec(dst + offset, dstLength - offset, accountIndex);
}

/**
 * Used to validate that an error result code from a Ledger library call
 * is equal CX_OK. If it is not CX_OK, then throw an ERROR_FAILED_CX_OPERATION
 * error that should be sent back to the callee.
 */
void ensureNoError(cx_err_t errorCode)
{
    if (errorCode != CX_OK) {
        THROW(SWO_FAILED_CX_OPERATION);
    }
}

void getPrivateKey(uint32_t *keyPathInput, uint8_t keyPathLength, cx_ecfp_private_key_t *privateKey)
{
    uint8_t privateKeyData[64];

    // Invoke the device methods for generating a private key.
    // Wrap in try/finally to ensure that private key information is cleaned up,
    // even if a system call fails.
    BEGIN_TRY
    {
        TRY
        {
            ensureNoError(os_derive_bip32_with_seed_no_throw(HDW_ED25519_SLIP10,
                                                             CX_CURVE_Ed25519,
                                                             keyPathInput,
                                                             keyPathLength,
                                                             privateKeyData,
                                                             NULL,
                                                             (unsigned char *) "ed25519 seed",
                                                             12));
            ensureNoError(cx_ecfp_init_private_key_no_throw(
                CX_CURVE_Ed25519, privateKeyData, 32, privateKey));
        }
        FINALLY
        {
            // Clean up the private key seed data, so that we cannot leak it.
            explicit_bzero(&privateKeyData, sizeof(privateKeyData));
        }
    }
    END_TRY;
}

// Generic method that signs the input with the key given by the derivation path
// that has been loaded into keyPath.
void sign(uint8_t *input, uint8_t *signatureOnInput)
{
    cx_ecfp_private_key_t privateKey;

    BEGIN_TRY
    {
        TRY
        {
            getPrivateKey(keyPath->keyDerivationPath, keyPath->pathLength, &privateKey);
            ensureNoError(
                cx_eddsa_sign_no_throw(&privateKey, CX_SHA512, input, 32, signatureOnInput, 64));
        }
        FINALLY
        {
            // Clean up the private key, so that we cannot leak it.
            explicit_bzero(&privateKey, sizeof(privateKey));
        }
    }
    END_TRY;
}

#define l_CONST        48  // ceil((3 * ceil(log2(r))) / 16)
#define BLS_KEY_LENGTH 32
#define SEED_LENGTH    32

void hash(cx_hash_t           *hashContext,
          uint32_t             mode,
          const unsigned char *in,
          unsigned int         len,
          unsigned char       *out,
          unsigned int         out_len)
{
    ensureNoError(cx_hash_no_throw(hashContext, mode, in, len, out, out_len));
}

void updateHash(cx_hash_t *hashContext, const unsigned char *in, unsigned int len)
{
    return hash(hashContext, 0, in, len, NULL, 0);
}

// We must declare the functions for the static analyzer to be happy. Ideally we
// would have access to the declarations from the Ledger SDK.
void cx_hkdf_extract(const cx_md_t        hash_id,
                     const unsigned char *ikm,
                     unsigned int         ikm_len,
                     unsigned char       *salt,
                     unsigned int         salt_len,
                     unsigned char       *prk);
// void cx_hkdf_expand(const cx_md_t        hash_id,
//                     const unsigned char *prk,
//                     unsigned int         prk_len,
//                     unsigned char       *info,
//                     unsigned int         info_len,
//                     unsigned char       *okm,
//                     unsigned int         okm_len);

// static const uint8_t l_bytes[2] = {0, l_CONST};

/** This implements the bls key generation algorithm specified in
 * https://datatracker.ietf.org/doc/html/draft-irtf-cfrg-bls-signature-04#section-2.3,
 * The optional parameter key_info is hardcoded to an empty string. Uses sha256
 * as the hash function. The generated key has length 32, and dst should have at
 * least that length, or the function throws an error.
 */
void blsKeygen(const uint8_t *seed,
               size_t         seedLength,

               uint8_t *dst,
               size_t   dstLength)
{
    cx_err_t error = 0;
    if (dstLength < BLS_KEY_LENGTH) {
        THROW(SWO_BUFFER_OVERFLOW);
    }
    if (seedLength != SEED_LENGTH) {
        THROW(SWO_INVALID_TRANSACTION);
    }

    uint8_t ikm[SEED_LENGTH + 1];
    memcpy(ikm, seed, SEED_LENGTH);
    ikm[SEED_LENGTH] = 0;

    uint8_t salt[32] = {66, 76, 83, 45, 83, 73, 71, 45, 75, 69,
                        89, 71, 69, 78, 45, 83, 65, 76, 84, 45};  // "BLS-SIG-KEYGEN-SALT-"
    size_t  salt_len = 20;

    uint8_t prk[32];
    uint8_t okm[48];

    unsigned char info_dummy[1] = {0};

    cx_bn_t y1, y2, sk, shift;
    CX_CHECK(cx_bn_lock(8, 0));
    CX_CHECK(cx_bn_alloc(&y1, 32));
    CX_CHECK(cx_bn_alloc(&y2, 32));
    CX_CHECK(cx_bn_alloc(&sk, 32));
    CX_CHECK(cx_bn_alloc(&shift, 32));

    // shift = 2^248
    CX_CHECK(cx_bn_set_u32(shift, 0));
    CX_CHECK(cx_bn_set_bit(shift, 248));

    cx_bn_t zero;
    CX_CHECK(cx_bn_alloc(&zero, 48));
    CX_CHECK(cx_bn_set_u32(zero, 0));

    int diff;

    cx_bn_t r;
    CX_CHECK(cx_bn_alloc_init(&r, 32, r_bls, sizeof(r_bls)));
    do {
        // salt = SHA256(salt)
        CX_CHECK(cx_hash_sha256(salt, salt_len, salt, sizeof(salt)));
        salt_len = sizeof(salt);

        // HKDF-Extract
        cx_hkdf_extract(CX_SHA256, ikm, sizeof(ikm), salt, sizeof(salt), prk);

        // HKDF-Expand

        cx_hkdf_expand(CX_SHA256, prk, sizeof(prk), info_dummy, 0, okm, sizeof(okm));

        // Reverse bytes for little-endian interpretation
        for (int i = 0; i < 24; i++) {
            uint8_t tmp = okm[i];
            okm[i]      = okm[47 - i];
            okm[47 - i] = tmp;
        }

        // y1 = first 31 bytes
        uint8_t y1_buf[32] = {0};
        memcpy(y1_buf, okm, 31);
        CX_CHECK(cx_bn_init(y1, y1_buf, 32));

        // y2 = last 17 bytes
        uint8_t y2_buf[32] = {0};
        memcpy(y2_buf, okm + 31, 17);
        CX_CHECK(cx_bn_init(y2, y2_buf, 32));

        // y2 *= 2^248
        CX_CHECK(cx_bn_mul(y2, y2, shift));

        // sk = (y1 + y2) mod r
        CX_CHECK(cx_bn_mod_add(sk, y1, y2, r));
        CX_CHECK(cx_bn_cmp(sk, zero, &diff));

    } while (diff == 0);

    // Export 48-byte scalar
    CX_CHECK(cx_bn_export(sk, dst, BLS_KEY_LENGTH));

end:
    CX_CHECK(cx_bn_destroy(&y1));
    CX_CHECK(cx_bn_destroy(&y2));
    CX_CHECK(cx_bn_destroy(&sk));
    CX_CHECK(cx_bn_destroy(&shift));
    CX_CHECK(cx_bn_unlock());
}
void getBlsPrivateKey(uint32_t *keyPathInput,
                      uint8_t   keyPathLength,
                      uint8_t  *privateKey,
                      size_t    privateKeySize)
{
    cx_ecfp_private_key_t privateKeySeed;
    BEGIN_TRY
    {
        TRY
        {
            getPrivateKey(keyPathInput, keyPathLength, &privateKeySeed);
            blsKeygen(privateKeySeed.d, sizeof(privateKeySeed.d), privateKey, privateKeySize);
        }
        FINALLY
        {
            explicit_bzero(&privateKeySeed, sizeof(privateKeySeed));
        }
    }
    END_TRY;
}

size_t hashAndLoadU64Ratio(uint8_t *cdata, uint8_t *dst, uint8_t sizeOfDst)
{
    uint64_t numerator   = U8BE(cdata, 0);
    uint64_t denominator = U8BE(cdata, 8);
    updateHash((cx_hash_t *) &tx_state->hash, cdata, 16);
    int numLength = numberToText(dst, sizeOfDst, numerator);
    memmove(dst + numLength, " / ", 3);
    numberToText(dst + numLength + 3, sizeOfDst - (numLength + 3), denominator);
    return 16;
}
