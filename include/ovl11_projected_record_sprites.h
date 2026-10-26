#ifndef OVL11_PROJECTED_RECORD_SPRITES_H
#define OVL11_PROJECTED_RECORD_SPRITES_H

#include "common.h"

/* Pixel bounds, including the margin used for partially visible sprites:
 * -49 <= x <= 369, -49 <= y <= 289. Returns 0 or 1. */
s32 Ovl11IsRecordSpriteOnScreen(s32 x, s32 y);

/* Project three halfwords using the current GTE transform. Positive-depth,
 * on-screen points write {pixel x, pixel y, projection depth} to screen[3]
 * and return 1. Failure returns 0 without writing screen. Uses shared scratch;
 * not reentrant. The fourth scratch halfword is padding, not an input. */
s32 Ovl11ProjectRecordSprite(u16 *position, s32 *screen);

/* Draw the current scene's selected record list, two fixed records and the
 * ten-record list. Header kind 0..9 chooses a sprite mapper; other kinds draw
 * nothing. Requires initialized scene work/lists, GTE state, sprite sources
 * and active ordering table. Filters on season/enable flags and selection key,
 * projects (x+150,y-100,z-150) from coordinate low halfwords, and links packets
 * at depth >> 2. Kind/offset-table indices and OT capacity are unchecked.
 * Flag 0x100 applies a table-driven y offset to a local copy, not the record. */
void Ovl11DrawSceneRecordSprites(void);

#endif
