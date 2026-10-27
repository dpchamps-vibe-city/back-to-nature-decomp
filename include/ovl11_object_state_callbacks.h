#ifndef OVL11_OBJECT_STATE_CALLBACKS_H
#define OVL11_OBJECT_STATE_CALLBACKS_H

#include "common.h"
#include "game_types.h"

/* Entry-callback slots 6/7/5 of gOvl11ObjectStateEntryCallbacks, respectively.
 * Their storage and the dispatchers remain in the separate state service. */

/* Subtract the unsigned low halfword of gOvl11StateValueDebit from +AC.
 * Store the low halfword, then clamp to zero if that halfword is negative
 * as s16. This is wrapping halfword arithmetic, not a wide saturating
 * subtract. Return 0. The same callback also serves frame slot 6. */
s32 Ovl11ApplyStateValueDebit(Ovl11StateValueView *object);

/* Entry slot 7 permits transition without side effects; no arguments read. */
int Ovl11StateEntryNoop(void);

/* Map IDs 108/162/165/17A/17B and selector +30 to message entries, requesting
 * window mode 2 when recognized. ID 108's selectors 1B/1F set game flags
 * 9E/9F on first use and choose distinct first/repeat messages. ID 17A/9
 * tests the shared +99D8 selector. Return -1 even when no message is selected. */
s32 Ovl11RequestObjectStateMessage(Ovl11D4030Arg *object);

#endif
