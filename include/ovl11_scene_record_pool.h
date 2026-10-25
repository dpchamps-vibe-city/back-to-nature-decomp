#ifndef OVL11_SCENE_RECORD_POOL_H
#define OVL11_SCENE_RECORD_POOL_H

#include "common.h"

/* Clear fifteen 0x30-byte records: zero the primitive header, position,
 * base rotation and additive rotation offsets; mark flags=0x8000 (free).
 * Deliberately leave the +0x18 delta-angle triple and other bytes untouched.
 * This does not free any vertex-blend allocation or clear its pointer. */
void Ovl11ClearSceneRecords(void);

/* Read fifteen scene-map entries at the current header+0x948. Skip entries
 * flagged 0x8000 and silently skip installation when the pool is full. Copy
 * three 8-byte transform chunks, initialize the projected primitive source,
 * and copy the low halfword of map flags. Flag 0x4000 also allocates a vertex
 * blend and sets its timer=0/duration from the map (allocation must succeed).
 * Finally reset the five dependent scene-animation/rotation/visibility
 * controls, even if no record was installed. Does not clear the pool first. */
void Ovl11InstallSceneRecords(void);

#endif
