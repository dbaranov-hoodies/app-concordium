#include "globals.h"

#include <os.h>
#include <cx.h>
#include <io.h>
#include <parser.h>
#include <status_words.h>
#include <string.h>

#include "apdu/apdu_response.h"
#include "app_crypto.h"
#include "format.h"
#include "numberHelpers.h"
#include "export_private_key.h"
#include "derivation_path.h"

// This class allows for the export of a number of very specific private keys.
// These private keys are made exportable as they are used in computations that
// are not feasible to carry out on the Ledger device. The key derivation paths
// that are allowed are restricted so that it is not possible to export keys
// that are used for signing.
static exportPrivateKeyContext_t *ctx = &global.exportPrivateKeyContext;

void exportPrivateKeySeed(void) {
    cx_ecfp_private_key_t privateKey;
    BEGIN_TRY {
        TRY {
            derivation_path_t *dp = &global_derivation_path;
            const uint8_t lastSubPathIndex = PATH_INDEX_LEGACY_EXPORT_KEY;
            const uint8_t key_count = ctx->exportBoth ? 2U : 1U;
            uint8_t tx = 0;

            for (uint8_t k = 0; k < key_count; k++) {
                uint8_t lastSubPath = (k == 0) ? LEGACY_PRF_KEY : LEGACY_ID_CRED_SEC;
                dp->nodes[lastSubPathIndex] = lastSubPath;
                dp->len = (uint8_t) (lastSubPathIndex + 1);
                harden_derivation_path(dp);
                getPrivateKey(dp, &privateKey);
                for (int i = 0; i < KEY_LENGTH; i++) {
                    G_io_apdu_buffer[tx++] = privateKey.d[i];
                }
            }

            sendSuccess(tx);
        }
        FINALLY {
            explicit_bzero(&privateKey, sizeof(privateKey));
        }
    }
    END_TRY;
}

void exportPrivateKeyBls(void) {
    uint8_t privateKey[KEY_LENGTH];
    BEGIN_TRY {
        TRY {
            derivation_path_t *dp = &global_derivation_path;
            const uint8_t lastSubPathIndex = PATH_INDEX_LEGACY_EXPORT_KEY;
            const uint8_t key_count = ctx->exportBoth ? 2U : 1U;
            uint8_t tx = 0;

            for (uint8_t k = 0; k < key_count; k++) {
                uint8_t lastSubPath = (k == 0) ? LEGACY_PRF_KEY : LEGACY_ID_CRED_SEC;
                dp->nodes[lastSubPathIndex] = lastSubPath;
                dp->len = (uint8_t) (lastSubPathIndex + 1);
                harden_derivation_path(dp);
                getBlsPrivateKey(dp, privateKey, sizeof(privateKey));
                if (tx + sizeof(privateKey) > sizeof(G_io_apdu_buffer)) {
                    THROW(ERROR_BUFFER_OVERFLOW);
                }
                memmove(G_io_apdu_buffer + tx, privateKey, sizeof(privateKey));
                tx += sizeof(privateKey);
            }

            sendSuccess(tx);
        }
        FINALLY {
            explicit_bzero(&privateKey, sizeof(privateKey));
        }
    }
    END_TRY;
}

void exportPrivateKey(void) {
    if (ctx->exportSeed) {
        exportPrivateKeySeed();
    } else {
        exportPrivateKeyBls();
    }
}
