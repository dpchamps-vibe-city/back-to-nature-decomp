/*
 * Selection-menu transaction, ovl_11:800FFDCC–80103770.
 *
 * Public lifecycle: ovl11_selection_mode_state.h. Catalog query, transactional
 * candidate trial and installed result callbacks: ovl11_selection_candidate.h.
 * The record grid has four pages of twenty records. Flag editing works on
 * seven individual bits plus five aggregate bits; item editing builds an
 * eight-ID work set from nine pages of six-byte inventory entries. Commit
 * consumes the work items, publishes a sprite request and selects a result
 * callback. Catalog IDs remain neutral: their gameplay meaning is unproven.
 *
 * One installed callback drives the whole menu. A callback change resets the
 * shared cursor bytes; it is not equivalent to a direct helper call. Trial
 * validation restores flags/IDs after each candidate, whereas commit repeats
 * the trial and writes the selected record. No screen is reentrant.
 *
 * Existing signed/unsigned and word/callback views of the same work storage
 * are retained; they describe witnessed access widths, not separate objects.
 * Membership, exclusive state and evidence-limited edges: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_selection_mode_state.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "packet_work.h"
#include "screen_fade.h"
#include "cd_audio.h"
#include "sprite_grid.h"
#include "message_window.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "integer_power.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_texture_stream.h"
#include "ovl11_texture_cell_cache.h"
#include "game_types.h"
#include "beveled_panel.h"
#include "ovl11_slot_decorations.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_selection_candidate.h"
#include "game_flags.h"
#include "ovl11_menu_exit_flags.h"
#include "ui_sprites.h"

void Ovl11ResetSelectionModeState(void) {
    s32 i;

    Ovl11_D_801273DC = 0;
    gOvl11LastSelectionHandler = 0;
    D_801273E4_state = 0;
    D_801273E5_origin = 0;
    gOvl11SelectionConfirmedRecord = 0xFF;
    Ovl11_D_801273E7 = 0;
    Ovl11_D_801273E8 = 0;
    D_801273E9_selection = 0;
    D_801273EA_table = 0;
    D_801273EB = 0;
    D_801273EE_flags = 0;
    for (i = 0; i < 8; i++) {
        D_8012CF00_ids[i] = 0;
    }
    D_801273F0 = 0;
    gOvl11SelectionRecordIndex = 0;
    D_80127424 = 0;
}

void Ovl11PromptRecordGridEntry(void);
void SetVal8005E2BC(s32 arg0);

void Ovl11EnterSelectionModeDefault(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    Ovl11ResetSelectionModeState();
    Ovl11ResetTextureCellCache();
    D_801273E4_state = 1;
    D_801273E5_origin = 0;
    Ovl11_D_801273DC = Ovl11PromptRecordGridEntry;
    PlaySoundEffect(3);
    Ovl11ServiceTextureStream(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}

extern void Ovl11ServiceSelectionRecordGrid(void);

void Ovl11EnterSelectionModeAlternate(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    Ovl11ResetSelectionModeState();
    Ovl11ResetTextureCellCache();
    D_801273E4_state = 1;
    D_801273E5_origin = 1;
    Ovl11_D_801273DC = Ovl11ServiceSelectionRecordGrid;
    PlaySoundEffect(3);
    Ovl11ServiceTextureStream(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}

u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void func_80017A38(s16 arg0, s16 arg1);

void Ovl11ServiceSelectionMode(void) {
    s32 orderingTable;
    u32 savedPalette;

    orderingTable = gActiveRenderContext->field_D8 + 0x78;
    if (D_801273E4_state != 0) {
        if (D_801273E4_state == 1) {
            DrawScrollingSpriteGrid(orderingTable);
            if ((IsMessageWindowStatusFinal() != 0) && (Ovl11ServiceTextureStream(0) == 0)) {
                CloseMessageWindow();
                D_801273E4_state = 3;
            }
        } else {
            DrawScrollingSpriteGridInContext(orderingTable);
            if (D_801273E4_state == 8) {
                if (GetCdAudioState() == 0) {
                    D_801273E4_state = 3;
                    BeginScreenFadeDecrease(0xA, 0, 2);
                }
            } else {
                if (gOvl11LastSelectionHandler != Ovl11_D_801273DC) {
                    D_801273EA_table = 0;
                    D_801273E9_selection = 0;
                    Ovl11_D_801273E8 = 0;
                    Ovl11_D_801273E7 = 0;
                }
                savedPalette = func_80017A64();
                func_80017A48(3U);
                func_80017A38(1, 2);
                Ovl11ClearTextureCellActivity();
                gOvl11LastSelectionHandler = Ovl11_D_801273DC;
                Ovl11_D_801273DC();
                func_80017A48(savedPalette);
                func_80017A38(0, 2);
            }
        }
    }
}

/* Catalog lookup and the record-grid prompt/selection chain. */
s32 Ovl11SelectionCatalogContainsId(s16 id) {
    s32 index;

    for (index = 0; gSelectionCatalog[index].id != 0xE2; index++) {
        if (gSelectionCatalog[index].id == id) {
            return 1;
        }
    }
    return 0;
}

extern void Ovl11PromptSelectionGridMode(void);
extern void Ovl11FinishSelectionMenu(void);

void Ovl11PromptRecordGridEntry(void) {
    RequestMessageWindow(3, 0x3B6);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            gOvl11SelectionHandler = (s32)Ovl11PromptSelectionGridMode;
        } else {
            gOvl11SelectionHandler = (s32)Ovl11FinishSelectionMenu;
        }
    }
}

extern void Ovl11ServiceSelectionFlagSlots(void);

void Ovl11PromptSelectionGridMode(void) {
    s32 var;
    s16 val;

    RequestMessageWindow(3, 0x3B7);
    var = GetMessageWindowStatus();
    if (var == 2) {
        val = GetMessageWindowChoice();
        if (val == 1) {
            gOvl11SelectionHandler = (s32)Ovl11ServiceSelectionRecordGrid;
        } else if (val == 2) {
            gOvl11SelectionHandler = (s32)Ovl11ServiceSelectionFlagSlots;
        } else {
            gOvl11SelectionHandler = (s32)Ovl11FinishSelectionMenu;
        }
    }
}

void Ovl11DrawSelectionRecordNames(u8 arg0);
s32 Ovl11TestSelectionRecordRequirements(s32 arg0);
void Ovl11FinishSelectionMenu(void);
void Ovl11PromptSelectionGridMode(void);
void Ovl11ShowSelectionRecord(void);
void Ovl11ShowSelectionRecordRequirements(void);
void Ovl11ChooseSelectionRecordAction(void);

void Ovl11ServiceSelectionRecordGrid(void) {
    s8 column;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B8);
    Ovl11DrawSelectionRecordNames(Ovl11_D_801273E8);
    if (IsMessageWindowStatusFinal()) {
        switch ((u32)((GfxObj *)gPad0Input)->field_8 & 0x870) {
        case 0x800:
            CloseMessageWindow();
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
            return;
        case 0x20:
            CloseMessageWindow();
            PlaySoundEffect(1);
            Ovl11_D_801273DC = Ovl11PromptSelectionGridMode;
            return;
        case 0x10:
            if (((Ovl11SelectionRecords *)&D_8006C838)->records[Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7][0] != 0) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                Ovl11_D_801273DC = Ovl11ShowSelectionRecord;
                return;
            }
            PlaySoundEffect(2);
            break;
        case 0x40:
            if (((Ovl11SelectionRecords *)&D_8006C838)->records[Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7][0] == 0) {
                PlaySoundEffect(2);
            } else if (Ovl11TestSelectionRecordRequirements((u8)(Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7)) == 0) {
                CloseMessageWindow();
                PlaySoundEffect(2);
                Ovl11_D_801273DC = Ovl11ShowSelectionRecordRequirements;
                return;
            } else {
                CloseMessageWindow();
                PlaySoundEffect(0);
                Ovl11_D_801273DC = Ovl11ChooseSelectionRecordAction;
                return;
            }
            break;
        }
        switch (*(u32 *)gPad0Input & 0xF000) {
        case 0x1000:
            PlaySoundEffect(5);
            Ovl11_D_801273E7--;
            if (Ovl11_D_801273E7 == -1 || Ovl11_D_801273E7 == 9) {
                Ovl11_D_801273E7 += 10;
            }
            break;
        case 0x4000:
            PlaySoundEffect(5);
            Ovl11_D_801273E7++;
            if (Ovl11_D_801273E7 == 10 || Ovl11_D_801273E7 == 20) {
                Ovl11_D_801273E7 -= 10;
            }
            break;
        case 0x8000:
            PlaySoundEffect(5);
            Ovl11_D_801273E7 -= 10;
            if (Ovl11_D_801273E7 < 0) {
                Ovl11_D_801273E7 += 20;
                Ovl11_D_801273E8--;
            }
            break;
        case 0x2000:
            PlaySoundEffect(5);
            Ovl11_D_801273E7 += 10;
            if (Ovl11_D_801273E7 >= 20) {
                Ovl11_D_801273E7 -= 20;
                Ovl11_D_801273E8++;
            }
            break;
        }
        if (Ovl11_D_801273E8 < 0) {
            Ovl11_D_801273E8 = 3;
        } else if (Ovl11_D_801273E8 >= 4) {
            Ovl11_D_801273E8 = 0;
        }
    }
    column = Ovl11_D_801273E7 / 10;
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54,
                 column * 144 + 23, (Ovl11_D_801273E7 - column * 10) * 14 + 24);
    gOvl11SelectionRecordIndex = Ovl11_D_801273E7 + Ovl11_D_801273E8 * 20;
}

void Ovl11DrawSelectionRecordSlots(u8 arg0, u8 arg1);
void Ovl11ServiceSelectionRecordGrid(void);

void Ovl11ShowSelectionRecord(void) {
    s32 buttons;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B9);
    Ovl11DrawSelectionRecordSlots((u8)gOvl11SelectionRecordIndex, 0);
    if (IsMessageWindowStatusFinal() != 0) {
        buttons = ((GfxObj *)gPad0Input)->field_8;
        if (buttons & 0x800) {
            CloseMessageWindow();
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        } else if (buttons & 0x70) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            Ovl11_D_801273DC = Ovl11ServiceSelectionRecordGrid;
        }
    }
}

void Ovl11ShowSelectionRecordRequirements(void) {
    s32 buttons;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3BA);
    Ovl11DrawSelectionRecordSlots((u8)gOvl11SelectionRecordIndex, 1);
    if (IsMessageWindowStatusFinal() != 0) {
        buttons = ((GfxObj *)gPad0Input)->field_8;
        if (buttons & 0x800) {
            CloseMessageWindow();
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        } else if (buttons & 0x70) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            Ovl11_D_801273DC = Ovl11ServiceSelectionRecordGrid;
        }
    }
}

void Ovl11LoadSelectionWorkSet(s32 arg0);
void Ovl11CommitSelectionItemGrid(void);
void Ovl11ServiceSelectionFlagSlots(void);

void Ovl11ChooseSelectionRecordAction(void) {
    s32 state;
    s16 choice;

    Ovl11DrawSelectionRecordSlots((u8)gOvl11SelectionRecordIndex, 0);
    RequestMessageWindow(3, 0x3BB);
    state = GetMessageWindowStatus();
    if (state == 2) {
        choice = GetMessageWindowChoice();
        if (choice == 1) {
            gOvl11SelectionConfirmedRecord = gOvl11SelectionRecordIndex;
            Ovl11LoadSelectionWorkSet((u8)gOvl11SelectionRecordIndex);
            BeginScreenFadeIncrease(10, 0, 2);
            Ovl11_D_801273DC = Ovl11CommitSelectionItemGrid;
        } else if (choice == 2) {
            Ovl11LoadSelectionWorkSet((u8)gOvl11SelectionRecordIndex);
            Ovl11_D_801273DC = Ovl11ServiceSelectionFlagSlots;
        } else if (choice == 3) {
            Ovl11_D_801273DC = Ovl11ServiceSelectionRecordGrid;
        } else {
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

void func_80017A38 (s16 arg0, s16 arg1);
s32 Ovl11LookupSelectionTextOffset (u16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Ovl11DrawSelectionRecordNames(u8 arg0) {
    s32 temp_s0;
    s32 temp_t0;
    s32 base;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    u16 temp_a0;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s0 = temp_t0 + 0x60;
    base = temp_t0 + 0x70;
    DrawMenuPanel((u32 *)(temp_t0 + 0x74), 1, 0xE, 0x10, 0x124, 0x94);
    DrawAnimatedUiSprite0D(temp_s0, 6, 0x5A);
    DrawAnimatedUiSprite0C(temp_s0, 0x12A, 0x5A);
    func_80017A38(0, 2);
    var_s5 = 0x1C;
    for (var_s2 = 0; var_s2 < 2; var_s2++) {
        var_s1 = 0x15;
        for (var_s0 = 0; var_s0 < 0xA; var_s0++) {
            temp_a0 = *((u16 *) ((u8 *) &gSelectionRecordTable[0] + (((var_s2 * 0xA) + ((arg0 & 0xFF) * 0x14) + var_s0) * 0x16)));
            if (temp_a0 == 0) {
                break;
            }
            func_80017B3C(base, (s32) ((void *) ((u8 *) D_8005175C + (*gTextBankOffsets + Ovl11LookupSelectionTextOffset(temp_a0)))), var_s5, var_s1);
            var_s1 = (s32) (s16) (var_s1 + 0xE);
        }
        var_s5 = (s32) (s16) (var_s5 + 0x90);
    }
    func_80017A38(1, 2);
}

s32 Ovl11LookupSelectionTextOffset(u16 id) {
    s32 index;

    for (index = 0; gSelectionCatalog[index].id != 0xE2; index++) {
        if (gSelectionCatalog[index].id == id) {
            return gSelectionCatalog[index].textOffset;
        }
    }
    return 0x2206;
}

s32 Ovl11TestSelectionUnlockedMask(u16 arg0);
s32 Ovl11SelectionItemExists(s32 arg0);

s32 Ovl11TestSelectionRecordRequirements(s32 arg0) {
    u8 *base;
    s32 mask;
    s32 count;
    s32 i;
    u16 *p;

    base = (u8 *)&gSelectionRecordTable[0] + ((arg0 & 0xFF) * 0x16);
    mask = 1;
    count = 0;
    do {
        if ((*(u16 *)(base + 4) & mask) && Ovl11TestSelectionUnlockedMask(mask) == 0) {
            return 0;
        }
        mask = (mask * 2) & 0xFFFE;
        count++;
    } while (count < 12);
    i = 0;
    p = (u16 *)(base + 6);
    for (; i < 8; i++) {
        if (Ovl11SelectionItemExists(*p) == 0) {
            return 0;
        }
        p++;
    }
    return 1;
}

s32 Ovl11TestSelectionUnlockedMask (u16 arg0);
s32 Ovl11SelectionItemExists (s32 arg0);
void SetVal8005E2BC (s32 arg0);
void Ovl11LoadTextureCellRecord (u16 arg0);
void Ovl11DrawCachedTextureCell (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void Ovl11LoadSelectionWorkSet (s32 arg0);

void Ovl11DrawSelectionRecordSlots(u8 arg0, u8 arg1) {
    s32 sp18;
    s32 *sp1C;
    s32 sp20;
    s32 sp24;
    u16 var_s4;
    u16 var_s5;
    s32 temp_s3;
    u8 *temp_v1;
    s32 var_s6;

    u16 *var_s2;

    u16 var_s7;
    u32 *temp_s0;
    u8 *temp_fp;

    sp18 = arg1;
    temp_v1 = (u8 *)gActiveRenderContext->field_D8;
    var_s6 = 0;
    temp_s0 = (u32 *)(temp_v1 + 0x74);
    temp_s3 = (s32)(temp_v1 + 0x70);
    sp1C = (s32 *)(temp_v1 + 0x68);
    sp20 = (s32)(temp_v1 + 0x6C);
    sp24 = (s32)(temp_v1 + 0x60);
    temp_fp = (void *) ((u8 *) gSelectionRecordTable + ((arg0 & 0xFF) * 0x16));
    DrawMenuPanel(temp_s0, 1, 0x16, 0x10, 0x10E, 0x4A);
    var_s4 = 0x84;
    var_s5 = 0x14;
    var_s7 = 1;
    DrawMenuPanel(temp_s0, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(temp_s3, (s32) ((void *) ((u8 *) &D_8005396A + *gTextBankOffsets)), 0x21, 0x20);
    func_80017B3C(temp_s3, (s32) ((void *) ((u8 *) ((void *) ((u8 *) &D_8005396A + 0x12)) + *gTextBankOffsets)), 0x21, 0x6A);
    var_s2 = gOvl11SelectionFlagSlotIds;
    for (; var_s6 < 7; var_s6++) {
        if (*(u16 *)(temp_fp + 4) & var_s7) {
            Ovl11DrawTextureCell(sp1C, *var_s2, var_s4, var_s5);
            if (sp18 == 1 && Ovl11TestSelectionUnlockedMask(var_s7) == 0) {
                Ovl11DrawSelectedSlotMarker(sp24, var_s4, var_s5);
            }
        } else {
            Ovl11DrawEmptySlot(sp20, var_s4, var_s5);
        }
        var_s4 += 0x22;
        if ((u32) var_s4 >= 0xEBU) {
            var_s4 = 0x84;
            var_s5 += 0x22;
        }
        var_s7 *= 2;
        var_s2++;
    }
    if (*(u16 *)(temp_fp + 4) & 0xF80) {
        Ovl11DrawTextureCell(sp1C, 0x79U, var_s4, var_s5);
        if (sp18 == 1 && Ovl11TestSelectionUnlockedMask(var_s7) == 0) {
            Ovl11DrawSelectedSlotMarker(sp24, var_s4, var_s5);
        }
    } else {
        Ovl11DrawEmptySlot(sp20, var_s4, var_s5);
    }
    var_s4 = 0x84;
    var_s5 = 0x5E;
    var_s2 = (u16 *)(temp_fp + 6);
    for (var_s6 = 0; var_s6 < 8; var_s6++) {
        Ovl11DrawTextureCell(sp1C, *var_s2, var_s4, var_s5);
        if (sp18 == 1 && Ovl11SelectionItemExists(*var_s2) == 0) {
            Ovl11DrawSelectedSlotMarker(sp24, var_s4, var_s5);
        }
        var_s4 += 0x22;
        if ((u32)var_s4 >= 0xEBU) {
            var_s4 = 0x84;
            var_s5 += 0x22;
        }
        var_s2++;
    }
}

s32 Ovl11TestSelectionUnlockedMask(u16 arg0) {
    if (arg0 < 0x80U) {
        if ((gSelectionUnlockedFlags & arg0) == 0) {
            return 0;
        }
    } else if ((gSelectionUnlockedFlags & 0x80) == 0) {
        return 0;
    }
    return 1;
}

s32 Ovl11SelectionItemExists(s32 rawId) {
    u16 id;
    u8 matches;
    s32 index;

    id = rawId & 0xFFFF;
    matches = 0;

    if (id == 0) {
        return 1;
    }

    for (index = 0; index < pow_int(2, ((SelectionItemSearchView *)D_8006C838)->capacityExponent + 1); index++) {
        if (((SelectionItemSearchView *)D_8006C838)->secondaryItems[index].id == id) {
            matches = 1;
            break;
        }
    }

    for (index = 0; index < 0x40; index++) {
        if (((SelectionItemSearchView *)D_8006C838)->storedItems[index].id == id) {
            matches = (matches + 1) & 0xFF;
            break;
        }
    }

    return matches != 0;
}

void Ovl11LoadSelectionWorkSet(s32 recordIndex) {
    u16 *sourceId;
    u16 *destinationId;
    s32 remaining;
    u16 id;
    u8 *record;

    destinationId = &gOvl11SelectionWorkIds[0];
    record = (u8 *)&gSelectionRecordTable[0] + ((recordIndex & 0xFF) * 0x16);
    sourceId = (u16 *)(record + 6);
    remaining = 7;
    gOvl11SelectionWorkFlags = *(u16 *)(record + 4);
    do {
        id = *sourceId;
        sourceId++;
        remaining--;
        *destinationId = id;
        destinationId++;
    } while (remaining >= 0);
}

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Ovl11DrawSelectionFlagSlots(void);
void Ovl11DrawSelectionWorkItems(void);
void Ovl11ShowSelectionFlagMessage(s8 arg0);
void Ovl11ServiceSelectionAggregateFlags(void);
s32 Ovl11TestSelectionSlotFlag(s8 arg0);
s32 Ovl11SelectionSlotMask(s8 arg0);
void Ovl11ServiceSelectionItemGrid(void);

/* Flag-slot editing shares the same installed-handler and work-set state. */
void Ovl11ServiceSelectionFlagSlots(void) {
    s32 ui;
    s32 text;
    u32 *panels;

    ui = gActiveRenderContext->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    DrawMenuPanel(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    DrawMenuPanel(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_8005396A + 0x28 + gTextBankOffsets[0]), 0x21, 0x20);
    func_80017B3C(text, (s32)(D_8005396A + gTextBankOffsets[0]), 0x21, 0x6A);
    Ovl11DrawSelectionFlagSlots();
    Ovl11DrawSelectionWorkItems();
    if (D_801273E4_state == 4) {
        Ovl11ShowSelectionFlagMessage(Ovl11_D_801273E7);
    } else if (D_801273E4_state == 5) {
        Ovl11ServiceSelectionAggregateFlags();
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3BC);
        if (IsMessageWindowStatusFinal()) {
            switch (((u32 *)gPad0Input)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                PlaySoundEffect(5);
                break;
            case 0x2000:
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                PlaySoundEffect(5);
                break;
            case 0x8000:
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                PlaySoundEffect(5);
                break;
            }
        }
        DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (IsMessageWindowStatusFinal()) {
            if (((u32 *)gPad0Input)[2] & 0x800) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                D_801273F0 = 0;
                Ovl11_D_801273DC = Ovl11ServiceSelectionItemGrid;
            } else if (((u32 *)gPad0Input)[2] & 0x20) {
                PlaySoundEffect(1);
                if (D_801273EE_flags != 0) {
                    D_801273EE_flags = 0;
                } else if (D_801273E5_origin == 0) {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = Ovl11PromptSelectionGridMode;
                } else {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = Ovl11ServiceSelectionRecordGrid;
                }
            } else if (((u32 *)gPad0Input)[2] & 0x10) {
                if (gSelectionUnlockedFlags & Ovl11SelectionSlotMask(Ovl11_D_801273E7)) {
                    CloseMessageWindow();
                    PlaySoundEffect(0);
                    D_801273E4_state = 4;
                } else {
                    PlaySoundEffect(2);
                }
            } else if (((u32 *)gPad0Input)[2] & 0x40) {
                if (Ovl11TestSelectionSlotFlag(Ovl11_D_801273E7) == 1) {
                    PlaySoundEffect(1);
                    if (Ovl11_D_801273E7 < 7) {
                        D_801273EE_flags &= ~Ovl11SelectionSlotMask(Ovl11_D_801273E7);
                    } else {
                        CloseMessageWindow();
                        D_801273E4_state = 5;
                    }
                } else {
                    if (gSelectionUnlockedFlags & Ovl11SelectionSlotMask(Ovl11_D_801273E7)) {
                        PlaySoundEffect(0);
                        if (Ovl11_D_801273E7 < 7) {
                            D_801273EE_flags |= Ovl11SelectionSlotMask(Ovl11_D_801273E7);
                        } else {
                            CloseMessageWindow();
                            D_801273E4_state = 5;
                        }
                    } else {
                        PlaySoundEffect(2);
                    }
                }
            }
        }
    }
}

void Ovl11ShowSelectionFlagMessage(s8 arg0) {
    RequestMessageWindow(3, gOvl11SelectionFlagSlotIds[arg0]);
    if (GetMessageWindowStatus() == 2) {
        gOvl11SelectionModePhase = 3;
    }
}

void func_80017D9C(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4);

void Ovl11ServiceSelectionAggregateFlags(void) {
    s32 *text;

    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3BD);
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x5C), 1, 0xA6, 0x58, 0x68, 0x48);
    if (IsMessageWindowStatusFinal()) {
        switch (((u32 *)gPad0Input)[0]) {
        case 0x1000:
        case 0x8000:
            D_801273E9_selection += 4;
            PlaySoundEffect(5);
            break;
        case 0x2000:
        case 0x4000:
            D_801273E9_selection++;
            PlaySoundEffect(5);
            break;
        }
        D_801273E9_selection -= (s8)(D_801273E9_selection / 5) * 5;
    }
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54, 0xAF, D_801273E9_selection * 14 + 0x5D);
    if (IsMessageWindowStatusFinal()) {
        if (((u32 *)gPad0Input)[2] & 0x20) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            D_801273E4_state = 3;
        } else if (((u32 *)gPad0Input)[2] & 0x40) {
            D_801273EE_flags ^= 1 << (D_801273E9_selection + 7);
            if ((D_801273EE_flags >> (D_801273E9_selection + 7)) & 1) {
                PlaySoundEffect(0);
            } else {
                PlaySoundEffect(1);
            }
        }
    }
    text = (s32 *)(gActiveRenderContext->field_D8 + 0x58);
    func_80017D9C(text, (u16 *)(D_8005399A + gTextBankOffsets[0]), 0xB1, 0x5A, ((D_801273EE_flags >> 7) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0xC + gTextBankOffsets[0]), 0xB1, 0x68, ((D_801273EE_flags >> 8) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x16 + gTextBankOffsets[0]), 0xB1, 0x76, ((D_801273EE_flags >> 9) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x26 + gTextBankOffsets[0]), 0xB1, 0x84, ((D_801273EE_flags >> 10) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x38 + gTextBankOffsets[0]), 0xB1, 0x92, ((D_801273EE_flags >> 11) ^ 1) & 1);
}

void Ovl11DrawSelectionFlagSlots(void) {
    u16 var_s0;
    u16 var_s1;
    s32 *temp_s3;
    s32 temp_s2;
    s32 temp_v0;
    s32 var_s4;
    u8 var_s6;

    var_s0 = 0x84;
    var_s1 = 0x14;
    var_s6 = 1;
    temp_v0 = gActiveRenderContext->field_D8;
    for (var_s4 = 0; var_s4 < 7; var_s4++) {
        temp_s3 = (s32 *)(temp_v0 + 0x68);
        temp_s2 = temp_v0 + 0x6C;
        if (D_801273EE_flags & var_s6) {
            Ovl11DrawTextureCell(temp_s3, gOvl11SelectionFlagSlotIds[var_s4], var_s0, var_s1);
        } else {
            Ovl11DrawEmptySlot(temp_s2, var_s0, var_s1);
        }
        var_s0 += 0x22;
        if ((u32) var_s0 >= 0xEBU) {
            var_s0 = 0x84;
            var_s1 = (var_s1 + 0x22) & 0xFFFF;
        }
        var_s6 <<= 1;
    }
    if (D_801273EE_flags & 0xF80) {
        Ovl11DrawTextureCell(temp_s3, 0x79U, var_s0, var_s1);
        return;
    }
    Ovl11DrawEmptySlot(temp_s2, var_s0, var_s1);
}

void Ovl11DrawSelectionWorkItems(void) {
    s32 sp10;
    u16 var_s2;
    u16 var_s4;
    s32 *temp_s6;
    s32 temp_s5;
    s32 temp_v0;
    u8 var_s3;
    s32 var_s7;

    var_s2 = 0x84;
    var_s4 = 0x5E;
    var_s3 = 1;
    temp_v0 = gActiveRenderContext->field_D8;
    temp_s6 = (s32 *)(temp_v0 + 0x68);
    temp_s5 = temp_v0 + 0x6C;
    sp10 = temp_v0 + 0x60;
    for (var_s7 = 0; var_s7 < 7; var_s7++) {
        if (((Ovl11IconFlagsView *)D_8006C838)->flags & var_s3) {
            Ovl11DrawTextureCell(temp_s6, gOvl11SelectionFlagSlotIds[var_s7], var_s2, var_s4);
            if (D_801273EE_flags & var_s3) {
                Ovl11DrawSelectedSlotMarker(sp10, var_s2, var_s4);
            }
        } else {
            Ovl11DrawEmptySlot(temp_s5, var_s2, var_s4);
        }
        var_s2 += 0x22;
        if ((u32) var_s2 >= 0xEBU) {
            var_s2 = 0x84;
            var_s4 = (var_s4 + 0x22) & 0xFFFF;
        }
        var_s3 <<= 1;
    }
    if (((Ovl11IconFlagsView *)D_8006C838)->flags & 0x80) {
        Ovl11DrawTextureCell(temp_s6, 0x79U, var_s2, var_s4);
        if (D_801273EE_flags & 0xF80) {
            Ovl11DrawSelectedSlotMarker(sp10, var_s2, var_s4);
        }
    } else {
        Ovl11DrawEmptySlot(temp_s5, var_s2, var_s4);
    }
}

s32 Ovl11TestSelectionSlotFlag(s8 arg0) {
    s32 callRet1;
    if (arg0 < 7) {
        callRet1 = Ovl11SelectionSlotMask(arg0);
        return ((u32)0) < ((u32)(gOvl11SelectionWorkFlags & callRet1));
    } else {
        return ((u32)0) < ((u32)(gOvl11SelectionWorkFlags & 0xF80));
    }
}

s32 Ovl11SelectionSlotMask(s8 arg0) {
    return (1 << arg0) & 0xFFFF;
}

void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void Ovl11DrawSelectionWorkIdRow(void);
void Ovl11DrawSelectionItemGridPage(void);
void Ovl11ShowSelectionGridItemMessage(s8 arg0, s8 arg1);
void Ovl11ConfirmSelectionItemGrid(void);
s32 Ovl11SelectionWorkSetContainsItem(s8 arg0, s8 arg1);
void *Ovl11GetSelectionGridItem(s8 arg0, s8 arg1);

/* Page zero is the primary secondary-item list; pages 1..8 select the
 * eight-entry rows beginning at resident work+450A. No item is consumed here. */
void Ovl11ServiceSelectionItemGrid(void) {
    s32 ui;
    s32 text;
    u32 *panels;
    s16 empty;
    s32 i;

    ui = gActiveRenderContext->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    DrawMenuPanel(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    DrawMenuPanel(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_800539E8 + gTextBankOffsets[0]), 0x21, 0x20);
    if (D_801273EA_table == 0) {
        func_80017B3C(text, (s32)(D_800539E8 + 0x16 + gTextBankOffsets[0]), 0x21, 0x6A);
    } else {
        func_80017B3C(text, (s32)(D_800539E8 + 0x28 + gTextBankOffsets[0]), 0x21, 0x6A);
        func_8001AAF4(D_801273EA_table, 1, text, 0x2A, 0x78);
        func_80017B3C(text, (s32)(D_800539E8 + 0x36 + gTextBankOffsets[0]), 0x33, 0x78);
        func_8001AAF4(8, 1, text, 0x3C, 0x78);
    }
    Ovl11DrawSelectionWorkIdRow();
    Ovl11DrawSelectionItemGridPage();
    empty = 8;
    for (i = 0; i < 8; i++) {
        if (D_8012CF00_ids[i] == 0) {
            empty = i;
            break;
        }
    }
    if (D_801273E4_state == 6) {
        Ovl11ShowSelectionGridItemMessage(Ovl11_D_801273E7, D_801273EA_table);
    } else if (D_801273E4_state == 7) {
        Ovl11ConfirmSelectionItemGrid();
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3BE);
        if (IsMessageWindowStatusFinal()) {
            switch (((u32 *)gPad0Input)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                PlaySoundEffect(5);
                break;
            case 0x2000:
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == 7) {
                    D_801273EA_table++;
                }
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                PlaySoundEffect(5);
                break;
            case 0x8000:
                if (Ovl11_D_801273E7 == 0 || Ovl11_D_801273E7 == 4) {
                    D_801273EA_table--;
                }
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                PlaySoundEffect(5);
                break;
            }
            if (((u32 *)gPad0Input)[0] & 0xA) {
                D_801273EA_table++;
                PlaySoundEffect(5);
            }
            if (((u32 *)gPad0Input)[0] & 5) {
                D_801273EA_table--;
                PlaySoundEffect(5);
            }
            if (D_801273EA_table < 0) {
                D_801273EA_table = 8;
            }
            if (D_801273EA_table > 8) {
                D_801273EA_table = 0;
            }
        }
        DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (IsMessageWindowStatusFinal()) {
            if (((u32 *)gPad0Input)[2] & 0x800) {
                for (i = 0; i < 8; i++) {
                    if (D_8012CF00_ids[i] != 0) {
                        CloseMessageWindow();
                        PlaySoundEffect(0);
                        D_801273E4_state = 7;
                        return;
                    }
                }
                PlaySoundEffect(2);
            } else if (((u32 *)gPad0Input)[2] & 0x10) {
                if (*(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                    CloseMessageWindow();
                    PlaySoundEffect(0);
                    D_801273E4_state = 6;
                } else {
                    PlaySoundEffect(2);
                }
            } else if (((u32 *)gPad0Input)[2] & 0x20) {
                PlaySoundEffect(1);
                if (*(u16 *)&D_801273F0 != 0) {
                    for (i = 0; i < 8; i++) {
                        D_8012CF00_ids[i] = 0;
                    }
                } else {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = Ovl11ServiceSelectionFlagSlots;
                }
            } else if (((u32 *)gPad0Input)[2] & 0x40) {
                if (*(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7) == 0) {
                    PlaySoundEffect(2);
                    return;
                }
                if (Ovl11SelectionWorkSetContainsItem(D_801273EA_table, Ovl11_D_801273E7) == 1) {
                    for (i = 0; i < 8; i++) {
                        if (D_8012CF00_ids[i] == *(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7)) {
                            PlaySoundEffect(1);
                            D_8012CF00_ids[i] = 0;
                            break;
                        }
                    }
                } else {
                    if (*(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                        if (empty < 8 && Ovl11GetItemType(*(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7)) != 1) {
                            PlaySoundEffect(0);
                            D_8012CF00_ids[empty] = *(s16 *)Ovl11GetSelectionGridItem(D_801273EA_table, Ovl11_D_801273E7);
                        } else {
                            PlaySoundEffect(2);
                        }
                    } else {
                        PlaySoundEffect(2);
                    }
                }
            }
        }
    }
}

void Ovl11ShowSelectionGridItemMessage(s8 arg0, s8 arg1) {
    s16 *p;
    s32 v;

    p = Ovl11GetSelectionGridItem(arg1, arg0);
    v = p[0];
    if (v == 0xA4) {
        RequestMessageWindow(3, Ovl11GetRecordItemId(p) + 0x3E8);
    } else {
        RequestMessageWindow(3, v);
    }
    if (GetMessageWindowStatus() == 2) {
        gOvl11SelectionModePhase = 3;
    }
}

extern void Ovl11CommitSelectionItemGrid(void);

void Ovl11ConfirmSelectionItemGrid(void) {
    RequestMessageWindow(3, 0x3BF);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            BeginScreenFadeIncrease(0xA, 0, 2);
            gOvl11SelectionHandler = (s32)Ovl11CommitSelectionItemGrid;
        } else {
            gOvl11SelectionModePhase = 3;
        }
    }
}

void Ovl11DrawSelectionWorkIdRow(void) {
    u16 x;
    u16 y;
    u16 id;
    s32 i;
    s32 j;
    s32 *context;

    x = 0x84;
    y = 0x14;
    context = (s32 *)(gActiveRenderContext->field_D8 + 0x68);
    for (i = 0; i < 8; i++) {
        id = D_8012CF00_ids[i];
        for (j = 0; j < pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1); j++) {
            if (((struct Ovl11ItemGridState *)D_8006C838)->slots[j].id == id) {
                break;
            }
        }
        if (j != pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1) && id != 0) {
            Ovl11DrawSlotSprite(id, 0, context, x, y, 8);
        } else {
            Ovl11DrawTextureCell(context, id, x, y);
        }
        x += 0x22;
        if (x >= 0xEBU) {
            x = 0x84;
            y += 0x22;
        }
    }
}

void Ovl11DrawSelectionItemGridPage(void) {
    u16 x;
    u16 y;
    s32 i;
    s32 j;
    s32 count;
    s32 type;
    s32 table;
    s32 *context0;
    s32 *context1;
    s32 *context2;
    Ovl11ItemState *slot;

    context0 = (s32 *)(gActiveRenderContext->field_D8 + 0x60);
    context2 = (s32 *)(gActiveRenderContext->field_D8 + 0x68);
    context1 = (s32 *)(gActiveRenderContext->field_D8 + 0x64);
    x = 0x84;
    y = 0x5E;
    if (D_801273EA_table == 0) {
        slot = (Ovl11ItemState *)&gPrimaryObjectSecondaryItemList;
        count = pow_int(2, slot[-27].id + 1);
    } else {
        table = D_801273EA_table - 1;
        slot = D_80070D42_slots[table];
        count = 8;
    }
    DrawAnimatedUiSprite0D((s32)context0, 0x75, 0x90);
    DrawAnimatedUiSprite0C((s32)context0, 0x10C, 0x90);
    for (i = 0; i < 8; i++, slot++, x += 0x22) {
        if (x >= 0xEBU) {
            x = 0x84;
            y += 0x22;
        }
        if (i >= count) {
            Ovl11DrawEmptySlot((s32)context2, x, y);
        } else {
            if (D_801273EA_table == 0) {
                if (slot->id == 0) {
                    Ovl11DrawEmptySlot((s32)context2, x, y);
                } else {
                    Ovl11DrawSlotSprite(slot->id, 0, context2, x, y, 8);
                }
            } else {
                Ovl11DrawTextureCell(context2, (u16)slot->id, x, y);
            }
            if (slot->id != 0) {
                type = Ovl11GetItemType(slot->id);
                if (D_801273EA_table == 0) {
                    if (type == 2) {
                        Ovl11DrawSlotQuantity((s32)context1, x, y, slot->unk4);
                    }
                } else {
                    if (type == 0 || type == 2) {
                        Ovl11DrawSlotQuantity((s32)context1, x, y, slot->unk4);
                    }
                }
                for (j = 0; j < 8; j++) {
                    if (D_8012CF00_ids[j] == slot->id) {
                        break;
                    }
                }
                if (j != 8 && (u16)slot->id != 0) {
                    Ovl11DrawSelectedSlotMarker((s32)context0, x, y);
                }
            }
        }
    }
}

s32 Ovl11SelectionWorkSetContainsItem(s8 arg0, s8 arg1) {
    u16 key;
    s32 i;

    key = *(u16 *)Ovl11GetSelectionGridItem(arg0, arg1);
    for (i = 0; i < 8; i++) {
        if (gOvl11SelectionWorkIds[i] == key) {
            break;
        }
    }
    return i != 8;
}

void *Ovl11GetSelectionGridItem(s8 page, s8 slot) {
    if (page == 0) {
        return (slot * 6) + (u8 *)&gPrimaryObjectSecondaryItemList;
    }
    return ((((page - 1) * 8) + slot) * 6) + &D_80070D42;
}

void Ovl11ReleaseSelectionWorkItems(void);
void Ovl11PublishSelectionSpriteRequest(u16 arg0, s16 arg1);

void Ovl11CommitSelectionItemGrid(void) {
    u16 id;
    s16 value;
    s16 index;
    s16 selection;

    if (IsScreenFadeComplete() != 0) {
        index = gOvl11SelectionConfirmedRecord;
        Ovl11_D_801273E7 = 0;
        Ovl11ReleaseSelectionWorkItems();
        selection = index;
        if (index != 0xFF) {
            id = ((Ovl11SelectionRecordArray *)D_8006C838)->records[index].id;
            value = (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value;
            Ovl11PublishSelectionSpriteRequest(id, value);
            Ovl11_D_801273DC = Ovl11SelectionUnchangedHandler;
            StartCdAudioTrack(0x16);
            D_801273E4_state = 8;
            return;
        }
        index = Ovl11ChooseEligibleSelection(&id, (u16 *)&value);
        Ovl11PublishSelectionSpriteRequest(id, value);
        StartCdAudioTrack(0x16);
        D_801273E4_state = 8;
        if (index == 0xFE) {
            Ovl11_D_801273DC = Ovl11SelectionNoCandidateHandler;
        } else if (index == selection) {
            Ovl11_D_801273DC = Ovl11SelectionSameRecordHandler;
        } else {
            if (value == (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value) {
                Ovl11_D_801273DC = Ovl11SelectionUnchangedHandler;
            } else if (value < (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value) {
                Ovl11_D_801273DC = Ovl11SelectionDecreaseHandler;
            } else {
                Ovl11_D_801273DC = Ovl11SelectionIncreaseHandler;
            }
        }
    }
}

void Ovl11ConsumeSelectionItem (s32 arg0);

void Ovl11ReleaseSelectionWorkItems(void) {
    u16 *id;
    s32 remaining;

    id = &gOvl11SelectionWorkIds[0];
    for (remaining = 7; remaining >= 0; remaining--) {
        if (*id != 0) {
            Ovl11ConsumeSelectionItem((s32) *id);
        }
        id += 1;
    }
}

/* Consume only the first matching record, preferring the secondary-item
 * list. An exhausted count clears both ID halfwords; only that first list
 * also releases its cached sprite. Truncate the count before its signed test. */
void Ovl11ConsumeSelectionItem(s32 rawId) {
    Ovl11SelectionItemCounter *item;
    s32 index;
    u16 id;

    id = rawId & 0xFFFF;
    index = 0;
    item = (Ovl11SelectionItemCounter *)&gPrimaryObjectSecondaryItemList;
    while (index < pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1)) {
        index++;
        if (item->id == id) {
            item->count = item->count - 1;
            if ((s16)item->count <= 0) {
                item->id = 0;
                item->detail = 0;
                Ovl11RemoveSlotSprite(id);
            }
            return;
        }
        item++;
    }

    index = 0;
    item = (Ovl11SelectionItemCounter *)((u8 *)D_8006C838 + 0x450A);
    do {
        index++;
        if (item->id == id) {
            item->count = item->count - 1;
            if ((s16)item->count <= 0) {
                item->id = 0;
                item->detail = 0;
            }
            return;
        }
        item++;
    } while (index < 0x40);
}

void Ovl11PublishSelectionSpriteRequest(u16 id, s16 value) {
    s32 flags;

    Ovl11InstallSlotSprite(id);
    ((struct Ovl11SpriteRequestState *) D_8006C838)->id = id;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->value = value;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->active = 1;
    flags = ((struct Ovl11SpriteRequestState *) D_8006C838)->flags;
    flags |= 0x2000;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->flags = flags;
}

/* Candidate trials, ID adjustments, result messages and record commit.
 * A failed trial may consume flags/IDs internally; the scanner deliberately
 * restores both before the next trial and before returning to its caller. */

s32 Ovl11ConsumeRequiredFlagBits(void *arg0);
s32 Ovl11ConsumeRequiredObjectIds(Ovl11IdAdjustmentObject *arg0);
s32 Ovl11AccumulateIdAdjustments(Ovl11IdAdjustmentObject *arg0, u16 *arg1);

s16 Ovl11ChooseEligibleSelection(u16 *outId, u16 *outValue) {
    u16 savedIds[8];
    u16 value;
    Ovl11IdAdjustmentObject *object;
    s32 index;
    s32 i;
    u16 savedFlags;
    u16 id;

    for (i = 0; i < 8; i++) {
        savedIds[i] = D_8012CF00_ids[i];
    }
    value = 0;
    savedFlags = D_801273EE_flags;
    id = 0;
    for (index = 0; gSelectionCatalog[index].id != 0xE2; index++) {
        for (i = 0; i < 8; i++) {
            D_8012CF00_ids[i] = savedIds[i];
        }
        object = gSelectionCatalog + index;
        D_801273EE_flags = savedFlags;
        value = object->value;
        if (Ovl11ConsumeRequiredFlagBits(object) &&
            Ovl11ConsumeRequiredObjectIds(object) &&
            Ovl11AccumulateIdAdjustments(object, &value)) {
            id = object->id;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        D_8012CF00_ids[i] = savedIds[i];
    }
    D_801273EE_flags = savedFlags;
    if (id == 0) {
        *outId = 0xE2;
        *outValue = 1;
        return 0xFE;
    }
    *outId = id;
    *outValue = value;
    for (index = 0; index < 0x50; index++) {
        if (((Ovl11SelectionRecords *) D_8006C838)->records[index][0] == id) {
            break;
        }
    }
    if (index == 0x50) {
        return 0xFF;
    }
    return (u8) index;
}

s32 Ovl11ConsumeRequiredFlagBits(void *object) {
    s32 bit = 0;
    s32 mask = 1;
    u16 requiredFlags = *(u16 *)((char *)object + 8);

    for (; bit < 12; bit++, mask = (mask << 1) & 0xFFFE) {
        if (requiredFlags & mask) {
            s32 availableFlags = D_801273EE_flags;
            if (availableFlags & mask) {
                D_801273EE_flags = mask ^ availableFlags;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

s32 Ovl11ClassifyRequiredId(u16 arg0);

s32 Ovl11ConsumeRequiredObjectIds(Ovl11IdAdjustmentObject *arg0) {
    s32 i;
    s32 j;
    u16 id;

    for (i = 0; i < 8; i++) {
        id = arg0->ids[i];
        if (id != 0) {
            switch (Ovl11ClassifyRequiredId(id)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; j < 8; j++) {
                    if (id == D_8012CF00_ids[j]) {
                        break;
                    }
                }
                if (j == 8) {
                    return 0;
                }
                D_8012CF00_ids[j] = 0;
                break;
            }
        }
    }
    return 1;
}

s32 Ovl11ApplyMappedIdAdjustment(Ovl11IdAdjustmentObject *arg0, s32 arg1, u16 *arg2);

s32 Ovl11AccumulateIdAdjustments(Ovl11IdAdjustmentObject *arg0, u16 *arg1) {
    s16 sum;
    s32 i;
    s32 j;
    u16 id;
    u16 mask;
    u16 *entry;
    u16 key;

    i = 0;
    mask = 1;
    sum = 0;
    for (; i < 12; i++, mask <<= 1) {
        id = 0;
        if (D_801273EE_flags & mask) {
            switch (mask) {
            case 1: id = 0x72; break;
            case 2: id = 0x73; break;
            case 4: id = 0x74; break;
            case 8: id = 0x75; break;
            case 0x10: id = 0x76; break;
            case 0x20: id = 0x77; break;
            case 0x40: id = 0x78; break;
            case 0x80: id = 0x17C; break;
            case 0x100: id = 0x17D; break;
            case 0x200: id = 0x17E; break;
            case 0x400: id = 0x17F; break;
            case 0x800: id = 0x180; break;
            }
            for (j = 0; arg0->adjustments[j].id != 0; j++) {
                if (arg0->adjustments[j].id == id) {
                    sum += (s8)arg0->adjustments[j].adjustment;
                    break;
                }
            }
            if (arg0->adjustments[j].id == 0) {
                return 0;
            }
        }
    }
    i = 0;
    entry = D_8012CF00_ids;
    for (; i < 8; i++, entry++) {
        key = *entry;
        if (key != 0) {
            switch (Ovl11ApplyMappedIdAdjustment(arg0, key, (u16 *)&sum)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; arg0->adjustments[j].id != 0; j++) {
                    if (arg0->adjustments[j].id == key) {
                        sum = (u16)sum + (s8)arg0->adjustments[j].adjustment;
                        break;
                    }
                }
                if (arg0->adjustments[j].id == 0) {
                    return 0;
                }
                break;
            }
        }
    }
    *arg1 += (u16)sum;
    return 1;
}

s32 Ovl11ClassifyRequiredId(u16 arg0) {
    s32 row;
    s32 i;
    s32 j;

    switch (arg0) {
    case 0x181:
        row = 0;
        break;
    case 0x182:
        row = 1;
        break;
    case 0x183:
        row = 2;
        break;
    case 0x184:
        row = 3;
        break;
    default:
        return 0;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++) {
            if (D_8012CF00_ids[i] == gOvl11RequiredIdClasses[row][j]) {
                D_8012CF00_ids[i] = 0;
                return 1;
            }
        }
    }
    return 2;
}

s32 Ovl11ApplyMappedIdAdjustment(Ovl11IdAdjustmentObject *object, s32 rawId, u16 *sum) {
    s32 index;
    s32 mappedId;
    s32 result;

    switch (rawId & 0xFFFF) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        mappedId = 0x181;
        break;
    case 0x8E:
    case 0x8F:
    case 0x90:
        mappedId = 0x182;
        break;
    case 0xE4:
        mappedId = 0x182;
        break;
    case 0xE3:
        mappedId = 0x183;
        break;
    case 0x8B:
    case 0x8C:
    case 0x8D:
        mappedId = 0x183;
        break;
    case 0x88:
    case 0x89:
    case 0x8A:
        mappedId = 0x184;
        break;
    case 0xE5:
        mappedId = 0x184;
        break;
    default:
        return 0;
    }

    for (index = 0; object->adjustments[index].id != 0; index++) {
        if (object->adjustments[index].id == mappedId) {
            *sum += (s8)object->adjustments[index].adjustment;
            break;
        }
    }
    result = 2;
    if (object->adjustments[index].id != 0) {
        result = 1;
    }
    return result;
}

void Ovl11SelectionUnchangedHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C0);
        if (GetMessageWindowStatus() == 2) {
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

void Ovl11CommitSelection(void);

void Ovl11SelectionSameRecordHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C1);
        if (GetMessageWindowStatus() == 2) {
            Ovl11CommitSelection();
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

void Ovl11SelectionNoCandidateHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C2);
        if (GetMessageWindowStatus() == 2) {
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

/* Executable imports GetMessageWindowStatus and GetMessageWindowChoice. */

void Ovl11SelectionIncreaseHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C3);
        if (GetMessageWindowStatus() == 2) {
            if ((s16)GetMessageWindowChoice() == 1) {
                Ovl11CommitSelection();
            }
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

/* Executable imports GetMessageWindowStatus and GetMessageWindowChoice. */

void Ovl11SelectionDecreaseHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C4);
        if (GetMessageWindowStatus() == 2) {
            if ((s16)GetMessageWindowChoice() == 1) {
                Ovl11CommitSelection();
            }
            Ovl11_D_801273DC = Ovl11FinishSelectionMenu;
        }
    }
}

s16 Ovl11ChooseEligibleSelection(u16 *outId, u16 *outValue);

void Ovl11CommitSelection(void) {
    u16 id;
    u16 value;
    s32 index;
    s32 i;
    u8 *base;
    s32 offset;
    u16 *dest;

    index = Ovl11ChooseEligibleSelection(&id, &value);
    if (index == 0xFF) {
        for (i = 0; i < 0x50; i++) {
            if (((Ovl11SelectionRecords *)D_8006C838)->records[i][0] == 0) {
                index = (s16)i;
                break;
            }
        }
    }
    base = (u8 *)D_8006C838;
    offset = index * 0x16;
    dest = (u16 *)((u8 *)((Ovl11SelectionRecordArray *)D_8006C838)->records[0].ids + offset);
    i = 0;
    ((Ovl11SelectionRecordArray *)base)->records[index].id = id;
    ((Ovl11SelectionRecordArray *)base)->records[index].value = value;
    ((Ovl11SelectionRecordArray *)base)->records[index].flags = D_801273EE_flags;
    do {
        *dest++ = D_8012CF00_ids[i];
        i++;
    } while (i < 8);
    switch ((s16)id) {
    case 0xD6:
        SetGameFlag(0x5B, 1);
        break;
    case 0xD7:
        SetGameFlag(0x5C, 1);
        break;
    case 0xD1:
        SetGameFlag(0x5D, 1);
        break;
    }
}

void Ovl11FinishSelectionMenu(void) {
    char *base;

    gOvl11SelectionModePhase = 0;
    base = (char *)&D_8006C838;
    *(s32 *)(base + 0xC) |= 0x40000;
    Ovl11PostMenuExitFlags();
    PlaySoundEffect(3);
    *(s32 *)(base + 0x5234) |= 0x2000;
    ResetMessageWindow();
}
