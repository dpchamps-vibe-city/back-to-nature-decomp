#ifndef OVL25_RECORD_STATE_TIMER_H
#define OVL25_RECORD_STATE_TIMER_H

#include "common.h"

/* Set the signed state at record byte offset +2. On a state change only,
 * initialize the signed timer at +4 to timerTicks; requesting the current
 * state preserves its timer. Ovl25TickSceneRecords decrements a positive
 * timer once per update, so the unit is update ticks, not seconds.
 *
 * record must provide at least three halfwords. State values and timer signs
 * are unchecked; no other fields are changed and no value is returned. */
void Ovl25SetRecordState(s16 *record, s16 newState, s16 timerTicks);

#endif
