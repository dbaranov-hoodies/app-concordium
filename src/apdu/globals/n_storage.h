#ifndef G_N_STORAGE_H
#define G_N_STORAGE_H

#include <stdint.h>
#include <os_pic.h>

/**
 * Global structure for NVM data storage.
 */
typedef struct internal_storage_t {
    uint8_t dummy1_allowed;
    uint8_t dummy2_allowed;
    uint8_t initialized;
} internal_storage_t;

extern const internal_storage_t N_storage_real;
#define N_storage (*(volatile internal_storage_t *)PIC(&N_storage_real))

#endif  // G_N_STORAGE_H