#pragma once

#include "cbor_context.h"
#include "sign_transfer_context.h"
#include "sign_transfer_schedule.h"
#include "ui/register_data.h"

typedef struct {
    union {
        signTransferContext_t signTransferContext;
        signTransferWithScheduleContext_t signTransferWithScheduleContext;
        signRegisterData_t signRegisterData;
    };
    cborContext_t cborContext;
} transactionWithDataBlob_t;
