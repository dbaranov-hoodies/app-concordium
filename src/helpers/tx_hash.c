#include "globals.h"

#include <string.h>

#include <os.h>
#include <cx.h>
#include <io.h>
#include <parser.h>
#include <status_words.h>

#include "app_crypto.h"
#include "base58check.h"
#include "numberHelpers.h"

#include "tx_hash.h"

static tx_state_t *tx_state = &global_tx_state;
static accountSender_t *accountSender = &global_account_sender;

/**
 * Generic method for hashing and validating header and type for a transaction.
 * Use hashAccountTransactionHeaderAndKind or hashUpdateHeaderAndType
 * instead of using this method directly.
 */
static int hashHeaderAndType(uint8_t *cdata,
                             uint8_t dataLength,
                             uint8_t headerLength,
                             uint8_t validType) {
    if (dataLength < headerLength + 1) {
        PRINTF("Issue with length\n");
        THROW(ERROR_INVALID_TRANSACTION);
    }
    updateHash((cx_hash_t *) &tx_state->hash, cdata, headerLength);
    cdata += headerLength;

    uint8_t type = cdata[0];
    if (type != validType) {
        PRINTF("Received kind is different than the expected one\n");
        THROW(ERROR_INVALID_TRANSACTION);
    }
    updateHash((cx_hash_t *) &tx_state->hash, cdata, 1);

    return headerLength + 1;
}

int hashAccountTransactionHeaderAndKind(uint8_t *cdata,
                                        uint8_t dataLength,
                                        uint8_t validTransactionKind) {
    size_t outputSize = sizeof(accountSender->sender);
    if (base58check_encode(cdata, ADDRESS_LENGTH, accountSender->sender, &outputSize) == -1) {
        PRINTF("The received address bytes are not valid base85 encoded\n");
        THROW(ERROR_INVALID_TRANSACTION);
    }
    accountSender->sender[BASE58_ADDRESS_LENGTH] = '\0';

    return hashHeaderAndType(cdata,
                             dataLength,
                             ACCOUNT_TRANSACTION_HEADER_LENGTH,
                             validTransactionKind);
}

int hashUpdateHeaderAndType(uint8_t *cdata, uint8_t dataLength, uint8_t validUpdateType) {
    return hashHeaderAndType(cdata, dataLength, UPDATE_HEADER_LENGTH, validUpdateType);
}

int handleHeaderAndToAddress(uint8_t *cdata,
                             uint8_t dataLength,
                             uint8_t kind,
                             uint8_t *recipientDst,
                             size_t recipientSize,
                             uint8_t *feesDst,
                             size_t feesSize) {
    size_t keyPathLength = parse_derivation_path(cdata, dataLength);
    cdata += keyPathLength;
    uint8_t remainingDataLength = dataLength - keyPathLength;

    if (cx_sha256_init(&tx_state->hash) != CX_SHA256) {
        THROW(ERROR_FAILED_CX_OPERATION);
    }
    int headerLength = hashAccountTransactionHeaderAndKind(cdata, remainingDataLength, kind);

    uint64_t energy_amount_u64 = U8BE(cdata, ENERGY_OFFSET_IN_HEADER);

    amount_to_gtu_display((uint8_t *) feesDst, feesSize, energy_amount_u64);

    cdata += headerLength;
    remainingDataLength -= headerLength;

    uint8_t toAddress[ADDRESS_LENGTH];
    if (remainingDataLength < ADDRESS_LENGTH) {
        THROW(ERROR_INVALID_TRANSACTION);
    }
    memmove(toAddress, cdata, ADDRESS_LENGTH);
    updateHash((cx_hash_t *) &tx_state->hash, toAddress, ADDRESS_LENGTH);

    if (base58check_encode(toAddress, sizeof(toAddress), recipientDst, &recipientSize) == -1) {
        THROW(ERROR_INVALID_TRANSACTION);
    }
    recipientDst[BASE58_ADDRESS_LENGTH] = '\0';
    return keyPathLength + headerLength + ADDRESS_LENGTH;
}

#define U64_RATIO_SEPARATOR     " / "
#define U64_RATIO_SEPARATOR_LEN 3

size_t hashAndLoadU64Ratio(uint8_t *cdata, uint8_t *dst, uint8_t sizeOfDst) {
    uint64_t numerator = U8BE(cdata, 0);
    uint64_t denominator = U8BE(cdata, 8);
    updateHash((cx_hash_t *) &tx_state->hash, cdata, U64_RATIO_BYTES);
    int numLength = number_to_text(dst, sizeOfDst, numerator);
    memmove(dst + numLength, U64_RATIO_SEPARATOR, U64_RATIO_SEPARATOR_LEN);
    number_to_text(dst + numLength + U64_RATIO_SEPARATOR_LEN,
                   sizeOfDst - (numLength + U64_RATIO_SEPARATOR_LEN),
                   denominator);
    return U64_RATIO_BYTES;
}
