#pragma once

#include <parser.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TX_TRANSFER_TO_PUBLIC_INITIAL = 25,
    TX_TRANSFER_TO_PUBLIC_REMAINING_AMOUNT = 26,
    TX_TRANSFER_TO_PUBLIC_PROOF = 27
} transferToPublicState_t;

typedef struct {
    uint8_t amount[30];
    uint8_t recipientAddress[57];
    uint16_t proofSize;
    transferToPublicState_t state;
} signTransferToPublic_t;

void handle_sign_transfer_to_public(const command_t *cmd,
                                    volatile unsigned int *flags,
                                    bool isInitialCall);
