#ifndef OVL25_SPRITE_SET_SERVICE_H
#define OVL25_SPRITE_SET_SERVICE_H

#include "common.h"
#include "game_types.h"

/* ovl_25 must be resident, its sprite sources initialized by the scene loader,
 * and the active render context, packet cursor and GTE transform configured.
 * Draw calls append packets and may advance source animation; all indices and
 * packet capacity are unchecked. Projection scratch is shared/non-reentrant.
 *
 * Object records are 0x78 bytes: two 0x30-byte sprite sources use the four
 * halfwords at +0x10. The first source is projected after subtracting
 * (150,100,150); the second uses the unshifted position. These are source/GTE
 * coordinate units, not screen pixels. Flag-4 sources do not advance animation.
 * A failed projection still submits using the previous screen scratch.
 * No draw routine waits for VSync or measures elapsed time. */
void Ovl25DrawObjectRecordPair(Ovl25ObjectRecord *object); /* states 0..8 */
/* States 9..12; decrements the signed-halfword timer in state 12, then sets 11. */
void Ovl25DrawAuxiliaryRecord(Ovl25ObjectRecord *object);
/* Choose animation 0..4 by strict thresholds on the first work record's +C
 * word (0x28000,0x1E000,0x14000,0xA000); draw unless its state is 0 or 8. */
void Ovl25DrawThresholdIndicator(void);
void Ovl25DrawPairedSceneSprites(void);
/* Mutates three sequence phases, position halfwords, signed step and effect
 * latch. Timing counts calls; emits sound 0x4D when the cycle crosses y>=0.
 * The byte at cycle-phase address+2 is an independent stage/counter. */
void Ovl25UpdateSequenceSprites(void);
/* Set/clear animation-service flag 4 across 26 sources: 4 primary, 6 pairs,
 * 2 auxiliary pairs and 6 sequence sources. Does not hide packets or reset
 * frames/timers, and does not suppress this module's other sequence counters. */
void Ovl25PauseSpriteSets(void);
void Ovl25ResumeSpriteSets(void);
/* Fixed animation/frame bytes from the primary source, centered with the
 * current HWD0/VWD0 viewport dimensions (pixel units). */
void Ovl25DrawPrimarySprite(void);

#endif
