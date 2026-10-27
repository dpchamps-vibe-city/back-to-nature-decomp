#ifndef OVL11_OBJECT_RECORD_RESET_SLOTS_COMMAND_H
#define OVL11_OBJECT_RECORD_RESET_SLOTS_COMMAND_H

#include "common.h"

/* ovl_11 adapter: resolve each non--1 argument as a value-bank index; -1
 * passes through without reading. The first two indices and resolved IDs
 * are signed halfwords; the other two retain full signed word width.
 * Then invoke Ovl11ResetObjectRecordCommand and return 1. Its selector 42/45
 * special cases apply to the RESOLVED first ID. All four values are read
 * before reset effects begin. No index validation; see
 * ovl11_object_record_reset_command.h for field effects. Original: ovl_11:800E946C. */
s32 Ovl11ResetObjectRecordsFromSlotsCommand(s16 firstSlot, s16 secondSlot, s32 thirdSlot, s32 fourthSlot);

#endif
