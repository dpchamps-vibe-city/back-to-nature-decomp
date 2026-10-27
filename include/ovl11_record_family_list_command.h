#ifndef OVL11_RECORD_FAMILY_LIST_COMMAND_H
#define OVL11_RECORD_FAMILY_LIST_COMMAND_H

#include "common.h"

/* ovl_11 browser adapter. interactive == 0 resets the shared browser position;
 * otherwise draw/process its four-row familyKind list (filter fixed to 1).
 * Store its result into unchecked gOvl11ValueSlots[resultSlot]: the selected
 * zero-based physical pool index, -1 while waiting, or -2 for cancel/reset.
 * Return 0 only for -1: cancellation/reset take the command's SUCCESS edge.
 * Draw/input/cursor effects belong to the shared non-reentrant browser; see
 * ovl11_record_family_list.h. Original identity: ovl_11:800ED5C0. */
s32 Ovl11RunRecordFamilyListCommand(s16 familyKind, s16 interactive, s32 resultSlot);

#endif
