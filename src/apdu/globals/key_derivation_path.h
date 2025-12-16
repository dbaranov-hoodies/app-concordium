#ifndef G_KEY_DERIVATION_PATH_H
#define G_KEY_DERIVATION_PATH_H

#include "stdint.h"

typedef struct {
    uint8_t identity;
    uint8_t accountIndex;

    // Max length of path is 8. Currently we expect to receive the root, i.e. purpose and coin type
    // as well. This could be refactored into having those values hardcoded if we determine they
    // will be static.
    uint8_t pathLength;
    uint32_t keyDerivationPath[8];
    uint32_t rawKeyDerivationPath[8];
} keyDerivationPath_t;

extern keyDerivationPath_t g_path;

#endif  // G_KEY_DERIVATION_PATH_H