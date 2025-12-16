
#include "account_sender.h"
#include "instruction_context.h"
#include "key_derivation_path.h"
#include "tx_state.h"
instructionContext_t g_instructionContext;
tx_state_t           g_tx_state;

keyDerivationPath_t g_path;

accountSender_t global_account_sender;