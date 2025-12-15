/*******************************************************************************
 *
 *   (c) 2016 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <os_io_legacy.h>
#include <parser.h>

#include "dispatcher.h"
#include "globals.h"

keyDerivationPath_t path;
tx_state_t global_tx_state;

const internal_storage_t N_storage_real;

void* global_state;

// Main entry of application that listens for APDU commands that will be
// received from the computer. The APDU commands control what flow is activated,
// i.e. which control flow is initiated.
void app_main() {
    // Length of APDU command received in G_io_apdu_buffer
    int input_len = 0;
    volatile unsigned int flags = 0;

    // Structured APDU command
    command_t cmd;
    io_init();
    explicit_bzero(&global_tx_state, sizeof(global_tx_state));
    ui_menu_main();

    // Initialize the NVM data if required
    if (N_storage.initialized != 0x01) {
        internal_storage_t storage;
        storage.dummy1_allowed = 0x00;
        storage.dummy2_allowed = 0x00;
        storage.initialized = 0x01;
        nvm_write((void*)&N_storage, &storage, sizeof(internal_storage_t));
    }

    for (;;) {
        // Receive command bytes in G_io_apdu_buffer
        if ((input_len = io_recv_command()) < 0) {
            return;
        }

        // Parse APDU command from G_io_apdu_buffer
        if (!apdu_parser(&cmd, G_io_apdu_buffer, input_len)) {
            io_send_sw(SW_WRONG_DATA_LENGTH);
            continue;
        }

        bool isInitialCall = false;
        if (global_tx_state.currentInstruction == -1) {
            explicit_bzero(&global, sizeof(global));
            global_tx_state.currentInstruction = cmd.ins;
            isInitialCall = true;
        }

        // Dispatch structured APDU command to apdu_dispatcher
        if (apdu_dispatcher(&cmd, &flags, isInitialCall) < 0) {
            PRINTF("=> apdu_dispatcher failure\n");
            return;
        }
    }
}
