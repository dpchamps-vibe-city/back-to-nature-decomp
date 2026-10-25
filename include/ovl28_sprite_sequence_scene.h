#ifndef OVL28_SPRITE_SEQUENCE_SCENE_H
#define OVL28_SPRITE_SEQUENCE_SCENE_H

/* Per-frame entry for the resident ovl_28 sprite-sequence scene, originally
 * 0x800B7E24. gScenePhase must be in 0..6 (normally initially zero).
 * Saves/restores the text drawing mode around its selected phase handler.
 *
 * Loads two sprite assets through the shared graphics-heap scratch area and
 * queues sound loads. Then drives a 20-slot scrolling-sprite pool and an
 * independent presentation path, with screen/volume fades around them.
 * Scrolling starts at VWD0 pixels and steps upward by Q12 pixel increments;
 * the initial step is 0x800 (half a pixel per active call). Presentations
 * choose an ID from a progression-indexed 25-column table and an x coordinate
 * via Rand, hold for 60 active calls, and apply an 80x80 color ramp.
 *
 * Data indices/phase are not validated; a full sprite pool drops requests.
 * Scene exit clears the phase and requests ID 13 when the game-state u16 at
 * +0xE7A0 is >=10, otherwise ID 20. Both decisions retain their original
 * numeric meanings: this interface assigns no speculative gameplay label.
 * Uses the current render context, CD loader, sound driver and packet cursor;
 * not reentrant and not safe to call after unloading this overlay. */
void Ovl28UpdateSpriteSequenceScene(void);

#endif
