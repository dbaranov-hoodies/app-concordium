#include "globals.h"

static signTransferContext_t *ctx = &global.withDataBlob.signTransferContext;
static cborContext_t *memo_ctx = &global.withDataBlob.cborContext;
static tx_state_t *tx_state = &global_tx_state;

#define P1_INITIAL           0x00
#define P1_INITIAL_WITH_MEMO 0x01
#define P1_MEMO              0x02
#define P1_AMOUNT            0x03

void handleSignTransfer(uint8_t *cdata, uint8_t lc, volatile unsigned int *flags) {
    uint8_t offset =
        handleHeaderAndToAddress(cdata, lc, TRANSFER, ctx->displayStr, sizeof(ctx->displayStr));
    cdata += offset;
    uint8_t remainingDataLength = lc - offset;

    // Build display value of the amount to transfer, and also add the bytes to the hash.
    if (remainingDataLength < 8) {
        THROW(ERROR_BUFFER_OVERFLOW);
    }
    uint64_t amount = U8BE(cdata, 0);
    amountToGtuDisplay(ctx->displayAmount, sizeof(ctx->displayAmount), amount);
    updateHash((cx_hash_t *) &tx_state->hash, cdata, 8);

    // Display the transaction information to the user (recipient address and amount to be sent).
    startTransferDisplay(false, flags);

    // Tell the main process to wait for a button press.
    *flags |= IO_ASYNCH_REPLY;
}

void finishMemo() {
    ctx->state = TX_TRANSFER_AMOUNT;
    sendSuccessNoIdle();
}

static void handleTransferHeader(uint8_t *cdata, uint8_t dataLength, bool has_memo) {
    if (ctx->state != TX_TRANSFER_INITIAL) {
        THROW(ERROR_INVALID_STATE);
    }

    uint8_t transfer_kind = has_memo ? TRANSFER_WITH_MEMO : TRANSFER;

    uint8_t remainingDataLength = dataLength;
    uint8_t offset = handleHeaderAndToAddress(cdata,
                                              remainingDataLength,
                                              transfer_kind,
                                              ctx->displayStr,
                                              sizeof(ctx->displayStr));
    cdata += offset;
    remainingDataLength -= offset;

    // if (has_schedule) {
    //     // TBD
    // }

    if (has_memo) {
        if (remainingDataLength < 2) {
            THROW(ERROR_BUFFER_OVERFLOW);
        }

        memo_ctx->cborLength = U2BE(cdata, 0);
        if (memo_ctx->cborLength > MAX_MEMO_SIZE) {
            THROW(ERROR_INVALID_PARAM);
        }

        updateHash((cx_hash_t *) &tx_state->hash, cdata, 2);
        remainingDataLength -= 2;
    }

    ctx->state = TX_TRANSFER_MEMO_INITIAL;
    sendSuccessNoIdle();
}

static void handleTransferMemo(uint8_t *cdata, uint8_t dataLength) {
    if (ctx->state != TX_TRANSFER_MEMO_INITIAL && ctx->state != TX_TRANSFER_MEMO) {
        THROW(ERROR_INVALID_STATE);
    }
    if (ctx->state == TX_TRANSFER_MEMO_INITIAL) {
        updateHash((cx_hash_t *) &tx_state->hash, cdata, dataLength);
        readCborInitial(cdata, dataLength);
        if (memo_ctx->cborLength == 0) {
            finishMemo();
        } else {
            ctx->state = TX_TRANSFER_MEMO;
            sendSuccessNoIdle();
        }
    } else if (ctx->state == TX_TRANSFER_MEMO) {
        updateHash((cx_hash_t *) &tx_state->hash, cdata, dataLength);

        readCborContent(cdata, dataLength);
        if (memo_ctx->cborLength != 0) {
            // The memo size is <=256 bytes, so we should always have received the
            // complete memo by this point
            THROW(ERROR_INVALID_STATE);
        }

        finishMemo();
    }
}

/// @brief  Defines state transitions while for handleSignTransferUniversal
/// with optional MEMO and SCHEDULE fields
/// HEADER ----> MEMO? -------> SCHEDULE? --> AMOUNT --> FEES --> FINAL
/// @param block_type
static void update_state(TransferBlockType block_type, uint8_t p2) {
    bool has_memo = (p2 & P2_HAS_MEMO) != 0;
    bool has_schedule = (p2 & P2_HAS_SCHEDULE) != 0;
    switch (block_type) {
        case P1_IS_HEADER:
            if (has_memo) {
                ctx->state = TX_TRANSFER_MEMO;
            } else {
                if (has_schedule) {
                    ctx->state = TX_TRANSFER_SCHEDULE;
                } else {
                    ;
                }
            }
            break;
        case P1_IS_MEMO:
            if (ctx->state == TX_TRANSFER_MEMO_INITIAL) {
                ctx->state = TX_TRANSFER_MEMO;
            } else {
                if (has_schedule) {
                    ctx->state = TX_TRANSFER_SCHEDULE;
                } else {
                    ctx->state = TX_TRANSFER_AMOUNT;
                }
            }
            break;
        case P1_IS_SCHEDULE:
            ctx->state = TX_TRANSFER_AMOUNT;
            break;

        case P1_IS_AMOUNT:
            ctx->state = TX_TRANSFER_FEES;
            break;
        case P1_IS_FEES:
            ctx->state = TX_FINAL;
            break;
        default:
            break;
    }
}

void handleSignTransferUniversal(uint8_t *cdata,
                                 uint8_t p1,
                                 uint8_t p2,
                                 uint8_t lc,
                                 bool isInitialCall,
                                 volatile unsigned int *flags) {
    if (isInitialCall) {
        ctx->state = TX_TRANSFER_INITIAL;
    }
    uint8_t dataLength = lc;

    TransferBlockType block_type = (TransferBlockType) p1;

    switch (block_type) {
        case P1_IS_HEADER:
            handleTransferHeader(cdata, lc, p2);
            update_state(block_type, p2);
            break;

        case P1_IS_MEMO:
            handleTransferMemo(cdata, lc);
            update_state(block_type, p2);
            break;

        case P1_IS_SCHEDULE:
            // not supported yet
            THROW(ERROR_INVALID_PARAM);

            break;
        case P1_IS_AMOUNT:
            // Build display value of the amount to transfer, and also add the bytes to the
            // hash.
            if (dataLength < 8) {
                THROW(ERROR_BUFFER_OVERFLOW);
            }
            uint64_t amount = U8BE(cdata, 0);
            amountToGtuDisplay(ctx->displayAmount, sizeof(ctx->displayAmount), amount);
            updateHash((cx_hash_t *) &tx_state->hash, cdata, 8);
            update_state(block_type, p2);
            break;

        case P1_IS_FEES:
            // Build display value of the energy to transfer, and also add the bytes to the
            // hash.
            if (dataLength < 8) {
                THROW(ERROR_BUFFER_OVERFLOW);
            }
            uint64_t fees = U8BE(cdata, 0);
            amountToGtuDisplay(ctx->displayFees, sizeof(ctx->displayFees), fees);
            updateHash((cx_hash_t *) &tx_state->hash, cdata, 8);
            update_state(block_type, p2);
            break;

        case P1_IS_FINAL:
            startTransferDisplay(true, flags);
            break;
    }
}

void handleSignTransferWithMemo(uint8_t *cdata,
                                uint8_t p1,
                                uint8_t dataLength,
                                volatile unsigned int *flags,
                                bool isInitialCall) {
    if (isInitialCall) {
        ctx->state = TX_TRANSFER_INITIAL;
    }
    uint8_t remainingDataLength = dataLength;
    if (p1 == P1_INITIAL_WITH_MEMO && ctx->state == TX_TRANSFER_INITIAL) {
        handleTransferHeader(cdata, dataLength, true);
    } else if (p1 == P1_MEMO) {
        handleTransferMemo(cdata, dataLength);
    } else if (p1 == P1_AMOUNT && ctx->state == TX_TRANSFER_AMOUNT) {
        // Build display value of the amount to transfer, and also add the bytes to the
        // hash.
        if (remainingDataLength < 8) {
            THROW(ERROR_BUFFER_OVERFLOW);
        }
        uint64_t amount = U8BE(cdata, 0);
        amountToGtuDisplay(ctx->displayAmount, sizeof(ctx->displayAmount), amount);
        updateHash((cx_hash_t *) &tx_state->hash, cdata, 8);
        startTransferDisplay(true, flags);
    } else {
        THROW(ERROR_INVALID_STATE);
    }
}
