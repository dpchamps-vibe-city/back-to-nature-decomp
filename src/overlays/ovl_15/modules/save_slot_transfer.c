/*
 * Two-bank save-slot selection and transfer, ovl_15:80132A8C-80134770.
 *
 * Initialization clears both five-entry mark banks and the private staging
 * buffer. The selector records kind/index pairs; the bank renderer previews
 * those same records. Staging then moves the save-work selections out before
 * moving resident selections in, recording each displaced record's byte
 * address in its mark. Restore consumes those addresses into free resident
 * records and reapplies placement. No transaction rollback is provided.
 *
 * Selection, pool-transfer and row-placement headers publish different parts
 * of this one lifetime protocol. Helpers keep their original external linkage.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "packet_work.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "beveled_panel.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "ovl15_row_placement.h"
#include "ovl15_save_slot_record_pool.h"
#include "random.h"
#include "ovl15_save_slot_selection.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "text_engine.h"

void Ovl15PlaceRowXSlot(RowXPlacement *output, s16 slot) {
    s16 row = slot / 5;
    s16 column = slot % 5;
    s32 direction;

    output->x = gOvl15RowXPlacementTable[row][0] + column * 0x190;
    output->y = gOvl15RowXPlacementTable[row][1];
    output->z = gOvl15RowXPlacementTable[row][2];
    direction = Rand(4);
    output->direction = direction;
    output->mode = 4;
}

s32 Ovl15PlaceRowZSlot(RowZPlacement *output, s16 slot) {
    s16 selected = slot;
    s16 row;
    s16 column;
    s32 mode;

    if (selected == -1) {
        row = 4;
        column = 0;
    } else if (selected == -2) {
        row = 5;
        column = 0;
    } else {
        row = selected / 5;
        column = selected % 5;
    }
    output->x = gOvl15RowZPlacementTable[row][0];
    output->y = gOvl15RowZPlacementTable[row][1];
    output->z = gOvl15RowZPlacementTable[row][2] + column * 600;
    output->direction = (row & 1) ? 1 : 3;
    mode = 5;
    output->mode = mode;
    return mode;
}

/* Internal allocation helper retains external linkage. Transfer capacity is
 * validated by the caller; these paths intentionally do not test for NULL. */
void *Ovl15FindFreePoolRecord(Ovl15RecordPools *arg0, s16 arg1, s16 *arg2);

void Ovl15StageSelectedSlotTransfers(void) {
    s16 index;
    s16 sourceIndex;
    s16 kind;
    s32 i;
    u8 *buffer;

    buffer = gOvl15StagedSlotRecords;
    index = 0;
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[1][i].kind;
        sourceIndex = Ovl15Slots[1][i].recordIndex;
        if (kind != -1) {
            switch (kind) {
            case 0:
                ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)(gOvl15SaveWorkBase + sourceIndex * 0xB4), sourceIndex);
                memcpy(buffer, gOvl15SaveWorkBase + sourceIndex * 0xB4, 0xB4);
                Ovl15Slots[1][i].stagedAddress = (s32)buffer;
                buffer += 0xB4;
                memset(gOvl15SaveWorkBase + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)(gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8), sourceIndex);
                memcpy(buffer, gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8, 0xB8);
                Ovl15Slots[1][i].stagedAddress = (s32)buffer;
                buffer += 0xB8;
                memset(gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8, 0, 0xB8);
                break;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[0][i].kind;
        sourceIndex = Ovl15Slots[0][i].recordIndex;
        if (kind != -1) {
            switch (kind) {
            case 0:
                buffer = Ovl15FindFreePoolRecord((Ovl15RecordPools *)gOvl15SaveWorkBase, 0, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0xB4);
                memset((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                buffer = Ovl15FindFreePoolRecord((Ovl15RecordPools *)gOvl15SaveWorkBase, 1, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8, 0xB8);
                memset(&((Ovl15TransferRuntimeRecords *)D_8006C838)->records_B8[sourceIndex], 0, 0xB8);
                break;
            }
        }
    }
}

/* Callee scans the selected record pool and writes its index through arg2. */

void Ovl15RestoreStagedSlotRecords(void) {
    s16 index;
    s32 i;
    void *dest;

    index = 0;
    for (i = 0; i < 5; i++) {
        if (Ovl15Slots[1][i].kind != -1) {
            switch (Ovl15Slots[1][i].kind) {
            case 0:
                dest = Ovl15FindFreePoolRecord((Ovl15RecordPools *)&gSmallTaggedRecordBase, 0, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].stagedAddress, 0xB4);
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)&gSmallTaggedRecordBase + index * 0xB4), index);
                break;
            case 1:
            case 2:
                dest = Ovl15FindFreePoolRecord((Ovl15RecordPools *)&gSmallTaggedRecordBase, 1, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].stagedAddress, 0xB8);
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)&gTaggedRecordBase + index * 0xB8), index);
                break;
            }
        }
    }
}

void *Ovl15FindFreePoolRecord(Ovl15RecordPools *pools, s16 kind, s16 *index) {
    void *record = 0;
    s32 i;

    switch (kind) {
    case 0:
        for (i = 0; i < 10; i++) {
            if (pools->small[i].type == 0 && !(pools->small[i].flags & 0x02000000)) {
                record = &pools->small[i];
                *index = i;
                break;
            }
        }
        break;
    case 1:
        for (i = 0; i < 20; i++) {
            if (pools->large[i].type == 0 && !(pools->large[i].flags & 0x02000000)) {
                record = &pools->large[i];
                *index = i;
                break;
            }
        }
        break;
    }
    return record;
}

s16 Ovl15CountOccupiedPoolUnits(u8 *poolBase, s16 kind) {
    u8 *record;
    s16 units;
    s32 previousFixedCount;
    s32 index;
    s32 nextFixedCount;
    u32 reservedMask;

    record = poolBase;
    units = 0;
    switch (kind) {
    case 0:
        reservedMask = 0x02000000;
        nextFixedCount = 0x10000;
        for (index = 0; index < 10; index++) {
            if ((*(u16 *)record != 0) || (*(u32 *)(record + 0x34) & reservedMask)) {
                previousFixedCount = nextFixedCount;
                nextFixedCount += 0x10000;
                units = previousFixedCount >> 16;
            }
            record += 0xB4;
        }
        return units;
    case 1:
        for (index = 0; index < 20; index++) {
            if (*(u16 *)(record + 0x708) != 0) {
                units += 1;
                if (*(u16 *)(record + 0x7BA) != 0) {
                    units += 1;
                }
            }
            record += 0xB8;
        }
        break;
    default:
        break;
    }
    return units;
}

/* Selection and preview of the same mark banks consumed by transfer above. */

void Ovl15DrawSlotRecordRow(s16 id, s16 variant, s32 text,
                          s16 meter, s16 x, s16 y);

/* Draw the resident/staged mark banks, compacting occupied entries vertically.
 * Kind zero indexes 0xB4-byte records; the other kinds use 0xB8 at +0x708.
 * Only drawing state changes; the marks and record collections are read-only.
 */
void Ovl15DrawSlotBanks(void) {
    s16 kind;
    s16 variant;
    s32 nextX;
    u8 *records;
    s32 previousY;
    s32 largeOffset;
    s32 nextY;
    s32 remainingSlots;
    s32 y;
    s16 x;
    s32 bank;
    s32 nextBank;
    u8 *record;
    Ovl15Slot *slot;
    Ovl15Slot *slotTable;

    x = 0x28;
    y = 0x22;
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x28), 1, 0x24, 0x10, 0x70, 0x8D);
    DrawTextLine(gActiveRenderContext->field_D8 + 0x14, (s32) ((void *) ((u8 *) &gLivestockTransferDirectionTextAnchor + *gTextBankOffsets)), 0x3C, 0x14);
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x28), 1, 0xAC, 0x10, 0x70, 0x8D);
    DrawTextLine(gActiveRenderContext->field_D8 + 0x14, (s32) ((void *) ((u8 *) ((void *) ((u8 *) &gLivestockTransferDirectionTextAnchor + 0xE)) + *gTextBankOffsets)), 0xC4, 0x14);
    slotTable = &Ovl15Slots[0][0];
    bank = 0;
    do {
        if (bank != 0) {
            records = gOvl15SaveWorkBase;
        } else {
            records = (u8 *)&gSmallTaggedRecordBase;
        }
        /* One next-column birth at the join retains the latch narrowing.
         * Cache the full table address, then form the byte offset first:
         * direct indexing instead hoists only the symbol's high half.
         */
        nextX = x + 0x88;
        nextBank = bank + 1;
        slot = (Ovl15Slot *)(bank * 0x28 + (u32)slotTable);
        /* Keep the signed row advance/narrowing order across draw calls. */
        nextY = (y << 16) + 0x180000;
        remainingSlots = 4;
        do {
            kind = slot->kind;
            if (kind != -1) {
                if (kind == 0) {
                    record = records + slot->recordIndex * 0xB4;
                    variant = *(s16 *)(record + 0xAE);
                } else {
                    largeOffset = slot->recordIndex * 0xB8;
                    record = records + (largeOffset + 0x708);
                    variant = *(s16 *)(records + largeOffset + 0x7B6);
                }
                Ovl15DrawSlotRecordRow(*(s16 *)record, variant, (s32)(record + 4), *(s16 *)(record + 0x16), x, y);
                previousY = nextY;
                nextY += 0x180000;
                y = previousY >> 16;
            }
            remainingSlots--;
            slot++;
        } while (remainingSlots >= 0);
        x = (s16)nextX;
        bank = nextBank;
        y = 0x22;
    } while (bank < 2);
}

s32 Ovl15StepSlotSelectionInput (s16 arg0);
void Ovl15ResetSlotSelection (void);
s16 Ovl15FindSlotRecordIndexOrCount (u8 *arg0, s16 arg1);
void Ovl15DrawSlotRecordDetails (u8 *arg0, s16 arg1, s16 arg2);
s32 Ovl15IsSlotRecordMarked (s32 arg0, s16 arg1);
void Ovl15UnmarkSlotRecord (s32 arg0, s16 arg1);
void Ovl15MarkSlotRecord (s32 arg0, s16 arg1);

s32 Ovl15UpdateSlotSelectionMenu(s16 arg0) {
    s16 *var_a0;
    s32 temp_a3;
    s16 temp_v1_2;
    s16 temp_v1_3;
    u16 *var_s0;
    s32 *var_v1;
    s32 temp_s2;
    s32 temp_s4;
    s32 temp_t0;
    s32 temp_v1;
    s32 var_a2;
    s32 temp_v0;
    u16 *temp_s0;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s2 = temp_t0 + 4;
    temp_s4 = temp_t0 + 0x14;
    DrawMenuPanel((u32 *) (temp_t0 + 0x28), 1, 0x18, 0x20, 0x110, 0x74);
    DrawMenuPanel((u32 *) (gActiveRenderContext->field_D8 + 0x24), 1, 0x50, 0x10, 0xA0, 0x20);
    DrawTextLine(temp_s4, (s32) ((void *) ((u8 *) &D_80053604 + *gTextBankOffsets)), 0xD0, 0x1A);
    DrawSpriteFrameRects(temp_s2, (s32)&gOvl15MenuSpriteSources[1], D_80137658[gOvl15SlotCategory], 0, 0xB8, 0x12);
    StepUiSpriteFourFramePhase(0);
    DrawAnimatedUiSprite0D(temp_s2, 0x20, 0x18);
    DrawAnimatedUiSprite0C(temp_s2, 0x108, 0x18);
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_v1 & 0x2000) {
        PlaySoundEffect(5);
        gOvl15SlotCategory++;
        if (gOvl15SlotCategory >= 3) {
            gOvl15SlotCategory = 0;
        }
        gOvl15SlotFirstVisible = gOvl15SlotCursor = 0;
    } else if (temp_v1 & 0x8000) {
        PlaySoundEffect(5);
        gOvl15SlotCategory--;
        if (gOvl15SlotCategory < 0) {
            gOvl15SlotCategory = 2;
        }
        gOvl15SlotFirstVisible = 0;
        gOvl15SlotCursor = 0;
    }
    DrawUiSprite0BFrame1(temp_s2, 0x20, (s16) ((((s16) gOvl15SlotCursor - (s16) gOvl15SlotFirstVisible) * 0x18) + 0x3C));
    if ((s16) gOvl15SlotFirstVisible > 0) {
        DrawAnimatedUiSprite29(temp_s2, 0x120, 0x30);
    }
    if (arg0 == 0) {
        var_s0 = (u16 *) &D_80070D18;
    } else {
        var_s0 = (u16 *) (gOvl15SaveWorkBase + 0x2378);
    }
    for (var_a2 = 0; var_a2 < 9; var_a2++) {
        temp_v0 = var_s0[var_a2];
        temp_a3 = (u16) temp_v0;
        if (temp_a3 == 0xFFFF) {
            var_s0 = (u16 *) (D_800535C6 + *gTextBankOffsets);
            gOvl15MenuTextScratch[var_a2] = var_s0[0];
            gOvl15MenuTextScratch[var_a2 + 1] = var_s0[1];
            gOvl15MenuTextScratch[var_a2 + 2] = var_s0[2];
            gOvl15MenuTextScratch[var_a2 + 3] = var_s0[3];
            gOvl15MenuTextScratch[var_a2 + 4] = temp_a3;
            break;
        }
        gOvl15MenuTextScratch[var_a2] = temp_v0;
    }
    DrawTextLine(temp_s4, (s32) gOvl15MenuTextScratch, 0x52, 0x1A);
    return Ovl15StepSlotSelectionInput(arg0);
}
/* Both return paths in 801344E8 produce sign-extended halfwords. */
s16 Ovl15FindSlotRecordIndexOrCount(u8 *arg0, s16 arg1);
void Ovl15DrawSlotRecordDetails(u8 *arg0, s16 arg1, s16 arg2);
s32 Ovl15IsSlotRecordMarked(s32 arg0, s16 arg1);
void Ovl15UnmarkSlotRecord(s32 arg0, s16 arg1);
void Ovl15MarkSlotRecord(s32 arg0, s16 arg1);

s32 Ovl15StepSlotSelectionInput(s16 arg0) {
    s32 sp18;
    s16 temp_v0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s4;
    s32 temp_s7;
    s32 temp_v1;
    s32 var_a2;
    s32 var_v0;
    s32 temp_v1_2;
    u8 *var_s3;

    sp18 = 0;
    if ((arg0 << 0x10) != 0) {
        var_s3 = gOvl15SaveWorkBase;
    } else {
        var_s3 = (u8 *) &gSmallTaggedRecordBase;
    }
    temp_a1 = gActiveRenderContext->field_D8;
    temp_s7 = temp_a1 + 0x14;
    DrawMenuPanelWithText((u32 *) (temp_a1 + 0x18), temp_s7, (s32) &gLivestockTransferInstructionsTextAnchor[*gTextBankOffsets]);
    temp_s4 = Ovl15FindSlotRecordIndexOrCount(var_s3, -1);
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_v1 & 0x1000) {
        PlaySoundEffect(5);
        gOvl15SlotCursor -= 1;
    } else if (temp_v1 & 0x4000) {
        PlaySoundEffect(5);
        gOvl15SlotCursor += 1;
    }
    temp_v1_2 = temp_s4 - 1;
    if ((s16) gOvl15SlotCursor < 0) {
        gOvl15SlotCursor = temp_v1_2;
    }
    if ((s32) temp_v1_2 < (s16) gOvl15SlotCursor) {
        gOvl15SlotCursor = 0;
    }
    if (temp_s4 >= 5) {
        if (((s16) gOvl15SlotFirstVisible + 3) < (s16) gOvl15SlotCursor) {
            gOvl15SlotFirstVisible = gOvl15SlotCursor - 3;
        }
        if ((s16) gOvl15SlotCursor < (s16) gOvl15SlotFirstVisible) {
            gOvl15SlotFirstVisible = (s16) gOvl15SlotCursor;
        }
    }
    if (temp_s4 == 0) {
        gOvl15SlotCursor = 0;
        gOvl15SlotFirstVisible = 0;
    }
    if (temp_s4 < (s16) gOvl15SlotFirstVisible) {
        DrawAnimatedUiSprite29(temp_s7, 0x120, 0x30);
    }
    if (((s16) gOvl15SlotFirstVisible + 4) < temp_s4) {
        DrawAnimatedUiSprite28(temp_s7, 0x120, 0x80);
    }
    DrawUiSprite0BFrame1(temp_s7, 0x20, (s16) ((((s16) gOvl15SlotCursor - (s16) gOvl15SlotFirstVisible) * 0x18) + 0x3C));
    for (var_a2 = 0; var_a2 < 4 && (var_v0 = var_a2 + 1, temp_s4 >= (s16) gOvl15SlotFirstVisible + var_v0); var_a2++) {
        temp_s1 = var_a2 + 1;
        temp_s0 = var_a2 * 0x18;
        temp_v0 = temp_s0 + 0x36;
        DrawNumberText((s16) ((u16) gOvl15SlotFirstVisible + temp_s1), 2, gActiveRenderContext->field_D8 + 0x14, 0x20, (s16) (s32) temp_v0);
        Ovl15DrawSlotRecordDetails(var_s3, Ovl15FindSlotRecordIndexOrCount(var_s3, (s16) ((u16) gOvl15SlotFirstVisible + temp_s1)), (s16) (temp_s0 + 0x30));
        if (Ovl15IsSlotRecordMarked((s32) var_s3, Ovl15FindSlotRecordIndexOrCount(var_s3, (s16) ((u16) gOvl15SlotFirstVisible + temp_s1))) == 1) {
            DrawUiSprite1E(temp_s7, 0x20, temp_v0);
        }
    }
    temp_a0 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_a0 & 0x20) {
        PlaySoundEffect(1);
        sp18 = -1;
    } else if (temp_a0 & 0x40) {
        if (temp_s4 == 0) {
            PlaySoundEffect(2);
        } else if (Ovl15IsSlotRecordMarked((s32) var_s3, Ovl15FindSlotRecordIndexOrCount(var_s3, (s16) (gOvl15SlotCursor + 1))) == 1) {
            Ovl15UnmarkSlotRecord((s32) var_s3, Ovl15FindSlotRecordIndexOrCount(var_s3, (s16) (gOvl15SlotCursor + 1)));
            PlaySoundEffect(1);
        } else if ((s16) gOvl15SlotMarkCount >= 5) {
            PlaySoundEffect(2);
        } else {
            Ovl15MarkSlotRecord((s32) var_s3, Ovl15FindSlotRecordIndexOrCount(var_s3, (s16) (gOvl15SlotCursor + 1)));
            PlaySoundEffect(0);
        }
    } else if (temp_a0 & 0x800) {
        PlaySoundEffect(0);
        sp18 = 1;
    }
    return sp18;
}

void Ovl15MarkSlotRecord(s32 records, s16 index) {
    s32 bank;
    s32 slot;
    s16 category;

    bank = records != (s32)&gSmallTaggedRecordBase;
    category = gOvl15SlotCategory;
    for (slot = 0; slot < 5; slot++) {
        if (Ovl15Slots[bank][slot].kind == -1) {
            break;
        }
    }
    if (slot < 5) {
        Ovl15Slots[bank][slot].kind = category;
        Ovl15Slots[bank][slot].recordIndex = index;
        Ovl15Slots[bank][slot].stagedAddress = 0;
        gOvl15SlotMarkCount += 1;
    }
}

void Ovl15UnmarkSlotRecord(s32 records, s16 index) {
    s32 bank;
    s32 slot;
    s16 category;

    category = gOvl15SlotCategory;
    slot = 0;
    bank = (records != (s32)&gSmallTaggedRecordBase);
    for (; slot < 5; slot++) {
        if (Ovl15Slots[bank][slot].kind == category && Ovl15Slots[bank][slot].recordIndex == index) {
            break;
        }
    }
    if (slot < 5) {
        Ovl15Slots[bank][slot].kind = -1;
        Ovl15Slots[bank][slot].recordIndex = -1;
        Ovl15Slots[bank][slot].stagedAddress = 0;
        gOvl15SlotMarkCount -= 1;
    }
}

s32 Ovl15IsSlotRecordMarked(s32 arg0, s16 arg1) {
    s16 *p;
    s16 v;
    s32 i;
    s32 idx;

    v = gOvl15SlotCategory;
    i = 0;
    idx = (arg0 != (s32)&gSmallTaggedRecordBase);
    p = (s16 *)Ovl15Slots;
    p += idx * 0x14;
    for (; i < 5; i++) {
        if (p[i * 4] == v && p[i * 4 + 1] == arg1) {
            return 1;
        }
    }
    return 0;
}

void Ovl15DrawSlotMeterIcons(s16 arg0, s16 arg1, s16 arg2);

/* Overlay-local table passed by address to func_80015EE8 (ovl_15 data). */

void Ovl15DrawSlotRecordRow(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5) {
    s32 var_v1;

    arg1 = (arg1 << 0x10) != 0;
    var_v1 = 0;
    if (arg0 == 0x10A) {
        var_v1 = 0xE;
        arg1 = 0;
    }
    if (arg0 == 0x109) {
        var_v1 = 0xF;
    }
    if (arg0 == 0x160) {
        var_v1 = 6;
    }
    if (arg0 == 0x161) {
        var_v1 = 7;
    }
    if (arg0 == 0x162) {
        var_v1 = 8;
    }
    if (arg0 == 0x163) {
        var_v1 = 9;
    }
    if (arg0 == 0x164) {
        var_v1 = 0xA;
    }
    if (arg0 == 0x165) {
        var_v1 = 0xB;
    }
    if (arg0 == 0x166) {
        var_v1 = 0xD;
    }
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl15MenuSpriteSources[1], var_v1, arg1 & 0xFF, (s16) (s32) arg4, (s16) (s32) arg5);
    DrawTextLine(gActiveRenderContext->field_D8 + 0x14, arg2, (s32) (s16) (arg4 + 0x18), (s32) arg5);
    Ovl15DrawSlotMeterIcons((s16) (arg3 / 25), (s16) (arg4 + 0x5A), (s16) (arg5 + 0xC));
}

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ char pad_02[0x14];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0x2];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ char pad_20[0x14];
    /* 0x34 */ u32 unk34;
    /* 0x38 */ char pad_38[0x76];
    /* 0xAE */ s16 unkAE;
} Ovl15Rec;

s32 Ovl15ReadSignedSlotValue(Ovl15Func80134444Arg *arg0);

void Ovl15DrawSlotRecordDetails(u8 *arg0, s16 arg1, s16 arg2) {
    Ovl15Rec *var_s2;
    u8 *var_v1;
    s32 temp_s5;
    s16 var_s4;
    s16 var_s1;
    s16 var_a0;
    s16 var_a1;
    void *var_a2;
    u32 i;

    temp_s5 = gActiveRenderContext->field_D8 + 0x14;
    var_s4 = 0x58;
    if (gOvl15SlotCategory == 0) {
        var_s2 = (Ovl15Rec *) (arg0 + arg1 * 0xB4);
        var_a1 = var_s2->unkAE;
        var_a2 = (void *) ((u8 *) var_s2 + 4);
    } else {
        var_s2 = (Ovl15Rec *) (arg0 + (arg1 * 0xB8 + 0x708));
        var_v1 = (u8 *) arg0 + arg1 * 0xB8;
        var_a1 = *(s16 *) (var_v1 + 0x7B6);
        var_a2 = (void *) ((u8 *) var_s2 + 4);
    }
    var_s1 = var_s4 + 0x58;
    Ovl15DrawSlotRecordRow(var_s2->unk0, var_a1, (s32) var_a2, var_s2->unk16, 0x40, arg2);
    for (i = 0; i < 0xF; i++) {
        gOvl15SlotDetailText[i] = 0xFFD;
    }
    if (var_s2->unk34 & 0x40) {
        gOvl15SlotDetailText[0] = 0x90;
    }
    gOvl15SlotDetailText[4] = 0x71;
    WriteDecimalText(var_s2->unk1E + 1, &gOvl15SlotDetailText[5], 2);
    gOvl15SlotDetailText[8] = 0;
    gOvl15SlotDetailText[9] = 0x26;
    gOvl15SlotDetailText[10] = 0x24;
    *((u16 *) WriteDecimalText(Ovl15ReadSignedSlotValue((Ovl15Func80134444Arg *) ((u8 *) var_s2 + 0x1A)), &gOvl15SlotDetailText[11], 2)) = 0xFFFF;
    var_a0 = var_s4 + 0x58;
    DrawTextLine(temp_s5, (s32) gOvl15SlotDetailText, (s32) var_a0, (s32) arg2);
    DrawUiSprite2BFrame(temp_s5, (s16) (var_a0 + 0x10), arg2, var_s2->unk1C);
}

s32 Ovl15ReadSignedSlotValue(Ovl15Func80134444Arg *arg0) {
    return arg0->unk0;
}

void Ovl15DrawSlotMeterIcons(s16 arg0, s16 arg1, s16 arg2) {
    s32 i;
    s16 v;

    v = arg1;
    for (i = 0; i < arg0; i++) {
        DrawUiSprite2A(gActiveRenderContext->field_D8 + 4, v, arg2);
        v -= 8;
    }
}

/* ordinal -1 counts qualifying records; positive ordinals select a record
 * index. The signed-halfword result/count and word-sized scan index are
 * distinct contracts: widening the return or narrowing the scan changes C.
 * Q16 count steps retain the target's sign-extension placement. */
s16 Ovl15FindSlotRecordIndexOrCount(u8 *arg0, s16 arg1) {
    s16 var_t0;
    s16 var_t6;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    u8 *var_a0;

    var_t0 = 0;
    var_t6 = 0;
    switch (gOvl15SlotCategory) {
    case 0: {
        s32 temp;
        s16 m1;
        s32 m2;
        s16 m3;
        s32 m4;
        var_a2 = 0;
        m1 = 0x10A;
        m2 = 0x02020000;
        m3 = 0x109;
        m4 = 0x40000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((*(u16 *) (var_a0 + 0) == m1 && !(*(s32 *) (var_a0 + 0x34) & m2)) || *(u16 *) (var_a0 + 0) == m3) {
                if (!(*(s32 *) (var_a0 + 0x34) & m4)) {
                    temp = var_a3;
                    var_a3 += 0x10000;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB4;
        } while (var_a2 < 0xA);
        break;
    }
    case 1: {
        s32 temp;
        var_a2 = 0;
        var_v1 = 0x10000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((u32) ((*(u16 *) (var_a0 + 0x708)) - 0x160) < 3U) {
                if ((*(u16 *) (var_a0 + 0x7BA)) == 0) {
                    temp = var_v1;
                    var_v1 += var_a3;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB8;
        } while (var_a2 < 0x14);
        break;
    }
    case 2: {
        s32 temp;
        var_a2 = 0;
        var_v1 = 0x10000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((u32) ((*(u16 *) (var_a0 + 0x708)) - 0x164) < 2U) {
                if ((*(u16 *) (var_a0 + 0x7BA)) == 0) {
                    temp = var_v1;
                    var_v1 += var_a3;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB8;
        } while (var_a2 < 0x14);
        break;
    }
    default:
        break;
    }
    if (arg1 == -1) {
        return var_t0;
    } else {
        return var_t6;
    }
}

void Ovl15ResetSlotSelection(void);

void Ovl15InitializeSlotSelection(void) {
    s16 i;
    s16 j;

    Ovl15ResetSlotSelection();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            Ovl15Slots[i][j].kind = -1;
            Ovl15Slots[i][j].recordIndex = -1;
            Ovl15Slots[i][j].stagedAddress = 0;
        }
    }
    memset(gOvl15StagedSlotRecords, 0, 0x398);
}

void Ovl15ResetSlotSelection(void) {
    gOvl15SlotCategory = 0;
    gOvl15SlotFirstVisible = 0;
    gOvl15SlotCursor = 0;
    gOvl15SlotMarkCount = 0;
    memset(gOvl15SlotRecordScratch, 0, 0x1568);
}
