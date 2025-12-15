#include <stdint.h>
#include <stddef.h>

// ================= cx_bn_* =================
typedef struct { int dummy; } cx_bn_t;
typedef struct { int dummy; } cx_ecpoint_t;

cx_bn_t* cx_bn_alloc(void) { return (cx_bn_t*)1; }
cx_bn_t* cx_bn_alloc_init(void* src, size_t size) { return (cx_bn_t*)1; }
void cx_bn_set_u32(cx_bn_t* bn, uint32_t val) {}
void cx_bn_mod_add(cx_bn_t* r, cx_bn_t* a, cx_bn_t* b, cx_bn_t* m) {}
void cx_bn_mod_invert_nprime(cx_bn_t* r, cx_bn_t* m) {}
void cx_bn_destroy(cx_bn_t* bn) {}
void cx_bn_unlock(cx_bn_t* bn) {}
void cx_bn_lock(cx_bn_t* bn) {}
void cx_bn_export(cx_bn_t* bn, uint8_t* out) {}
int cx_bn_cmp(cx_bn_t* a, cx_bn_t* b) { return 0; }

// ================= cx_ecpoint_* =================
cx_ecpoint_t* cx_ecpoint_alloc(void) { return (cx_ecpoint_t*)1; }
void cx_ecpoint_init(cx_ecpoint_t* p, void* curve, void* pt) {}
void cx_ecpoint_scalarmul_bn(cx_ecpoint_t* p, cx_bn_t* b) {}
void cx_ecpoint_export_bn(cx_ecpoint_t* p, cx_bn_t* b) {}
void cx_ecpoint_neg(cx_ecpoint_t* p) {}
void cx_ecpoint_destroy(cx_ecpoint_t* p) {}

// ================= cx_hash =================
void cx_hash_sha256(void* hash, int mode, const unsigned char* data, size_t len, unsigned char* out) {}

// ================= os_longjmp / try_context =================
void os_longjmp(void* ctx, int val) {}
void try_context_set(void* ctx) {}
void* try_context_get(void) { return NULL; }

// ================= BLS / wallet =================
void* getBlsPrivateKey(void) { return NULL; }

// ================= UI / display =================
void getIdentityAccountDisplayNewPath(...) {}
void getIdentityAccountDisplay(...) {}
void uiVerifyAddress(...) {}

// ================= Base58 / utils =================
uint32_t base58check_encode(const uint8_t* in, uint32_t len, char* out, uint32_t out_len) { return 0; }

// ================= Global state =================
struct global_t { int dummy; } global;
