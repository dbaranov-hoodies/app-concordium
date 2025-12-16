#ifndef G_DERIVATION_PATH_KEYS_H
#define G_DERIVATION_PATH_KEYS_H
typedef enum {
    LEGACY_ID_CRED_SEC = 0,
    LEGACY_PRF_KEY = 1,
    // New path
    NEW_ID_CRED_SEC = 2,
    NEW_PRF_KEY = 3
} derivation_path_keys_t;

#endif  // G_DERIVATION_PATH_KEYS_H