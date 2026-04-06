#pragma once

#include <parser.h>
#include <stdint.h>

#include "app_sizes.h"

typedef struct {
    uint32_t version;
    uint32_t sourceLength;
    uint32_t remainingSourceLength;
    uint8_t sourceHash[COMMON_HASH_SIZE];
    char sourceHashDisplay[COMMON_HASH_SIZE * 2 + 1];
    char versionDisplay[11];
} deployModule_t;

void handle_deploy_module(const command_t *cmd);
