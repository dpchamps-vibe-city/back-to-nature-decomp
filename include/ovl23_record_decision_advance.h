#ifndef OVL23_RECORD_DECISION_ADVANCE_H
#define OVL23_RECORD_DECISION_ADVANCE_H

#include "common.h"
#include "game_types.h"

/* Update the s32 position at record+0xC in signed Q12 units. Only control
 * states 2/3 move, using the corresponding work-area speed times 4096 / 10
 * (signed truncation). Direction 6 subtracts; all other directions add.
 * Clamp at -48 and work.trackExtent+119 world units. At either endpoint,
 * records other than index 2 enter control state 1 facing inward, preserving
 * the third control halfword. The two endpoint tests run independently.
 * Requires initialized overlay work and arithmetic within signed s32 range. */
void Ovl23AdvanceDecisionRecordPosition(Ovl23Func800BB0C8Arg *record);

#endif
