#pragma once

#include <parser.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool stopDelegation;
    uint8_t displayCapital[30];
    uint8_t displayRestake[4];
    uint8_t displayDelegationTarget[30];
    bool hasCapital;
    bool hasRestakeEarnings;
    bool hasDelegationTarget;
} signConfigureDelegationContext_t;

void handle_sign_configure_delegation(const command_t *cmd, volatile unsigned int *flags);
