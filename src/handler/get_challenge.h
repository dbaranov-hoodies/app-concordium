#pragma once

/** Random challenge size in bytes (uint64_t) returned by GET_CHALLENGE. */
#define CHALLENGE_SIZE 8

/** GET_CHALLENGE APDU (INS 0x23): P1=0, P2=0, LC=0. */
void handle_get_challenge(void);
