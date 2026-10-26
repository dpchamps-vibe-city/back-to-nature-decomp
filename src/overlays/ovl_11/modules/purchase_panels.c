/*
 * Catalog and resource purchase panels (overlay-11 callback services).
 *
 * The catalog variants share a selection record, descriptor, message table
 * and phase machine. The resource panel reuses that selection protocol for
 * per-unit purchases and a five-option transaction paid in money AND resource
 * units. All panels borrow the global interaction/message state; none is
 * reentrant. Their callback contracts are in ovl11_purchase_panels.h.
 *
 * Selection records contain parallel text-pointer, item-ID, attribute and
 * detail-message arrays at +0/+28/+3C/+46, with a cursor at +5C. Keep the
 * original byte/word views and by-value descriptors: their signed loads and
 * aggregate ABI are shared with the generic interaction helpers.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "message_window.h"
#include "ovl11_purchase_panels.h"
#include "game_flags.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_progress_status.h"
#include "ovl11_selection_record_access.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_selection_prompt_steps.h"

void ovl_11_func_8011D400(s32 *arg0);

s32 Ovl11InitTimeDependentCatalog(void) {
    s16 daytime;
    u8 *record;
    void **texts;
    s16 *itemIds;
    s8 *attributes;
    s16 *detailMessages;

    daytime = (u32) (D_80070CF8_halfword - 6) < 12U;
    record = gOvl11PrimarySelectionRecord;
    ovl_11_func_8011D400((s32 *) record);
    texts = (void **) record;
    itemIds = (s16 *) ((u8 *) texts + 0x28);
    attributes = (s8 *) ((u8 *) texts + 0x3C);
    detailMessages = (s16 *) ((u8 *) texts + 0x46);
    if (!daytime) {
        *(s8 *) ((u8 *) texts + 0x5A) = 15;
        *(s8 *) ((u8 *) texts + 0x5B) = 20;
        *texts = (void *) ((D_80051BF0 + 2) + gTextBankOffsets[1]);
        *(s16 *) ((u8 *) texts + 0x28) = 0xA9;
        *(s8 *) ((u8 *) texts + 0x3C) = 0;
        *(s16 *) ((u8 *) texts + 0x46) = 0x112;
        texts++;
        itemIds++;
        attributes++;
        detailMessages++;
        *texts++ = (void *) ((D_80051BF0 + 0x1C) + gTextBankOffsets[1]);
        *itemIds++ = 0x134;
        *attributes++ = 0;
        *detailMessages++ = 0x113;
        *texts++ = (void *) ((D_80051BF0 + 0x3C) + gTextBankOffsets[1]);
        *itemIds++ = 0xAA;
        *attributes++ = 0;
        *detailMessages++ = 0x114;
    } else {
        *(s8 *) ((u8 *) texts + 0x5A) = 10;
        *(s8 *) ((u8 *) texts + 0x5B) = 15;
        *texts = (void *) (D_80051B90 + gTextBankOffsets[1]);
        *(s16 *) ((u8 *) texts + 0x28) = 0xA8;
        *(s8 *) ((u8 *) texts + 0x3C) = 0;
        *(s16 *) ((u8 *) texts + 0x46) = 0x10D;
        texts++;
        itemIds++;
        attributes++;
        detailMessages++;
        *texts++ = (void *) ((D_80051B90 + 0x12) + gTextBankOffsets[1]);
        *itemIds++ = 0x130;
        *attributes++ = 0;
        *detailMessages++ = 0x10E;
        *texts++ = (void *) ((D_80051B90 + 0x1E) + gTextBankOffsets[1]);
        *itemIds++ = 0x131;
        *attributes++ = 0;
        *detailMessages++ = 0x10F;
        *texts++ = (void *) ((D_80051B90 + 0x32) + gTextBankOffsets[1]);
        *itemIds++ = 0x132;
        *attributes++ = 0;
        *detailMessages++ = 0x110;
        *texts++ = (void *) ((D_80051B90 + 0x48) + gTextBankOffsets[1]);
        *itemIds++ = 0x133;
        *attributes++ = 0;
        *detailMessages++ = 0x111;
    }
    gOvl11InteractionDrawMode = 0;
    gOvl11CatalogPanelDescriptor.unk4 = -1;
    gOvl11CatalogPanelDescriptor.unk6 = -1;
    gOvl11CatalogPanelDescriptor.unk8 = 2;
    gOvl11CatalogPanelDescriptor.unkA = 0x116;
    gOvl11CatalogPanelDescriptor.unk10 = 4;
    gOvl11CatalogPanelDescriptor.unk0 = gOvl11PrimarySelectionRecord;
    *texts = (void *) (D_80051BE6 + gTextBankOffsets[1]);
    *itemIds = 0xAB;
    *attributes = 0;
    *detailMessages = 0x115;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x5C) = 0;
    if (!daytime) {
        gOvl11CatalogPanelMessages[0] = 0x10C;
    } else {
        gOvl11CatalogPanelMessages[0] = 0x10B;
    }
    gOvl11CatalogPanelMessages[1] = 0x4F;
    gOvl11CatalogPanelMessages[2] = 0x117;
    gOvl11CatalogPanelMessages[3] = 0x118;
    gOvl11CatalogPanelMessages[4] = 0x119;
    gOvl11CatalogPanelState = 0;
    gOvl11CatalogPanelResumeState = 6;
    return 1;
}

s32 Ovl11InitFixedCatalog(void) {
    ovl_11_func_8011D400((s32 *) gOvl11PrimarySelectionRecord);
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x5A) = 0xD;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x5B) = 0x12;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x28) = 0xAC;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x5C) = 0;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x3C) = 0;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x3D) = 0;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x3E) = 0;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x3F) = 0;
    *(s8 *) (gOvl11PrimarySelectionRecord + 0x40) = 0;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x46) = 0x12C;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x2A) = 0x135;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x48) = 0x129;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x2C) = 0x136;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x4A) = 0x12A;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x2E) = 0x137;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x4C) = 0x12B;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x30) = 0xAB;
    *(s16 *) (gOvl11PrimarySelectionRecord + 0x4E) = 0x12D;
    gOvl11InteractionDrawMode = 0;
    gOvl11CatalogPanelDescriptor.unk4 = -1;
    gOvl11CatalogPanelDescriptor.unk6 = -1;
    gOvl11CatalogPanelDescriptor.unk8 = 2;
    gOvl11CatalogPanelDescriptor.unkA = 0x12E;
    gOvl11CatalogPanelDescriptor.unk10 = 4;
    gOvl11CatalogPanelMessages[0] = 0x128;
    gOvl11CatalogPanelDescriptor.unk0 = gOvl11PrimarySelectionRecord;
    gOvl11CatalogPanelMessages[1] = 0x4F;
    *(void **) (gOvl11PrimarySelectionRecord + 0) = (void *) (D_80051C36 + *D_80054BC0);
    *(void **) (gOvl11PrimarySelectionRecord + 4) = (void *) ((D_80051C36 + 0x12) + *D_80054BC0);
    *(void **) (gOvl11PrimarySelectionRecord + 8) = (void *) ((D_80051C36 + 0x2C) + *D_80054BC0);
    *(void **) (gOvl11PrimarySelectionRecord + 0xC) = (void *) ((D_80051C36 + 0x38) + *D_80054BC0);
    *(void **) (gOvl11PrimarySelectionRecord + 0x10) = (void *) ((D_80051C36 - 0x50) + *D_80054BC0);
    gOvl11CatalogPanelMessages[2] = 0x12F;
    gOvl11CatalogPanelMessages[3] = 0x130;
    gOvl11CatalogPanelMessages[4] = 0x131;
    gOvl11CatalogPanelState = 0;
    gOvl11CatalogPanelResumeState = 6;
    return 1;
}

void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 Ovl11RequestCatalogPanelMessage(s32 arg0);
s32 ovl_11_func_8011D06C(s32 arg0);

/* State 5 waits for message status 2 before resuming. A selected item is
 * charged here and handed to the object only on the terminal update. */
s32 Ovl11UpdateCatalogPurchasePanel(void) {
    s32 cost;
    s32 message;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11CatalogPanelState) {
    case 0:
        Ovl11RequestCatalogPanelMessage(0);
        gOvl11CatalogPanelState = 5;
        gOvl11CatalogPanelResumeState = 1;
        gOvl11CatalogSelectedId = 0;
        return 0;
    case 1:
        gOvl11InteractionDrawMode = 4;
        gOvl11CatalogPanelState = 2;
        gOvl11InteractionStep = 0;
        gOvl11InteractionResumeStep = 0;
        return 0;
    case 2:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 0, gOvl11CatalogPanelDescriptor)) {
        case 0:
            break;
        case 1:
            gOvl11CatalogSelectedId = Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord);
            cost = ovl_11_func_800D6014(*(u16 *)&gOvl11CatalogSelectedId);
            if (ovl_11_func_8011D06C(cost) == 1) {
                ovl_11_func_800F2354(-cost, 1);
                gOvl11CatalogPanelState = 5;
                gOvl11CatalogPanelResumeState = 4;
                gOvl11InteractionDrawMode = 0;
                message = 3;
            } else {
                gOvl11CatalogSelectedId = 0;
                gOvl11CatalogPanelState = 5;
                gOvl11CatalogPanelResumeState = 3;
                message = 1;
            }
            Ovl11RequestCatalogPanelMessage(message);
            break;
        case 2:
            break;
        case 3:
            gOvl11CatalogPanelState = 5;
            gOvl11CatalogPanelResumeState = 6;
            gOvl11InteractionDrawMode = 0;
            break;
        }
        return 0;
    case 3:
        Ovl11RequestCatalogPanelMessage(2);
        gOvl11CatalogPanelState = 5;
        gOvl11CatalogPanelResumeState = 1;
        return 0;
    case 4:
        if (gOvl11CatalogPanelMessages[3] != 0x118) {
            if (gOvl11CatalogSelectedId == 0xAB) {
                gOvl11CatalogPanelMessages[4] = 0x200;
            } else {
                gOvl11CatalogPanelMessages[4] = 0x131;
            }
        }
        Ovl11RequestCatalogPanelMessage(4);
        gOvl11CatalogPanelState = 5;
        gOvl11CatalogPanelResumeState = 6;
        return 0;
    case 5:
        if (GetMessageWindowStatus() == 2) {
            gOvl11CatalogPanelState = gOvl11CatalogPanelResumeState;
        }
        return 0;
    case 6:
    default:
        if (gOvl11CatalogSelectedId != 0) {
            if ((((Ovl11CBB9CGlobals *)D_8006C838)->items[gOvl11CatalogSelectedId].u0.flags & 0x400) != 0) {
                ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, gOvl11CatalogSelectedId, 3);
            } else {
                ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, gOvl11CatalogSelectedId, 2);
            }
        }
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        return 1;
    }
    return 0;
}

/* The controller uses five message roles; clamp rather than wrap. */
s32 Ovl11RequestCatalogPanelMessage(s32 messageIndex) {
    if (messageIndex < 0) {
        messageIndex = 0;
    }
    if (messageIndex >= 5) {
        messageIndex = 4;
    }
    CloseMessageWindow();
    return RequestMessageWindow(2, gOvl11CatalogPanelMessages[messageIndex]);
}

s32 Ovl11FindFirstUnsetResourceOption (void);
void ovl_11_func_8011D400 (s32 *arg0);
s32 ovl_11_func_800D6014 (u16 arg0);

s32 Ovl11InitResourcePurchasePanel(void) {
    s32 firstUnset;
    s32 option;

    firstUnset = Ovl11FindFirstUnsetResourceOption();
    gOvl11ResourceOptionIndex = firstUnset;
    option = 4;
    if (firstUnset != -1) {
        option = firstUnset;
    }
    ovl_11_func_8011D400((s32 *) gOvl11PrimarySelectionRecord);
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x28)) = 0;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3C)) = 1;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x46)) = 0x133;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3D)) = 1;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x48)) = (s16) (option + 0x134);
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5A)) = 0x11;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5B)) = 0x11;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x2A)) = 0;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5C)) = 0;
    gOvl11ResourcePanelDescriptor.unk8 = 2;
    gOvl11ResourcePanelDescriptor.unkA = 0x139;
    gOvl11ResourcePanelDescriptor.unkC = 2;
    gOvl11ResourcePanelDescriptor.unk4 = -1;
    gOvl11ResourcePanelDescriptor.unk6 = -1;
    gOvl11ResourcePanelDescriptor.unkE = 0x141;
    gOvl11ResourcePanelDescriptor.unk10 = 4;
    gOvl11ResourcePanelDescriptor.unk0 = gOvl11PrimarySelectionRecord;
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 0)) = (void *) ((u8 *) &D_80051D34 + *D_80054BC0);
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 4)) = (void *) ((u8 *) ((u8 *) &D_80051D34 - 0x5D8) + (*D_80054BC0 + gOvl11ResourceOptionTextOffsets[option]));
    gOvl11QuantityPrompt.unk4 = (void *) ((u8 *) &D_80051D34 + 0x98 + *D_80054BC0);
    gOvl11QuantityPrompt.unkA = ovl_11_func_800D6014(0x3FU);
    gOvl11QuantityPrompt.unk8 = 1;
    ovl_11_func_8011D400((s32 *) gOvl11SecondarySelectionRecord);
    (*(s16 *) ((u8 *) gOvl11SecondarySelectionRecord + 0x46)) = 0x133;
    (*(s16 *) ((u8 *) gOvl11SecondarySelectionRecord + 0x28)) = 0x3F;
    (*(s8 *) ((u8 *) gOvl11SecondarySelectionRecord + 0x3C)) = 1;
    (*(s16 *) ((u8 *) gOvl11SecondarySelectionRecord + 0x5C)) = 0;
    gOvl11InteractionDrawMode = 0;
    gOvl11ResourcePanelState = 0;
    gOvl11ResourcePanelResumeState = 8;
    return 1;
}

void SetVal8005E2BC(s32 arg0);
s32 Ovl11UpdateResourceQuantitySelection(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);

/* Per-unit purchases cap stored units at 999. Option transactions instead
 * deduct both entries of the signed {money, units} table, then set flag 0x38.
 * State 8 is the message wait; state 9 publishes the selected result message. */
s32 Ovl11UpdateResourcePurchasePanel(void) {
    s32 choice;

    switch (gOvl11ResourcePanelState) {
    case 0:
        gOvl11ResourcePanelState = 8;
        gOvl11ResourcePanelResumeState = 1;
        RequestMessageWindow(2, 0x132);
        return 0;
    case 1:
        gOvl11ResourcePanelState = 2;
        gOvl11InteractionStep = 0;
        gOvl11InteractionResumeStep = 0;
        gOvl11ResourcePanelDescriptor.unk0 = gOvl11PrimarySelectionRecord;
        return 0;
    case 2:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 0, *(Ovl11D8012D7B8 *)&gOvl11ResourcePanelDescriptor)) {
        case 0:
            break;
        case 1:
            gOvl11ResourcePanelState = 8;
            gOvl11ResourcePanelResumeState = 9;
            if (Ovl11CountPrecedingSelectionEntries(*(StructD548 *)gOvl11PrimarySelectionRecord) == 0) {
                if (gSaveCounter19 >= 999) {
                    gOvl11ResourceResultMessage = 0x13A;
                    gOvl11ResourceResultResumeState = 1;
                } else {
                    gOvl11ResourceResultMessage = 0x13B;
                    gOvl11ResourceResultResumeState = 3;
                }
            } else if (GetGameFlag(0x38) == 1) {
                gOvl11ResourceResultMessage = 0x144;
                gOvl11ResourceResultResumeState = 1;
            } else if (gOvl11ResourceOptionIndex == -1) {
                gOvl11ResourceResultMessage = 0x146;
                gOvl11ResourceResultResumeState = 1;
            } else {
                gOvl11ResourceResultMessage = gOvl11ResourceOptionIndex + 0x13C;
                gOvl11ResourceResultResumeState = 5;
            }
            CloseMessageWindow();
            break;
        case 2:
            break;
        case 3:
            gOvl11ResourcePanelState = 8;
            gOvl11ResourcePanelResumeState = 10;
            gOvl11InteractionDrawMode = 0;
            break;
        }
        return 0;
    case 3:
        gOvl11ResourcePanelState = 4;
        gOvl11InteractionStep = 0;
        gOvl11InteractionResumeStep = 0;
        gOvl11ResourcePanelDescriptor.unk0 = gOvl11SecondarySelectionRecord;
        return 0;
    case 4:
        switch (Ovl11UpdateResourceQuantitySelection(&gOvl11InteractionStep, 0, 5, *(Ovl11BAFCParams *)&gOvl11ResourcePanelDescriptor, (ovl_11_DD48_arg0 *)&gOvl11QuantityPrompt)) {
        case 0:
            break;
        case 1:
            gOvl11ResourcePanelState = 8;
            gOvl11ResourcePanelResumeState = 9;
            if (ovl_11_func_8011D06C(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA) == 1) {
                if (gOvl11QuantityPrompt.unk8 + ((Ovl11ResourceGlobals *)D_8006C838)->amount < 1000) {
                    ovl_11_func_800F2354(-(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA), 1);
                    gOvl11ResourceResultMessage = 0x143;
                    gOvl11ResourceResultResumeState = 10;
                    ((Ovl11ResourceGlobals *)D_8006C838)->amount += gOvl11QuantityPrompt.unk8;
                } else {
                    gOvl11ResourceResultMessage = 0x142;
                    gOvl11ResourceResultResumeState = 3;
                }
            } else {
                gOvl11ResourceResultMessage = 0x4F;
                gOvl11ResourceResultResumeState = 3;
            }
            CloseMessageWindow();
            break;
        case 2:
            break;
        case 3:
            gOvl11ResourcePanelState = 8;
            gOvl11ResourcePanelResumeState = 1;
            break;
        }
        return 0;
    case 5:
        ovl_11_func_80118B84(0, -1, 0);
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x5F);
        gOvl11ResourcePanelState = 6;
        return 0;
    case 6:
        if (GetMessageWindowStatus() == 5) {
            gOvl11ResourcePanelState = 7;
        }
        return 0;
    case 7:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 1:
            choice = 4;
            if (gOvl11ResourceOptionIndex != -1) {
                choice = gOvl11ResourceOptionIndex;
            }
            if (ovl_11_func_8011D06C(gOvl11ResourceOptionCosts[choice][0]) == 1) {
                if (((Ovl11ResourceGlobals *)D_8006C838)->amount >= gOvl11ResourceOptionCosts[choice][1]) {
                    gOvl11ResourceResultMessage = 0x144;
                    gOvl11ResourcePanelState = 8;
                    gOvl11ResourcePanelResumeState = 9;
                    gOvl11ResourceResultResumeState = 10;
                    ((Ovl11ResourceGlobals *)D_8006C838)->amount -= gOvl11ResourceOptionCosts[choice][1];
                    ovl_11_func_800F2354(-gOvl11ResourceOptionCosts[choice][0], 1);
                    SetGameFlag(0x38, 1);
                    Ovl11ArmProgressPendingTimer();
                } else {
                    gOvl11ResourceResultMessage = 0x145;
                    gOvl11ResourcePanelState = 8;
                    gOvl11ResourcePanelResumeState = 9;
                    gOvl11ResourceResultResumeState = 1;
                }
            } else {
                gOvl11ResourceResultMessage = 0x4F;
                gOvl11ResourcePanelState = 8;
                gOvl11ResourcePanelResumeState = 9;
                gOvl11ResourceResultResumeState = 1;
            }
            CloseMessageWindow();
            break;
        case 2:
            CloseMessageWindow();
            gOvl11ResourceResultMessage = 0x141;
            gOvl11ResourcePanelState = 8;
            gOvl11ResourcePanelResumeState = 9;
            gOvl11ResourceResultResumeState = 1;
            break;
        }
        return 0;
    case 8:
        if (GetMessageWindowStatus() == 2) {
            gOvl11ResourcePanelState = gOvl11ResourcePanelResumeState;
        }
        return 0;
    case 9:
        if (gOvl11ResourceResultMessage != -1) {
            RequestMessageWindow(2, gOvl11ResourceResultMessage);
        }
        gOvl11ResourcePanelState = 8;
        gOvl11ResourcePanelResumeState = gOvl11ResourceResultResumeState;
        return 0;
    case 10:
    default:
        gOvl11InteractionDrawMode = 0;
        return 1;
    }
    return 0;
}

s32 Ovl11FindFirstUnsetResourceOption(void) {
    s32 i;

    i = 0;
    do {
        if (GetGameFlag((i + 0x3A) & 0xFFFF) == 0) {
            return i;
        }
        i++;
    } while (i < 5);
    return -1;
}

s32 ovl_11_func_8011CF40(s16 arg0, u16 *arg1);
void ovl_11_func_8011DD48(ovl_11_DD48_arg0 *arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);

/* Six relative interaction steps. Return 0 while pending, 1 on quantity
 * confirmation, 3 on cancellation, -1 outside the supplied step window.
 * Selection changes return through the detail message before reopening the
 * quantity prompt; keep the by-value descriptor's five-word calling ABI. */
s32 Ovl11UpdateResourceQuantitySelection(s32 *step, s32 baseStep, s32 drawMode,
                       Ovl11BAFCParams descriptor, ovl_11_DD48_arg0 *prompt) {
    s32 result;
    StructD548 *record;

    if ((u32)(*step - baseStep) >= 6U) {
        return -1;
    }
    record = descriptor.record;
    result = 0;
    switch (*step - baseStep) {
    case 0:
        Ovl11SetInteractionStepsOffset(3, 0, baseStep);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        ovl_11_func_8011DD48(prompt, (s32)(D_80051DBE + gTextBankOffsets[1]), 1,
            (s16)ovl_11_func_800D6014((u16)Ovl11GetSelectionEntryId(*record)), 0);
        gOvl11InteractionDrawMode = drawMode;
        Ovl11SetInteractionStepsOffset(2, 4, baseStep);
        break;
    case 4:
        switch (ovl_11_func_8011CF40(0x3E7, (u16 *)&prompt->field_8)) {
        case 0:
            break;
        case 1:
            result = 1;
            CloseMessageWindow();
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, Ovl11GetSelectionMessageId(*record));
            Ovl11SetInteractionStepsOffset(1, 3, baseStep);
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(descriptor.field_C, descriptor.field_E);
            Ovl11SetInteractionStepsOffset(1, 5, baseStep);
            break;
        }
        break;
    case 5:
        result = 3;
        break;
    }
    return result;
}
