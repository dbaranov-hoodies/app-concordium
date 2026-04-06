#pragma once

#include <parser.h>

/**
 * INS_GET_PUBLIC_KEY: derive path from CDATA, optional display, then export via UI / APDU.
 * See /doc/ins_public_key.md; P1/P2 in get_public_key.h.
 */
void handle_get_public_key(const command_t *cmd, volatile unsigned int *flags);
