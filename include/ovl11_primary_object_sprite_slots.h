#ifndef OVL11_PRIMARY_OBJECT_SPRITE_SLOTS_H
#define OVL11_PRIMARY_OBJECT_SPRITE_SLOTS_H

#include "common.h"
#include "game_types.h"

/* Object storage must cover +0x3FC. Initialize ten XYZ positions at +0x120
 * (stride 0x10), clear s16 +0x32, and initialize five sources at +0x2D4
 * (stride 0x34) from fixed-ID offset entry 1. Returns 0. */
s32 Ovl11InitObjectSpriteSlots(u8 *object);

/* Project/draw the first five +0x120 positions using the FIRST embedded
 * source. Unless resident flag 0x08000000 freezes it, that source advances
 * after every position, even one culled by projection. Returns 0. */
u32 Ovl11DrawEmbeddedSpriteSlots(u8 *object);

/* Copy ten position quads to +0x1C0 (stride 0x10). At +0x3D4 (stride 4),
 * set animation to inactive sentinel 5 and frame to index&1. Returns 0. */
s32 Ovl11InitSharedSpriteSlots(u8 *object);

/* Draw active shared-source slots (animation != 5), with the per-slot
 * animation/frame pair. The caller must first initialize the shared source
 * and honor gOvl11SharedSlotSpriteEnabled. Does not step it. Returns 0. */
u32 Ovl11DrawSharedSpriteSlots(u8 *object);

/* Copy XYZ in native object units, then add signed X/Z cell offsets selected
 * by the unchecked +0x38 index. Mode 2 selects the second offset bank;
 * mode 4 halves offsets with signed division toward zero. Other modes use
 * full offsets from the first bank. Does not modify the object. */
void Ovl11ComputeGridOffsetPosition(Ovl11CE034Object *object,
                                   Ovl11CE034Position *position, s32 mode);

#endif
