/*
 * interaction_volume_actions — reconstructed translation unit.
 *
 * Dispatch interaction-volume records through their type-specific state and
 * message actions and the primary interaction input gate.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_interaction_volume_actions.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "sound_commands.h"
#include "ovl11_selection_message.h"
#include "message_window.h"
#include "ovl11_calendar_panel.h"
#include "ovl11_item_transfer_menu.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_choice_messages.h"
#include "game_flags.h"
#include "ovl11_selection_mode_state.h"
#include "ovl11_interaction_sequence.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_interaction_id_predicates.h"
#include "ovl11_area_slot_flags.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_date_message.h"
#include "ovl11_interaction_dispatch.h"
#include "selection_panel.h"
#include "ovl11_nearby_record_scans.h"
#include "ovl11_pending_event_queue.h"
#include "ovl11_save_counters.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_options_menu.h"
#include "ovl11_status_pages.h"
#include "ovl11_item_interaction_map.h"
#include "ovl11_queued_item_interaction.h"
#include "ovl11_record_pair_claim.h"
#include "ovl11_condition_evaluation.h"

void Ovl11TryBeginItem68VolumeAction (Recon_ovl_11_func_800C7270_A0View *arg0);
void Ovl11TryBeginSetBVolumeAction (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 Ovl11TryBeginClassifiedItemVolumeAction (Recon_ovl_11_func_800C7270_A0View *arg0, s16 arg1, s16 arg2);
void Ovl11TryBeginItem69VolumeAction (Recon_ovl_11_func_800C7270_A0View *arg0);
void Ovl11TryBeginItem47VolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *unusedVolume);
void Ovl11TryBeginSetAVolumeAction (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
void Ovl11TryBeginSetCVolumeAction (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 Ovl11TryBeginProbedObjectVolumeAction (Ovl11C8808Object *arg0, s16 *arg1, s16 *arg2);
void Ovl11TryBeginFlaggedItemVolumeAction (Recon_ovl_11_func_800C8334_A0View *arg0);
void Ovl11TryBeginItem3FVolumeAction (Recon_ovl_11_func_800C7270_A0View *arg0);
s32 ovl_11_func_800F3F7C (void);
void Ovl11TryBeginItem69SlotAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
void Ovl11TryBeginItem68SlotAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);

/* The shared return tail and repeatedly assigned result retain the original
 * switch construction. A nonzero volume type can be returned even when its
 * facing/item gate rejects a state change; this is not a success boolean. */
s32 Ovl11DispatchInteractionVolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0) {
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
        temp_a3 = gOvl11FacingProbeOffsets[*(u16 *) ((u8 *) arg0 + 0x38)];
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
                return var_v0;
            case 1:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11OpenOptionsMenu();
                    var_v0_2 = 1;
                    goto block_28;
                }
                return var_v0;
            case 23:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartMessageInteraction();
                    var_v0_2 = 0x17;
                    goto block_28;
                }
                return var_v0;
            case 2:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    var_v0 = 0;
                    if (GetGameFlag(2U) != 1) {
                        Ovl11BeginMenuChoice();
                        var_v0_2 = 2;
                        goto block_28;
                    }
                }
                return var_v0;
            case 8:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11OpenCalendarPanel();
                    var_v0_2 = 8;
                    goto block_28;
                }
                return var_v0;
            case 3:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11RequestChoiceMessages();
                    var_v0_2 = 3;
                    goto block_28;
                }
                return var_v0;
            case 4:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartSceneYRotationInteraction();
                    var_v0_2 = 4;
                    goto block_28;
                }
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
                    } else if (Ovl11QueueItemInteraction(temp_a0) != 0) {
                        arg0->unk6C |= 0x4000;
                        Ovl11ChangeObjectStateDirect(arg0, 0x6B, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
                    }
                    return 0x14;
                }
                return var_v0;
            case 22:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11StartSceneZRotationInteraction();
                    ((struct Ovl11InteractionState *) D_8006C838)->flags |= 0x10000000;
                    ((struct Ovl11InteractionState *) D_8006C838)->flags |= 0x08000000;
                    return 0x16;
                }
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
                return var_v0;
            case 16:
                temp_v1_3 = *(s16 *) ((u8 *) temp_v0 + 8);
                if ((temp_v1_3 == (*(u16 *) ((u8 *) arg0 + 0x38))) || (var_v0 = 0, (temp_v1_3 == 5))) {
                    RequestMessageWindow(2, *(s16 *) ((u8 *) temp_v0 + 4));
                    return 0x10;
                }
                return var_v0;
            case 39:
                Ovl11SetConditionState(7);
                return 0x27;
            case 9:
                Ovl11TryBeginItem68VolumeAction(arg0);
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
                    Ovl11TryBeginSetBVolumeAction((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0x21;
                }
                return var_v0;
            case 34:
                var_v0 = 0x22;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11TryBeginClassifiedItemVolumeAction(arg0, *(s16 *) ((u8 *) arg0 + 0x8A), *(s16 *) ((u8 *) temp_v0 + 0xC));
                    return 0x22;
                }
                return var_v0;
            case 26:
                var_v0 = 0x1A;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11TryBeginItem69VolumeAction(arg0);
                    return 0x1A;
                }
                return var_v0;
            case 12:
                var_v0 = 0xC;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11TryBeginItem47VolumeAction((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xC;
                }
                return var_v0;
            case 14:
                var_v0 = 0xE;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11TryBeginSetAVolumeAction((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xE;
                }
                return var_v0;
            case 15:
                var_v0 = 0xF;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    Ovl11TryBeginSetCVolumeAction((Recon_ovl_11_func_800C8334_A0View *) arg0, temp_v0);
                    return 0xF;
                }
                return var_v0;
            case 10:
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) != (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    return 0;
                }
                if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                    if (Ovl11TakeSaveCounter21(1U) != 0) {
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
                Ovl11TryBeginProbedObjectVolumeAction((Ovl11C8808Object *) arg0, coord, temp_v0);
                return 0x24;
            case 37:
                Ovl11ComputeGridOffsetPosition((Ovl11CE034Object *) arg0, &position, 1);
                coord[0] = (u16) position.vx;
                coord[1] = (u16) position.vy;
                coord[2] = (u16) position.vz;
                temp_v0_3 = Ovl11FindNearbyItemId(coord, (s16 *) ((u8 *) arg0 + 0xFC));
                if (temp_v0_3 != 0) {
                    if ((temp_v0_3 == 0x177) || ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0)) {
                        Ovl11BeginInteraction((s16) Ovl11GetItemInteractionId(temp_v0_3));
                        Ovl11ResetPrimaryObjectRequest();
                        goto block_78;
                    }
                    goto block_101;
                }
block_78:
                return 0x25;
            case 31:
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    RequestMessageWindow(2, Ovl11GetDateMessageEntry());
                }
                return 0x1F;
            case 32:
                Ovl11TryBeginFlaggedItemVolumeAction((Recon_ovl_11_func_800C8334_A0View *) arg0);
                return 0x20;
            case 21:
                Ovl11BeginInteraction(0x1B);
                Ovl11ResetPrimaryObjectRequest();
                return 0x15;
            case 6:
                var_v0 = 6;
                if ((*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
                    var_v0 = 6;
                    if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                        Ovl11TryShowQueuedEventMessage();
                        return 6;
                    }
                }
                return var_v0;
            case 41:
                Ovl11TryBeginItem68SlotAction(arg0, temp_v0);
                return 0x29;
            case 40:
                Ovl11TryBeginItem69SlotAction(arg0, temp_v0);
                return 0x28;
            case 13:
                Ovl11TryBeginItem3FVolumeAction(arg0);
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
                return var_v0;
            case 49:
                var_v0 = 0;
                if ((*(s16 *) ((u8 *) temp_v0 + 8)) == (*(u16 *) ((u8 *) arg0 + 0x38))) {
                    PlaySoundEffect(7);
                    Ovl11ChangeObjectStateDirect(arg0, 0x6E, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
                    return 0x31;
                }
                return var_v0;
            default:
                break;
            }
        }
    }
block_101:
    return 0;
}

/* The type-12 dispatcher supplies the volume record in a1; this arm does
 * not inspect it. Keep the common two-argument arm interface. */
void Ovl11TryBeginItem47VolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *unusedVolume) {
    if (arg0->unk8A == 0x47) {
        Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x5C, arg0->unk38 + 0x17, -2);
        PlaySoundEffect(0x13);
    } else {
        RequestMessageWindow(2, 0x201);
    }
}

void Ovl11TryBeginSetCVolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 temp_a2;

    if (Ovl11IsInteractionItemSetC(arg0->unk8A) != 0) {
        temp_a2 = arg0->unk38;
        if (temp_a2 == arg1[4]) {
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x5A, temp_a2 + 0x17, -2);
        }
    } else {
        RequestMessageWindow(2, 0x1A4);
    }
}

void Ovl11TryBeginSetAVolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 temp_a2;

    if (Ovl11IsInteractionItemSetA(arg0->unk8A) != 0) {
        temp_a2 = arg0->unk38;
        if (temp_a2 == arg1[4]) {
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x5B, temp_a2 + 0x17, -2);
        }
    } else {
        RequestMessageWindow(2, 0x1A2);
    }
}

void Ovl11TryBeginSetBVolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 temp_a2;

    if (Ovl11IsInteractionItemSetB(arg0->unk8A) != 0) {
        temp_a2 = arg0->unk38;
        if (temp_a2 == arg1[4]) {
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x59, temp_a2 + 0x17, -2);
        }
    } else {
        RequestMessageWindow(2, 0x1A3);
    }
}

void Ovl11TryBeginItem3FVolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0x3F || temp_v1 == 0x13D) {
        Ovl11ChangeObjectStateDirect(arg0, 0x5D, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        Ovl11AddSaveCounter19(0U, 1U);
        PlaySoundEffect(7);
        return;
    } else if (temp_v1 == 0) {
        if (Ovl11TakeSaveCounter19(1U) != 0) {
            PlaySoundEffect(7);
            item[0] = 0x3F;
            item[2] = 1;
            Ovl11InstallPrimaryObjectRequest(item);
        }
        return;
    }
}

void Ovl11TryBeginItem69SlotAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 matches;

    if (arg0->unk8A == 0x69) {
        matches = 0;
        if (arg0->unk38 == arg1[4]) {
            matches = 1;
        }
        if (arg1[4] == 6 && (arg0->unk38 == 1 || arg0->unk38 == 3)) {
            matches = 1;
        }
        if (arg1[4] == 7 && (arg0->unk38 == 2 || arg0->unk38 == 0)) {
            matches = 1;
        }
        if (matches) {
            ((Recon_ovl_11_func_800C7270_A0View *)arg0)->unk6C |= 0x4000;
            PlaySoundEffect(7);
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0,
                0x5E, arg0->unk38 + 0x17, -2);
            Ovl11SetAreaSlotFlag(arg1[2], 0);
        }
    } else {
        RequestMessageWindow(2, 0x1E5);
    }
}

void Ovl11TryBeginItem68SlotAction(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 matches;

    if (arg0->unk8A == 0x68) {
        matches = 0;
        if (arg0->unk38 == arg1[4]) {
            matches = 1;
        }
        if (arg1[4] == 6 && (arg0->unk38 == 1 || arg0->unk38 == 3)) {
            matches = 1;
        }
        if (arg1[4] == 7 && (arg0->unk38 == 2 || arg0->unk38 == 0)) {
            matches = 1;
        }
        if (matches) {
            ((Recon_ovl_11_func_800C7270_A0View *)arg0)->unk6C |= 0x4000;
            PlaySoundEffect(7);
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0,
                0x5F, arg0->unk38 + 0x17, -2);
            Ovl11SetAreaSlotFlag(arg1[2], 1);
        }
    } else {
        RequestMessageWindow(2, 0x1E4);
    }
}

void Ovl11TryBeginFlaggedItemVolumeAction(Recon_ovl_11_func_800C8334_A0View *arg0) {
    ItemData *item;
    s32 var_a2;

    if (arg0->unk8A == 0) {
        RequestMessageWindow(2, 0x20A);
        return;
    }
    item = &gItemDataTable[arg0->unk8A];
    var_a2 = 0;
    if (((item->u0.flags & 0x9000) == 0x9000) || ((*(s32 *) ((u8 *) item + 0x14)) == -1)) {
        var_a2 = 1;
    }
    if (var_a2 != 0) {
        Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x61, arg0->unk38 + 0x17, -2);
        PlaySoundEffect(9);
    }
}

s32 Ovl11TryBeginProbedObjectVolumeAction(Ovl11C8808Object *arg0, s16 *arg1, s16 *arg2) {
    s16 temp_v1;
    struct_80076220 *temp_v0;

    temp_v0 = Ovl11FindNearbyActiveActor(arg1, arg0->point);
    if (temp_v0 != NULL) {
        temp_v1 = arg2[2];
        if (temp_v1 == 0x2E) {
            if (*(s16 *) temp_v0 != ((struct Ovl11C8808State *) D_8006C838)->selected) {
                return 0;
            }
        } else if (*(s16 *) temp_v0 != temp_v1) {
            return 0;
        }
        if ((arg0->unk38 == arg2[4]) || (arg2[4] == 5)) {
            Ovl11ResetPrimaryObjectRequest();
            if ((*(s16 *) temp_v0 == 6) && (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 0x1B)) {
                Ovl11BeginInteraction(0x16);
                return 1;
            }
            arg0->unk6C |= 0x800000;
            Ovl11SetConditionState(3);
            arg0->unk22 = *(u16 *) temp_v0;
            SetGameFlag(0x46U, 1U);
            return 1;
        }
        return 0;
    }
    return 0;
}

void Ovl11TryBeginItem68VolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;
    s32 temp_v0;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0) {
        temp_v0 = Ovl11TakeSaveCounter18(1U);
        if (temp_v0 == 1) {
            item[0] = 0x68;
            item[2] = (u16) temp_v0;
            Ovl11InstallPrimaryObjectRequest(item);
            return;
        }
        RequestMessageWindow(2, 0x19C);
        return;
    }
    if (temp_v1 == 0x68) {
        arg0->unk6C &= 0xFFFF3FFF;
        Ovl11AddSaveCounter18(0U, 1U);
        PlaySoundEffect(7);
        Ovl11ChangeObjectStateDirect(arg0, 0x65, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return;
    }
    RequestMessageWindow(2, 0x19C);
}

void Ovl11TryBeginItem69VolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;
    s32 temp_v0;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0) {
        temp_v0 = Ovl11TakeSaveCounter20(1U);
        if (temp_v0 == 1) {
            item[0] = 0x69;
            item[2] = (u16) temp_v0;
            Ovl11InstallPrimaryObjectRequest(item);
            return;
        }
        RequestMessageWindow(2, 0x19F);
        return;
    }
    if (temp_v1 == 0x69) {
        arg0->unk6C &= 0xFFFF3FFF;
        Ovl11AddSaveCounter20(0U, 1U);
        PlaySoundEffect(7);
        Ovl11ChangeObjectStateDirect(arg0, 0x64, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return;
    }
    RequestMessageWindow(2, 0x19F);
}

s32 Ovl11TryBeginClassifiedItemVolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0, s16 arg1, s16 arg2) {
    s16 temp_v1;
    s16 var_a1_2;
    s32 temp_v0;

    if (arg1 == 0) {
        RequestMessageWindow(2, 0x1E7);
        goto fail;
    }
    if (Ovl11IsInteractionItemSetA(*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
fail:
        return 0;
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    var_a1_2 = 0;
    if (temp_v1 == 0x125) {
        var_a1_2 = 2;
    } else if (temp_v1 == 0xE8) {
        var_a1_2 = 1;
    }
    temp_v0 = Ovl11ClaimRecordPairSlot(arg2, var_a1_2);
    if (temp_v0 == 0) {
        arg0->unk6C &= 0xFFFF3FFF;
        Ovl11ChangeObjectStateDirect(arg0, 0x66, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return 1;
    } else {
        RequestMessageWindow(2, temp_v0);
        return 0;
    }
}

s32 Ovl11DispatchInteractionVolumeAction(Recon_ovl_11_func_800C7270_A0View *arg0);

s32 Ovl11DispatchPrimaryInteractionInput(Recon_ovl_11_func_800C7270_A0View *arg0, u16 arg1) {
    struct struct_8006C838_view *state;

    if (((struct Ovl11SelectionResetMode *) &D_8007AFF0)->mode != 0) {
        return 1;
    }
    state = (struct struct_8006C838_view *) D_8006C838;
    if (state->field_0C & 0x10000000) {
        return 0;
    }
    if (arg1 & 0x100) {
        Ovl11OpenSelectionPanel();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg1 & 0x800) {
        Ovl11OpenStatusPages();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg0->unk6C & 0x40000108) {
        return 0;
    }
    if (arg1 & 0x10) {
        Ovl11OpenItemTransferMenu();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg1 & 0x40) {
        if (Ovl11ProbeInteractionVolume(gLoadedAreaHeaderCopy, (Ovl11Func800BDCE8Arg1 *) &((Ovl11C7010Object *) arg0)->unk100, 1) != NULL) {
            return 1;
        }
        return Ovl11DispatchInteractionVolumeAction(arg0);
    }
    return 0;
}
