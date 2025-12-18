#ifndef G_TX_STATE_H
#define G_TX_STATE_H

#include <stdint.h>

#include <lcx_sha256.h>
// Helper object used when computing the hash of a transaction,
// and to keep track of the state of a multi command APDU flow.
typedef struct {
    cx_sha256_t hash;
    uint8_t     transactionHash[32];
    int         currentInstruction;
} tx_state_t;
extern tx_state_t g_tx_state;

#endif  // G_TX_STATE_H