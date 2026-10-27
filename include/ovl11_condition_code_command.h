#ifndef OVL11_CONDITION_CODE_COMMAND_H
#define OVL11_CONDITION_CODE_COMMAND_H

#include "common.h"

/* Resolve codeOrSlot from the shared value bank iff fromSlot != 0; the signed
 * slot index is unchecked. Queue the code's low 16 bits with payload 1 when
 * payloadMode == 0, otherwise 0. The queue wrapper's filter/capacity rules
 * apply (ovl11_condition_evaluation.h); this command does not report rejection.
 * Then pass controlFlags to mode-control handler 800E7798 as its fourth
 * argument, with the first three zero, even when the code was not queued.
 * Always returns 1. ovl_11 must be resident; shared condition/work state must
 * be initialized. Original identity: ovl_11:800E87A0. */
s32 Ovl11QueueConditionCodeCommand(s16 codeOrSlot, s16 controlFlags, s32 fromSlot, s32 payloadMode);

#endif
