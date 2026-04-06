#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TX_TRANSFER_INITIAL = 49,
    TX_TRANSFER_MEMO_INITIAL = 50,
    TX_TRANSFER_MEMO = 51,
    TX_TRANSFER_AMOUNT = 52
} simpleTransferState_t;

typedef struct {
    unsigned char displayStr[57];
    uint8_t displayAmount[30];
    uint8_t energy_amount_str[30];
    simpleTransferState_t state;
} signTransferContext_t;
