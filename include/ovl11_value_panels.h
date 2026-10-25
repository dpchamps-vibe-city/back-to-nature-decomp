#ifndef OVL11_VALUE_PANELS_H
#define OVL11_VALUE_PANELS_H

#include "common.h"

/* Draw the fixed upper-right value panel. Compact reads D_80071A5C; wide
 * reads gSelectedThresholdBalance. Both format eight numeric positions and
 * append the variant's runtime text-bank label into gOvl11TextFormatScratch.
 * Require the text bank and current render context; consume packet-work
 * storage and ordering-table entries. Coordinates are screen pixels. */
void Ovl11DrawCompactValuePanel(void);
void Ovl11DrawWideValuePanel(void);

/* Draw a width-116 quantity/value box at (x,y), using a BY-VALUE copy of
 * the existing prompt record. unk0 is an optional heading text address;
 * unk4 is appended after quantity (three numeric positions). The next row
 * formats value (five positions) with the primary value label. Height is
 * 34 without a heading or 48 with it; row spacing is 14 pixels. The explicit
 * quantity/value parameters are used, not the record's unk8/unkA fields.
 * Clobbers the shared text scratch and consumes packet-work storage. */
void Ovl11DrawQuantityPromptRows(Ovl11D8012D538 prompt, s16 quantity,
                                s32 value, s16 x, s16 y);

/* Copy a 0xFFFF-terminated label, including its terminator, into dst.
 * The loaded text bank is required; dst must have sufficient space.
 * These callbacks are also used by the selection-list renderer. The glyphs'
 * meaning is not established; primary/secondary names identify variants. */
void Ovl11CopyPrimaryValueLabel(u16 *dst);
void Ovl11CopySecondaryValueLabel(u16 *dst);

#endif
