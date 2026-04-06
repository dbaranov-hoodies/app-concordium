#pragma once

#include <parser.h>

void handle_sign_credential_deployment(const command_t *cmd,
                                       volatile unsigned int *flags,
                                       bool isInitialCall);
