#pragma once

#include <parser.h>
#include <stdint.h>

#include "app_sizes.h"

typedef enum {
    UPDATE_CONTRACT_INITIAL = 60,
    UPDATE_CONTRACT_NAME_FIRST = 61,
    UPDATE_CONTRACT_NAME_NEXT = 62,
    UPDATE_CONTRACT_PARAMS_FIRST = 63,
    UPDATE_CONTRACT_PARAMS_NEXT = 64,
    UPDATE_CONTRACT_END = 65
} updateContractState_t;

typedef struct {
    uint64_t amount;
    uint8_t moduleRef[COMMON_MODULE_REF_SIZE];
    char amountDisplay[30];
    char indexDisplay[30];
    char subIndexDisplay[30];
    uint32_t nameLength;
    uint32_t remainingNameLength;
    uint32_t paramsLength;
    uint32_t remainingParamsLength;
    updateContractState_t state;
} updateContract_t;

void handle_update_contract(const command_t *cmd);
