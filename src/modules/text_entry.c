/*
 * Text entry and indexed selection, EXE 8002301C..800245F4.
 *
 * Two input modes share preview storage, text/message services and a
 * confirmation prompt. Indexed selection writes a category and item index
 * to a caller record. Text entry appends up to eight u16 character codes
 * selected on a fourteen-column grid with two table-driven character sets.
 * State is GP-owned here; pad, render and text-bank pointers are external.
 *
 * Public lifecycle and buffer constraints: text_entry.h. Evidence for the
 * complete run and evidence-limited boundaries: notes/file-groupings.md.
 * The six existing dead helper stubs remain in original order; moving them
 * into this module is not a claim that their C has been reconstructed.
 */

#include "common.h"
#include "screen_primitives.h"
#include "globals_override.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "include_asm.h"
#include "text_entry.h"

u16 gIndexedSelectionState;
s16 gIndexedSelectionCursor;
u16 gIndexedSelectionConfirmation;

void ResetIndexedSelection(void) {
    gIndexedSelectionState = 0;
    gIndexedSelectionCursor = 0;
    gIndexedSelectionConfirmation = 0;
}

struct IndexedSelectionRecord *gIndexedSelectionRecord;

/* Common definitions preserve the original small-data ownership. */
void BeginIndexedSelection(s32 recordAddress) {
    ResetIndexedSelection();
    gIndexedSelectionRecord = (struct IndexedSelectionRecord *)recordAddress;
    gIndexedSelectionState = 1;
}

void DrawAndProcessIndexedSelection(void);

s32 UpdateIndexedSelection(void) {
    if (gIndexedSelectionState != 0) {
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x6C), 0, 0, (*(s16 *)&HWD0), (*(s16 *)&VWD0), 0, 1);
        DrawAndProcessIndexedSelection();
        switch (gIndexedSelectionState) {
        case 4:
            gIndexedSelectionState = 0;
            return -1;
        case 5:
            gIndexedSelectionState = 0;
            return 1;
        default:
            return 2;
        }
    } else {
        return 0;
    }
}

u16 gTextEntryState;
s32 gTextEntryAllowCancel;
s16 gTextEntryAuxState;
u16 gTextEntryLength;
s32 gTextEntryHasCharacter;
u16 gTextEntryConfirmation;
u16 gTextEntryCursor;
s16 gTextEntryCharacterSet;
s32 gTextEntryAlternateLayout;

void ResetTextEntry(void) {
    gTextEntryState = 0;
    gTextEntryAllowCancel = 1;
    gTextEntryConfirmation = 0;
    gTextEntryCursor = 0;
    gTextEntryCharacterSet = 0;
    gTextEntryAuxState = 0;
    gTextEntryLength = 0;
    gTextEntryAlternateLayout = 0;
    gTextEntryHasCharacter = 0;
}

u16 *gTextEntryBuffer;

void ClearTextEntryBuffer(void);

void BeginTextEntryAlternateLayout(u16 *buffer) {
    ResetTextEntry();
    gTextEntryBuffer = buffer;
    ClearTextEntryBuffer();
    gTextEntryAllowCancel = 1;
    gTextEntryAlternateLayout = 1;
    gTextEntryState = 1;
}

void BeginRequiredTextEntry(u16 *buffer) {
    ResetTextEntry();
    gTextEntryBuffer = buffer;
    ClearTextEntryBuffer();
    gTextEntryAllowCancel = 0;
    gTextEntryState = 1;
}

void BeginTextEntry(u16 *buffer) {
    ResetTextEntry();
    gTextEntryBuffer = buffer;
    ClearTextEntryBuffer();
    gTextEntryAllowCancel = 1;
    gTextEntryState = 1;
}

void DrawAndProcessTextEntry(void);

s32 UpdateTextEntry(void) {
    if (gTextEntryState != 0) {
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x6C), 0, 0, (*(s16 *)&HWD0), (*(s16 *)&VWD0), 0, 1);
        DrawAndProcessTextEntry();
        switch (gTextEntryState) {
        case 3:
            gTextEntryState = 0;
            return -1;
        case 4:
            gTextEntryState = 0;
            return 1;
        default:
            return 2;
        }
    } else {
        return 0;
    }
}

void DrawIndexedSelectionValue(void);
void ProcessIndexedSelectionConfirmation(void);
void DrawIndexedSelectionLabels(void);
void ProcessIndexedSelectionInput(void);

void SetVal8005E2BC(s32 arg0);

void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

void DrawAndProcessIndexedSelection(void) {
    s32 temp_s0;
    s16 q;

    temp_s0 = gActiveRenderContext->field_D8 + 0x60;
    DrawIndexedSelectionValue();
    if (gIndexedSelectionState == 3) {
        ProcessIndexedSelectionConfirmation();
        return;
    }
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3A9);
    if (gIndexedSelectionState == 1) {
        s16 v = gIndexedSelectionCursor;
        func_800248B0(temp_s0, (s16) ((v << 6) + 0x24), 0x42);
    } else if (gIndexedSelectionState == 2) {
        q = (s16) (gIndexedSelectionCursor / 7);
        func_800248B0(temp_s0, (s16) (((gIndexedSelectionCursor - q * 7) * 0x27) + 0x24), (s16) (q * 0xE + 0x5E));
    }
    DrawIndexedSelectionLabels();
    ProcessIndexedSelectionInput();
}

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80024A10(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);

/* Existing overlapping views preserve signed/unsigned load widths. */
typedef struct {
    /* 0x0 */ char pad_0[0x2];
    /* 0x2 */ s16 category;
    /* 0x4 */ u16 index;
} IndexedSelectionSignedCategoryView;

void DrawIndexedSelectionValue(void) {
    s32 base;
    s32 sum;

    base = gActiveRenderContext->field_D8 + 0x64;
    func_80022580(gActiveRenderContext->field_D8 + 0x68, 1, 0xAB, 0x20, 0x85, 0x12);
    sum = *gTextBankOffsets + (s32)&D_80053946;
    func_80017B3C(base, sum, 0xAD, 0x23);
    if (gIndexedSelectionState >= 2) {
        func_80024A10(base, 0xFD, 0x23, gIndexedSelectionRecord->category);
    }
    if (gIndexedSelectionState >= 3) {
        func_8001AAF4((s16)(((IndexedSelectionSignedCategoryView *)gIndexedSelectionRecord)->index + 1), 2, base, 0x115, 0x23);
    }
}

s16 *func_8001AA7C(s32 arg0, u16 *arg1);

void DrawIndexedSelectionLabels(void) {
    s32 base;
    s32 var_s1;
    s32 var_s3;
    s32 var_s4;
    s16 end_val;
    s32 inc;
    s32 temp_s0;
    s32 temp_t0;

    temp_t0 = gActiveRenderContext->field_D8;
    base = temp_t0 + 0x64;
    func_80022580(temp_t0 + 0x68, 1, 0x14, 0x38, 0x117, 0x67);
    var_s1 = 0;
    var_s4 = 0x2A0000;
    var_s3 = 0x1E0000;
    end_val = -1;
    inc = 0x400000;
    do {
        func_80024A10(base, var_s3 >> 16, 0x3C, (s16) var_s1);
        *func_8001AA7C(var_s1, &gTextEntryPreviewBuffer[0]) = end_val;
        func_80017B3C(base, (s32) &gTextEntryPreviewBuffer[0], var_s4 >> 16, 0x3C);
        var_s4 += inc;
        var_s1 += 1;
        var_s3 += inc;
    } while (var_s1 < 4);
    var_s1 = 0;
    do {
        temp_s0 = var_s1 + 1;
        func_8001AAF4((s16) temp_s0, 2, base, (s16) (((var_s1 % 7) * 0x27) + 0x1E), (s32) (s16) (((var_s1 / 7) * 0xE) + 0x58));
        var_s1 = temp_s0;
    } while (var_s1 < 0x1E);
}

s32 CommitIndexedSelectionCategory(void);
void CommitIndexedSelectionIndex(void);
void CancelIndexedSelection(void);
s32 ReturnToIndexedSelectionCategory(void);
s16 MoveIndexedSelectionCategoryCursor(s32 a0);
s16 MoveIndexedSelectionItemCursor(s32 a0);

/* Confirmation, cancellation and movement callbacks depend on the current
 * selection level. Only movement's result is consumed; the other callbacks'
 * historical mixed return declarations are retained, not reinterpreted. */
void ProcessIndexedSelectionInput(void) {
    s32 (*var_a3)(void);
    s32 (*var_a2)(void);
    s16 (*var_a1)(s32);
    SomeStruct *gfx;
    s16 prev;
    s16 ret;
    s32 cmd;

    if (IsMessageWindowStatusFinal() != 0) {
        if (gIndexedSelectionState == 1) {
            var_a3 = CommitIndexedSelectionCategory;
            var_a2 = CancelIndexedSelection;
            var_a1 = MoveIndexedSelectionCategoryCursor;
        } else if (gIndexedSelectionState == 2) {
            var_a3 = CommitIndexedSelectionIndex;
            var_a2 = ReturnToIndexedSelectionCategory;
            var_a1 = MoveIndexedSelectionItemCursor;
        } else {
            return;
        }
        cmd = ((SomeStruct *)gPad0Input)->field_0x8;
        if (cmd == 0x40) {
            var_a3();
            PlaySoundEffect(0);
            return;
        }
        if (cmd == 0x20) {
            var_a2();
            PlaySoundEffect(1);
            return;
        }
        prev = gIndexedSelectionCursor;
        gIndexedSelectionCursor = var_a1(prev);
        if (gIndexedSelectionCursor != prev) {
            PlaySoundEffect(5);
        }
    }
}

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", CommitIndexedSelectionCategory);

void CommitIndexedSelectionIndex(void) {
    ((Recon_func_80023710_Pointee0View *)gIndexedSelectionRecord)->unk4 = (u16)gIndexedSelectionCursor;
    gIndexedSelectionCursor = 0;
    gIndexedSelectionConfirmation = 0;
    gIndexedSelectionState = 3;
    CloseMessageWindow();
}

void CancelIndexedSelection(void) {
    gIndexedSelectionState = 4;
    CloseMessageWindow();
}

s32 ReturnToIndexedSelectionCategory(void) {
    gIndexedSelectionState = 1;
    gIndexedSelectionCursor = gIndexedSelectionRecord->category;
    return (s32)gIndexedSelectionRecord;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", MoveIndexedSelectionCategoryCursor);

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", MoveIndexedSelectionItemCursor);

void DrawEntryConfirmationPrompt(s32 arg0, s32 arg1);

typedef struct {
    /* 0x0 */ s32 header;
    /* 0x4 */ u16 index;
} IndexedSelectionHeaderView;

/* Word-width pad reads are intentional: retain this view rather than
 * narrowing loads through PadInputState's individual halfwords. */
typedef struct {
    /* 0x0 */ s32 repeatButtons;
    /* 0x4 */ char pad_4[0x4];
    /* 0x8 */ s32 pressedButtons;
} EntryPadCommandView;

void ProcessIndexedSelectionConfirmation(void) {
    s32 temp_v1;
    EntryPadCommandView *input;

    DrawEntryConfirmationPrompt(0x3A7, gIndexedSelectionConfirmation);
    if (IsMessageWindowStatusFinal() != 0) {
        input = (EntryPadCommandView *)gPad0Input;
        temp_v1 = input->pressedButtons;
        if (temp_v1 == 0x40) {
            if (gIndexedSelectionConfirmation == 0) {
                PlaySoundEffect(0);
                temp_v1 = 5;
                gIndexedSelectionState = temp_v1;
                CloseMessageWindow();
                return;
            }
            goto block_5;
        }
        if (temp_v1 == 0x20) {
block_5:
            PlaySoundEffect(1);
            gIndexedSelectionCursor = ((IndexedSelectionHeaderView *)gIndexedSelectionRecord)->index;
            gIndexedSelectionState = 2;
            CloseMessageWindow();
            return;
        }
        if (input->repeatButtons & 0x5000) {
            PlaySoundEffect(5);
            temp_v1 = gIndexedSelectionConfirmation;
            temp_v1 += 1;
            temp_v1 &= 1;
            gIndexedSelectionConfirmation = temp_v1;
        }
    }
}

extern void DrawTextEntryPreview(s32 arg0);
extern void DrawTextEntryGrid(void);
extern void ProcessTextEntryInput(void);
extern void ProcessTextEntryConfirmation(void);

void DrawTextEntryCursor(u16 arg0);

void DrawAndProcessTextEntry(void) {
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3A8);
    DrawTextEntryPreview(gTextEntryAlternateLayout);
    if (gTextEntryState == 2) {
        ProcessTextEntryConfirmation();
        return;
    }
    DrawTextEntryCursor(gTextEntryCursor);
    DrawTextEntryGrid();
    ProcessTextEntryInput();
}

/* The buffer installed by 80023130/23170/231AC. */

void ClearTextEntryBuffer(void) {
    char *destination;
    s32 remaining;
    u16 terminator;

    terminator = 0xFFFF;
    remaining = 8;
    destination = (char *)gTextEntryBuffer + 0x10;
    do {
        *(u16 *)destination = terminator;
        remaining -= 1;
        destination -= 2;
    } while (remaining >= 0);
}

void BuildTextEntryPreviewAlternate(u16 *arg0, u32 arg1);
void BuildTextEntryPreview(u16 *arg0, u32 arg1);

void DrawTextEntryPreview(s32 arg0) {
    u32 i = 0;
    u16 keep = 0xFFD;
    u16 *p;
    void (*fp)(u16 *, u32);

    for (i = 0; i < 0x10; i++) {
        gTextEntryPreviewBuffer[i] = keep;
    }
    p = &gTextEntryPreviewBuffer[0];
    p[0xF] = 0xFFFF;
    if (arg0) {
        fp = BuildTextEntryPreviewAlternate;
    } else {
        fp = BuildTextEntryPreview;
    }
    fp(&gTextEntryPreviewBuffer[0], i);
    func_80022580(gActiveRenderContext->field_D8 + 0x68, 1, 0xAB, 0x20, 0x85, 0x12);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x64, (s32)&gTextEntryPreviewBuffer[0], 0xAD, 0x23);
}

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", BuildTextEntryPreviewAlternate);

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", BuildTextEntryPreview);

void func_80017A38(s16 arg0, s16 arg1);

void DrawTextEntryGrid(void) {
    s32 sum;

    func_80022580(gActiveRenderContext->field_D8 + 0x68, 1, 0x2D, 0x40, 0xE6, 0x56);
    if (gTextEntryCharacterSet == 0) {
        sum = *gTextBankOffsets + (s32)&D_800537B2;
    } else {
        sum = *gTextBankOffsets + (s32)&D_80053888;
    }
    func_80017A38(1, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x64, sum, 0x2F, 0x42);
    func_80017A38(0, 0);
}

s32 MoveTextEntryCursor(s32 arg0, s32 arg1, u32 arg2);
s32 GetTextEntryCode(s32 arg0, s32 arg1);

void ProcessTextEntryInput(void) {
    SomeStruct *gfx;
    s16 var_s1;
    u16 temp_s0;
    s32 temp_v0;

    var_s1 = -1;
    if ((gTextEntryState != 2) && (IsMessageWindowStatusFinal() != 0)) {
        gfx = (SomeStruct *)gPad0Input;
        switch ((u32)gfx->field_0x8) {
        case 0x800:
            var_s1 = 2;
            if ((gTextEntryLength != 0) && (gTextEntryHasCharacter != 0)) {
                var_s1 = 0;
                CloseMessageWindow();
                gTextEntryConfirmation = 0;
                gTextEntryState = 2;
            }
            break;
        case 0x20:
            var_s1 = 1;
            if (gTextEntryLength != 0) {
                gTextEntryLength -= 1;
                gTextEntryBuffer[gTextEntryLength] = 0xFFFF;
            } else {
                var_s1 = 2;
                if (gTextEntryAllowCancel == 1) {
                    var_s1 = 1;
                    CloseMessageWindow();
                    gTextEntryState = 3;
                }
            }
            break;
        case 0x40: {
            s16 d0;
            u16 cursor;
            d0 = (s16)gTextEntryCursor;
            cursor = gTextEntryCursor;
            if (d0 != 0xD) {
                if (d0 == 0x53) {
                    var_s1 = 2;
                    if ((gTextEntryLength != 0) && (gTextEntryHasCharacter != 0)) {
                        var_s1 = 0;
                        CloseMessageWindow();
                        gTextEntryConfirmation = 0;
                        gTextEntryBuffer[gTextEntryLength + 1] = 0xFFFF;
                        gTextEntryState = 2;
                    }
                } else {
                    var_s1 = 0;
                    if ((u16)gTextEntryLength < 8U) {
                        if ((s16)cursor == 0x1B) {
                            gTextEntryBuffer[gTextEntryLength] = 0xFFD;
                        } else {
                            gTextEntryBuffer[gTextEntryLength] = GetTextEntryCode((s32)(*(volatile u16 *)&gTextEntryCursor), (s32)gTextEntryCharacterSet);
                            gTextEntryHasCharacter = 1;
                        }
                        gTextEntryLength += 1;
                    }
                }
            } else {
            case 0x5:
            case 0xA:
                gTextEntryCharacterSet = ((u16)gTextEntryCharacterSet + 1) & 1;
                var_s1 = 5;
            }
            break;
        }
        default:
            temp_s0 = gTextEntryCursor;
            temp_v0 = MoveTextEntryCursor((s32)temp_s0, *(u16 *)gPad0Input, (u32)gTextEntryCharacterSet);
            gTextEntryCursor = temp_v0;
            if (temp_s0 != (s16)temp_v0) {
                var_s1 = 5;
            }
            break;
        }
        if (var_s1 >= 0) {
            PlaySoundEffect(var_s1);
        }
    }
}

void ProcessTextEntryConfirmation(void) {
    s32 temp_v1;
    SomeStruct *gfx;

    DrawEntryConfirmationPrompt(0x3A6, gTextEntryConfirmation);
    if (IsMessageWindowStatusFinal() != 0) {
        gfx = (SomeStruct *)gPad0Input;
        temp_v1 = gfx->field_0x8;
        if (temp_v1 == 0x40) {
            if (gTextEntryConfirmation == 0) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                gTextEntryState = 4;
                return;
            }
            goto block_5;
        }
        if (temp_v1 == 0x20) {
block_5:
            CloseMessageWindow();
            PlaySoundEffect(1);
            gTextEntryState = 1;
            return;
        }
        if (gfx->field_0x0 & 0x5000) {
            PlaySoundEffect(5);
            gTextEntryConfirmation = (gTextEntryConfirmation + 1) & 1;
        }
    }
}

void DrawEntryConfirmationPrompt(s32 arg0, s32 arg1) {
    s32 tmp = arg1 & 0xFFFF;
    s32 sum;
    s32 base;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, arg0);
    if (IsMessageWindowStatusFinal() != 0) {
        base = gActiveRenderContext->field_D8 + 0x58;
        sum = *gTextBankOffsets + (s32)&D_8005175C;
        func_80017B3C(base, sum, 0x93, 0x4E);
        func_80022580(gActiveRenderContext->field_D8 + 0x5C, 1, 0x91, 0x4C, 0x28,
                      0x1E);
        func_800248B0(gActiveRenderContext->field_D8 + 0x54, 0x99, (s16)(tmp * 0xC + 0x54));
    }
}

/* Fourteen-column cursor: the low bit of moveKind selects the axis and
 * bit 1 selects increment. Bounds are inclusive, then a set-specific
 * handler skips holes in the grid. Keep the explicit u16 masks and the
 * branch-local setup order: both affect allocation in the original code. */

s32 AdjustTextEntryRow(s32, s32, s32);

s32 MoveTextEntryCursor(s32 cursor, s32 buttons, u32 characterSet) {
    s32 column;
    s32 row;
    s32 cursor16;
    s32 buttons16;
    s32 increment;
    s32 moveKind;
    s32 *coordinate;
    s32 (*adjust)(s32, s32, s32);
    u8 bound;
    s32 oldCoordinate;
    u32 result;

    cursor16 = cursor & 0xFFFF;
    buttons16 = buttons & 0xFFFF;

    if (buttons16 == 0x1000) {
        moveKind = 0;
    } else if (buttons16 == 0x4000) {
        moveKind = 2;
    } else if (buttons16 == 0x8000) {
        moveKind = 1;
    } else if (buttons16 == 0x2000) {
        moveKind = 3;
    } else {
        return cursor16;
    }

    if (characterSet >= 2) {
        return cursor16;
    }

    increment = moveKind & 2;
    column = ((u32)cursor16 % 14) & 0xFFFF;
    row = ((u32)cursor16 / 14) & 0xFFFF;

    if (moveKind & 1) {
        bound = *((u8 *)gTextEntryColumnBounds[characterSet] + row);
        adjust = (s32 (*)(s32, s32, s32))gTextEntryColumnHandlers[characterSet];
        coordinate = &column;
    } else {
        coordinate = &row;
        bound = *((u8 *)gTextEntryRowBounds[characterSet] + column);
        adjust = AdjustTextEntryRow;
    }

    oldCoordinate = *coordinate;
    if (increment != 0) {
        *coordinate = oldCoordinate + 1;
    } else {
        *coordinate = oldCoordinate - 1;
    }

    if (*coordinate < 0) {
        *coordinate = bound;
    }

    if ((s32)bound < *coordinate) {
        *coordinate = 0;
    }

    *coordinate = adjust(column, row, increment);
    result = ((u16)column + (row * 0xE)) & 0xFFFF;
    return result;
}

s32 AdjustTextEntryColumnSet0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp;

    if (arg1 != 5) {
        goto _800243C0;
    }
    temp = arg0 - 1;
    if ((u32) temp < 4U) {
        if (arg2 == 0) {
            goto _800243C8;
        }
        return 5;
    }
_800243A4:
    temp = arg0 - 6;
    if ((u32) temp < 7U) {
        if (arg2 == 0) {
            return 5;
        }
        return 0xD;
    }
_800243C0:
    return arg0;
_800243C8:
    return 0;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/text_entry", AdjustTextEntryRow);

s32 AdjustTextEntryColumnSet1(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 < 2) {
        if ((u32)(arg0 - 10) < 3) {
            if (arg2 == 0) {
                return 9;
            }
            return 13;
        }
    }
    if (arg1 == 5) {
        return 13;
    }
    return arg0;
}

void DrawTextEntryCursor(u16 arg0) {
    u16 temp;
    s32 quot;
    s32 rem;
    s32 add;

    temp = arg0;
    quot = temp / 14;
    rem = temp % 14;
    add = rem >= 5;
    if (rem >= 0xA) {
        add += 1;
    }
    if (rem >= 0xD) {
        add += 2;
    }
    rem += add;
    func_800248B0(gActiveRenderContext->field_D8 + 0x60,
                  (s16) (rem * 9 + 0x32),
                  (s16) (quot * 14 + 0x48));
}

typedef s32 (*FuncType)(s32, s32, s32);

s32 GetTextEntryCode(s32 arg0, s32 arg1) {
    u16 temp;
    u32 quot;
    u32 rem;
    FuncType fn;

    /* Keep the prologue's sw ra ahead of the branch: without the barrier the
       post-reload scheduler moves it into the beqz delay slot. */
    __asm__ volatile("" ::: "memory");

    if ((u32)arg1 >= 2) {
        return 0xFFFF;
    }
    temp = arg0;
    quot = temp / 14;
    rem = temp % 14;
    fn = (FuncType)gTextEntryCodeHandlers[arg1];
    return fn(rem, quot, temp);
}

s32 GetTextEntryCodeSet0(s32 arg0, s32 arg1) {
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;
    s32 prod;

    var_a0 = arg0;
    var_v1 = 0;
    if (var_a0 < 5) {
        var_v0 = 5;
    } else {
        var_v1 = 0x20;
        if (var_a0 < 0xA) {
            var_a0 -= 5;
            var_v0 = 5;
        } else {
            var_v1 = 0x40;
            var_a0 -= 0xA;
            var_v0 = 3;
        }
    }
    prod = arg1 * var_v0;
    return (var_v1 + (prod + var_a0)) & 0xFFFF;
}

u16 GetTextEntryCodeSet1(s32 column, s32 row) {
    u16 *codes;
    s32 index;

    codes = gTextEntryCodeTableSet1;
    index = row * 10 + column;
    return codes[index];
}
