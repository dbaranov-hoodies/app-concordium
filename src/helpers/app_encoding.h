#pragma once

#include <parser.h>

/** Max decimal digits for a full-range uint64 (including '\0' sizing in fixed buffers). */
#define UINT64_MAX_DECIMAL_DIGITS 20

/**
 * Read uint64_t big-endian from buffer at offset (uses U4BE from the SDK parser).
 */
#define U8BE(buf, off) \
    (((uint64_t) (U4BE(buf, off)) << 32) | ((uint64_t) (U4BE(buf, off + 4)) & 0xFFFFFFFF))
