#ifndef SPRITE_SOURCE_H
#define SPRITE_SOURCE_H

#include "common.h"
#include "game_types.h"

/* Tag-0xE sprite source/animation records. Header and referenced tables must
 * remain valid for every later update/draw. Tables, indices and pointer words
 * are unchecked. Historical partial record views are retained in this ABI;
 * they refer to the same first 0x30 bytes as SpriteSourceData, not new objects.
 */

/* exe:80015704. Requires a four-byte-aligned tag-0xE header. On failure,
 * prints INIT ERROR and repeatedly breaks into the debugger; it does not
 * return an error. Clears state, sets field_8=0x1000, binds five relative
 * tables, records the header address and clears the image-record address. */
void InitSpriteSourceData(SpriteSourceData *out, SpriteDataHeader *header);

/* exe:80015814/28. Operate on the first flag halfword; mask uses low 16 bits. */
void AddSpriteSourceFlags(u16 *flags, s32 mask);
void ClearSpriteSourceFlags(u16 *flags, s32 mask);

/* exe:80015840. Select animation, reset frame/timer and clear 0x100/0x200. */
void SelectSpriteAnimation(ObjectState *source, u8 animation);
/* exe:8001585C. Select frame and reset its timer; retain flags/animation. */
void SelectSpriteAnimationFrame(ObjectState *source, u8 frame);
/* exe:80015868. Texture/palette coordinate offsets; signed halfword stores.
 * Pixel/page interpretation depends on the selected renderer and entry. */
void SetSpriteSourceTextureOffsets(Struct_800154CC *source, s16 uOffset,
                                   s16 vOffset, s16 paletteXOffset,
                                   s16 paletteYOffset);
/* exe:80015894. Store the image-record address at source+0x18, no upload. */
void SetSpriteSourceRecordAddress(SomeStruct *source, s32 recordAddress);

/* exe:8001589C. Current frame control byte >> 7, no mutation. */
u32 GetSpriteFrameControlHighBit(SpriteSourceData *source);
/* exe:80015A18. Unchecked signed-low-halfword animation index. */
u16 GetSpriteAnimationFrameCount(SpriteSourceData *source, s32 animation);

/* exe:800158D8. Set the raw shared latch, normally cleared before service. */
void SetSpriteAnimationUpdateLatch(s32 value);
/* exe:800158E4. One animation update-call tick. field_0 bit 2 suppresses all
 * effects. Otherwise clears result bits 0x100/0x200, increments the timer,
 * and processes a frame when it reaches ceil(duration/2). Controls select
 * frames in the current animation; 0xFF holds the terminal frame. Sets the
 * shared update latch to 1 even when this call does not change a frame.
 * Rendering wrappers may call this themselves: avoid double-ticking. */
void UpdateSpriteAnimation(SpriteSourceData *source);

#endif
