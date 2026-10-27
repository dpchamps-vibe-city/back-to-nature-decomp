#ifndef OVL11_RECORD_STATUS_MESSAGE_H
#define OVL11_RECORD_STATUS_MESSAGE_H

#include "common.h"
#include "game_types.h"

/* Publish record+4 in text substitution slot 0, and a shared two-position
 * rendering of 21-u16@+B2 (plus FFFF) in slot 1. Then request archive-0 text
 * selected by u16@+AE, u16@+B2 and signed-halfword@+16 threshold bands.
 * Fields are deliberately neutral: their meanings depend on record kind.
 *
 * record must remain readable through the message's lifetime, including its
 * inline text; the substitution slots/scratch remain installed afterwards.
 * Non-reentrant: another text request can overwrite both. Count is unchecked
 * (values >21 take the formatter's minus-glyph path). No record is modified,
 * no message is serviced/waited for, and the request's result is discarded.
 * The original narrow pointer view is retained, but the text offset is BYTES.
 */
void Ovl11RequestRecordStatusMessage(UnkStruct800DF4F0 *record);

#endif
