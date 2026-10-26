#ifndef OVL11_PRIMARY_OBJECT_DRAW_H
#define OVL11_PRIMARY_OBJECT_DRAW_H

#include "common.h"
#include "game_types.h"

/* Draw the primary object using the supplied XYZ origin (engine world units).
 * Projection adds (+150,-100,-150), narrows to s16, then selects the OT depth.
 * This services animation, the timed effect, companion and saved-record draw
 * variants, but does not present a frame. The renderer/GTE, packet cursors,
 * object sprite sources and selected saved-record pointer must be initialized.
 * Shared projection scratch makes this non-reentrant. The trailing argument
 * is passed as 1 by the area-frame caller and remains unread.
 * Original identity: ovl_11:800CA24C. */
void Ovl11DrawPrimaryObjectFrame(Ovl11SpritePositionView *origin, s32 unused);

/* Synchronize direction and animation of Ovl11SavedInteractionObject from
 * object +38/+3C and the sprite frame at +264. Mode 1 selects animation 0;
 * modes 2/4 select low-direction-byte+1. A matching frame index does likewise;
 * otherwise a high frame-control bit copies direction into saved +7D.
 * Direction is stored as u16 but read as u8; do not widen those byte reads.
 * This updates animation state, not packets. Original: ovl_11:800CA5D4. */
void Ovl11SyncSavedObjectAnimation(Ovl11C7010Object *object);

#endif
