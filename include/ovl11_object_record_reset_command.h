#ifndef OVL11_OBJECT_RECORD_RESET_COMMAND_H
#define OVL11_OBJECT_RECORD_RESET_COMMAND_H

#include "common.h"

/* ovl_11:0x800E7504. Reset each supplied ID in 1..36; ignore others.
 * For each 0x1D4-byte object record: copy words +30..3C to +60..6C, clear
 * +70/+74/+78 and byte +B4, set +1E bit 1000, clear +20 bits C000, and reset
 * the timed sprite effect at +E0. Duplicate IDs are processed repeatedly.
 * If firstId == 42, ignore the other arguments and reset ALL IDs 1..36.
 * If firstId == 45, only set work +4450 bit 2; do not reset any record.
 * Other arguments are raw IDs, not value-slot indices. Always returns 1.
 */
s32 Ovl11ResetObjectRecordCommand(s16 firstId, s16 secondId,
                                 s32 thirdId, s32 fourthId);

#endif
