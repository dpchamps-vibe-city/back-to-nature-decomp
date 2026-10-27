#ifndef OVL11_OBJECT_STATE_SELECTION_COMMAND_H
#define OVL11_OBJECT_STATE_SELECTION_COMMAND_H

#include "common.h"

/* ovl_11 command, always returns 1. recordSelector >= 50 selects a compact
 * record at work+(selector-50)*0xF8 and writes its direction at +0xDE22;
 * no bounds check or primary-object update is performed in that arm.
 * Otherwise (including negative selectors), change the primary u16 state at
 * work+0x5200 if unequal, clear work+C bit 0x08000000 during the synchronous
 * Ovl11UpdatePrimaryObject call, then restore the ENTIRE original work+C
 * word, discarding that call's changes to it. An equal state skips the call.
 * Other update side effects remain. Last operands unused. Shared work/object
 * storage must be initialized. Original identity: ovl_11:800E9284. */
s32 Ovl11SelectObjectStateOrDirectionCommand(s16 stateOrDirection, s16 recordSelector, s32 unused2, s32 unused3);

#endif
