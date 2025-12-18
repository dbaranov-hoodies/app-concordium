#ifndef G_INTERNAL_STORAGE_H
#define G_INTERNAL_STORAGE_H

#include <stdint.h>

#define N_storage (*(volatile internal_storage_t *) PIC(&N_storage_real))

typedef struct internal_storage_t {
    uint8_t dummy1_allowed;
    uint8_t dummy2_allowed;
    uint8_t initialized;
} internal_storage_t;

extern const internal_storage_t N_storage_real;

#endif  // G_INTERNAL_STORAGE_H