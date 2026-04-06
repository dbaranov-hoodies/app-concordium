#pragma once

#include <parser.h>
#include <stdbool.h>
#include <stdint.h>

#include "app_sizes.h"

typedef enum {
    TX_PUBLIC_INFO_FOR_IP_INITIAL = 22,
    TX_PUBLIC_INFO_FOR_IP_VERIFICATION_KEY = 23,
    TX_PUBLIC_INFO_FOR_IP_THRESHOLD = 24
} publicInfoForIpState_t;

typedef struct {
    bool showIntro;
    uint8_t publicKeysLength;
    char publicKey[68];
    uint8_t threshold[COMMON_THRESHOLD_SIZE];
    char idCredPub[48 * 2 + 1];
    char credId[48 * 2 + 1];

    char keyType[2 + 1];
    publicInfoForIpState_t state;
} signPublicInformationForIp_t;

void handle_sign_public_information_for_ip(const command_t *cmd,
                                           volatile unsigned int *flags,
                                           bool isInitialCall);
