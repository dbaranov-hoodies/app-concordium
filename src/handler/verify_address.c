#include "globals.h"

#include <string.h>

#include <os.h>
#include <cx.h>
#include <io.h>
#include <parser.h>
#include <status_words.h>

#include "app_crypto.h"
#include "app_sizes.h"
#include "base58check.h"

#include "verify_address.h"

static verifyAddressContext_t *ctx = &global.verifyAddressContext;

void handle_verify_address(const command_t *cmd, volatile unsigned int *flags) {
    uint8_t *cdata = cmd->data;
    uint8_t p1 = cmd->p1;
    uint8_t p2 = cmd->p2;
    uint8_t lc = cmd->lc;

    derivation_path_t *derivation_path = &global_derivation_path;
    init_derivation_path(derivation_path);
    uint32_t cred_counter = 0;
    verify_address_parse_key_path(cdata, p1, p2, lc, derivation_path, &cred_counter);

    switch (derivation_path->variant) {
        case DERIVATION_PATH_VARIANT_NEW:
            getIdentityAccountDisplayNewPath(ctx->display,
                                             sizeof(ctx->display),
                                             derivation_path->nodes[2],
                                             derivation_path->nodes[3],
                                             cred_counter);
            break;

        case DERIVATION_PATH_VARIANT_LEGACY:
            getIdentityAccountDisplayLegacyPath(ctx->display,
                                                sizeof(ctx->display),
                                                derivation_path->nodes[4],
                                                cred_counter);
            break;

        case DERIVATION_PATH_VARIANT_FULL:
            unharden_derivation_path(derivation_path);
            getIdentityAccountDisplayNewPath(ctx->display,
                                             sizeof(ctx->display),
                                             derivation_path->nodes[2],
                                             derivation_path->nodes[3],
                                             derivation_path->nodes[4]);

            break;

        default:
            break;
    }

    uint8_t credId[BLS_G1_COORD_SIZE];
    uint8_t prf[KEY_LENGTH];

    /* getBlsPrivateKey expects hardened path (0x80000000 | node); parsers output unhardened */
    harden_derivation_path(derivation_path);

    BEGIN_TRY {
        TRY {
            getBlsPrivateKey(derivation_path, prf, sizeof(prf));
            cx_err_t error = getCredId(prf, sizeof(prf), cred_counter, credId, sizeof(credId));

            if (error != 0) {
                THROW(ERROR_INVALID_STATE);
            }
        }
        FINALLY {
            explicit_bzero(prf, sizeof(prf));
        }
    }
    END_TRY;

    uint8_t accountAddress[ADDRESS_LENGTH];
    size_t hash_size = 0;
    hash_size = cx_hash_sha256(credId, sizeof(credId), accountAddress, sizeof(accountAddress));
    if (hash_size != CX_SHA256_SIZE) {
        THROW(ERROR_FAILED_CX_OPERATION);
    }
    size_t addressLength = sizeof(ctx->address);

    base58check_encode(accountAddress, sizeof(accountAddress), ctx->address, &addressLength);
    ctx->address[BASE58_ADDRESS_LENGTH] = '\0';

    uiVerifyAddress(flags);
}
