#ifndef G_ACCOUNT_SENDER_H
#define G_ACCOUNT_SENDER_H
#include <stdint.h>
// Helper struct that is used to hold the account sender
// address from an account transaction header.
typedef struct {
    uint8_t sender[57];
} accountSender_t;
extern accountSender_t global_account_sender;

#endif  // G_ACCOUNT_SENDER_H