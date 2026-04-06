#pragma once

#include <stdint.h>

#include "app_sizes.h"

typedef struct {
    uint32_t cborLength;
    uint32_t displayUsed;
    uint8_t display[COMMON_DISPLAY_SIZE];
    uint8_t majorType;
} cborContext_t;
