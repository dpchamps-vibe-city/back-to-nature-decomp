/*
 * text_engine — reconstructed translation unit.
 *
 * U16 message scripts, window paging, text rendering and paired sprite reads;
 * owns the shared small-data text state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/libapi.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/stdio.h"
#include "psyq/stdarg.h"
#include "screen_primitives.h"
#include "aux_sprite_display.h"
#include "packet_work.h"
#include "u16_string.h"
#include "text_engine.h"
#include "sound_commands.h"
#include "vblank_timer.h"
#include "sprite_upload.h"
#include "message_buffer_access.h"
#include "ui_sprites.h"
#include "psyq/memory.h"
#include "beveled_panel.h"
#include "integer_power.h"

void ClearTextScriptPointer(void);

void InvalidateTextScript(void) {
    ClearTextScriptPointer();
}

void SetTextSourcePointer(s32 arg0, s32 arg1) {
    s32 var_a0;
    s32 idx;

    var_a0 = arg0;
    idx = var_a0 & 0xFFFF;
    if ((u32) (var_a0 & 0xFFFF) >= 0xB) {
        var_a0 = 0xA;
    }
    idx = var_a0 & 0xFFFF;
    gTextSourceAddresses[idx] = arg1;
}

u16 gTextLetterSpacing;
u16 gTextLineSpacing;

void SetTextSpacing(s16 letterSpacing, s16 lineSpacing) {
    gTextLetterSpacing = letterSpacing;
    gTextLineSpacing = lineSpacing;
}

u32 gTextPalette;

void SetTextPalette(u32 arg0) {
    u32 var_a0;

    var_a0 = arg0;
    if (var_a0 >= 7U) {
        var_a0 = 6;
    }
    gTextPalette = var_a0;
}

u32 GetTextPalette(void) {
    return gTextPalette;
}

s16 gTextSpeedInterval;

/* Speed indices >=3 choose index 1. The stored table value is a VSync
 * interval, not the public index. */
void SetTextSpeedIndex(u32 arg0) {
    if (arg0 >= 3) {
        arg0 = 1;
    }

    gTextSpeedInterval = gTextSpeedIntervals[arg0];
}

/* Inverse speed mapping: interval 0 -> index 0, 5 -> 2, otherwise 1. */
s32 GetTextSpeedIndex(void) {
    s16 mode;

    mode = gTextSpeedInterval;
    if (mode == 0) {
        return 0;
    }
    if (mode == 5) {
        return 2;
    }
    return 1;
}

u16 gTextState;

/* Triggers/starts the event tracked by D_8005E446: transitions the flag from
 * 0 (idle) to 1 (started).  If the flag is already non-zero (already started
 * or in another state) this is a no-op, preventing double-starts. */
void BeginTextRun(void) {
    if (gTextState == 0) {
        gTextState = 1;
    }
}

u16 gTextScriptIndex;

/* Commits/ends the event tracked by D_8005E446: if the flag is 1 (started) or
 * 3 (another active state), clears the flag back to 0 (idle) and increments
 * the D_8005E444 completion counter.  This pairs with func_80017ACC which sets
 * the flag to 1 to begin the event. */
void EndTextPage(void) {
    if ((gTextState == 1) || (gTextState == 3)) {
        gTextState = 0;
        gTextScriptIndex += 1;
    }
}

s32 gTextInputEnabled;

/* Gates controller sampling by the window driver; nonzero means enabled. */
void SetTextInputEnabled(s32 enabled) {
    gTextInputEnabled = enabled;
}

/* Returns the stored gate value without normalizing it to one. */
s32 IsTextInputEnabled(void) {
    return gTextInputEnabled;
}

s16 gTextPortraitSelector;

s16 GetTextPortraitSelector(void) {
    return gTextPortraitSelector;
}

/* Game callees. */
extern u8 *DrawTextStream(s32 *arg0, u8 *arg1, u16 *arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

void DrawTextLine(s32 otAddress, s32 textAddress, s32 inputX, s32 inputY) {
    s32 x = (s16)inputX;
    s32 y = (s16)inputY;

    SetPacketWorkCursor(DrawTextStream(otAddress, ReservePacketWorkBytes(0), textAddress, x, y, 0x1000, 0,
                                0, 0, 0));
}

s32 UpdateTextWindow(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4, s16 arg5);

s32 DrawTextWindow(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    return UpdateTextWindow(arg0, arg1, arg2, arg3, arg4, arg5);
}

s16 GetTextRowY(void);

s32 GetTextPenY(void) {
    s32 callRet1;
    callRet1 = GetTextRowY();
    return callRet1;
}

u16 gTextPublishedEndToken;

u16 GetTextTerminatingToken(void) {
    return gTextPublishedEndToken;
}

s16 gTextUploadedSlot;

u16 GetTextSlotMarker(void) {
    return (u16)gTextUploadedSlot;
}

s16 gTextLoadedSlot;

void SetTextSlot(s32 arg0) {
    gTextUploadedSlot = arg0;
    gTextLoadedSlot = arg0;
}

/* Row engines are defined below; keep their halfword argument contracts in
 * scope for the public wrappers rather than relying on C89 implicit int. */
s32 DrawTextRowsCentered(s32 otAddress, s32 packets, u16 *text, s16 x, s16 y,
                        s16 width, s32 scale, s16 rowStep, s16 letterSpacing);
s32 DrawTextRowsRightAligned(s32 otAddress, s32 packets, u16 *text, s16 x, s16 y,
                            s16 width, s32 scale, s16 rowStep, s16 letterSpacing);

/* Each row of the block is centred inside width; the row engine starts at the
 * given y and advances by one line spacing plus 16. The original reads the letter
 * spacing as a signed halfword even though the shared global is u16. */
void DrawCenteredTextBlock(s32 arg0, u16 *text, s32 arg2, s32 arg3, s16 width) {
    s16 x;
    s16 y;

    x = arg2;
    y = arg3;
    SetPacketWorkCursor(DrawTextRowsCentered(arg0, ReservePacketWorkBytes(0), text,
        x, y, width, 0x1000,
        (s16)(gTextLineSpacing + 0x10), (s32)(s16)gTextLetterSpacing));
}

/* Right-aligned counterpart of DrawCenteredTextBlock: the row engine starts at the
 * given y and advances by one line spacing plus 16. Like the centred wrapper it
 * reads the letter spacing as a signed halfword even though the shared global is u16. */
void DrawRightAlignedTextBlock(s32 otAddress, u16 *text, s32 inputX, s32 inputY, s16 width) {
    s16 x;
    s16 y;

    x = inputX;
    y = inputY;
    SetPacketWorkCursor(DrawTextRowsRightAligned(otAddress, ReservePacketWorkBytes(0), text,
        x, y, width, 0x1000,
        (s16)(gTextLineSpacing + 0x10), (s32)(s16)gTextLetterSpacing));
}

u8 *DrawTextStream(s32 *arg0, u8 *arg1, u16 *, s16, s16, s32, s32, s32, s32, s32); /* extern */

void DrawTextLineSemiTransparent(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4) {
    SetPacketWorkCursor(DrawTextStream(arg0, ReservePacketWorkBytes(0), arg1, arg2, arg3, 0x1000, 0, arg4, 0, 0));
}

/*
 * Halfword strings terminated by 0xFFFF, original exe:80017E34-80017F88.
 * These operations neither allocate storage nor consult text-renderer state.
 * The unused copy entry remains in its original position between append and
 * compare; all four entries are part of this state-owning text translation unit.
 */
/* Return the destination's new terminator, not its start. Keep one value
 * variable across the first copy and loop; the repeated source read is part
 * of the original copy-loop construction. */
u16 *AppendU16String(u16 *destination, u16 *source) {
    u16 terminator;
    u16 value;

    while (*destination != 0xFFFF) {
        destination++;
    }
    *destination = *source;
    value = *source;
    if (value != 0xFFFF) {
        terminator = 0xFFFF;
        source++;
        do {
            value = *source;
            *++destination = value;
        } while (*source++ != terminator);
    }
    return destination;
}

/* Copy the terminator as well; unlike AppendU16String, return no pointer. */
void CopyU16String(u16 *destination, u16 *source) {
    u16 terminator;
    u16 value;

    *destination = *source;
    value = *source;
    if (value != 0xFFFF) {
        terminator = 0xFFFF;
        source++;
        do {
            value = *source;
            *++destination = value;
        } while (*source++ != terminator);
    }
}

/* The second pointer advances after the terminator test. This also lets the
 * compiler rotate the loop tail into the original entry-jump layout. */
s32 CompareU16String(u16 *left, u16 *right) {
    for (;;) {
        if (*left < *right) {
            return -1;
        }
        if (*right < *left) {
            return 1;
        }
        left++;
        if (*right == 0xFFFF) {
            break;
        }
        right++;
    }
    return 0;
}

/* Compare each input position with either reference at that position.
 * On a mismatch the ordering result is against primary, never alternate.
 * All three pointers advance together, and an accepted 0xFFFF ends the scan.
 * Keep the initial jump past the advance/test tail and the reused raw value. */
s32 CompareU16StringAgainstPair(u16 *input, u16 *primary, u16 *alternate) {
    s32 raw;
    s32 current;
    s32 primaryValue, alternateValue;
    u32 maskedValue;
    s32 result;
    u16 terminator = 0xFFFF;

    goto compare;
    for (;;) {
    advance:
        if (current == terminator) goto done;
        input++;
        primary++;
        alternate++;
    compare:
        raw = *input;
        maskedValue = raw & 0xFFFF;
        current = raw;
        primaryValue = *primary;
        if (maskedValue == primaryValue) {
            result = 0;
            goto advance;
        }
        alternateValue = *alternate;
        if (maskedValue == alternateValue) {
            result = raw = 0;
            goto advance;
        }
        result = (maskedValue < primaryValue) ? -1 : 1;
        break;
    }
done:
    return result;
}

/* Expand ASCII formatting into the shared halfword script, draw it, then fill
 * each row's 8x12 character cells. Only newlines in the format advance row;
 * a substituted string contributes every character to the current row. */
void DrawFormattedText(u8 *text, ...) {
    typedef u8 *TextFormatString;
    char number[16];
    u16 rowCells[5];
    va_list args;
    Ovl11FarInitView *runtime;
    u8 *source;
    u8 *string;
    s16 row;
    s16 cellCount;
    s32 cellX;
    s16 letterSpacing;
    s16 lineSpacing;
    s32 count; /* Glyph count, saved palette, then row index. */
    s32 *ot;

    if ((D_8006C844 & 0x800000) != 0) {
        return;
    }
    runtime = (Ovl11FarInitView *)&D_8007AFF0;
    if (runtime->field_2548C != 0) {
        return;
    }
    if (runtime->field_254A0 != 0) {
        return;
    }
    if (IsAuxSpriteDisplayEnabled() == 1) {
        return;
    }

    for (count = 0; count < 5; count++) {
        rowCells[count] = 0;
    }
    row = 0;
    source = text;
    va_start(args, text);
    count = 0;
    if (*source == 0) {
        D_8005F028[0] = 0xFFFF;
    } else {
        /* The first character is already checked; enter past the advance.
         * Keep the outer scan indexed and the substitution scan separate. */
        goto parse_character;
        for (;;) {
            source++;
            if (count >= 0x40) {
                break;
            }
            if (*source == 0) {
                D_8005F028[count] = 0xFFFF;
                break;
            }
        parse_character:
            if (*source != '%') {
                D_8005F028[count] = ConvertAsciiToTextCode(*source);
                count++;
                if (*source == '\n') {
                    row = (s16)(row + 1);
                } else {
                    rowCells[row]++;
                }
            } else {
                source++;
                string = (u8 *)number;
                switch (*source) {
                case 'd':
                    sprintf(number, D_8005E2C0, va_arg(args, s32));
                    break;
                case 'x':
                    sprintf(number, D_8005E2C4, va_arg(args, s32));
                    break;
                case 'i':
                    sprintf(number, D_8005E2C8, va_arg(args, s32));
                    break;
                case 's':
                    string = va_arg(args, TextFormatString);
                    break;
                default:
                    continue;
                }
                /* The original does not check the staging-buffer limit here. */
                while (*string != 0) {
                    D_8005F028[count] = ConvertAsciiToTextCode(*string);
                    count++;
                    rowCells[row]++;
                    string++;
                }
            }
        }
    }
    va_end(args);

    letterSpacing = (s16)gTextLetterSpacing;
    lineSpacing = (s16)gTextLineSpacing;
    D_8005F028[63] = 0xFFFF;
    gTextLetterSpacing = 0;
    gTextLineSpacing = 0;
    count = GetTextPalette();
    SetTextPalette(0);
    /* These early returns deliberately leave palette and spacing cleared. */
    if (gTextBlockWidth >= 0x141) {
        return;
    }
    if (gTextBlockHeight >= 0x79) {
        return;
    }
    ot = (s32 *)gActiveRenderContext->field_120;
    gActiveRenderContext->field_118 = (s32)DrawTextStream(ot,
        (u8 *)gActiveRenderContext->field_118, D_8005F028,
        (s16)(gTextBlockOriginX + (u16)gTextBlockWidth),
        (s16)(gTextBlockOriginY + (u16)gTextBlockHeight), 0x1000, 0, 0, 0, 1);
    SetTextPalette(count);
    for (count = 0; count <= row; count++) {
        cellCount = (s16)rowCells[count];
        if (cellCount != 0) {
            cellX = gTextBlockOriginX;
            if (count == 0) {
                cellX = cellX + (u16)gTextBlockWidth;
            }
            DrawScreenTileRect((u32 *)ot, (s16)cellX,
                (s16)(gTextBlockOriginY + (u16)gTextBlockHeight + count * 12),
                (s16)(cellCount << 3), 12, 0xFFFFFF, 0);
        }
    }
    gTextLetterSpacing = letterSpacing;
    gTextLineSpacing = lineSpacing;
    gTextBlockWidth = gTextPenX - gTextBlockOriginX;
    gTextBlockHeight = gTextPenY - gTextBlockOriginY;
}

/* Tentative definitions: the target reaches all four symbols via $gp, so this
 * translation unit must define them (common symbols) rather than declare them
 * extern — see configs/project-profile.md small-data threshold (-G8). */
u16 gTextBlockOriginX;
u16 gTextBlockOriginY;
s16 gTextBlockWidth;
s16 gTextBlockHeight;

void SetTextBlockOrigin(s32 arg0, s32 arg1) {
    gTextBlockOriginX = arg0;
    gTextBlockOriginY = arg1;
    gTextBlockWidth = 0;
    gTextBlockHeight = 0;
}

void ResetTextBlockSize(void) {
    gTextBlockWidth = 0;
    gTextBlockHeight = 0;
}

/* Callee prototypes (project convention: local declarations). */
u16 *FindTextToken(u16 *, u16, s32, s32);
s32 CountTextMarkers(u16 *, u16, s16, u16 *);
s32 SkipTextPage(u16 *, s32);
s32 MeasureTextSpanLength(u16 *, u16, s16);
void DrawTextWindowLines(s32, s32, s32, s32);
s32 UpdateChoiceCursor(s16, s16, u16 *, s16);
s32 MapChoiceIndexToStatus(s32);
void DrawTextSlotImage(u_long *, s16, s16, s16);
void SetTextSpeedIndex(u32);
s32 GetTextSpeedIndex(void);
void EndTextPage(void);
u16 GetTextState(void);
void ResetTextState(void);
void LoadTextSlot(s32, s32);
s32 ResolveTextSource(s32);
void SeekTextToBookmark(void);
void EnterChoiceState(void);
void BeginTextRun(void);

/* TU-owned globals (GP-relative — tentative definitions). */
u16 gTextPrevState;
s16 gTextTpage;
s32 gTextSlotLoadAsync;
s32 gTextSlotLoadDone;
s32 gTextSlotImageVisible;
u8 *gTextSlotBuffer;
s32 gTextInstantPageMode;
u16 gTextEndToken;
s32 gTextHoldAtEnd;
s32 gTextTypeSoundToggle;
s16 gTextLineCount;
s16 gTextLineScroll;
u16 gTextChoiceIndex;
u16 *gTextScriptCursor;    /* extern decl in globals_override.h; tentative def here */
     /* extern decl in globals_override.h; tentative def here */
s32 gTextPageTimer;
u16 *gTextScriptTable;
s32 gTextRendering;
s32 gTextScrolledFull;

/* Steps and draws the text window over the u16 script table arg1: a new
 * table is scanned once (0x4000 entries go to func_8001A284, D_8005E444
 * counts entries up to the 0xFFFF terminator) and the window state is reset.
 * Each call then reacts to the func_800195F4 state, pages through the
 * table (0xFFFE and 0xFFC markers), sets the two draw areas
 * around the text at (arg2, arg3) and places the cursor at
 * (arg2 - 4, arg3 - 0x54). Returns the (u16) status, or an early result for
 * states 2 and 7-9.
 *
 * The inverted halfword status and staged negative coordinates preserve
 * branch-local result/cursor calculations at baseline optimization. They
 * recover exactly (u16)status, arg2 - 4 and textY - 0x54; keep their widths
 * and assignment boundaries when changing this routine.
 * Other matching-sensitive details:
 *
 * - A ternary argument is written inside its call. Arguments are expanded
 *   left to right, so the earlier ones are loaded before the branch, as in
 *   the target (func_8001929C, func_8001945C and func_80019E80 calls).
 * - The scan loop computes next = i + 1 before its if and tests next < 4,
 *   which is the target's addiu a0,s1,1 / move s1,a0 counter.
 * - entry (the first walk), list (case 3), area1/area2 (the draw areas) and
 *   step are separate variables. Each keeps its pseudo short, which gives
 *   global-alloc the target's order: cursorY > cursorX > result > arg2 >
 *   arg3 (s4, s5, s6, s7, fp).
 * - status + 0xFFFBU is unsigned so the conversion to s16 is not narrowed
 *   into an addiu of -5; the target adds 0xFFFB in 32 bits.
 * - In the arms that make calls, the calls come before the result and
 *   cursor assignments.
 * - *D_8005E4A8 == 0xFFFE re-reads through the pointer just stored, which
 *   keeps the store ahead of the load.
 * - (s16) on field_2 puts arg3 first in the rect.y addition. */
s32 UpdateTextWindow(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4,
                  s16 arg5) {
    RECT rect;
    s32 height;
    s32 textY;
    s32 savedState;
    s32 cursorX;
    s32 result;
    s32 cursorY;
    struct { u16 value; } rawStatus;
    struct { s32 value; } rawX, rawY;
    s32 step;
    s32 input4;
    s32 input8;
    s32 status;
    s32 value;
    u16 *found;
    u16 *marker;
    u16 *entry;
    u16 *list;
    DR_AREA *area1;
    DR_AREA *area2;

    height = (s32)arg5;
    if (gTextScriptTable != arg1) {
        gTextScriptTable = arg1;
        gTextScriptCursor = arg1;
        gTextScriptIndex = 0;
        entry = arg1;
        if (*entry != 0xFFFF) {
            do {
                if (*entry & 0x4000) {
                    ResolveTextSource(*entry);
                }
                entry++;
                gTextScriptIndex = (u16)(gTextScriptIndex + 1);
            } while (*entry != 0xFFFF);
        }
        gTextPageTimer = 0;
        gTextLineScroll = 0;
        gTextLineCount = 3;
        gTextRendering = 1;
        ResetTextState();
        gTextScrolledFull = 0;
        gTextSlotLoadAsync = 0;
        gTextSlotBuffer = 0;
        gTextTpage = -1;
        gTextSlotImageVisible = 0;
        gTextSlotLoadDone = 0;
        gTextPortraitSelector = 0;
        gTextInstantPageMode = 0;
        gTextHoldAtEnd = 0;
        gTextTypeSoundToggle = 0;
    }
    step = 1;
    status = 0;
    savedState = GetTextSpeedIndex();
    if (gTextInputEnabled != 0) {
        input4 = ((GfxObj *)gPad0Input)->field_4;
        input8 = ((GfxObj *)gPad0Input)->field_8;
    } else {
        input4 = 0;
        input8 = 0;
    }
    value = GetTextState();
    switch (value) {
    case 0:
        status = 0;
        if (input4 & 0x60) {
            SetTextSpeedIndex(0U);
        }
        goto mainbody;
    case 1:
        if (gTextRendering == 0) {
            status = 1;
            if ((input8 & 0x60) && (gTextLineCount >= 4)) {
                gTextRendering = 1;
                gTextLineCount = 0;
                status = 0;
            }
        } else {
            status = 6;
            if (input4 & 0x60) {
                step = 2;
            }
        }
        goto mainbody;
    case 2:
        ClearMessageReadBufferBusy();
        if (input8 & 0x60) {
            status = 5;
            if (gTextPrevState == 2) {
                gTextScriptTable = 0;
                return 2;
            }
            goto mainbody;
        }
        status = 5;
        goto mainbody;
    case 3:
        status = 3;
        if (input8 & 0x60) {
            status = 0;
            EndTextPage();
            list = gTextScriptCursor;
            if (list[gTextScriptIndex - 1] == 0xFFFE) {
                if (gTextScrolledFull == 0) {
                    gTextLineCount = (u16)gTextLineCount -
                        CountTextMarkers(list, 0xFFFE,
                            gTextScriptIndex != 0 ? (s16)gTextScriptIndex : 1, 0);
                } else {
                    gTextLineCount = -1;
                }
            }
        }
        goto mainbody;
    case 7:
    case 8:
    case 9:
        status = gTextPrevState;
        if (UpdateChoiceCursor(arg2, arg3, &gTextChoiceIndex,
                          (s16)(status + 0xFFFBU)) != 0) {
            gTextScriptTable = 0;
            return MapChoiceIndexToStatus((gTextChoiceIndex + 1) & 0xFFFF) & 0xFFFF;
        }
        goto mainbody;
    default:
    mainbody:
        gTextPrevState = (u16)value;
        SeekTextToBookmark();
        EnterChoiceState();
        textY = arg3;
        if (gTextInstantPageMode == 0) {
            if (gTextRendering == 1) {
                if (CountTextMarkers(gTextScriptCursor, 0xFFFE,
                        gTextScriptIndex != 0 ? (s16)gTextScriptIndex : 1, 0) >= 4U) {
                    gTextScrolledFull = 1;
                    if (gTextPageTimer == 0) {
                        BeginTextRun();
                        gTextLineCount = (u16)gTextLineCount + 1;
                    }
                    if (HasVSyncIntervalElapsed(1, &gTextPageTimer, 1) == 1) {
                        gTextLineScroll = (u16)gTextLineScroll + step;
                    }
                    if ((s16)gTextLineScroll >= 0xE) {
                        EndTextPage();
                        gTextScriptCursor = (u16 *)SkipTextPage(gTextScriptCursor, 1);
                        rawStatus.value = ~status;
                        result = (u16)~rawStatus.value;
                        rawX.value = 4 - arg2;
                        cursorX = -rawX.value;
                        rawY.value = 0x54 - arg3;
                        cursorY = -rawY.value;
                        gTextPageTimer = 0;
                        gTextLineScroll = 0;
                        goto tailmerge;
                    } else {
                        textY = (s32)(s16)(arg3 - (u16)gTextLineScroll);
                        goto tail_189E4;
                    }
                } else {
                    gTextLineScroll = 0;
                    goto tail_189E4;
                }
            } else {
                gTextLineScroll = 0;
                goto tail_189E4;
            }
        } else {
            rawStatus.value = ~status;
            result = (u16)~rawStatus.value;
            rawX.value = -arg2;
            cursorX = -(rawX.value + 4);
            rawY.value = 0x54 - textY;
            cursorY = -rawY.value;
            if (gTextRendering == 1) {
                gTextScrolledFull = gTextRendering;
                gTextLineCount = 4;
                marker = FindTextToken(gTextScriptCursor, 0xFFC, 1, 0);
                if (gTextPageTimer != 0) {
                    found = FindTextToken(gTextScriptCursor, 0xFFFE, 4, 0);
                    if (found != NULL) {
                        if (found[1] != 0xFFFF) {
                            if ((marker != NULL) && ((u32)marker < (u32)found)) {
                                found = FindTextToken(marker, 0xFFFE, 1, 0);
                            }
                            gTextScriptCursor = found + 1;
                        }
                    } else if (marker != NULL) {
                        gTextScriptCursor = marker + 1;
                        if (*gTextScriptCursor == 0xFFFE) {
                            gTextScriptCursor++;
                        }
                    }
                }
                gTextPageTimer = 1;
                marker = FindTextToken(gTextScriptCursor, 0xFFC, 1, 0);
                found = FindTextToken(gTextScriptCursor, 0xFFFE, 4, 0);
                if (found != NULL) {
                    if ((marker != NULL) && ((u32)marker < (u32)found)) {
                        gTextScriptIndex =
                            (u16)MeasureTextSpanLength(gTextScriptCursor, 0xFFC, 0);
                        rawStatus.value = ~status;
                        result = (u16)~rawStatus.value;
                        rawX.value = 4 - arg2;
                        cursorX = -rawX.value;
                        rawY.value = 0x54 - arg3;
                        cursorY = -rawY.value;
                        goto tailmerge;
                    } else {
                        {
                            u16 *cur;
                            s32 i;
                            s32 next;

                            i = 0;
                            value = 0;
                            cur = gTextScriptCursor;
                            rawStatus.value = ~status;
                            result = (u16)~rawStatus.value;
                            rawX.value = 4 - arg2;
                            cursorX = -rawX.value;
                            rawY.value = 0x54 - arg3;
                            cursorY = -rawY.value;
                            do {
                                value = value + MeasureTextSpanLength(cur, 0xFFFE, 0);
                                next = i + 1;
                                if (i < 3 && *cur != 0xFFFE) {
                                    while (*++cur != 0xFFFE) {
                                    }
                                }
                                cur++;
                                i = next;
                            } while (next < 4);
                            gTextScriptIndex = value;
                        }
                        goto tailmerge;
                    }
                } else {
                    found = FindTextToken(gTextScriptCursor, 0xFFFF, 1, 0);
                    gTextScriptIndex = (u16)MeasureTextSpanLength(gTextScriptCursor,
                        (marker != NULL && (u32)marker < (u32)found) ? 0xFFC : 0xFFFF, 0);
                    goto tail_189E4;
                }
            }
            goto tailmerge;
        }
    tail_189E4:
        rawStatus.value = ~status;
        result = (u16)~rawStatus.value;
        rawX.value = 4 - arg2;
        cursorX = -rawX.value;
        rawY.value = 0x54 - arg3;
        cursorY = -rawY.value;
    tailmerge:
        if ((gTextHoldAtEnd == 0) && ((s16)gTextLineCount >= 4)) {
            gTextRendering = 0;
            BeginTextRun();
        }
        area1 = (DR_AREA *)ReservePacketWorkBytes(0xC);
        area2 = (DR_AREA *)ReservePacketWorkBytes(0xC);
        rect.x = arg2;
        rect.y = arg3 + (s16)gActiveRenderContext->field_2;
        rect.w = 0x140 - arg2;
        rect.h = (s16)height;
        SetDrawArea(area2, &rect);
        SetDrawArea(area1, (RECT *)gActiveRenderContext);
        addPrim(arg0, area1);
        DrawTextWindowLines((s32)arg0, (s32)gTextScriptCursor, arg2, textY);
        addPrim(arg0, area2);
        SetTextSpeedIndex(savedState);
        LoadTextSlot(gTextTpage, gTextSlotLoadAsync);
        DrawTextSlotImage((u_long *)gActiveRenderContext->field_D8, (s16)cursorX, (s16)cursorY,
                      gTextSlotImageVisible != 0 ? gTextTpage : -1);
        gTextPublishedEndToken = gTextEndToken;
        return result;
    }
}

void ClearTextScriptPointer(void) {
    gTextScriptTable = 0;
}

/* TU-owned GP globals (tentative definitions -> .comm, so cc1 emits the
 * target's %gp_rel accesses). Types match include/globals.h where present. */
s16 gTextPenX;
s16 gTextPenY;
s16 gTextRevealCount;
u32 gTextTypeTimer;
u16 gTextLineStartX;
u16 gTextPauseIndex;

/* Game callees. */
s16 GetPairedTpage(s32 tpage);
void BindTextSlotImage(s16 arg0);
void *DrawGlyphSprite(s32 *ordering_table, u8 *packet, u32 glyph, s32 x,
                    s16 y, u8 red, u8 green, u8 blue, u32 palette,
                    s32 semitransparent);
s32 ResolveTextSource(s32 arg0);

u8 *DrawTextStream(s32 *arg0, u8 *arg1, u16 *arg2, s16 arg3, s16 arg4,
                  s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    /* Keep the raw halfword separate from the word-sized decode/return scratch. */
    struct {
        u16 value;
    } token;
    s32 terminator;
    s32 xProduct;
    s32 work0;
    s32 work1;
    s32 work2;
    s32 work3;
    s32 work4;
    s32 work5;
    s32 x;
    u16 remasked;
    u32 flag;
    u32 flagtmp;
    u16 glyph;

    if (arg8 == 0) {
        gTextLineStartX = (u16)arg3;
        if (arg6 == 1) {
            gTextRevealCount = 0;
            if (gTextState == 0) {
                if (HasVSyncIntervalElapsed(gTextSpeedInterval, &gTextTypeTimer, 1) == arg6) {
                    if (gTextTypeSoundToggle != 0) {
                        PlaySoundEffect(4);
                        gTextTypeSoundToggle = 0;
                    } else {
                        gTextTypeSoundToggle = arg6;
                    }
                    gTextScriptIndex = (u16)(gTextScriptIndex + 1);
                } else if (arg2[gTextScriptIndex] >= 0xE01U ||
                           (arg2[0] & 0x4000) != 0) {
                    gTextScriptIndex = (u16)(gTextScriptIndex + 1);
                }
            }
        }
    }

    flagtmp = (u32)gTextState - 7;
    flag = flagtmp < 3U;

    gTextPenX = arg3;
    gTextPenY = (u16)arg4;

    if (arg6 != 1) {
        goto inner;
    } else {
        goto outer_test;
    }

outer_body:
    work5++;
    gTextRevealCount = (u16)work5;
    if (gTextState == 1) {
        /* advance */
    } else if ((s16)gTextPauseIndex == (s16)work5) {
        /* advance */
    } else if (flag != 0) {
        /* advance */
    } else {
        gTextState = 0;
    }

    do {
inner:
        token.value = *arg2;
        work0 = token.value;
        terminator = 0xFFFF;
        work2 = work0 & 0xFFFF;
        work1 = work0;

        work0 = terminator;
        if (work2 == work0) {
            if (arg6 != 0 && arg8 == 0 && flag == 0) {
                gTextState = 2;
            }
            goto done;
        }

        if (work2 == 0xFFFE) {
            gTextPenY = gTextLineSpacing + ((u16)gTextPenY + 0xC);
            gTextPenX = (u16)gTextLineStartX;
            if (arg9 != 0) {
                gTextPenX = gTextBlockOriginX;
                arg2++;
                goto loop_latch;
            }
            goto advance;
        }

        if (work2 == 0xFFC) {
            if (gTextState == 1) {
                goto advance;
            }
            gTextState = 3;
            gTextPauseIndex = (u16)gTextRevealCount;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFFB) {
            goto advance;
        }

        if ((u32)((work1 - 0xFF8) & 0xFFFF) < 3U) {
            goto advance;
        }

        if ((u32)((work1 - 0xE38) & 0xFFFF) < 0x1B8U) {
            glyph = *(volatile u16 *)arg2;
            arg2++;
            gTextTpage = GetPairedTpage(glyph);
            goto loop_latch;
        }

        if (work2 == 0xFF7) {
            BindTextSlotImage(gTextTpage);
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF6) {
            gTextSlotImageVisible = 0;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF5) {
            gTextSlotLoadAsync = 1;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF4) {
            gTextSlotImageVisible = 1;
            arg2++;
            goto loop_latch;
        }

        if ((u32)((work1 - 0xE25) & 0xFFFF) < 0xBU) {
            gTextPortraitSelector = (s16)(0xE2F - work1);
            arg2++;
            goto loop_latch;
        }

        remasked = work1 & 0xFFFF;
        if (work2 == 0xFF3) {
            gTextInstantPageMode = 1;
            arg2++;
            goto loop_latch;
        }

        if (remasked == 0xFF2) {
            goto advance;
        }

        if (remasked == 0xFF1) {
            gTextHoldAtEnd = 1;
            arg2++;
            goto loop_latch;
        }

        if (work1 & 0x4000) {
            work4 = ResolveTextSource(*arg2);
            if (work4 == 0) {
                work4 = gTextSourceAddresses[*arg2 & 0xFFF];
                if (work4 == 0) {
                    goto advance;
                }
            }
            arg1 = DrawTextStream(
                arg0, arg1, (u16 *)work4, gTextPenX, (s16)gTextPenY,
                arg5, arg6, arg7, 1, 0);
            goto advance;
        }

        if ((work1 & 0xFFF) != 0xFFD) {
            xProduct = (gTextPenX - arg3) * arg5;
            if (xProduct < 0) {
                xProduct += 0xFFF;
            }
            x = (s32)(s16)(arg3 + (xProduct >> 12));
            work3 = (gTextPenY - arg4) * arg5;
            if (work3 < 0) {
                work3 += 0xFFF;
            }
            arg1 = (u8 *)DrawGlyphSprite(arg0, arg1, *arg2, x,
                                         (s16)(s32)(s16)(arg4 + (work3 >> 12)),
                                         0x80, 0x80, 0x80, gTextPalette, arg7);
        }
        gTextPenX = gTextLetterSpacing + ((u16)gTextPenX + 8);

advance:
        arg2++;
loop_latch:
        ;
    } while (arg6 != 1);
outer_test:
    work5 = (u16)gTextRevealCount;
    if ((u16)gTextScriptIndex > (s16)gTextRevealCount) {
        goto outer_body;
    }
done:
    gTextEndToken = arg2[-1];
    work1 = (s32)arg1;
    return (u8 *)work1;
}

/* TU-owned globals (GP-relative — tentative definitions). */

s16 GetTextRowY(void) {
    s16 result;
    s32 tmp;

    result = gTextPenY;

    if (gTextScriptCursor[gTextScriptIndex - 1] == 0xFFFE) {
        tmp = result - 12;
        result = (s16)(tmp - gTextLineSpacing);
    }

    return result;
}

/*
 * ASSEMBLY EXCEPTION: the historical scheduling-ceiling argument is
 * superseded by a no-asm candidate with exact instruction order and one
 * allocation difference (79/81). See notes/asm-exception-evidence.md for
 * its scope; that result does not prove that clean C is impossible.
 *
 * The active hybrid retains C for sprite/CLUT/branch work and existing asm
 * for the entry and interleaved packet-linking windows. Eight redundant
 * bindings have been removed together, leaving eight explicit bindings.
 * The raw tail still names registers literally: removing a binding does
 * not remove that machine-state requirement. The tail spans multiple SDK
 * operations, so it has not been hidden behind a generic packet macro.
 * Historical research: notes/research/func_80019070-prologue-allocation-and-arg2-truncation.md.
 */

/*
 * Existing exact hybrid with a measured reduction of redundant bindings.
 * This function remains explicitly allowlisted for register-asm and
 * embedded-asm in .pi/autoloop.json; it is not a general policy precedent.
 */
void *DrawGlyphSprite(s32 *ordering_table, u8 *packet, u32 glyph,
                    s32 x, s16 y, u8 red, u8 green, u8 blue,
                    u32 palette, s32 semitransparent)
{
    SPRT *sprt;
    register s32 *ot __asm__("$11");
    register u32 glyph_reg __asm__("$6");
    register s16 sprite_x __asm__("$13");
    register u32 texture_load __asm__("$14");
    u32 texture_u;
    register u32 palette_load __asm__("$5");
    u32 palette_index;
    u32 header_len;
    register u32 header_code __asm__("$3");
    u32 address_mask;
    register u32 uv_u __asm__("$3");
    u32 uv_v;
    s32 semitrans_reg;
    register s16 y_reg __asm__("$12");
    u32 red_reg;
    u8 green_reg;
    u8 blue_reg;
    u8 code;
    DR_TPAGE *tpage;

    __asm__ volatile(
        "addu %0,$5,$zero\n\t"
        "addiu %1,$zero,4\n\t"
        "addiu %2,$zero,100\n\t"
        "addu %3,$4,$zero\n\t"
        "andi %4,$6,0xffff\n\t"
        "sll $7,$7,16\n\t"
        "sra %5,$7,16\n\t"
        "andi %6,%4,15\n\t"
        "andi %4,%4,240\n\t"
        : "=r"(sprt), "=r"(header_len), "=r"(header_code),
          "=r"(ot), "=r"(glyph_reg), "=r"(sprite_x),
          "=r"(texture_load)
        :
        : "$7", "memory");
    __asm__ volatile("lw %0,32($sp)" : "=r"(palette_load) : : "memory");

    texture_u = texture_load;
    palette_index = palette_load;
    setlen(sprt, header_len);
    setcode(sprt, header_code);
    __asm__ volatile(
        "lw %0,36($sp)\n\t"
        "lh %1,16($sp)\n\t"
        "lbu %2,20($sp)\n\t"
        "lbu %3,24($sp)\n\t"
        "lbu %4,28($sp)"
        : "=r"(semitrans_reg), "=r"(y_reg), "=r"(red_reg),
          "=r"(green_reg), "=r"(blue_reg)
        :
        : "memory");
    glyph_reg >>= 4;

    if (palette_index >= 6) {
        palette_index = 0;
    }

    setClut(sprt, 0x380, gTextPaletteClutRows[palette_index]);
    setRGB0(sprt, red_reg, green_reg, blue_reg);
    __asm__ volatile("" ::: "memory");

    code = 0x64;
    if (semitrans_reg != 0) {
        code = 0x66;
    }

    setcode(sprt, code);
    __asm__ volatile("" ::: "memory");
    __asm__ volatile(
        "lui %0,0xff\n\t"
        "ori %0,%0,0xffff\n\t"
        "lui %1,0xe100"
        : "=r"(address_mask), "=r"(red_reg));
    uv_u = texture_u * 8;
    uv_v = (glyph_reg * 3) << 2;
    __asm__ volatile(
        "sb $3,12($8)\n\t"
        "addiu $3,$zero,8\n\t"
        "sb $2,13($8)\n\t"
        "addiu $2,$zero,12\n\t"
        "lui $6,0xff00\n\t"
        "sh $3,16($8)\n\t"
        "lw $3,0($8)\n\t"
        "and $5,$8,$4\n\t"
        "sh $13,8($8)\n\t"
        "sh $12,10($8)\n\t"
        "sh $2,18($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $3,$3,$6\n\t"
        "and $2,$2,$4\n\t"
        "or $3,$3,$2\n\t"
        "sw $3,0($8)\n\t"
        "addiu $8,$8,20\n\t"
        "lw $2,0($11)\n\t"
        "addiu $3,$zero,1\n\t"
        "and $2,$2,$6\n\t"
        "or $2,$2,$5\n\t"
        "sw $2,0($11)\n\t"
        "sb $3,3($8)\n\t"
        "lw $3,0($8)\n\t"
        "ori $7,$7,0x60e\n\t"
        "sw $7,4($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $3,$3,$6\n\t"
        "and $2,$2,$4\n\t"
        "or $3,$3,$2\n\t"
        "sw $3,0($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $4,$8,$4\n\t"
        "and $2,$2,$6\n\t"
        "or $2,$2,$4\n\t"
        "sw $2,0($11)"
        : "+r"(sprt)
        : "r"(ot), "r"(address_mask), "r"(red_reg),
          "r"(sprite_x), "r"(y_reg), "r"(uv_u), "r"(uv_v)
        : "memory");

    return (u8 *)sprt + sizeof(DR_TPAGE);
}

s32 MeasureTextSpanLength(u16 *arg0, u16 arg1, s16 arg2);

u16 *FindTextToken(u16 *arg0, u16 arg1, s32 arg2, s32 arg3) {
    u16 *cur;
    u16 *base;
    u16 *result;
    s32 temp;
    s32 count;
    u16 sentinel;

    cur = arg0;
    base = cur;
    count = 0;
    result = 0;
    if (arg2 == 0) {
        return 0;
    }
    sentinel = 0xFFFF;
    for (;;) {
        temp = MeasureTextSpanLength(base, *cur, 0);
        if (arg3 != 0 && temp >= arg3) {
            break;
        }
        if (*cur == sentinel) {
            if (arg1 == *cur) {
                result = cur;
            }
            break;
        }
        if (*cur == arg1) {
            count++;
            if (count >= arg2) {
                result = cur;
                break;
            }
        }
        cur++;
    }
    return result;
}

/* Game callee (signature from include/functions.h; local declaration per
 * project convention). */

s32 CountTextMarkers(u16 *arg0, u16 arg1, s16 arg2, u16 *arg3) {
    u16 sp10;
    u16 *cur;
    s32 sum;
    s32 count;
    s32 temp;
    s32 masked;
    s32 r;

    cur = arg0;
    sum = 0;
    count = 0;

    for (;;) {
        if (arg2 == 0 || count < arg2) {
            temp = *cur;
            masked = temp & 0xFFFF;
            if (masked == 0xFFFF) {
                count++;
                break;
            }
            if (masked == arg1) {
                sum = (sum + 1) & 0xFFFF;
            } else if (masked != 0xFFFE && (temp & 0x4000) &&
                       ((r = ResolveTextSource(*(volatile u16 *)cur)) != 0 ||
                        (r = gTextSourceAddresses[*cur & 0xFFF]) != 0)) {
                if (arg2 != 0) {
                    sum = (sum + CountTextMarkers((u16 *)r, arg1, (s16)(arg2 - count), &sp10)) & 0xFFFF;
                    count += sp10;
                } else {
                    sum = (sum + CountTextMarkers((u16 *)r, arg1, 0, 0)) & 0xFFFF;
                }
            }
            count++;
            cur++;
        } else {
            break;
        }
    }
    if (arg3 != 0) {
        *arg3 = count;
    }
    return sum;
}

/* Extern callees (matched definitions: src/func_800191B4.c, src/func_8001945C.c). */
u16 *FindTextToken(u16 *arg0, u16 arg1, s32 arg2, s32 arg3);

/* Shared u16 global — tentative definition (project idiom for GP-relative
 * access; also defined in func_80019030.c and func_80018B98.c). */

s32 SkipTextPage(u16 *arg0, s32 arg1) {
    s32 result;
    s32 temp;

    result = FindTextToken(arg0, 0xFFFE, arg1, 0);
    temp = (MeasureTextSpanLength(arg0, 0xFFFE, 0) + 1) & 0xFFFF;
    if (result != 0) {
        result += 2;
        gTextScriptIndex -= temp;
    }
    return result;
}

/* Game callee (signature from include/functions.h; local declaration per
 * project convention). */

/* arg1 is u16: the target's entry `andi s3,a1,0xffff` is the zero-extension
 * conversion emitted in assign_parms' conversion window, which precedes the
 * s16 arg2 decode and gives the target's prologue order (masked arg1 before
 * the count sll/sra).
 *
 * The table fallback keeps the loaded value in its own web (`value`), tests
 * it, and only then merges it into `nin` on the nonzero path: this is what
 * produces the target's `lw v0,0(v0); beqz v0; addu a0,v0,0` — the load
 * lands in $v0 and the move to $a0 survives instead of coalescing away.
 * `goto skip_rec` is required so the shared recursion statement is written
 * once and reached from both the func-result path and the table-value path,
 * matching the target's single use block. */
s32 MeasureTextSpanLength(u16 *arg0, u16 arg1, s16 arg2) {
    u16 *ptr;
    s32 index;
    s32 temp;
    s32 masked;
    s32 nin;
    s32 value;

    ptr = arg0;
    index = 0;

    for (;;) {
        if (arg2 != 0 && index >= arg2) {
            break;
        }
        temp = *ptr;
        masked = temp & 0xFFFF;
        if (masked == 0xFFFF || masked == arg1) {
            index++;
            break;
        }
        if (masked != 0xFFFE && (temp & 0x4000)) {
            nin = ResolveTextSource(*(volatile u16 *)ptr);
            if (nin == 0) {
                value = gTextSourceAddresses[*ptr & 0xFFF];
                if (value == 0) {
                    goto skip_rec;
                }
                nin = value;
            }
            index += MeasureTextSpanLength((u16 *)nin, arg1, (s16)((arg2 != 0) ? (arg2 - index) : 0));
        skip_rec:
            ;
        }
        index++;
        ptr++;
    }
    return index & 0xFFFF;
}

/* Game callees. */

void DrawTextWindowLines(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 x = (s16)arg2;
    s32 y = (s16)arg3;

    SetPacketWorkCursor(DrawTextStream(arg0, ReservePacketWorkBytes(0), arg1, x, y, 0x1000, 1,
                                0, 0, 0));
}

u16 GetTextState(void) {
    return gTextState;
}

void ResetTextState(void) {
    gTextScriptIndex = 0;
    gTextState = 0;
}

s32 DrawCenteredTextRow(s32, s32, s32, s16, s16, s16, s32, s16);

s32 DrawTextRowsCentered(s32 arg0, s32 arg1, u16 *arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7, s16 arg8) {
    TextCopyBlock buf;
    u16 *src;
    u16 *cur;
    u32 found;
    s32 done;

    src = arg2;
    done = 0;
    do {
        buf = *(TextCopyBlock *)src;
        buf.data[40] = 0xFFFF;
        for (found = 0; found < 41; found++) {
            cur = buf.data + found;
            if (*cur == 0xFFFE) {
                *cur = 0xFFFF;
                found++;
                break;
            }
            if (*cur == 0xFFFF) {
                done = 1;
                found++;
                break;
            }
        }
        src += found;
        arg1 = DrawCenteredTextRow(arg0, arg1, (s32)buf.data, arg3, arg4, arg5, arg6, arg8);
        arg4 += arg7;
    } while (done != 1);
    return arg1;
}

/* extern */

void SetTextSpacing(s16 arg0, s16 arg1);

s32 MeasureTextWidth(u16 *arg0, s32 arg1, s16 arg2);

s32 DrawCenteredTextRow(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7) {
    s32 temp_s0;
    s32 temp_s1;

    SetTextSpacing(arg7, 0);
    temp_s0 = (s32) (arg5 - MeasureTextWidth(arg2, arg6, arg7)) / 2;
    temp_s1 = (s32) arg3 + temp_s0;
    SetPacketWorkCursor(DrawTextStream(arg0, ReservePacketWorkBytes(0), arg2, (s16) temp_s1, arg4, arg6, 0, 0, 0, 0));
    return ReservePacketWorkBytes(0);
}

s32 MeasureTextWidth(u16 *arg0, s32 arg1, s16 arg2) {
    s32 temp_a0_2;
    s32 var_s1;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    u16 *var_s0;
    u16 temp_a0;

    var_s0 = arg0;
    var_s1 = 0;
    if ((u32) ((*var_s0 + 2) & 0xFFFF) >= 2U) {
        do {
            temp_a0 = *var_s0;
            if ((temp_a0 & 0xF000) == 0x4000) {
                temp_a0_2 = gTextSourceAddresses[temp_a0 & 0xFFF];
                if (temp_a0_2 != 0) {
                    var_s1 += MeasureTextWidth((u16 *)temp_a0_2, arg1, arg2);
                }
            } else {
                var_v0_2 = arg1;
                if (arg1 < 0) {
                    var_v0_2 = arg1 + 0x1FF;
                }
                var_s1 += var_v0_2 >> 9;
                var_s1 += arg2;
            }
            var_s0++;
        } while ((u32) ((*var_s0 + 2) & 0xFFFF) >= 2U);
    }
    if ((var_s0[-1] & 0xFFF) == 0x6C) {
        var_v0_3 = arg1 * 3;
        if (var_v0_3 < 0) {
            var_v0_3 += 0xFFF;
        }
        var_s1 -= var_v0_3 >> 0xC;
    }
    return var_s1 - arg2;
}

/* extern */

s32 DrawRightAlignedTextRow(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7) {
    s32 temp_s0;

    SetTextSpacing(arg7, 0);
    temp_s0 = (arg3 + arg5) - MeasureTextWidth(arg2, arg6, arg7);
    SetPacketWorkCursor(DrawTextStream(arg0, ReservePacketWorkBytes(0), arg2, (s16) temp_s0, arg4, arg6, 0, 0, 0, 0));
    return ReservePacketWorkBytes(0);
}

s32 DrawRightAlignedTextRow(s32, s32, s32, s16, s16, s16, s32, s16);

s32 DrawTextRowsRightAligned(s32 arg0, s32 arg1, u16 *arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7, s16 arg8) {
    TextCopyBlock buf;
    u16 *src;
    u16 *cur;
    u32 found;
    s32 done;

    src = arg2;
    done = 0;
    do {
        buf = *(TextCopyBlock *)src;
        buf.data[40] = 0xFFFF;
        for (found = 0; found < 41; found++) {
            cur = buf.data + found;
            if (*cur == 0xFFFE) {
                *cur = 0xFFFF;
                found++;
                break;
            }
            if (*cur == 0xFFFF) {
                done = 1;
                found++;
                break;
            }
        }
        src += found;
        arg1 = DrawRightAlignedTextRow(arg0, arg1, (s32)buf.data, arg3, arg4, arg5, arg6, arg8);
        arg4 += arg7;
    } while (done != 1);
    return arg1;
}

void DrawChoiceCursor(s32 arg0, s16 arg1, s16 arg2);

s32 UpdateChoiceCursor(s16 arg0, s16 arg1, u16 *arg2, s16 arg3) {
    SomeStruct *unk;
    s32 result;
    s32 value;
    s32 temp;
    s32 var_v0;
    s32 var_a0;

    result = 0;
    unk = (SomeStruct *)gPad0Input;
    if (unk->field_0x0 & 0x4000) {
        PlaySoundEffect(5);
        var_a0 = 0;
        value = *arg2 + 1;
        *arg2 = value;
        if ((s16)value < arg3) {
            var_a0 = value;
        }
        *arg2 = var_a0;
    } else if (unk->field_0x0 & 0x1000) {
        PlaySoundEffect(5);
        temp = *arg2 - 1;
        *arg2 = temp;
        if ((s16)temp < 0) {
            var_v0 = arg3 - 1;
        } else {
            var_v0 = temp;
        }
        *arg2 = var_v0;
    } else if (unk->field_0x8 & 0x40) {
        PlaySoundEffect(0);
        result = (s16)*arg2 + 1;
    }
    if (result == 0) {
        DrawChoiceCursor(gActiveRenderContext->field_D8, (s16)(arg0 - 2),
                      (s16)(arg1 + ((s16)gTextLineSpacing + 0xC) * (s16)*arg2));
    }
    return result;
}

void DrawChoiceCursor(s32 arg0, s16 arg1, s16 arg2) {
    DrawUiSprite0BFrame1(arg0, (s16)(arg1 + 8), (s16)(arg2 + 6));
}


s32 MapChoiceIndexToStatus(s32 arg0) {
    u32 var_v1;

    var_v1 = (arg0 - 1) & 0xFFFF;
    if (var_v1 >= 4U) {
        var_v1 = 0;
    }
    return gTextChoiceStatusCodes[var_v1];
}

/* GPU primitive initializer: allocate and chain an SPRT sprite with a
 * DR_TPAGE drawing-mode primitive.  When cond == -1 the function is a
 * no-op (no texture / no sprite).  Sibling family at func_8001526C-
 * func_80015644.
 *
 * The field-write order (clut before RGB, XY0 before w/h) is load-bearing:
 * it gives the clut constant a specific local-allocation slot relative to
 * the code/RGB/w-h quantities so the target's register roles (clut in $a1,
 * w/h in $v1, hoisted tag load in $a0) fall out of the scheduler.  The
 * remaining statements use expression order that reproduces the target
 * machine stream byte-for-byte. */
void DrawTextSlotImage(u_long *ot, s16 x0, s16 y0, s16 cond) {
    s32 mask;
    SPRT *s0;
    DR_TPAGE *v0;

    if (cond != -1) {
        s0 = (SPRT *)ReservePacketWorkBytes(0x14);
        v0 = (DR_TPAGE *)ReservePacketWorkBytes(8);
        setSprt(s0);
        s0->clut = 0x7A80;
        setRGB0(s0, 0x80, 0x80, 0x80);
        setXY0(s0, x0, y0);
        s0->w = 0x50;
        s0->h = 0x50;
        s0->u0 = 0;
        s0->v0 = 0xAF;
        mask = 0xFFFFFF;
        s0->tag = (s0->tag & 0xFF000000) | (*ot & mask);
        *ot = (*ot & 0xFF000000) | ((u_long)s0 & mask);
        setlen(v0, 1);
        v0->code[0] = 0xE100068F;
        v0->tag = (v0->tag & 0xFF000000) | (*ot & mask);
        *ot = (*ot & 0xFF000000) | ((u_long)v0 & mask);
    }
}

/*
 * Convert a display-list texture token into its paired texture page by
 * reversing it within the 0xE38..0xFEF range: the lowest token maps to the
 * highest page and vice versa. The display-list interpreter func_80018B98
 * performs the same reversal inline for the 0xE25..0xE2F range
 * (0xE2F - token), storing it to D_8005E470. The result here is tracked in
 * D_8005E454 and consumed as the tpage bits of a GPU draw-mode (0xE1)
 * command by func_80019E80, which treats -1 as "no texture".
 */
s16 GetPairedTpage(s32 tpage) {
    u32 masked;
    s16 result;

    masked = tpage & 0xFFFF;
    result = (s16) (0xFEF - masked);
    return result;
}

void BindTextSlotImage(s16 arg0) {
    if ((gTextUploadedSlot != arg0) && (gTextSlotBuffer != 0)) {
        UploadSpriteImages(gTextSlotBuffer);
        ClearMessageReadBufferBusy();
        gTextUploadedSlot = arg0;
        gTextSlotLoadDone = 0;
    }
}

/* GP-relative CD table state reached GP-relatively by this TU.
 * Tentative definitions merged via -fcommon with sibling definitions. */
u32 gTextPendingSlot;

void LoadTextSlot(s32 arg0, s32 arg1) {
    s32 slot;
    s32 offset;
    u8 *buf;

    slot = (s16) arg0;
    if (slot == gTextLoadedSlot) {
        return;
    }
    if (slot < 0) {
        return;
    }
    offset = slot * 3 * 2048;
    if (arg1 != 0 && gTextSlotLoadDone != 0) {
        goto retry;
    }
    if (GetAvailableMessageReadBuffer() == NULL) {
        SystemError(0x54, 0);
    }
    CdReadArchiveBytes(1, offset, 0x1800, (u8 *)&gMessageWindowReadBuffer, 1, 1);
    gTextSlotLoadDone = 1;
    gTextPendingSlot = slot;
retry:
    while (1) {
        buf = CdReadArchiveBytes(1, offset, 0x1800, (u8 *)&gMessageWindowReadBuffer, 1, 0);
        gTextSlotBuffer = buf;
        if (buf != NULL) {
            gTextLoadedSlot = *(u16 *)&gTextPendingSlot;
            return;
        }
        if (arg1 != 0) {
            return;
        }
    }
}

/* extern */
/* TU-owned globals (GP-relative — tentative definitions). */

void SeekTextToBookmark(void) {
    s32 temp_v0;
    u16 var_a3;
    u16 *ptr;

    var_a3 = 1;
    ptr = gTextScriptCursor;
    if (gTextScriptIndex != 0) {
        var_a3 = gTextScriptIndex;
    }
    temp_v0 = FindTextToken(ptr, 0xFFB, 1, var_a3);
    if (temp_v0 != 0) {
        gTextScriptCursor = temp_v0 + 2;
        gTextPageTimer = 0;
        gTextLineScroll = 0;
        gTextLineCount = 3;
        gTextRendering = 1;
        ResetTextState();
        gTextScrolledFull = 0;
        if (*gTextScriptCursor == 0xFFFE) {
            gTextScriptCursor += 1;
        }
    }
}

/* extern */
                      /* extern */

/* TU-owned globals (GP-relative — tentative definitions). */

void EnterChoiceState(void) {
    u16 *v;
    u16 count;
    u32 i;

    count = 1;
    if (gTextScriptIndex != 0) {
        count = gTextScriptIndex;
    }
    i = 0;
    do {
        v = FindTextToken(gTextScriptCursor, gTextChoiceStartTokens[i], 1, count);
        if (v != 0) {
            gTextScriptCursor = v + 1;
            if (*gTextScriptCursor == 0xFFFE) {
                gTextScriptCursor = v + 2;
            }
            gTextLineCount = 3;
            gTextPageTimer = 0;
            gTextLineScroll = 0;
            gTextRendering = 1;
            gTextScrolledFull = 0;
            gTextChoiceIndex = 0;
            gTextState = gTextChoiceStates[i];
            gTextScriptIndex = MeasureTextSpanLength(gTextScriptCursor, 0xFFFF, 0);
            return;
        }
        i++;
    } while (i < 3);
}

/* Game callees (signatures from include/functions.h; local declaration per
 * project convention). */
s32 LookupItemSlotValue(void);
s16 *WritePaddedNumberText(s32 arg0, s16 *arg1, s32 arg2);
void InsertRecordListText(s32 arg0);

/* Tentative definition: the target reaches D_8005E490 via $gp, so this
 * translation unit owns the symbol. */
s16 gTextNumberBuffer;

s32 ResolveTextSource(s32 arg0) {
    s32 result = 0;
    s32 value;
    s32 neg1;
    unsigned char *dst;

    switch (arg0 & 0xFFF) {
    case 10:
        result = (s32)((unsigned char *)&D_8006C838 + 0x51C8);
        break;
    case 11:
        result = (s32)&D_80075AD8;
        break;
    case 12:
        result = (s32)&D_800759E8;
        break;
    case 13:
        result = (s32)&D_80070D18;
        break;
    case 14:
        result = (s32)&D_8007AFBC;
        break;
    case 15: {
        unsigned char *base;

        /* Two-stage base formation keeps the +0x8000 materialized at
         * runtime (lui/addiu/ori/addu) instead of folding it into the load
         * offset; same idiom as matched sibling func_80021E60. */
        value = (s32)&D_8006C838;
        base = (unsigned char *)(value + 0x8000);
        switch (*(u16 *)(base + 0x676C)) {
        case 0:
            result = (s32)((unsigned char *)&D_8006C838 + 0x51C8);
            break;
        case 1:
            result = D_80054BC0[0] + (s32)&D_8005175C;
            break;
        case 2:
            result = D_80054BC0[0] + (s32)&D_80051768;
            break;
        default:
            result = 0;
            break;
        }
        break;
    }
    case 16:
        result = (s32)WritePaddedNumberText(LookupItemSlotValue(), &gTextNumberBuffer, 4);
        break;
    case 17:
        result = (s32)WritePaddedNumberText(gPendingItemValueCount, &gTextNumberBuffer, 4);
        break;
    case 18:
        result = (s32)WritePaddedNumberText(gSaveCounter18, &gTextNumberBuffer, 4);
        break;
    case 19:
        result = (s32)WritePaddedNumberText(gSaveCounter19, &gTextNumberBuffer, 4);
        break;
    case 20:
        result = (s32)WritePaddedNumberText(gSaveCounter20, &gTextNumberBuffer, 4);
        break;
    case 21:
        result = (s32)WritePaddedNumberText(gSaveCounter21, &gTextNumberBuffer, 4);
        break;
    case 22: {
        unsigned char *base;
        base = (unsigned char *)&D_8006C838;
        base += 0x8000;
        neg1 = -1;
        value = *(s16 *)(base + 0x19CA);
        goto check_neg1;
    }
    case 23: {
        unsigned char *base;
        base = (unsigned char *)&D_8006C838;
        base += 0x8000;
        value = *(s16 *)(base + 0x19CE);
        neg1 = -1;
    }
    check_neg1:
        if (value != neg1) {
            result = (s32)&D_800749F8 + value * 0xB8;
        }
        break;
    case 24: {
        unsigned char *base;
        base = (unsigned char *)&D_8006C838;
        base += 0x8000;
        result = *(s32 *)(base + 0x6620);
        break;
    }
    case 25:
    case 26: {
        unsigned char *flags;
        dst = (unsigned char *)&D_800749F8;
        flags = (unsigned char *)&D_8006C838;
        flags += 0x81F0;
        value = 0;
        while (value < 20) {
            if (*(s32 *)flags & 0x400) {
                result = (s32)dst;
            }
            dst += 0xB8;
            value++;
            flags += 0xB8;
        }
        break;
    }
    case 33: {
        unsigned char *base;
        s32 address;
        s16 *dest;
        dest = &gTextNumberBuffer;
        address = (s32)&D_8006C838;
        base = (unsigned char *)(address + 0x8000);
        result = (s32)WritePaddedNumberText(*(s16 *)(base + 0x26DA) + 1, dest, 4);
        break;
    }
    case 27:
        InsertRecordListText(0);
        result = (s32)gTextRecordListBuffer;
        break;
    case 28:
        InsertRecordListText(1);
        result = (s32)gTextRecordListBuffer;
        break;
    case 29:
        InsertRecordListText(2);
        result = (s32)gTextRecordListBuffer;
        break;
    case 30:
        InsertRecordListText(3);
        result = (s32)gTextRecordListBuffer;
        break;
    case 31:
        InsertRecordListText(4);
        result = (s32)gTextRecordListBuffer;
        break;
    case 32:
        InsertRecordListText(5);
        result = (s32)gTextRecordListBuffer;
        break;
    case 34: {
        unsigned char *f;
        unsigned char *flags;
        s32 mask;

        mask = 0x20000;
        f = (unsigned char *)&D_8006C838;
        flags = f + 0x7AB8;
        value = 0;
        while (value < 10) {
            if (*(s32 *)(flags + 0x30) & mask) {
                result = (s32)flags;
            }
            value++;
            flags += 0xB4;
        }
        break;
    }
    default:
        result = 0;
        break;
    }
    return result;
}

/* Tentative definitions for GP-relative access (target uses %gp_rel) */

void InsertRecordListText(s32 arg0) {
    s32 quotient = arg0 / 3;
    s32 product;
    s32 offset;
    FuncPtr_80049078 *base;
    FuncPtr_80049078 *entry;
    u16 *p_table;
    u16 *p_gap;
    s32 fn_result;
    s32 gap_count;
    s32 byte_size;
    s32 sentinel;

    /* Clear sentinel. */
    gTextRecordListBuffer[0] = 0xFFFF;

    offset = gTextScriptIndex * 2;
    base = D_80049078;
    product = quotient * 3;
    p_table = (u16 *)((unsigned char *)gTextScriptCursor + offset);
    product = arg0 - product;
    entry = &base[product];
    fn_result = (*entry)(quotient);

    /* Scan gap: count consecutive non-0xFFFF entries after first */
    gap_count = 0;
    if (*p_table != 0xFFFF) {
        sentinel = 0xFFFF;
        p_gap = p_table;
        do {
            gap_count++;
            p_gap++;
        } while (*p_gap != sentinel);
    }

    /* Shift data to fill the gap (length is the scanned gap count) */
    byte_size = fn_result * 2;
    memmove((unsigned char *)p_table + byte_size, (unsigned char *)p_table + 2, gap_count * 2);

    /* Copy sentinel into the gap */
    memcpy((unsigned char *)p_table, (unsigned char *)&gTextRecordListBuffer[0], byte_size);

    /* Restore sentinel */
    gTextRecordListBuffer[0] = 0xFFFF;
}

u16 *AppendRecordNameText(u16 *arg0, s32 arg1, s32 arg2);

s32 BuildUnflaggedRecordListText(s32 arg0) {
    u16 *var_a0;
    s32 var_s0;
    s32 i;

    var_a0 = gTextRecordListBuffer;
    var_s0 = (s32)&gTaggedRecordBase;
    for (i = 0; i < 20; i++) {
        if (!(*(s32 *)((char *)var_s0 + 0x34) & 0x10000)) {
            var_a0 = AppendRecordNameText(var_a0, var_s0, arg0);
        }
        var_s0 += 0xB8;
    }
    return var_a0 - gTextRecordListBuffer;
}

s32 BuildFlaggedRecordListText(s32 arg0) {
    u16 *var_a0;
    s32 var_s0;
    s32 i;

    var_a0 = gTextRecordListBuffer;
    var_s0 = (s32)&gTaggedRecordBase;
    for (i = 0; i < 20; i++) {
        if (*(s32 *)((char *)var_s0 + 0x34) & 0x10000) {
            var_a0 = AppendRecordNameText(var_a0, var_s0, arg0);
        }
        var_s0 += 0xB8;
    }
    return var_a0 - gTextRecordListBuffer;
}

s32 BuildSmallRecordListText(s32 arg0) {
    u16 *var_a0;
    s32 var_s1;
    s32 i;

    var_a0 = gTextRecordListBuffer;
    var_s1 = (s32)&gSmallTaggedRecordBase;
    for (i = 0; i < 10; i++) {
        var_a0 = AppendRecordNameText(var_a0, var_s1, arg0);
        var_s1 += 0xB4;
    }
    return var_a0 - gTextRecordListBuffer;
}

u16 *AppendRecordNameText(u16 *arg0, s32 arg1, s32 arg2) {
    s32 temp_v1;
    s32 flag_100;
    s32 flag_bit;

    temp_v1 = *(s32 *)((char *)arg1 + 0x34);
    flag_100 = temp_v1 & 0x100;
    if (arg2) {
        flag_bit = (((u32)temp_v1 >> 19) ^ 1) & 1;
    } else {
        flag_bit = temp_v1 & 0x80000;
    }
    if (flag_100 && flag_bit) {
        arg0 = AppendU16String(AppendU16String(arg0, (u16 *)(arg1 + 4)), gTextRecordNameSuffix);
    }
    return arg0;
}

s16 *WriteDecimalText(s32 arg0, s16 *arg1, s32 arg2);

s16 *WritePaddedNumberText(s32 arg0, s16 *arg1, s32 arg2) {
    s16 *p;

    p = WriteDecimalText(arg0, arg1, arg2 - 1);
    *(u16 *)p = 0xFFFF;
    if ((u16)*arg1 != 0xFFD) {
        return arg1;
    }
    do {
        arg1++;
    } while ((u16)*arg1 == 0xFFD);
    return arg1;
}


s32 ConvertAsciiToTextCode(s32 arg0) {
    u8 ch = arg0;
    u8 flags = D_8005E025[ch];

    if (flags & 4) {
        return ch + 0x10;
    }
    if (flags & 3) {
        return (u16)(ch - 0x41);
    }

    switch (ch - 0xA) {
        case 23:
            return 99;
        case 53:
            return 98;
        case 35:
            return 105;
        case 30:
            return 106;
        case 31:
            return 107;
        case 0:
            return 0xFFFE;
        default:
            return 4093;
    }
}

s16 *WriteDecimalText(s32 arg0, s16 *arg1, s32 arg2) {
    s32 negative;
    s32 i;
    s32 seen_nonzero;
    s8 buffer[16];

    /* Cap arg2 */
    if (arg2 >= 10) {
        arg2 = 10;
    }

    /* Check sign and negate */
    negative = (arg0 >> 31) & 1;
    if (negative) {
        arg0 = -arg0;
    }

    /* Convert to base-10 digits (for loop) */
    for (i = 0; i < arg2; i++) {
        buffer[i] = arg0 % 10;
        arg0 /= 10;
    }

    /* Store special marker if negative */
    if (negative) {
        buffer[arg2 - 1] = 10;
    }

    /* Convert digits to display characters */
    seen_nonzero = 0;
    i = arg2 - 1;
    if (i >= 0) {
        do {
            if ((u8)buffer[i] == 10) {
                *arg1 = 0x69;
            } else if ((seen_nonzero == 0) && ((u8)buffer[i] == 0) && (i != 0)) {
                *arg1 = 0xFFD;
            } else {
                seen_nonzero += (u8)buffer[i];
                *arg1 = (u8)buffer[i] + 0x40;
            }
            arg1++;
        } while (--i >= 0);
    }

    return arg1;
}


u16 *WriteSeasonNameText(s32 arg0, u16 *arg1) {
    s32 var_a0;
    u16 *var_v1;
    u16 temp_v0;

    var_v1 = (u16 *)((u8 *)&gTextSeasonNames + arg0 * 0xC);
    var_a0 = 5;
    do {
        temp_v0 = *var_v1;
        var_v1 = (u16 *)((u8 *)var_v1 + 2);
        var_a0 -= 1;
        *arg1 = temp_v0;
        arg1 = (u16 *)((u8 *)arg1 + 2);
    } while (var_a0 >= 0);
    return arg1;
}


u16 *WriteWeekdayNameText(s32 arg0, u16 *arg1) {
    s32 var_a0;
    u16 *var_v1;
    u16 temp_v0;

    var_v1 = (u16 *)((u8 *)&gTextWeekdayNames + arg0 * 6);
    var_a0 = 2;
    do {
        temp_v0 = *var_v1;
        var_v1 = (u16 *)((u8 *)var_v1 + 2);
        var_a0 -= 1;
        *arg1 = temp_v0;
        arg1 = (u16 *)((u8 *)arg1 + 2);
    } while (var_a0 >= 0);
    return arg1;
}

extern s16 *WriteDecimalText(s32 arg0, s16 *arg1, s32 arg2);
extern void DrawTextLine(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void DrawNumberText(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4) {
    s16 *it;
    s16 *p;
    s32 digits;
    s32 i;

    if (arg1 != -1) {
        if (arg1 >= 10) {
            return;
        }
        digits = arg1;
    } else {
        digits = 9;
    }

    it = (s16 *)&gTextNumberDrawBuffer;
    *((u16 *)WriteDecimalText((s32)arg0, it, digits)) = 0xFFFF;

    p = it;
    if (arg1 == -1) {
        i = 0;
        if (digits > 0 && *(u16 *)&gTextNumberDrawBuffer == 0xFFD) {
            do {
                i++;
                p++;
            } while (i < digits && (u16)*p == 0xFFD);
        }
    }

    DrawTextLine(arg2, (s32)p, (s32)arg3, (s32)arg4);

}

void CopyTextString(u16 *dst, u16 *src) {
    u16 ch;

    do {
        ch = *src++;
        *dst++ = ch;
    } while (ch != 0xFFFF);
}

/* Game callees (signatures from include/functions.h; not included to avoid
 * SDK type conflicts — project convention, see func_80011370.c). */
void SetTextPalette(u32 arg0);
u32 GetTextPalette(void);
void DrawTextLine(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void DrawMenuPanelWithText(u32 *arg0, s32 arg1, s32 arg2) {
    u32 temp_s3;

    temp_s3 = GetTextPalette();
    SetTextPalette(3U);
    DrawMenuPanel(arg0, 0, 0x1A, 0xA4, 0x10B, 0x3E);
    DrawTextLine(arg1, arg2, 0x1E, 0xA8);
    SetTextPalette(temp_s3);
}

void ResetTextSlotState(void) {
    s16 temp_v1;
    s16 temp_v0;

    temp_v1 = -1;
    temp_v0 = -3;
    gTextTpage = temp_v1;
    gTextLoadedSlot = temp_v1;
    gTextUploadedSlot = temp_v0;
}

void ResetTextSlotState(void);

s16 GetPairedTpage(s32 arg0);

void LoadTextSlot(s32 arg0, s32 arg1);

void LoadTextSlotForTpage(u16 arg0) {
    s16 callRet3;
    ResetTextSlotState();
    callRet3 = GetPairedTpage(arg0);
    LoadTextSlot(callRet3, 0);
    BindTextSlotImage(callRet3);
}

void DrawTextSlotImage(u_long *ot, s16 x0, s16 y0, s16 cond);

void DrawTextSlotImageAt(s32 arg0, u16 arg1, s16 arg2, s16 arg3) {
    s16 callRet4;
    callRet4 = GetPairedTpage(arg1);
    DrawTextSlotImage((u_long *)arg0, ((s16)(arg2 + -4)), ((s16)(arg3 + -0x54)), callRet4);
}

u16 GetTextSlotMarker(void);

void BindTextSlotImage(s16 slot);

s32 EnsureTextSlotLoaded(s32 arg0) {
    s16 temp_s0;
    s16 temp_v0;
    s32 masked;

    masked = arg0 & 0xFFFF;
    temp_s0 = GetTextSlotMarker();
    temp_v0 = GetPairedTpage(masked);
    if (temp_s0 != temp_v0) {
        LoadTextSlot(temp_v0, 1);
    }
    if (gTextLoadedSlot == temp_v0) {
        BindTextSlotImage(temp_v0);
        return 1;
    }
    return 0;
}

/* Forward declarations matching the callees' own definitions. */

void DrawTextSlotImageForToken(u32 *arg0, u16 arg1, s16 arg2, s16 arg3) {
    DrawTextSlotImage(arg0, arg2, arg3, GetPairedTpage(arg1));
}

/* Scans three data arrays for an entry whose first s16 field is in [0x15, 0x1A).
 * Returns the s16 at offset 4 of the first matching entry, or 0 if none found.
 * Each entry is 6 bytes (3 halfwords) with s16 fields at offsets 0 and 4. */
s32 LookupItemSlotValue(void) {
    s32 s0;
    s32 i;
    s32 count;
    s16 *base;
    s16 exponent;
    s16 field0;

    base = (s16 *)0;
    count = 0;
    s0 = 0;
    while (s0 < 3) {
        switch (s0) {
            case 0:
                base = (s16 *)&D_80071A84;
                count = 1;
                break;
            case 1:
                base = (s16 *)&gItemStackSlotBase;
                exponent = base[-0x39];
                count = pow_int(2, exponent + 1);
                break;
            case 2:
                base = (s16 *)&gItemOverflowSlotBase;
                count = 64;
                break;
        }

        for (i = 0; i < count; i++) {
            field0 = base[i * 3];
            if (field0 < 0x1A) {
                if (field0 >= 0x15) {
                    return base[i * 3 + 2];
                }
            }
        }
        s0++;
    }

    return 0;
}

void InvalidateTextSlot(void) {
    gTextUploadedSlot = 0x3E8;
}
