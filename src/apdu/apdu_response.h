#pragma once

#include <stdint.h>

/** APDU response body length for bare status words (e.g. rejection). */
#define ERROR_RESPONSE_LENGTH 2

/**
 * Send a user rejection back to the caller (SWO_CONDITIONS_NOT_SATISFIED), then return to main menu.
 */
void sendUserRejection(void);

/**
 * Same as sendUserRejection but does not return to the menu.
 */
void sendUserRejectionNoIdle(void);

/**
 * Send success with empty payload, no idle menu.
 */
void sendSuccessNoIdle(void);

/**
 * Send success with tx bytes already in G_io_apdu_buffer, no idle menu.
 */
void sendSuccessResultNoIdle(uint8_t tx);

/**
 * Send success and return to main menu; tx is length of response body (status words appended).
 */
void sendSuccess(uint8_t tx);
