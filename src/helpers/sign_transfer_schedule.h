#pragma once

#include <parser.h>

#include "time.h"

/**
 * Scheduled-transfer-with-schedule context (lives in `instructionContext`) and parsing helpers.
 * Used by `handler/sign_transfer_with_schedule.c` and `sign_transfer_with_schedule_and_memo.c`.
 */

typedef enum {
    TX_TRANSFER_WITH_SCHEDULE_INITIAL = 28,
    TX_TRANSFER_WITH_SCHEDULE_TRANSFER_PAIRS = 29,
    TX_TRANSFER_WITH_SCHEDULE_MEMO_START = 55,
    TX_TRANSFER_WITH_SCHEDULE_MEMO = 56,
} transferWithScheduleState_t;

typedef struct {
    uint8_t transactionType;
    transferWithScheduleState_t state;

    unsigned char displayStr[57];
    uint8_t remainingNumberOfScheduledAmounts;
    uint8_t scheduledAmountsInCurrentPacket;

    uint8_t displayAmount[30];
    uint8_t energy_amount_str[30];
    uint8_t displayTimestamp[25];

    tm time;

    // Buffer to hold the incoming databuffer so that we can iterate over it.
    uint8_t buffer[255];
    uint8_t pos;
} signTransferWithScheduleContext_t;

void processNextScheduledAmount(uint8_t *buffer);
void handle_transfer_pairs(uint8_t *cdata, uint8_t dataLength, volatile unsigned int *flags);
void finish_memo_scheduled(volatile unsigned int *flags);
