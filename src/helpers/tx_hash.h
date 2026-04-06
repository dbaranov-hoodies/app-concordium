#pragma once

#include <stddef.h>
#include <stdint.h>

/** Byte offset of energy field in Concordium account transaction header. */
#define ENERGY_OFFSET_IN_HEADER 40

/** Serialized u64 numerator + u64 denominator (e.g. commission rate). */
#define U64_RATIO_BYTES 16

int hashAccountTransactionHeaderAndKind(uint8_t *cdata,
                                        uint8_t dataLength,
                                        uint8_t validTransactionKind);

int hashUpdateHeaderAndType(uint8_t *cdata, uint8_t dataLength, uint8_t validUpdateType);

int handleHeaderAndToAddress(uint8_t *cdata,
                             uint8_t dataLength,
                             uint8_t kind,
                             uint8_t *recipientDst,
                             size_t recipientSize,
                             uint8_t *feesDst,
                             size_t feesSize);

size_t hashAndLoadU64Ratio(uint8_t *cdata, uint8_t *dst, uint8_t sizeOfDst);
