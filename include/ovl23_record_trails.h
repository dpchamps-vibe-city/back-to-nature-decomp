#ifndef OVL23_RECORD_TRAILS_H
#define OVL23_RECORD_TRAILS_H

#include "common.h"

/* Two shared overwrite rings: 54 decision-record nodes and 12 actor nodes.
 * X is signed Q12, laneY is a screen-pixel row (-1 hides the node), and state
 * selects the marker frame. These are scene services, not independent buffers.
 * Push wraps an upper-bound cursor before writing and increments afterward;
 * cursors must be initialized and nonnegative. No lower-bound check exists. */
void Ovl23PushDecisionTrailNode(s32 xQ12, s16 laneY, s16 state);
void Ovl23PushActorTrailNode(s32 xQ12, s16 laneY, s16 state);

/* Positions all decision nodes at +23 and actor nodes at -27 world units.
 * Enables the first six lanes from the anchor table, hides the remaining
 * nodes, and sets both next cursors to 6. Existing state fields are retained. */
void Ovl23ResetRecordTrails(void);

/* Draws visible nodes using camera X, HWD0 and the active render context's
 * +0xD0 OT slot. Actor nodes use animation 4, decision nodes animation 3.
 * Updates the shared sprite's animation/frame state but not ring contents;
 * setup must have installed the sprite source. Not reentrant. */
void Ovl23DrawRecordTrails(void);

#endif
