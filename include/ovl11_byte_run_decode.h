#ifndef OVL11_BYTE_RUN_DECODE_H
#define OVL11_BYTE_RUN_DECODE_H

#include "common.h"

/* Decode recordCount records after encoded[0], the delimiter byte.
 * A non-delimiter record emits that byte; a delimiter record consumes two
 * more bytes (value, unsigned repeat count) and emits that many copies.
 * A zero repeat count emits nothing. recordCount counts records, NOT input
 * bytes or output bytes. No terminator is written and no length is returned.
 * The delimiter is read even for a nonpositive recordCount. Supply valid
 * input and enough destination storage for the expanded data; neither bound
 * is checked. Overlapping buffers are not supported by the calling contract.
 */
void Ovl11DecodeByteRuns(u8 *destination, u8 *encoded, s32 recordCount);

#endif
