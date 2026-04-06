#include "globals.h"

#include <string.h>

#include <os.h>
#include <cx.h>
#include <io.h>
#include <status_words.h>

#include "apdu/apdu_response.h"
#include "app_crypto.h"
#include "app_sizes.h"
#include "getPublicKey.h"
#include "numberHelpers.h"
#include "display.h"

static exportPublicKeyContext_t *ctx = &global.exportPublicKeyContext;
static tx_state_t *tx_state = &global_tx_state;

/**
 * Derive the public-key for the given path, and then write it to
 * the APDU buffer to be returned to the caller.
 */
void sendPublicKey(bool compare) {
    uint8_t publicKey[KEY_LENGTH];
    getPublicKey(publicKey);

    // tx is holding the offset in the buffer we have written to. It is a convention to call this tx
    // for Ledger apps.
    uint8_t tx = 0;

    // Write the public-key to the APDU buffer.
    for (uint8_t i = 0; i < sizeof(publicKey); i++) {
        G_io_apdu_buffer[i] = publicKey[i];
        tx++;
    }

    if (ctx->signPublicKey) {
        uint8_t signedPublicKey[ED25519_SIGNATURE_LENGTH];
        sign(publicKey, signedPublicKey);
        if (sizeof(signedPublicKey) > sizeof(G_io_apdu_buffer) - tx) {
            THROW(ERROR_BUFFER_OVERFLOW);
        }
        memmove(G_io_apdu_buffer + tx, signedPublicKey, sizeof(signedPublicKey));
        tx += sizeof(signedPublicKey);
    }

    // Send back success response including the public-key (and signature, if wanted).
    if (compare) {
        // Show the public-key so that the user can verify the public-key.
        send_success_result_no_idle(tx);
        toPaginatedHex(publicKey, sizeof(publicKey), ctx->publicKey, sizeof(ctx->publicKey));
        // Allow for receiving a new instruction even while comparing public keys.
        tx_state->currentInstruction = INSTRUCTION_NONE;
        uiComparePubkey();

    } else {
        send_success(tx);
    }
}
