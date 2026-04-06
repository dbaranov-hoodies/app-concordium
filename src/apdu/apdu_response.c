#include "globals.h"
#include "apdu_response.h"

#include <io.h>
#include <os.h>
#include <status_words.h>

#include "menu.h"

void sendUserRejection(void) {
    sendUserRejectionNoIdle();
    ui_menu_main();
}

void sendUserRejectionNoIdle(void) {
    G_io_apdu_buffer[0] = SWO_CONDITIONS_NOT_SATISFIED >> 8;
    G_io_apdu_buffer[1] = SWO_CONDITIONS_NOT_SATISFIED & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, ERROR_RESPONSE_LENGTH);
}

void sendSuccess(uint8_t tx) {
    G_io_apdu_buffer[tx++] = SWO_SUCCESS >> 8;
    G_io_apdu_buffer[tx++] = SWO_SUCCESS & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
    ui_menu_main();
}

void sendSuccessNoIdle(void) {
    sendSuccessResultNoIdle(0);
}

void sendSuccessResultNoIdle(uint8_t tx) {
    G_io_apdu_buffer[tx++] = SWO_SUCCESS >> 8;
    G_io_apdu_buffer[tx++] = SWO_SUCCESS & 0xFF;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
}
