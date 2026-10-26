#ifndef OVL25_EVENT_STATE_HANDLERS_H
#define OVL25_EVENT_STATE_HANDLERS_H

#include "common.h"

/* Ordered callbacks 0..5 of gOvl25EventPhaseHandlers. The outer dispatcher
 * resets phase 6 only after fade completion; these callbacks are not a full
 * independent scene loop. They mutate shared scene/work, message and fade
 * state, so only one event sequence may be active.
 * Message requests (archive 4, entries 31..34 hex) are repeated until status 2.
 * Target X moves in signed-halfword steps of 33; bounds and choice paths are
 * intentionally asymmetric. No elapsed-time scaling or input validation. */
void Ovl25BeginEventPhases(void);
void Ovl25WaitEventPrompt31(void);
void Ovl25MoveEventTargetForward(void);
void Ovl25MoveEventTargetBack(void);
void Ovl25BeginEventFadeIncrease(void);
void Ovl25WaitAndBeginEventFadeDecrease(void);

#endif
