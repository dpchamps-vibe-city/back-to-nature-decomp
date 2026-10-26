#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "sound_commands.h"
#include "ovl11_selection_message.h"
#include "message_window.h"
#include "ovl11_calendar_panel.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_choice_messages.h"
#include "game_flags.h"
#include "ovl11_selection_mode_state.h"
#include "ovl11_interaction_sequence.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_state_change.h"

void ovl_11_func_800FEC2C (void);
void ovl_11_func_800FFA40 (void);
s32 ovl_11_func_800F4B5C (s16 arg0);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800C893C (Recon_ovl_11_func_800C7270_A0View *arg0);
void ovl_11_func_800C846C (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 ovl_11_func_800C8AD4 (Recon_ovl_11_func_800C7270_A0View *arg0, s16 arg1, s16 arg2);
void ovl_11_func_800C8A08 (Recon_ovl_11_func_800C7270_A0View *arg0);
void ovl_11_func_800C8334 ();
void ovl_11_func_800C83FC (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
void ovl_11_func_800C838C (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 ovl_11_func_800F3AC4 (u16 arg0);
s32 ovl_11_func_800C8808 (Ovl11C8808Object *arg0, s16 *arg1, s16 *arg2);
void ovl_11_func_800CE034 (Ovl11CE034Object *arg0, VECTOR *arg1, s32 arg2);
u16 ovl_11_func_800BF2F4 (s16 *arg0, s16 *arg1);
s32 ovl_11_func_80118C28 (s16 id);
s32 ovl_11_func_80118A68 (s32 arg0);
void ovl_11_func_800C8764 (Recon_ovl_11_func_800C8334_A0View *arg0);
s32 ovl_11_func_800F24B0 (void);
void ovl_11_func_800C84DC (Recon_ovl_11_func_800C7270_A0View *arg0);
s32 ovl_11_func_800F3F7C (void);
void ovl_11_func_800C857C(Recon_ovl_11_func_800C7270_A0View *arg0, s16 *arg1); /* static */
void ovl_11_func_800C8670(Recon_ovl_11_func_800C7270_A0View *arg0, s16 *arg1); /* static */
s32 ovl_11_func_80121204(void);                      /* static */

s32 ovl_11_func_800C7AC0(Recon_ovl_11_func_800C7270_A0View *arg0) {
    s16 coord[3];
    u16 request[3];
    u16 item[3];
    VECTOR position;
    u16 event[3];
    s16 *temp_a3;
    s16 *temp_v0;
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_v0_3;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s16 var_a1;
    s16 var_s0;
    s32 temp_v0_4;
    s32 var_v0;
    s32 var_v0_2;
    u16 temp_s0;
    s32 temp_v0_2;
    u32 temp_v1;

    var_v0 = 0;
    if (!(arg0->unk6C & 0x100)) {
        temp_a3 = D_80123110[*(u16 *) ((u8 *) arg0 + 0x38)];
        coord[1] = *(u16 *) ((u8 *) arg0 + 0x104);
        coord[0] = (*(u16 *) ((u8 *) arg0 + 0x100)) + ((*(u16 *) ((u8 *) arg0 + 0x110)) + (u16) temp_a3[0]);
        coord[2] = (*(u16 *) ((u8 *) arg0 + 0x108)) + ((*(u16 *) ((u8 *) arg0 + 0x118)) + (u16) temp_a3[1]);
        temp_v0 = Ovl11FindFlaggedVolume((u8 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header, coord);
        if ((temp_v0 != NULL) && (temp_s0 = *(u16 *) ((u8 *) temp_v0 + 2), Ovl11ResetPrimaryObjectRequest(), temp_v1 = temp_s0 - 1, ((temp_v1 < 0x31U) != 0))) {
            switch (temp_s0) {
            case 45:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    var_v0 = 0;
                    if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                        var_v0 = 0;
                        if (gCalendarSeason != 3) {
                            temp_v0_2 = Ovl11TakeAgedItem();
                            if (temp_v0_2 != 0) {
                                request[0] = temp_v0_2;
                                request[2] = 1;
                                Ovl11InstallPrimaryObjectRequest(request);
                                return 0x2D;
                            }
                            goto block_101;
                        }
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 1:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800FEC2C();
                    var_v0_2 = 1;
                    goto block_28;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 23:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartMessageInteraction();
                    var_v0_2 = 0x17;
                    goto block_28;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 2:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    var_v0 = 0;
                    if (GetGameFlag(2U) != 1) {
                        ovl_11_func_800FFA40();
                        var_v0_2 = 2;
                        goto block_28;
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 8:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11OpenCalendarPanel();
                    var_v0_2 = 8;
                    goto block_28;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 3:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11RequestChoiceMessages();
                    var_v0_2 = 3;
                    goto block_28;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 4:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartSceneYRotationInteraction();
                    var_v0_2 = 4;
                    goto block_28;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 5:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    var_v0 = 0;
                    if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                        Ovl11EnterSelectionModeDefault();
                        var_v0_2 = 5;
                        goto block_28;
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 24:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    var_v0 = 0;
                    if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                        Ovl11EnterSelectionModeAlternate();
                        var_v0_2 = 0x18;
block_28:
                        ((struct Ovl11InteractionState *) D_8006C838)->flags |= 0x18000000;
                        return var_v0_2;
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 20:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    temp_a0 = *(s16 *) ((u8 *) arg0 + 0x8A);
                    if (temp_a0 == 0) {
                        if (((struct Ovl11InteractionState *) D_8006C838)->queuedItem != 0) {
                            item[0] = ((struct Ovl11InteractionState *) D_8006C838)->queuedItem;
                            item[1] = 0;
                            item[2] = 1;
                            Ovl11InstallPrimaryObjectRequest(item);
                            ((struct Ovl11InteractionState *) D_8006C838)->queuedItem = 0;
                        }
                    } else if (ovl_11_func_800F4B5C(temp_a0) != 0) {
                        arg0->unk6C |= 0x4000;
                        Ovl11ChangeObjectStateDirect(arg0, 0x6B, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
                    }
                    return 0x14;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 22:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartSceneZRotationInteraction();
                    ((struct Ovl11InteractionState *) D_8006C838)->flags |= 0x10000000;
                    ((struct Ovl11InteractionState *) D_8006C838)->flags |= 0x08000000;
                    return 0x16;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 30:
                temp_v1_2 = *(s16 *) ((u8 *) temp_v0 + 8);
                if ((temp_v1_2 == (*(u16 *) ((u8 *) arg0 + 0x38))) || (var_v0 = 0, (temp_v1_2 == 5))) {
                    var_s0 = *(s16 *) ((u8 *) temp_v0 + 4);
                    if (GetGameFlag(0x24U) == 1) {
                        var_s0 = 0x1B0;
                    }
                    if (GetGameFlag(0x1FU) == 1) {
                        var_s0 = 0x1B1;
                    }
                    RequestMessageWindow(2, var_s0);
                    return 0x10;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 16:
                temp_v1_3 = *(s16 *) ((u8 *) temp_v0 + 8);
                if ((temp_v1_3 == (*(u16 *) ((u8 *) arg0 + 0x38))) || (var_v0 = 0, (temp_v1_3 == 5))) {
                    RequestMessageWindow(2, *(s16 *) ((u8 *) temp_v0 + 4));
                    return 0x10;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 39:
                ovl_11_func_800F0FB0(7);
                return 0x27;
            case 9:
                ovl_11_func_800C893C(arg0);
                return 9;
            case 18:
                temp_a2 = *(s16 *) ((u8 *) temp_v0 + 8);
                if (temp_a2 != *(u16 *) ((u8 *) arg0 + 0x38)) {
                    return 0;
                }
                temp_v1_4 = *(s16 *) ((u8 *) arg0 + 0x8A);
                if (temp_v1_4 == 0x3F || temp_v1_4 == 0x13D) {
                    if (((struct Ovl11SceneHeader *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header)->id == 0x5B) {
                        Ovl11ChangeObjectStateDirect(arg0, 0x6A, temp_a2 + 0x17, -2);
                    }
                }
                return 0x12;
            case 33:
                var_v0 = 0x21;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C846C((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0x21;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 34:
                var_v0 = 0x22;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C8AD4(arg0, *(s16 *) ((u8 *) arg0 + 0x8A), *(s16 *) ((u8 *) temp_v0 + 0xC));
                    return 0x22;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 26:
                var_v0 = 0x1A;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C8A08(arg0);
                    return 0x1A;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 12:
                var_v0 = 0xC;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C8334((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xC;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 14:
                var_v0 = 0xE;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C83FC((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xE;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 15:
                var_v0 = 0xF;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    ovl_11_func_800C838C((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xF;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 10:
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) != (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    return 0;
                }
                if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                    if (ovl_11_func_800F3AC4(1U) != 0) {
                        item[0] = 0x84;
                        item[1] = 0;
                        item[2] = 1;
                        Ovl11InstallPrimaryObjectRequest(item);
                    } else {
                        return 0;
                    }
                }
                return 0xA;
            case 35:
                Ovl11RequestSelectionMessage((s32) (*(s16 *) ((u8 *) temp_v0 + 4)));
                return 0x23;
            case 36:
                coord[0] = *(u16 *) ((u8 *) temp_v0 + 0x1A);
                coord[1] = *(u16 *) ((u8 *) temp_v0 + 0x1C);
                coord[2] = *(u16 *) ((u8 *) temp_v0 + 0x1E);
                ovl_11_func_800C8808((Ovl11C8808Object *) arg0, coord, temp_v0);
                return 0x24;
            case 37:
                ovl_11_func_800CE034((Ovl11CE034Object *) arg0, &position, 1);
                coord[0] = (u16) position.vx;
                coord[1] = (u16) position.vy;
                coord[2] = (u16) position.vz;
                temp_v0_3 = ovl_11_func_800BF2F4(coord, (s16 *) ((u8 *) arg0 + 0xFC));
                if (temp_v0_3 != 0) {
                    if ((temp_v0_3 == 0x177) || ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0)) {
                        ovl_11_func_80118A68((s16) ovl_11_func_80118C28(temp_v0_3));
                        Ovl11ResetPrimaryObjectRequest();
                        goto block_78;
                    }
                    goto block_101;
                }
block_78:
                return 0x25;
            case 31:
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    RequestMessageWindow(2, ovl_11_func_80121204());
                }
                return 0x1F;
            case 32:
                ovl_11_func_800C8764((Recon_ovl_11_func_800C8334_A0View *) arg0);
                return 0x20;
            case 21:
                ovl_11_func_80118A68(0x1B);
                Ovl11ResetPrimaryObjectRequest();
                return 0x15;
            case 6:
                var_v0 = 6;
                if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                    var_v0 = 6;
                    if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                        ovl_11_func_800F24B0();
                        return 6;
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 41:
                ovl_11_func_800C8670(arg0, temp_v0);
                return 0x29;
            case 40:
                ovl_11_func_800C857C(arg0, temp_v0);
                return 0x28;
            case 13:
                ovl_11_func_800C84DC(arg0);
                return 0xD;
            case 47:
                var_v0 = 0x2F;
                if ((u32) ((*(u16 *) ((u8 *) arg0 + 0x38)) - 1) < 2U) {
                    var_v0 = 0x2F;
                    if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                        temp_v0_4 = ovl_11_func_800F3F7C();
                        if (temp_v0_4 == 1) {
                            event[0] = 0x97;
                            event[1] = 0;
                            event[2] = (s16) temp_v0_4;
                            Ovl11InstallPrimaryObjectRequest(event);
                        }
                        return 0x2F;
                    }
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 48:
                var_v0 = 0x30;
                if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                    if (GetGameFlag(((*(u16 *) ((u8 *) temp_v0 + 6)) + 0xA1) & 0xFFFF) == 0) {
                        event[0] = (u16) (*(s16 *) ((u8 *) temp_v0 + 4));
                        event[1] = 0;
                        event[2] = 1;
                        Ovl11InstallPrimaryObjectRequest(event);
                        SetGameFlag(((*(u16 *) ((u8 *) temp_v0 + 6)) + 0xA1) & 0xFFFF, 1U);
                    }
                    return 0x30;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            case 49:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    PlaySoundEffect(7);
                    Ovl11ChangeObjectStateDirect(arg0, 0x6E, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
                    return 0x31;
                }
                /* Duplicate return node #102. Try simplifying control flow for better match */
                return var_v0;
            default:
                break;
            }
        }
    }
block_101:
    return 0;
}
