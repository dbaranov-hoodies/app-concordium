#ifndef SIGN_TRANSFER_H
#define SIGN_TRANSFER_H

typedef enum : uint8_t {
    P1_IS_HEADER = 0x00,
    P1_IS_MEMO = 0x20,
    P1_IS_SCHEDULE = 0x30,
    P1_IS_AMOUNT = 0x40,
    P1_IS_FEES = 0x41,
    P1_IS_FINAL = 0x80
} TransferBlockType;

typedef enum {
    P2_HAS_MEMO = 1 << 0,     // 0x01
    P2_HAS_SCHEDULE = 1 << 1  // 0x02
} TransferFlags;

/**
 * Handles the signing flow, including updating the display, for the 'simple transfer'
 * account transaction.
 * @param cdata please see /doc/ins_transfer.md for details
 */
void handleSignTransfer(uint8_t *cdata, uint8_t lc, volatile unsigned int *flags);

/**
 * Handles the signing flow, including updating the display, for the 'simple transfer with memo'
 * account transaction.
 * @param cdata please see /doc/ins_transfer.md for details
 */
void handleSignTransferWithMemo(uint8_t *cdata,
                                uint8_t p1,
                                uint8_t dataLength,
                                volatile unsigned int *flags,
                                bool isInitialCall);

/**
 * Handles the signing flow, including updating the display, for the transfers with or without
 * memo/schedule account transaction.
 * @param cdata please see /doc/ins_transfer.md for details
 * Implements a state machine with the following state diagram
 * TX_TRANSFER_INITIAL
 */
void handleSignTransferUniversal(uint8_t *cdata,
                                 uint8_t p1,
                                 uint8_t p2,
                                 uint8_t dataLength,
                                 bool isInitialCall,
                                 volatile unsigned int *flags);

typedef enum {
    TX_TRANSFER_INITIAL = 49,
    TX_TRANSFER_MEMO_INITIAL = 50,
    TX_TRANSFER_MEMO = 51,
    TX_TRANSFER_AMOUNT = 52,
    TX_TRANSFER_FEES = 53,

    TX_TRANSFER_SCHEDULE = 28,
    TX_FINAL = 100,
} simpleTransferState_t;

typedef struct {
    unsigned char displayStr[57];
    uint8_t displayFees[30];
    uint8_t displayAmount[30];
    simpleTransferState_t state;
} signTransferContext_t;

#endif  // SIGN_TRANSFER_H