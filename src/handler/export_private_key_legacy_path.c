#include "globals.h"

#include <os.h>
#include <cx.h>
#include <io.h>
#include <parser.h>
#include <status_words.h>

#include "app_encoding.h"
#include "derivation_path.h"
#include "numberHelpers.h"

#include "export_private_key.h"
#include "export_private_key_legacy_path.h"

static exportPrivateKeyContext_t *ctx = &global.exportPrivateKeyContext;

void handle_export_private_key_legacy_path(const command_t *cmd, volatile unsigned int *flags) {
    uint8_t *dataBuffer = cmd->data;
    uint8_t p1 = cmd->p1;
    uint8_t p2 = cmd->p2;
    uint8_t lc = cmd->lc;

    if ((p1 != P1_LEGACY_PRF_KEY_AND_ID_CRED_SEC && p1 != P1_LEGACY_PRF_KEY &&
         p1 != P1_LEGACY_PRF_KEY_RECOVERY) ||
        (p2 != P2_LEGACY_KEY && p2 != P2_LEGACY_SEED)) {
        THROW(ERROR_INVALID_PARAM);
    }

    ctx->isNewPath = false;
    uint32_t identity;
    if (lc < 4) {
        THROW(ERROR_INVALID_PATH);
    }
    identity = U4BE(dataBuffer, 0);

    derivation_path_t *dp = &global_derivation_path;
    init_derivation_path(dp);
    dp->len = 5;
    dp->nodes[0] = LEGACY_PURPOSE;
    dp->nodes[1] = LEGACY_COIN_TYPE;
    dp->nodes[2] = ACCOUNT_SUBTREE;
    dp->nodes[3] = NORMAL_ACCOUNTS;
    dp->nodes[4] = identity;
    harden_derivation_path(dp);

    ctx->exportBoth = p1 == P1_LEGACY_PRF_KEY_AND_ID_CRED_SEC;
    ctx->exportSeed = p2 == P2_LEGACY_SEED;

    size_t offset = 0;
    memmove(ctx->display_credid + offset, "ID#", 3);
    offset += 3;
    bin_to_dec(ctx->display_credid + offset, sizeof(ctx->display_credid) - offset, identity);

    memmove(ctx->display_credid_title, "Credentials ID", EXPORT_PRIVATE_KEY_CREDID_TITLE_LEN);
    memmove(ctx->display_review_operation,
            "Review operation",
            EXPORT_PRIVATE_KEY_REVIEW_OPERATION_LEN);
    memmove(ctx->display_sign, "Sign operation", EXPORT_PRIVATE_KEY_SIGN_OPERATION_LEN);

    switch (p1) {
        case P1_LEGACY_PRF_KEY_AND_ID_CRED_SEC:
            memmove(ctx->display_sign_verb,
                    "to create credentials?",
                    EXPORT_PRIVATE_KEY_SIGN_VERB_LEN);
            memmove(ctx->display_review_verb,
                    "to create credentials",
                    EXPORT_PRIVATE_KEY_REVIEW_VERB_LEN);
            break;
        case P1_LEGACY_PRF_KEY_RECOVERY:
            memmove(ctx->display_sign_verb,
                    "to recover credentials?",
                    EXPORT_PRIVATE_KEY_SIGN_VERB_LEN);
            memmove(ctx->display_review_verb,
                    "to recover credentials",
                    EXPORT_PRIVATE_KEY_REVIEW_VERB_LEN);
            break;
        case P1_LEGACY_PRF_KEY:
            memmove(ctx->display_sign_verb,
                    "to decrypt credentials?",
                    EXPORT_PRIVATE_KEY_SIGN_VERB_LEN);
            memmove(ctx->display_review_verb,
                    "to decrypt credentials",
                    EXPORT_PRIVATE_KEY_REVIEW_VERB_LEN);
            break;
        default:
            THROW(SWO_INCORRECT_P1_P2);
    }
    uiExportPrivateKey(flags);
}
