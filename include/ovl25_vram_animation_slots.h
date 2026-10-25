#ifndef OVL25_VRAM_ANIMATION_SLOTS_H
#define OVL25_VRAM_ANIMATION_SLOTS_H

#include "common.h"

/* Clear 0xB4 bytes of shared primary animation storage and mark the eight
 * work-area mirror keys free (0xFFFF). Does not issue GPU commands. */
void Ovl25InitVramAnimationSlots(void);

/* tableAddress is a RAM asset-header address, not a disc offset. Inspect
 * eight 0x12-byte descriptors at +0x8A8 and register non-0xFFFF keys in the
 * shared slots. Existing keys and a full pool are silently ignored. */
void Ovl25RequestVramAnimations(s32 tableAddress);

/* Advance live slots once and issue MoveImage commands. The work-state flag
 * 0x08000000 pauses both modes. Sequence durations count updates; scroll
 * mode advances two scanlines after its delay tick, wrapping at the height.
 * The asset table at work+0x25388 must remain valid for sequence reloads. */
void Ovl25UpdateVramAnimations(void);

#endif
