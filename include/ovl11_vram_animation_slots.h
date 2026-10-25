#ifndef OVL11_VRAM_ANIMATION_SLOTS_H
#define OVL11_VRAM_ANIMATION_SLOTS_H

#include "common.h"
#include "game_types.h"

/* Zero the 0xB4-byte engine storage block; separately mark the eight work-area
 * mirror keys free (0xFFFF). These are distinct addresses, not one array. */
void Ovl11InitVramAnimationSlots(void);

/* Request the eight 0x12-byte descriptors at header+0x8A8. Ignore keys 0xFFFF,
 * duplicate keys and requests after capacity is exhausted. Header kind 15 is
 * suppressed when the work-area gate is 1 or 2. No allocation or validation. */
void Ovl11RequestVramAnimations(Ovl11SetupHeader *header);

/* Service eight 0x16-byte slots unless work flag 0x08000000 is set.
 * key bit 0x8000 selects descriptor sequencing; otherwise scroll vertically
 * by two VRAM rows on the configured delay, wrapping at the rectangle height.
 * MoveImage x/width use VRAM words; y/height use rows. Each invocation is one
 * animation tick; it does not wait for GPU completion. Sequence kind 0x5B is
 * paused while game flag 0x45 is zero. Stage 44/key bit 0 repeats sequences at
 * destination x+8 and x+16. Slot keys and descriptor indices must be valid. */
void Ovl11UpdateVramAnimations(void);

#endif
