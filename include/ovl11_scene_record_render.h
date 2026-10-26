#ifndef OVL11_SCENE_RECORD_RENDER_H
#define OVL11_SCENE_RECORD_RENDER_H

#include "common.h"

/* index must be 0..14. Copy three halfwords, without validation, from source.
 * Position is signed XYZ in world units; offset angles are added to base
 * angles for RotMatrixZXY (4096 units per turn). Other record fields survive.
 * Keep the existing void* ABI: callers supply several three-halfword views. */
void Ovl11SetSceneRecordPosition(s16 index, void *position);
void Ovl11SetSceneRecordRotationOffset(s16 index, void *angles);

/* Walk all fifteen records in order, skipping flags 0x8000 and 0x1000.
 * Flag 0x4000 updates/draws the installed vertex blend (must be valid).
 * Flag 0x80 bypasses projection culling and projection-scratch writes;
 * otherwise draw only for projected X in [-100,420], Y in [-100,340].
 * Compose position and base+offset ZXY rotation with the captured transform.
 * Restore the matrix afterward. Then flag 0x2000 advances base angles by
 * their deltas unless shared pause bit 0x08000000 is set, even if culled.
 * Requires initialized record/primitive storage and the current render context. */
void Ovl11DrawSceneRecords(void);

#endif
