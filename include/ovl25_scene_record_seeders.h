#ifndef OVL25_SCENE_RECORD_SEEDERS_H
#define OVL25_SCENE_RECORD_SEEDERS_H

#include "common.h"

/* Reset mode/event selectors, initialize record state and seed the twelve
 * tuning halfwords at work+4CE..4E4. No storage is allocated. Call during
 * scene loading, before mode/event dispatch; shared state is non-reentrant.
 */
void Ovl25ResetSceneState(void);

/* Clear pause/result/timer state, mark the two effect handles invalid (-1),
 * seed the three-halfword anchor at +3D0, and initialize six scene plus two
 * auxiliary records. Uses the SECOND table banks (+30/+10 bytes), writes
 * scene record halfwords +10..18 and word +1C, then copies eight bytes at +20.
 * Scene records are 0x78 bytes apart. Also writes auxiliary markers A/B.
 * Unmentioned fields are retained; this is not a whole-buffer clear.
 */
void Ovl25InitSceneRecordState(void);

/* Reseed the same records from the FIRST banks (offset zero), set the record
 * +12 halfword to zero, and preserve the +16/+18 halfwords that initialization
 * clears. The word +1C is again 0x32000, without assigning speculative units.
 * Used by transition/event clients. No input or result; table/storage sizes
 * and initialization are caller prerequisites.
 */
void Ovl25ResetSceneRecords(void);

#endif
