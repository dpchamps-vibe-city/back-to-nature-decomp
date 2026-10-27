/*
 * status_pages — reconstructed translation unit.
 *
 * Eight-page status display with resource loading, fade-controlled lifecycle,
 * page and row navigation, snapshot counters and page-specific renderers.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_status_pages.h"
#include "game_types.h"
#include "screen_primitives.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "ovl11_record_flag_counters.h"
#include "screen_fade.h"
#include "cd_loader.h"
#include "sound_commands.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sprite_grid.h"
#include "beveled_panel.h"
#include "sprite_render.h"
#include "ovl11_slot_decorations.h"
#include "ovl11_menu_exit_flags.h"
#include "ovl11_item_slot_commands.h"

s32 Ovl11CountStatusSmallRecords (void);
s32 Ovl11CountStatusNonzeroEntries (void);
s32 ovl_11_func_800F3C9C (u16 arg0);

void Ovl11SnapshotStatusPages(void) {
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    s32 var_s0;
    s32 var_s7;
    /* Template byte offset, then the name's byte address. */
    s32 text_position;
    s16 *counts;
    u16 temp_a0;
    u32 clamp_value;
    s32 var_a1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_t0;
    u8 *text;
    s32 var_a0_2;

    gOvl11StatusPageState = 0;
    gOvl11StatusPageIndex = 0;
    gOvl11StatusRowScroll = 0;
    gOvl11StatusAnimationTick = 0;
    for (var_s0 = 0; var_s0 < 14; var_s0++) {
        text_position = var_s0 * 2;
        temp_a0 = *(u16 *) ((u8 *) &D_80070D18 + text_position);
        text = D_800535C6;
        if (temp_a0 != 0xFFFF) {
            gOvl11StatusHeadingText[var_s0] = temp_a0;
        } else {
            gOvl11StatusHeadingText[var_s0++] = 0xFFD;
            text_position = (s32) &text[*gTextBankOffsets];
            for (; var_s0 < 14; text_position += 2, var_s0++) {
                gOvl11StatusHeadingText[var_s0] = *(u16 *) text_position;
                if (gOvl11StatusHeadingText[var_s0] == 0xFFFF) {
                    goto complete;
                }
            }
        }
    }
complete:
    var_a1 = 2;
    gOvl11StatusHeadingText[0xD] = 0xFFFF;
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44CE < 3U) {
        var_a1 = ((Ovl11FB908State *) D_8006C838)->field_44CE;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D0 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D0;
    } else {
        var_a1 = var_a1 + 1;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D2 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D2;
    } else {
        var_a1 = var_a1 + 1;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D4 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D4;
    } else {
        var_a1 = var_a1 + 1;
    }

    var_a1 *= 20;
    gOvl11StatusFlagScore = var_a1;
    gOvl11StatusOccupiedRecordCount = 0;
    for (var_s0 = 0; var_s0 < 80; var_s0++) {
        if (((Ovl11FB908State *) D_8006C838)->records[var_s0][0] != 0) {
            gOvl11StatusOccupiedRecordCount = (u16) gOvl11StatusOccupiedRecordCount + 1;
        }
    }
    if (((Ovl11FB908State *) D_8006C838)->field_E776 >= 0) {
        var_s7 = (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + (((Ovl11FB908State *) D_8006C838)->field_E778 * 0x1D4)))->field_99EC / 6000) & 0xFFFF;
        gOvl11StatusNamedRecords->unk0 = var_s7;
        gOvl11StatusNamedRecords->unk4 = Ovl11GetCounterRecordLabel();
        gOvl11StatusNamedRecords->unk9 = (u8) ((Ovl11FB908State *) D_8006C838)->field_E776 + 1;
    } else {
        var_s7 = 0;
        gOvl11StatusNamedRecords->unk4 = 0;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_E780 == 1) {
        gOvl11StatusNamedRecords[1].unk4 = (s32) &D_8007AFBC;
        var_s5 = (s16) ((Ovl11FB908State *) D_8006C838)->field_DBBA / 25;
        gOvl11StatusNamedRecords[1].unk0 = (s32) var_s5;
    } else {
        var_s5 = 0;
        gOvl11StatusNamedRecords[1].unk4 = NULL;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_91AC != 0) {
        gOvl11StatusNamedRecords[2].unk4 = (s32) &D_800759E8;
        var_s4 = (s16) ((Ovl11FB908State *) D_8006C838)->field_91C2 / 25;
        gOvl11StatusNamedRecords[2].unk0 = (s32) var_s4;
    } else {
        var_s4 = 0;
        gOvl11StatusNamedRecords[2].unk4 = NULL;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_929C != 0) {
        gOvl11StatusNamedRecords[3].unk4 = (s32) &D_80075AD8;
        var_s3 = (s16) ((Ovl11FB908State *) D_8006C838)->field_92B2 / 25;
        gOvl11StatusNamedRecords[3].unk0 = (s32) var_s3;
    } else {
        var_s3 = 0;
        gOvl11StatusNamedRecords[3].unk4 = NULL;
    }
    gOvl11StatusPoolCounts->unk2 = Ovl11CountStatusSmallRecords();
    (*(u16 *) ((u8 *) gOvl11StatusPoolCounts + 6)) = Ovl11CountStatusTaggedRecordsA();
    (*(u16 *) ((u8 *) gOvl11StatusPoolCounts + 0xA)) = Ovl11CountStatusTaggedRecordsB();
    (*(s16 *) ((u8 *) gOvl11StatusPoolCounts + 0xE)) = Ovl11CountStatusNonzeroEntries();
    var_a0_2 = 0x63;
    if ((u8) ((Ovl11FB908State *) D_8006C838)->field_44CD < 0x64U) {
        var_a0_2 = ((Ovl11FB908State *) D_8006C838)->field_44CD;
    }
    var_a2 = 0x3E7;
    (*(s16 *) ((u8 *) gOvl11StatusPoolCounts + 0x12)) = (s16) var_a0_2;
    counts = gOvl11StatusFourCounters;
    var_a0_2 = ((Ovl11FB908State *) D_8006C838)->field_4508;
    if (var_a0_2 < 0x3E8U) {
        var_a2 = var_a0_2;
    }
    var_a0_2 = 0x3E7;
    counts[1] = var_a2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->field_4502;
    if (clamp_value < 0x3E8U) {
        var_a0_2 = clamp_value;
    }
    var_a2_2 = 0x3E7;
    counts[3] = var_a0_2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->saveCounter21;
    if (clamp_value < 0x3E8U) {
        var_a2_2 = clamp_value;
    }
    var_t0 = 0x3E7;
    counts[5] = var_a2_2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->field_4500;
    if (clamp_value < 0x3E8U) {
        var_t0 = clamp_value;
    }
    counts[7] = var_t0;
    gOvl11StatusScore = (*(u16 *) ((u8 *) gOvl11StatusPoolCounts + 0xA)) + ((*(u16 *) ((u8 *) gOvl11StatusPoolCounts + 6)) + ((u16) gOvl11StatusPoolCounts->unk2 + ((gOvl11StatusFlagScore / 10) + (gOvl11StatusOccupiedRecordCount / 8) + var_s7 + var_s5 + var_s4 + var_s3)));
    for (var_s0 = 0; (u32) var_s0 < 21; var_s0++) {
        gOvl11StatusValueGrid[var_s0].unk2 = ((Ovl11FB908State *) D_8006C838)->values[ovl_11_func_800F3C9C(*(u16 *) &gOvl11StatusValueGrid[var_s0])];
    }
}

s32 Ovl11OpenStatusPages(void) {
    if (gOvl11StatusPageState == 0) {
        gOvl11StatusPageState = 1;
        BeginScreenFadeIncrease(10, 0, 2);
    } else {
        return gOvl11StatusPageState;
    }
}

void Ovl11SnapshotStatusPages(void);

void Ovl11LoadStatusPageResources(void) {
    u32 i;

    PlaySoundEffect(3);
    Ovl11SnapshotStatusPages();
    gOvl11StatusPageState = 1;
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x7004));
    memcpy(gOvl11StatusSpriteBank, (u8 *)(gGraphicsHeapBase + 0x4290), 0x2D74);
    for (i = 0; i < 5; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl11StatusSpriteSources + i * 0x30),
                      (SpriteDataHeader *)(gOvl11StatusSpriteBank + gOvl11StatusSpriteOffsets[i]));
    }
}

void Ovl11LoadStatusPageResources(void);
void Ovl11DrawAndProcessStatusPages(void);

/* Fade state is sampled before dispatch: transitions take effect next update. */
void Ovl11UpdateStatusPages(void) {
    s32 fadeComplete;

    fadeComplete = IsScreenFadeComplete();
    if (gOvl11StatusPageState >= 3 && gOvl11StatusPageState <= 5) {
        DrawScrollingSpriteGridInContext(gActiveRenderContext->field_D8 + 0x74);
    }
    switch (gOvl11StatusPageState) {
    case 1:
        if (fadeComplete == 1) {
            gOvl11StatusPageState = 2;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x20000;
            return;
        }
    default:
        return;
    case 2:
        Ovl11LoadStatusPageResources();
        BeginScreenFadeDecrease(0xA, 0, 2);
        gOvl11StatusPageState = 3;
        return;
    case 3:
        if (fadeComplete == 1) {
            gOvl11StatusPageState = 4;
            return;
        }
        break;
    case 4:
        Ovl11DrawAndProcessStatusPages();
        return;
    case 5:
        if (fadeComplete == 1) {
            BeginScreenFadeDecrease(0xA, 0, 2);
            gOvl11StatusPageState = 6;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x40000;
        }
        break;
    case 6:
        if (fadeComplete == 1) {
            gOvl11StatusPageState = 0;
            Ovl11PostMenuExitFlags();
        }
        break;
    }
}

void Ovl11DrawStatusPageArrows(void);
void Ovl11NextStatusPage(void);
void Ovl11PreviousStatusPage(void);
void Ovl11CloseStatusPages(void);

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Ovl11DrawAndProcessStatusPages(void) {
    s32 buttons;
    s32 nextTick;

    DrawMenuPanel(gActiveRenderContext->field_D8 + 0x70, 1, 0x10, 0x20, 0x120, 0xC0);
    DrawMenuPanel(gActiveRenderContext->field_D8 + 0x6C, 1, 0x3C, 0x10, 0xC8, 0x20);
    Ovl11DrawStatusPageArrows();
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &gOvl11StatusHeadingText, 0x3E, 0x1A);
    buttons = gPad0Input->repeatButtons;
    if (buttons & 0x2000) {
        PlaySoundEffect(5);
        gOvl11StatusRowScroll = 0;
        Ovl11NextStatusPage();
    } else if (buttons & 0x8000) {
        PlaySoundEffect(5);
        gOvl11StatusRowScroll = 0;
        Ovl11PreviousStatusPage();
    }
    if (gPad0Input->pressedButtons & 0x860) {
        Ovl11CloseStatusPages();
    }
    /* Closing still draws this update's selected page. */
    gOvl11StatusPageDrawers[(s16) gOvl11StatusPageIndex]();
    nextTick = gOvl11StatusAnimationTick + 1;
    gOvl11StatusAnimationTick = nextTick;
    if ((s16) nextTick >= 0x78) {
        gOvl11StatusAnimationTick = 0;
    }
}

void Ovl11CloseStatusPages(void) {
    PlaySoundEffect(3);
    BeginScreenFadeIncrease(10, 0, 2);
    gOvl11StatusPageState = 5;
}

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void Ovl11DrawStatusIconText(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s16 arg4);
void Ovl11DrawStatusFourCounters(void);
void Ovl11DrawStatusPoolCounts(void);
void Ovl11DrawStatusNamedRecords(void);
void Ovl11DrawStatusFlagIcons(void);
void Ovl11AppendStatusLabelCode(u16 *arg0, s32 arg1);
void Ovl11DrawStatusPrimaryMarks(s16 arg0, s16 arg1, s16 arg2);

void Ovl11DrawStatusSummaryPage(void) {
    s32 value;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) gOvl11StatusSpriteSources, 0, 0, 0x18, 0x30);
    Ovl11DrawStatusPrimaryMarks(((struct struct_8006C838_800FC358 *) D_8006C838)->field_4A84, 0x78, 0x3C);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &((struct struct_8006C838_800FC358 *) D_8006C838)->field_51C8, 0x30, 0x32);
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(gOvl11StatusFlagScore, &gOvl11NumberTextScratch, 3), 0x1E74);
    Ovl11DrawStatusIconText(3, 0xB, (s32) &gOvl11NumberTextScratch, 0x28, 0x48);
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(gOvl11StatusOccupiedRecordCount, &gOvl11NumberTextScratch, 3), 0x1E78);
    Ovl11DrawStatusIconText(3, 0xC, (s32) &gOvl11NumberTextScratch, 0x28, 0x60);
    Ovl11DrawStatusNamedRecords();
    value = ((struct struct_8006C838_800FC358 *) D_8006C838)->field_5224;
    if (value >= -9999999) {
        if (value > 9999999) {
            value = 9999999;
        }
    } else {
        value = -9999999;
    }
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(value, &gOvl11NumberTextScratch, 8), 0x1E86);
    Ovl11DrawStatusIconText(3, 0xA, (s32) &gOvl11NumberTextScratch, 0x8C, 0x30);
    Ovl11DrawStatusPoolCounts();
    Ovl11DrawStatusFlagIcons();
    Ovl11DrawStatusFourCounters();
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(gOvl11StatusScore, &gOvl11NumberTextScratch, 3), 0x1E74);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &gOvl11NumberTextScratch, 0xBA, 0x1A);
}

void Ovl11DrawStatusIconText(s32 bank, s16 animation, s32 text, s32 x, s16 y) {
    s16 iconX = x;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32)(gOvl11StatusSpriteSources + bank * 0x30), animation & 0xFF, 0, iconX, y);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, text, (s16)(iconX + 0x18), (s16)(y + 4));
}

void Ovl11DrawStatusFourCounters(void) {
    s16 temp_s0;
    s16 *var_s1;
    s16 *var_s2;
    s32 temp_a2;
    s32 temp_v1;
    s32 var_v0;
    u32 var_s0;

    var_s0 = 0;
    var_s1 = gOvl11StatusFourCounters;
    var_s2 = &gOvl11NumberTextScratch;
    for (; var_s0 < 4; var_s0++) {
        *((u16 *)func_8001A970((s32) var_s1[1], var_s2, 3)) = 0xFFFF;
        temp_v1 = (var_s0 + 1) * 0x18;
        temp_a2 = temp_v1 + 0x30;
        temp_s0 = var_s1[0];
        if (var_s0 & 2) {
            var_v0 = (temp_v1 + 0x48) << 0x10;
        } else {
            var_v0 = temp_a2 << 0x10;
        }
        Ovl11DrawStatusIconText(3, temp_s0, (s32) var_s2, 0xD4, (s16) (var_v0 >> 0x10));
        var_s1 += 2;
    }
}

void Ovl11DrawStatusPoolCounts(void) {
    Ovl11D80127220Entry *var_s1;
    u32 var_s2;

    var_s2 = 0;
    var_s1 = gOvl11StatusPoolCounts;
    for (; var_s2 < 5; var_s2++) {
        *((u16 *)func_8001A970((s32) var_s1->unk2, &gOvl11NumberTextScratch, 2)) = 0xFFFF;
        Ovl11DrawStatusIconText((s32) var_s1->unk0, (s16) var_s1->unk1, (s32) &gOvl11NumberTextScratch, 0x8C, (s16) (((var_s2 + 1) * 0x18) + 0x30));
        var_s1++;
    }
}

void Ovl11DrawStatusNamedRecordRow(s32 arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6);

void Ovl11DrawStatusNamedRecords(void) {
    Ovl11D80127234Entry *var_s0;
    s32 temp_v0;
    u32 var_s2;

    var_s2 = 0;
    var_s0 = gOvl11StatusNamedRecords;
    do {
        temp_v0 = var_s0->unk4;
        if (temp_v0 != 0) {
            Ovl11DrawStatusNamedRecordRow(var_s0->unk0, (s32) var_s0->unk8, var_s0->unk9, 0U, temp_v0, 0x18, (s16) ((var_s2 * 0x180000 + 0x780000) >> 0x10));
        }
        var_s2 += 1;
        var_s0 = (Ovl11D80127234Entry *) ((u8 *) var_s0 + 0xC);
    } while (var_s2 < 4U);
}

void Ovl11DrawStatusRecordMarks(s16 arg0, s16 arg1, s16 arg2);

void Ovl11DrawStatusNamedRecordRow(s32 marks, s32 bank, u8 animation, s32 frame, s32 text, s16 x, s16 y) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32)(gOvl11StatusSpriteSources + bank * 0x30), animation & 0xFF, frame & 0xFF, x, y);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, text, (s16)(x + 0x18), (s16)(y + 2));
    Ovl11DrawStatusRecordMarks((s16)marks, (s16)(x + 0x60), (s16)(y + 0x10));
}

void Ovl11DrawStatusFlagIcons(void) {
    Ovl11D80127328Entry *var_s0;
    u8 *base;
    s16 var_s6;
    s32 temp_v1;
    s32 var_a1;
    s32 var_s1;
    u32 var_s2;

    var_s2 = 0;
    var_a1 = 0x8C;
    var_s6 = 0xC0;
    base = (u8 *) &D_8006C838;
    var_s0 = gOvl11StatusFlagIcons;
    var_s1 = 0xA40000;
    do {
        if ((*(s32 *) (base + 0x44F8)) & var_s0->unk0) {
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) gOvl11StatusIndicatorSpriteSource, (s32) var_s0->unk4, 0, (s16) var_a1, (s16) var_s6);
            temp_v1 = (s16) (var_s1 >> 0x10);
            var_s1 += 0x180000;
            var_a1 = temp_v1;
        }
        var_s2 += 1;
        var_s0 += 1;
    } while (var_s2 < 6U);
}

void Ovl11DrawStatusClampedNumber(s16 arg0, s16 arg1, s16 arg2);

void Ovl11DrawStatusValueGrid(void) {
    Ovl11D80127274Entry *temp_s2;
    s16 temp_s1;
    s16 temp_s0;
    s32 var_s3;

    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) ((u8 *) D_800535E6 + gTextBankOffsets[0]), 0xBA, 0x14);
    var_s3 = 0;
    do {
        temp_s2 = (Ovl11D80127274Entry *) ((u8 *) gOvl11StatusValueGrid + (var_s3 * 6));
        temp_s1 = ((var_s3 / 7) * 0x58) + 0x24;
        temp_s0 = ((var_s3 % 7) * 0x18) + 0x32;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) gOvl11StatusValueGridSpriteSource, (s32) temp_s2->unk4, 0, (s16) temp_s1, (s16) temp_s0);
        Ovl11DrawStatusClampedNumber(temp_s2->unk2, (s16) (temp_s1 + 0x1E), (s16) (temp_s0 + 6));
        var_s3 += 1;
    } while ((u32) var_s3 < 0x15U);
}

void Ovl11DrawStatusClampedNumber(s16 arg0, s16 arg1, s16 arg2) {
    s16 var_a0;

    var_a0 = arg0;
    if (var_a0 >= 0x3E8) {
        var_a0 = 0x3E7;
    }
    *((u16 *)func_8001A970((s32) var_a0, &gOvl11NumberTextScratch, 3)) = 0xFFFF;
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &gOvl11NumberTextScratch, (s32) arg1, (s32) arg2);
}

void Ovl11DrawStatusRecordPageTitle(s32 arg0);

void Ovl11DrawAndScrollStatusRows(s16 arg0, void (*arg1)(s16, s16));

void Ovl11DrawStatusSmallRecordRow(s16 arg0, s16 arg1);

void Ovl11DrawStatusSmallRecordPage(void) {
    Ovl11DrawStatusRecordPageTitle(0);
    Ovl11DrawAndScrollStatusRows(gOvl11StatusSmallRecordCount, Ovl11DrawStatusSmallRecordRow);
}

s16 Ovl11ReadStatusSignedValue(s16 *arg0);
void Ovl11DrawStatusRecordDetails(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);

void Ovl11DrawStatusUnpaddedNumber(s16 arg0, s16 arg1, s16 arg2);

void Ovl11DrawStatusSmallRecordRow(s16 arg0, s16 arg1) {
    s16 temp_v0_2;
    s32 *var_s0;
    s32 temp_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    u8 var_a2_2;
    u8 var_a3_2;

    var_a3 = 0;
    var_s0 = &gSmallTaggedRecordBase;
    var_v1 = 0;
    var_a2 = 0x10000;
    for (; var_v1 < 0xA; var_v1++, var_s0 += 0x2D) {
        if (((*(u16 *) ((u8 *) var_s0 + 0)) != 0) && !((*(s32 *) ((u8 *) var_s0 + 0x34)) & 0x02000000)) {
            temp_v0 = var_a2;
            var_a2 += 0x10000;
            var_a3 = temp_v0 >> 0x10;
        }
        if (var_a3 == arg0) {
            break;
        }
    }
    if (var_v1 != 0xA) {
        temp_v0_2 = arg1 + 4;
        Ovl11DrawStatusUnpaddedNumber(arg0, 0x18, temp_v0_2);
        var_a3_2 = 0;
        var_a2_2 = 0xE;
        if ((*(u16 *) ((u8 *) var_s0 + 0)) != 0x10A) {
            var_a2_2 = 0xF;
            var_a3_2 = (*(u16 *) ((u8 *) var_s0 + 0xAE)) != 0;
        }
        Ovl11DrawStatusNamedRecordRow((s32) (s16) (((s16) (*(u16 *) ((u8 *) var_s0 + 0x16))) / 25), 1, var_a2_2, var_a3_2, (s32) ((var_s0 + 1)), 0x30, (s16) (s32) arg1);
        Ovl11DrawStatusRecordDetails((*(s32 *) ((u8 *) var_s0 + 0x34)) & 0x40, *(s16 *) ((u8 *) var_s0 + 0x1C), *(s16 *) ((u8 *) var_s0 + 0x1E), Ovl11ReadStatusSignedValue((s16 *) ((u8 *) var_s0 + 0x1A)), 0xB0, (s16) (s32) temp_v0_2);
    }
}

void Ovl11DrawStatusTaggedRecordRowA(s16 arg0, s16 arg1);

void Ovl11DrawStatusTaggedRecordPageA(void) {
    Ovl11DrawStatusRecordPageTitle(1);
    Ovl11DrawAndScrollStatusRows(gOvl11StatusTaggedRecordCountA, Ovl11DrawStatusTaggedRecordRowA);
}

void Ovl11DrawStatusUnpaddedNumber (s16 arg0, s16 arg1, s16 arg2);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Ovl11DrawStatusRecordMarks (s16 arg0, s16 arg1, s16 arg2);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_80024A10 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void Ovl11DrawStatusTaggedRecordRowA(s16 arg0, s16 arg1) {
    s32 *var_s1;
    s32 temp_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    u16 temp_v1;
    u8 var_a2_2;

    var_a3 = 0;
    var_s1 = &gTaggedRecordBase;
    var_v1 = 0;
    var_a2 = 0x10000;
    for (; var_v1 < 0x14; var_v1++, var_s1 += 0x2E) {
        if ((u32) ((*(u16 *) ((u8 *) var_s1 + 0)) - 0x160) < 4U) {
            temp_v0 = var_a2;
            var_a2 += 0x10000;
            var_a3 = temp_v0 >> 0x10;
        }
        if (var_a3 == arg0) {
            break;
        }
    }
    if (var_v1 != 0x14) {
        Ovl11DrawStatusUnpaddedNumber(arg0, 0x18, (s16) (arg1 + 4));
        var_a2_2 = 9;
        if ((*(u16 *) ((u8 *) var_s1 + 0xB2)) == 0) {
            temp_v1 = *(u16 *) ((u8 *) var_s1 + 0);
            var_a2_2 = 6;
            if (temp_v1 != 0x160) {
                var_a2_2 = 8;
                if (temp_v1 == 0x161) {
                    var_a2_2 = 7;
                }
            }
        }
        Ovl11DrawStatusNamedRecordRow((s32) (s16) (((s16) (*(u16 *) ((u8 *) var_s1 + 0x16))) / 25), 1, var_a2_2, 0U, (s32) ((var_s1 + 1)), 0x30, (s16) (s32) arg1);
        Ovl11DrawStatusRecordDetails((*(s32 *) ((u8 *) var_s1 + 0x34)) & 0x40, *(s16 *) ((u8 *) var_s1 + 0x1C), *(s16 *) ((u8 *) var_s1 + 0x1E), Ovl11ReadStatusSignedValue((s16 *) ((u8 *) var_s1 + 0x1A)), 0xB0, (s16) (arg1 + 4));
    }
}

void Ovl11DrawStatusTaggedRecordRowB(s16 arg0, s16 arg1);

void Ovl11DrawStatusTaggedRecordPageB(void) {
    Ovl11DrawStatusRecordPageTitle(2);
    Ovl11DrawAndScrollStatusRows(gOvl11StatusTaggedRecordCountB, Ovl11DrawStatusTaggedRecordRowB);
}

void Ovl11DrawStatusTaggedRecordRowB(s16 arg0, s16 arg1) {
    s32 *var_s1;
    s32 temp_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    u8 var_a2_2;

    var_a3 = 0;
    var_s1 = &gTaggedRecordBase;
    var_v1 = 0;
    var_a2 = 0x10000;
    for (; var_v1 < 0x14; var_v1++, var_s1 += 0x2E) {
        if ((u32) ((*(u16 *) ((u8 *) var_s1 + 0)) - 0x164) < 3U) {
            temp_v0 = var_a2;
            var_a2 += 0x10000;
            var_a3 = temp_v0 >> 0x10;
        }
        if (var_a3 == arg0) {
            break;
        }
    }
    if (var_v1 != 0x14) {
        Ovl11DrawStatusUnpaddedNumber(arg0, 0x18, (s16) (arg1 + 4));
        var_a2_2 = 0xD;
        if ((*(u16 *) ((u8 *) var_s1 + 0xB2)) == 0) {
            var_a2_2 = 0xC;
            if ((*(u16 *) ((u8 *) var_s1 + 0)) == 0x164) {
                var_a2_2 = 0xA;
            }
        }
        Ovl11DrawStatusNamedRecordRow((s32) (s16) (((s16) (*(u16 *) ((u8 *) var_s1 + 0x16))) / 25), 1, var_a2_2, 0U, (s32) ((var_s1 + 1)), 0x30, (s16) (s32) arg1);
        Ovl11DrawStatusRecordDetails((*(s32 *) ((u8 *) var_s1 + 0x34)) & 0x40, *(s16 *) ((u8 *) var_s1 + 0x1C), (s16) ((*(u16 *) ((u8 *) var_s1 + 0x1E)) + 1), Ovl11ReadStatusSignedValue((s16 *) ((u8 *) var_s1 + 0x1A)), 0xB0, (s16) (arg1 + 4));
    }
}

void Ovl11DrawStatusScrollUpArrow(void);
void Ovl11DrawStatusScrollDownArrow(void);

void Ovl11DrawAndScrollStatusRows(s16 count, void (*drawRow)(s16, s16)) {
    void (*draw)(s16, s16);
    s32 row;
    s16 y;

    draw = drawRow;
    if (count >= 8) {
        if (gPad0Input->repeatButtons & 0x1000) {
            PlaySoundEffect(5);
            gOvl11StatusRowScroll--;
        } else if (gPad0Input->repeatButtons & 0x4000) {
            PlaySoundEffect(5);
            gOvl11StatusRowScroll++;
        }
    }
    if (count < gOvl11StatusRowScroll + 7) {
        gOvl11StatusRowScroll = 0;
    }
    if (gOvl11StatusRowScroll < 0) {
        gOvl11StatusRowScroll = count - 7;
    }
    if (gOvl11StatusRowScroll > 0) {
        Ovl11DrawStatusScrollUpArrow();
    }
    if (gOvl11StatusRowScroll + 7 < count) {
        Ovl11DrawStatusScrollDownArrow();
    }
    row = gOvl11StatusRowScroll;
    y = 0x30;
    while (row < gOvl11StatusRowScroll + 7) {
        /* Callbacks use one-based rows; preserve signed-halfword wrap. */
        row += 1;
        row <<= 16;
        row >>= 16;
        draw(row, y);
        y += 0x18;
    }
}

void Ovl11DrawStatusRecordPageTitle(s32 arg0) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) &gOvl11StatusRecordSpriteSource, gOvl11StatusRecordPageIcons[arg0], 0, 0xBA, 0x14);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, *gTextBankOffsets + (s32) &D_80053604, 0xD2, 0x1A);
}

void func_80024A10(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void Ovl11DrawStatusRecordDetails(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    s32 var_v1;
    s16 *base;

    for (var_v1 = 0; var_v1 < 0xF; var_v1++) {
        (&gOvl11NumberTextScratch)[var_v1] = 0xFFD;
    }
    if (arg0 != 0) {
        gOvl11NumberTextScratch = 0x90;
    }
    base = &gOvl11NumberTextScratch;
    base[4] = 0x71;
    func_8001A970((s32) arg2, base + 5, 2);
    base[8] = 0;
    base[9] = 0x26;
    base[10] = 0x24;
    *((u16 *)func_8001A970((s32) arg3, base + 0xB, 2)) = 0xFFFF;
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) base, (s32) arg4, (s32) arg5);
    func_80024A10(gActiveRenderContext->field_D8 + 0x68, (s16) (arg4 + 0x10), arg5, arg1);
}

void Ovl11DrawStatusSevenRecordRow(s16 arg0, s16 arg1);

void Ovl11DrawStatusSevenRecordPage(void) {
    s32 var_s0;
    s16 var_a1;

    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) (D_8005360E + gTextBankOffsets[0]), 0xBA, 0x1A);
    var_a1 = 0x30;
    for (var_s0 = 0; var_s0 < 7; var_s0++) {
        Ovl11DrawStatusSevenRecordRow(var_s0, var_a1);
        var_a1 += 0x18;
    }
}

/* The original passes the signed row index; the callee masks it to a byte. */
void Ovl11DrawStatusNamedRecordRow (s32 arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6);

void Ovl11DrawStatusSevenRecordRow(s16 arg0, s16 arg1) {
    s16 *var_a1;
    s32 var_a3;
    s32 temp_s4;
    s32 var_a2;
    u16 *var_s1;
    u16 temp_v1;
    u8 *temp_s2;

    temp_s2 = gObjectRecordPool + (arg0 * 0xF8);
    temp_s4 = gActiveRenderContext->field_D8 + 0x68;
    Ovl11DrawStatusUnpaddedNumber((s16) (arg0 + 1), 0x18, (s16) (arg1 + 4));
    Ovl11DrawStatusNamedRecordRow((s32) (s16) (((s16) *(u16 *) (temp_s2 + 0x16)) / 25), 0, 7U, arg0, (s32) ((void *) ((u8 *) temp_s2 + 4)), 0x28, (s16) (s32) arg1);
    if (*(u16 *) (temp_s2 + 0xB8) & 0x20) {
        DrawSpriteFrameRects(temp_s4, (s32) gOvl11StatusSpriteSources, 8, 0, 0x96, (s16) (s32) arg1);
    }
    temp_v1 = *(u16 *) (temp_s2 + 0xB8);
    var_a2 = 0xD;
    if (!(temp_v1 & 2)) {
        var_a2 = 0xE;
        if (!(temp_v1 & 4)) {
            var_a2 = -1;
            if (temp_v1 & 1) {
                var_a2 = 0xF;
            }
        }
    }
    if (var_a2 >= 0) {
        var_s1 = (u16 *) ((u8 *) &D_80053618 + *gTextBankOffsets);
        DrawSpriteFrameRects(temp_s4, (s32) gOvl11StatusIndicatorSpriteSource, var_a2 & 0xFF, 0, 0x96, (s16) (s32) arg1);
        var_a1 = func_8001A970((s32) *(u16 *) (temp_s2 + 0xB4), &gOvl11NumberTextScratch, 2);
        *var_a1 = 0xFFD;
        var_a1++;
        var_a3 = arg1 + 4;
        while ((u16) (*var_a1 = *var_s1) != 0xFFFF) {
            var_s1++;
            var_a1++;
        }
        var_a1 = &gOvl11NumberTextScratch;
    } else {
        var_a3 = arg1 + 4;
        var_a1 = (s16 *) ((u8 *) &D_8005362C + *gTextBankOffsets);
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) var_a1, 0xC6, (s16) var_a3);
}

/* Page 6 reads four signed word pairs at work-area +49C4 (eight-byte
 * stride), the icon cycle at +44BA and the current text bank. The two
 * first-pair totals clamp to +/-9999999; the values themselves are unchanged.
 * Each row's left/right bar uses pair[1]/pair[0]: signed remainder modulo
 * 10000 scaled to 64 pixels, with optional two-digit higher-order labels.
 * Bars are not clamped; nonnegative values are needed for useful widths.
 * Labels distinguish >=5000, >=2000 and below 2000, with a both-zero case.
 * Rendering requires initialized packet/OT/sprite resources and reuses the
 * module's number-text scratch (its two-byte symbol is only a base anchor). */
void Ovl11DrawPairedValueRow(s16 row, s16 y);
void Ovl11DrawPairedValueBars(s32 *pair, s16 y);

void Ovl11DrawPairedValuePanel(void) {
    s16 rowY;
    s32 row;
    s32 textOt;
    s32 clampedValue;
    s32 value;

    textOt = gActiveRenderContext->field_D8 + 0x54;
    ((struct Ovl11NumberTextHeader *) &gOvl11NumberTextScratch)->marker = 0xFFD;
    func_80017B3C(textOt, (s32) (D_80053636 + gTextBankOffsets[0]), 0xB2, 0x14);
    func_80017B3C(textOt, (s32) (D_80053636 + 0x28 + gTextBankOffsets[0]), 0x20, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][0];
    if (value >= -9999999) {
        clampedValue = value;
        if (clampedValue > 9999999) {
            clampedValue = 9999999;
        }
    } else {
        clampedValue = -9999999;
    }
    gOvl11NumberTextScratch = 0x68;
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(clampedValue, &gOvl11NumberTextScratch + 2, 7), 0x1E86);
    func_80017B3C(textOt, (s32) &gOvl11NumberTextScratch, 0xAC, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][1];
    if (value >= -9999999) {
        clampedValue = value;
        if (clampedValue > 9999999) {
            clampedValue = 9999999;
        }
    } else {
        clampedValue = -9999999;
    }
    gOvl11NumberTextScratch = 0x69;
    Ovl11AppendStatusLabelCode((u16 *) func_8001A970(clampedValue, &gOvl11NumberTextScratch + 2, 7), 0x1E86);
    func_80017B3C(textOt, (s32) &gOvl11NumberTextScratch, 0xAC, 0x4E);
    rowY = 0x68;
    for (row = 0; row < 4; row++) {
        Ovl11DrawPairedValueRow(row, rowY);
        rowY += 0x20;
    }
}

void Ovl11DrawPairedValueRow(s16 row, s16 y) {
    s16 iconFrame;
    s32 secondValue;
    s32 firstBand;
    s32 test;
    s32 secondBand;
    void *label;
    s32 textOt = gActiveRenderContext->field_D8 + 0x54;

    iconFrame = ((struct Ovl11PairedValues *) D_8006C838)->field_44BA;
    iconFrame += 4;
    iconFrame -= row;
    func_80024A10(textOt, 0x18, y, iconFrame % 4);
    Ovl11DrawPairedValueBars(((struct Ovl11PairedValues *) D_8006C838)->pairs[row], y + 2);
    secondValue = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][1];
    secondBand = 0;
    if (secondValue < 0x1388) {
        secondBand = 2;
        if (secondValue >= 0x7D0) {
            secondBand = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0] < 0x1388;
    firstBand = 0;
    if (test) {
        firstBand = 2;
        if (((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0] >= 0x7D0) {
            firstBand = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0];
    if (test == 0 &&
        ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][1] == 0) {
        label = D_80053770 + gTextBankOffsets[0];
    } else {
        label = (u8 *) D_8005175C + (gTextBankOffsets[0] + gOvl11PairedValueLabelOffsets[secondBand][firstBand]);
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) label, 0xAC, (s32) y);
}

void Ovl11DrawPairedValueBars(s32 *pair, s16 y) {
    s32 bottomY;
    s16 fillY;
    s16 x0;
    s16 x1;
    s16 y0;
    s32 edgeY0;
    s32 edgeY1;
    s32 labelOt;
    s32 renderOt;
    s32 bottomEdge;
    s32 edge;
    u32 *outlineOt;
    u32 *fillOt;

    renderOt = gActiveRenderContext->field_D8;
    edge = 0;
    outlineOt = (u32 *)(renderOt + 0x60);
    fillOt = (u32 *)(renderOt + 0x5C);
    labelOt = renderOt + 0x58;
    do {
        x0 = 0x28;
        if (edge % 3 != 0) {
            x0 = 0xA8;
        }
        edgeY0 = y;
        bottomEdge = edge & 2;
        bottomY = y + 8;
        if (bottomEdge != 0) {
            edgeY0 = bottomY;
        }
        y0 = edgeY0;
        x1 = 0xA8;
        if (bottomEdge != 0) {
            x1 = 0x28;
        }
        edgeY1 = y;
        if (edge % 3 != 0) {
            edgeY1 = bottomY;
        }
        DrawScreenLine(outlineOt, x0, y0, x1, edgeY1, 0xE0B888, 0);
        edge += 1;
    } while (edge < 4);
    fillY = y + 1;
    DrawScreenFlatRect((s32 *)outlineOt, 0x29, fillY, 0x80, 7, 0xC8A060, 0);
    DrawScreenLine(fillOt, 0x68, y, 0x68, bottomY, 0xE0B888, 0);
    x1 = ((pair[1] % 10000) * 64) / 10000;
    DrawScreenGradientRect((s32 *)fillOt, 0x69 - x1, fillY, x1, 7, 0xE8A0B8, 0xD14070, 0xE8A0B8, 0xD14070, 0);
    if (pair[1] / 10000 != 0) {
        DrawSpriteFrameRects(labelOt, (s32)gOvl11StatusIndicatorSpriteSource, 0xA, 0, 0x28, (s16)(y - 0x18));
        Ovl11DrawSlotQuantity(labelOt, 0x40, y - 0x20, pair[1] / 10000 - (pair[1] / 1000000) * 100);
    }
    DrawScreenGradientRect((s32 *)fillOt, 0x69, fillY, ((pair[0] % 10000) * 64) / 10000, 7, 0x4D70A0, 0xA6B8D0, 0x4D70A0, 0xA6B8D0, 0);
    if (pair[0] / 10000 != 0) {
        DrawSpriteFrameRects(labelOt, (s32)gOvl11StatusIndicatorSpriteSource, 0xA, 0, 0x68, (s16)(y - 0x18));
        Ovl11DrawSlotQuantity(labelOt, 0x80, y - 0x20, pair[0] / 10000 - (pair[0] / 1000000) * 100);
    }
}

void Ovl11DrawStatusLevelIcon(u16 arg0, s16 arg1, s16 arg2);
void Ovl11DrawStatusThresholdIcons(u16 arg0, s32 arg1, s32 arg2);
s32 Ovl11GetStatusEntryLevel(u16 arg0);
void Ovl11DrawStatusLevelBar(s32 input_level, u32 input_value, s32 input_x, s32 input_y);
void Ovl11DrawStatusLevelPercent(s32 input_level, u32 input_value, s32 input_x, s32 input_y);

void Ovl11DrawStatusLevelPage(void) {
    s32 i;
    s32 value;

    func_80017B3C(gActiveRenderContext->field_D8 + 0x54,
                 (s32)(D_80053780 + gTextBankOffsets[0]), 0xBA, 0x1A);
    for (i = 0; i < 5; i++) {
        Ovl11DrawStatusLevelIcon(i, 0x28, (s16)(0x30 + i * 0x22));
        Ovl11DrawStatusThresholdIcons(((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0x48, (s16)(0x44 + i * 0x22));
        value = Ovl11GetStatusEntryLevel(i);
        Ovl11DrawStatusLevelBar(value, ((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0x50, (s16)(0x38 + i * 0x22));
        Ovl11DrawStatusLevelPercent(value, ((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0xD7, (s16)(0x3C + i * 0x22));
    }
}

s16 Ovl11FindStatusLevel(s16 entryBase);

s32 Ovl11GetStatusEntryLevel(u16 arg0) {
    if (arg0 >= 5) {
        arg0 = 4;
    }
    return Ovl11FindStatusLevel(gOvl11StatusLevelEntryBases[arg0]);
}

s32 Ovl11IsRequestedStatusLevel(s16 arg0);
s32 Ovl11QueryStatusLevelEntry(s16 arg0);

s16 Ovl11FindStatusLevel(s16 arg0) {
    s32 (*var_s2)(s16);
    s32 var_s5;
    s16 var_s3;
    s32 var_s0;

    var_s3 = 1;
    if ((u32) (gProgressRequest - arg0) < 5U) {
        var_s2 = Ovl11IsRequestedStatusLevel;
        var_s5 = 1;
    } else {
        var_s2 = Ovl11QueryStatusLevelEntry;
        var_s5 = 0;
    }
    var_s0 = 0;
    do {
        if (var_s2((s16) (arg0 + var_s0)) == 1) {
            var_s3 = var_s0 + 1;
        }
        var_s0 += 1;
    } while (var_s0 < 5);
    if (var_s5 != 0) {
        var_s3 = -var_s3;
    }
    return var_s3;
}

s32 Ovl11QueryStatusLevelEntry(s16 arg0) {
    s32 callRet1;
    callRet1 = Ovl11MatchItemSlotsCommand(arg0, 0, 2, 0);
    return callRet1;
}

s32 Ovl11IsRequestedStatusLevel(s16 arg0) {
    return gProgressRequest == arg0;
}

void Ovl11DrawStatusLevelIcon(u16 arg0, s16 arg1, s16 arg2) {
    if (arg0 >= 5) {
        arg0 = 4;
    }
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32)(&gOvl11StatusLevelSpriteSource)), gOvl11StatusLevelIconFrames[arg0 * 2], 0, arg1, arg2);
}

void Ovl11DrawStatusThresholdIcons(u16 arg0, s32 arg1, s32 arg2) {
    s16 var_a1;
    s16 var_s2;
    s32 temp_v1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s1;
    u16 *p;

    var_a1 = arg1;
    var_s2 = arg2;
    var_a2 = 0;
    p = gOvl11StatusLevelThresholds;
    var_s1 = (var_a1 << 0x10) + 0x80000;
    do {
        var_s0 = var_a2 + 1;
        if ((u32) (arg0 & 0xFFFF) >= (u16) p[var_s0]) {
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) gOvl11StatusLevelSpriteSource, var_a2 & 0xFF, 0, (s16) (s32) var_a1, var_s2);
            temp_v1 = var_s1 >> 0x10;
            var_s1 += 0x80000;
            var_a1 = (s16) temp_v1;
        }
        var_a2 = var_s0;
    } while (var_s0 < 4);
}

void Ovl11DrawStatusLevelBar(s32 input_level, u32 input_value, s32 input_x, s32 input_y) {
    s16 level;
    u16 value;
    s32 x;
    s32 y;
    s32 inner_y;
    s32 inner_x;
    s32 bottom;
    s32 right;
    s32 end_x;
    s32 start_x;
    s32 start_y;
    s16 width;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s32 end_y;
    s32 dark_color;
    s32 light_color;
    s32 top_edge;
    s32 i;
    u16 threshold;
    u32 *ot;

    level = input_level;
    value = input_value;
    i = 0;
    end_y = gActiveRenderContext->field_D8;
    ot = (u32 *) (end_y + 0x60);
    for (i = 0; i < 4; i++) {
        x = (s16) input_x;
        right = x + 0x83;
        y = (s16) input_y;
        bottom = y + 8;
        do {
            start_x = x;
            if (i % 3 == 0) {
                goto first_corner_selected;
            }
        } while (0);
        start_x = right;
first_corner_selected:
        x0 = start_x;
        start_y = y;
        top_edge = i < 2;
        if (top_edge == 0) {
            start_y = bottom;
        }
        y0 = start_y;
        end_x = x;
        if (top_edge != 0) {
            end_x = right;
        }
        x1 = end_x;
        end_y = y;
        if (i % 3 != 0) {
            end_y = bottom;
        }
        y1 = end_y;
        DrawScreenLine(ot, x0, y0, x1, y1, 0xE0B888, 0);
    }
    inner_x = x + 1;
    inner_y = y + 1;
    DrawScreenFlatRect((s32 *) ot, inner_x, inner_y, 0x82, 7, 0xC8A060, 0);
    if (level < 0) {
        level *= -1;
    }
    light_color = gOvl11StatusLevelLightColors[level - 1];
    dark_color = gOvl11StatusLevelDarkColors[level - 1];
    if (level == 5) {
        width = 0x82;
    } else {
        threshold = gOvl11StatusLevelThresholds[level - 1];
        width = (s16) (((value - threshold) * 0x82) /
                       (gOvl11StatusLevelThresholds[level] - threshold));
        if (width >= 0x83) {
            width = 0x82;
        }
    }
    DrawScreenGradientRect((s32 *) (gActiveRenderContext->field_D8 + 0x5C), inner_x, inner_y, width, 7,
                  dark_color, light_color, dark_color, light_color, 0);
}

void func_8001ABF0(u16 *dst, u16 *src);

void Ovl11DrawStatusLevelPercent(s32 input_level, u32 input_value, s32 input_x, s32 input_y) {
    s16 arg0;
    u16 arg1;
    s16 arg2;
    s16 arg3;
    s16 *var_a1;
    s16 var_a3;
    u16 temp_a1;
    u32 temp_t0;

    arg0 = input_level;
    arg1 = input_value;
    arg2 = input_x;
    arg3 = input_y;
    temp_t0 = arg1 & 0xFFFF;
    if (arg0 < 0) {
        var_a1 = (s16 *) ((u8 *) D_8005378C + *gTextBankOffsets);
    } else {
        var_a3 = 1;
        if (arg0 == 5) {
            var_a1 = (s16 *) ((u8 *) D_800537A2 + *gTextBankOffsets);
        } else {
            for (; var_a3 < 5 && temp_t0 >= gOvl11StatusLevelThresholds[var_a3]; var_a3++) {
            }
            temp_a1 = gOvl11StatusLevelThresholds[var_a3 - 1];
            func_8001ABF0(func_8001A970(((s32) ((temp_t0 - temp_a1) * 100) /
                                              (s32) (gOvl11StatusLevelThresholds[var_a3] - temp_a1) +
                                          (var_a3 - arg0) * 100) &
                                            0xFFFF,
                                        &gOvl11NumberTextScratch, 4),
                          (u16 *) ((u8 *) D_800535D0 + *gTextBankOffsets));
            var_a1 = &gOvl11NumberTextScratch;
        }
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) var_a1, (s32) arg2,
                  (s32) arg3);
}

s16 Ovl11ReadStatusSignedValue(s16 *value) {
    return *value;
}

void Ovl11DrawStatusUnpaddedNumber(s16 arg0, s16 arg1, s16 arg2) {
    s16 *it;
    s16 *var_a1;
    s32 var_a2;

    it = &gOvl11NumberTextScratch;
    *((u16 *)func_8001A970((s32)arg0, it, 0xE)) = 0xFFFF;
    var_a1 = it;
    var_a2 = 0;
    if ((u16)gOvl11NumberTextScratch == 0xFFD) {
        do {
            var_a2++;
            var_a1++;
        } while (var_a2 < 0xE && (u16)*var_a1 == 0xFFD);
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)var_a1, (s32)arg1, (s32)arg2);
}

/* Append exactly one label code, not a complete string. */
void Ovl11AppendStatusLabelCode(u16 *cursor, s32 labelOffset) {
    s32 offset = *gTextBankOffsets + labelOffset;
    cursor[0] = *(u16 *)((unsigned char *)&D_8005175C + offset);
    cursor[1] = 0xFFFF;
}

void Ovl11DrawStatusPageArrows(void) {
    s32 temp_s0;
    s32 temp_s1;

    temp_s1 = gActiveRenderContext->field_D8 + 0x68;
    temp_s0 = ((s16) gOvl11StatusAnimationTick / 30) & 0xFF;
    DrawSpriteFrameRects(temp_s1, (s32) gOvl11StatusIndicatorSpriteSource, 0, temp_s0, 0x20, 0x18);
    DrawSpriteFrameRects(temp_s1, (s32) gOvl11StatusIndicatorSpriteSource, 2, temp_s0, 0x108, 0x18);
}

void Ovl11DrawStatusScrollUpArrow(void) {
    s32 v = (((s16)gOvl11StatusAnimationTick) / 30) & 0xFF;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32)(&gOvl11StatusIndicatorSpriteSource)), 1, v, 0x120,
                  0x30);
}

void Ovl11DrawStatusScrollDownArrow(void) {
    s32 v = (((s16)gOvl11StatusAnimationTick) / 30) & 0xFF;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32)(&gOvl11StatusIndicatorSpriteSource)), 3, v, 0x120,
                  0xC8);
}

void Ovl11DrawStatusRepeatedIcon(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void Ovl11DrawStatusPrimaryMarks(s16 arg0, s16 arg1, s16 arg2) {
    Ovl11DrawStatusRepeatedIcon(arg0, 5, arg1, arg2);
}

void Ovl11DrawStatusRecordMarks(s16 arg0, s16 arg1, s16 arg2) {
    Ovl11DrawStatusRepeatedIcon(arg0, 4, arg1, arg2);
}

void Ovl11DrawStatusRepeatedIcon(s32 count, s16 animation, s16 rightX, s16 y) {
    s16 x;
    s32 i;

    x = rightX;
    for (i = 0; i < count; i++) {
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, ((s32) gOvl11StatusIndicatorSpriteSource), (u8) animation, 0, x, y);
        x -= 8;
    }
}

void Ovl11NextStatusPage(void) {
    gOvl11StatusPageIndex = gOvl11StatusPageIndex + 1;
    if (((s16) gOvl11StatusPageIndex == 2) && (Ovl11CountStatusSmallRecords() == 0)) {
        gOvl11StatusPageIndex = gOvl11StatusPageIndex + 1;
    }
    if ((s16) gOvl11StatusPageIndex == 3) {
        if (Ovl11CountStatusTaggedRecordsA() == 0) {
            gOvl11StatusPageIndex = gOvl11StatusPageIndex + 1;
        }
    }
    if (((s16) gOvl11StatusPageIndex == 4) && (Ovl11CountStatusTaggedRecordsB() == 0)) {
        gOvl11StatusPageIndex = gOvl11StatusPageIndex + 1;
    }
    if ((s16) gOvl11StatusPageIndex >= 8) {
        gOvl11StatusPageIndex = 0;
    }
}

void Ovl11PreviousStatusPage(void) {
    gOvl11StatusPageIndex = gOvl11StatusPageIndex - 1;
    if (((s16) gOvl11StatusPageIndex == 4) && (Ovl11CountStatusTaggedRecordsB() == 0)) {
        gOvl11StatusPageIndex = gOvl11StatusPageIndex - 1;
    }
    if ((s16) gOvl11StatusPageIndex == 3) {
        if (Ovl11CountStatusTaggedRecordsA() == 0) {
            gOvl11StatusPageIndex = gOvl11StatusPageIndex - 1;
        }
    }
    if (((s16) gOvl11StatusPageIndex == 2) && (Ovl11CountStatusSmallRecords() == 0)) {
        gOvl11StatusPageIndex = gOvl11StatusPageIndex - 1;
    }
    if ((s16) gOvl11StatusPageIndex < 0) {
        gOvl11StatusPageIndex = 7;
    }
}

s32 Ovl11CountStatusNonzeroEntries(void) {
    s32 var_a1;
    s32 var_v1;
    u16 *var_a0;
    char *base;

    var_a1 = 0;
    base = (char *)&D_8006C838;
    base += 0x901C;
    var_a0 = (u16 *)base;
    var_v1 = 0x62;
    do {
        if (*var_a0 != 0) {
            var_a1 += 1;
        }
        var_v1 -= 1;
        var_a0 += 2;
    } while (var_v1 >= 0);
    return var_a1;
}

s32 Ovl11CountStatusSmallRecords(void) {
    s32 count = 0;
    u32 mask = 0x02000000;
    char *base = (char *)&D_8006C838;
    s8 *p = (s8 *)(base + 0x7AE8);
    s32 n = 9;

    do {
        if (*(u16 *)(p - 0x34) != 0u && !(*(u32 *)p & mask)) {
            count++;
        }
        p += 0xB4;
        n--;
    } while (n >= 0);
    return count;
}

s32 Ovl11CountStatusTaggedRecordsA(void) {
    s32 count;
    char *base;
    s32 remaining;
    u16 *tag;

    count = 0;
    base = (char *)&D_8006C838;
    base += 0x81BC;
    tag = (u16 *)base;
    remaining = 19;
    do {
        if ((*tag - 0x160) < 4u) {
            count += 1;
        }
        tag = (u16 *)((char *)tag + 0xB8);
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}

s32 Ovl11CountStatusTaggedRecordsB(void) {
    s32 count;
    char *base;
    s32 remaining;
    u16 *tag;

    count = 0;
    base = (char *)&D_8006C838;
    base += 0x81BC;
    tag = (u16 *)base;
    remaining = 19;
    do {
        if ((*tag - 0x164) < 3u) {
            count += 1;
        }
        tag = (u16 *)((char *)tag + 0xB8);
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
