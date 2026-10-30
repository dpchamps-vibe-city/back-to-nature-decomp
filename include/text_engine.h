#ifndef TEXT_ENGINE_H
#define TEXT_ENGINE_H

#include "common.h"

/* U16 text uses glyph/command codes, not ASCII, and ends at 0xFFFF.
 * 0xFFFE separates rows/pages; 0x4000|id expands a registered text source.
 * Drawing appends GPU packets to the supplied ordering table and advances
 * the packet-work cursor. Coordinates are screen pixels; wrappers explicitly
 * narrow them to signed halfwords. Legacy s32 address parameters retain their
 * established ABI rather than silently changing callers' pointer contracts.
 * The shared pen, palette, reveal and window state make this API non-reentrant.
 * Independent string operations are published by u16_string.h. */

/* Invalidates the installed script; the next window update rescans/reset it. */
void InvalidateTextScript(void);
/* Slot is interpreted as u16; values >=11 select slot 10. Stores an address. */
void SetTextSourcePointer(s32 slot, s32 textAddress);
/* Extra pixels per glyph/row, in addition to each renderer's base advance. */
void SetTextSpacing(s16 letterSpacing, s16 lineSpacing);
/* Palette indices >=7 clamp to 6. Speed indices >=3 select index 1. */
void SetTextPalette(u32 palette);
u32 GetTextPalette(void);
void SetTextSpeedIndex(u32 index);
/* Any nonzero value enables the text-window's controller sampling. */
void SetTextInputEnabled(s32 enabled);
s32 IsTextInputEnabled(void);
s16 GetTextPortraitSelector(void);

/* One-shot text versus per-frame window progression. width is retained but
 * unused: the window's draw-area width is 320-x. Window status 10..13 encodes
 * confirmed choice 1..4; other statuses belong to the script/window lifecycle.
 * Keep calling the window with the same script until it completes. */
void DrawTextLine(s32 otAddress, s32 textAddress, s32 x, s32 y);
void DrawTextLineSemiTransparent(s32 *ot, u16 *text, s16 x, s16 y,
                               s32 semitransparent);
s32 DrawTextWindow(s32 *ot, u16 *script, s16 x, s16 y, s16 width, s16 height);
void DrawChoiceCursor(s32 otAddress, s16 x, s16 y);
/* Panel uses panelOt; its text uses the separately supplied text OT address. */
void DrawMenuPanelWithText(u32 *panelOt, s32 textOtAddress, s32 textAddress);
void SetTextBlockOrigin(s32 x, s32 y);

/* ASCII formatting at the current block position, followed by 8x12 row fills.
 * Supports %d, %x, %i and %s. Callers must keep the expansion below 64 codes
 * and literal-format newlines within five rows; substitutions are not bounded.
 * Width >320 or height >120 returns after clearing spacing and palette. */
void DrawFormattedText(u8 *text, ...);
/* Maps one ASCII byte to a glyph; newline -> 0xFFFE, unsupported -> 0xFFD. */
s32 ConvertAsciiToTextCode(s32 character);

/* Formatting writes codes to caller storage and returns the next halfword.
 * Decimal output is NOT terminated: callers append 0xFFFF themselves. digits
 * must be 1..10 (larger requests clamp to 10); 0xFFD marks leading blanks.
 * Season/weekday copies write exactly six/three codes, respectively. */
s16 *WriteDecimalText(s32 value, s16 *text, s32 digits);
u16 *WriteSeasonNameText(s32 season, u16 *text);
u16 *WriteWeekdayNameText(s32 weekday, u16 *text);
void CopyTextString(u16 *destination, u16 *source);
/* digits==-1 chooses nine columns and skips leading blanks; >=10 draws nothing. */
void DrawNumberText(s16 value, s16 digits, s32 otAddress, s16 x, s16 y);

/* The image slot is shared with the text stream's picture commands. SetTextSlot
 * installs both loaded/uploaded markers; invalidation forces the next upload.
 * EnsureTextSlotLoaded starts/polls the read and returns 1 after binding, else 0.
 * The token is converted through GetPairedTpage, not treated as a raw slot. */
void SetTextSlot(s32 slot);
u16 GetTextSlotMarker(void);
void InvalidateTextSlot(void);
s32 EnsureTextSlotLoaded(s32 token);
void DrawTextSlotImageForToken(u32 *ot, u16 token, s16 x, s16 y);

#endif
