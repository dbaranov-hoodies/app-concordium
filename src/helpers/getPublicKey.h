#pragma once

#include <parser.h>

#define P1_SKIP_DISPLAY    0x01
#define P2_SIGN_PUBLIC_KEY 0x01

/**
 * P1: 0x00 = show export on device; P1_SKIP_DISPLAY = silent export.
 * P2: 0x00 = public key only; P2_SIGN_PUBLIC_KEY = append signature on the key.
 * INS handler: handle_get_public_key() in get_public_key.c.
 */
void sendPublicKey(bool compare);

typedef struct {
    uint8_t display[21];
    char publicKey[68];
    bool signPublicKey;
} exportPublicKeyContext_t;
