/*
 * Input service for the resident interaction hit record and category flags.
 * Response, request, saved-record and message branches share one latch, but
 * do not all consume it or report their side effects as a handled input.
 * See ovl11_interaction_record_actions.h and notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_interaction_record_actions.h"
#include "sound_commands.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_state_change.h"
#include "psyq/stddef.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_interaction_record_prepare.h"
#include "ovl11_interaction_hit_action.h"
#include "ovl11_object_record_state.h"

void ovl_11_func_800F0FB0(s32 response);

s32 Ovl11HandleInteractionRequest80(Ovl11C8FA8Object *object) {
    u16 *hitRecord;
    u16 recordFlags;

    if (gOvl11InteractionHitFlags & 0x80) {
        hitRecord = gOvl11InteractionHitRecordView;
        recordFlags = hitRecord[15];
        if (!(recordFlags & 3)) {
            /* State 0x32 may change response/object state yet returns 0:
             * the dispatcher must still consider the remaining categories. */
            if (object->unk3C == 0x32) {
                if (GetGameFlag(2U) == 0) {
                    ovl_11_func_800F0FB0(9);
                    object->unk22 = hitRecord[0];
                    object->unk6C |= 0x800000;
                }
                return 0;
            }
            if (!(recordFlags & 0x1000)) {
                object->unk6C |= 0x800000;
            }
            ovl_11_func_800F0FB0(3);
            object->unk22 = hitRecord[0];
            Ovl11ResetPrimaryObjectRequest();
            return 1;
        }
        return 0;
    }
    return 0;
}

s32 ovl_11_func_8010B57C(u16 *object, s32 mode);

s32 Ovl11DispatchInteractionRecordInput(Ovl11C8FA8Object *object, u16 pressed) {
    s32 messageId;
    s32 itemId;
    u16 responseId;
    u16 recordId;

    if (pressed & 0x40) {
        if (Ovl11HandleInteractionRequest80(object) != 0) {
            return 1;
        }
        if (gOvl11InteractionHitFlags & 0x400) {
            if (!(D_80070C88 & 2)) {
                Ovl11SetObjectRecordMode(gOvl11InteractionHitRecordView, 6);
                Ovl11ResetPrimaryObjectRequest();
                return 1;
            }
            object->unk22 = *(u16 *)((u8 *)gOvl11InteractionHitRecordView + 0xAC);
            Ovl11ResetPrimaryObjectRequest();
        }
        if (gOvl11InteractionHitFlags & 0x200) {
            if (!(object->unk6C & 0x18000000)) {
                recordId = *(u16 *)((u8 *)gOvl11InteractionHitRecordView + 0);
                if (recordId == 0x15F) {
                    ((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 |= 0x20;
                    PlaySoundEffect(6);
                    object->unk38 = ((struct Ovl11C907CStateView *)D_8006C838)->field_92BE;
                    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)object, 0x39U, object->unk38 + 0x32, -2);
                    object->unk6C |= 0x100;
                } else if (recordId == 0x15E) {
                    ovl_11_func_8010B57C(&gStageMode2Record, 5);
                }
                return 1;
            }
            return (*(u16 *)((u8 *)gOvl11InteractionHitRecordView + 0)) != 0x15F;
        }
        if ((gOvl11InteractionHitFlags & 2) && (object->unk34 != 0)) {
            Ovl11SavedInteractionObject = gOvl11InteractionHitRecordView;
            PlaySoundEffect(6);
            responseId = *(u16 *)((u8 *)Ovl11SavedInteractionObject + 0x22);
            object->unk38 = responseId;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)object, 0x39U, object->unk38 + 0x32, -2);
            object->unk6C = (object->unk6C | 0x180) & 0xFFDDFFFF;
            Ovl11PrepareInteractionRecord((Ovl11InteractionRecord *)Ovl11SavedInteractionObject);
            return 1;
        }
        messageId = -1;
        if (gOvl11InteractionHitFlags & 0x8020) {
            itemId = -1;
            if (gOvl11InteractionHitFlags & 0x8000) {
                itemId = Ovl11GetSeasonalRecordId((Ovl11InteractionRecord *)gOvl11InteractionHitRecordView);
            }
            if (gOvl11InteractionHitFlags & 0x20) {
                itemId = (s32)(s16)(*(u16 *)((u8 *)gOvl11InteractionHitRecordView + 0));
            }
            switch (itemId) {
            case 0x3B:
            case 0x16C:
            case 0x16D:
            case 0x16E:
                messageId = 0x25F;
                break;
            case 0x3C:
            case 0x16F:
            case 0x170:
            case 0x171:
                messageId = 0x260;
                break;
            case 0x3D:
            case 0x172:
            case 0x173:
            case 0x174:
                if (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 == 0xE) {
                    if (GetGameFlag(0xD7U) == 1) {
                        messageId = 0x283;
                    } else {
                        messageId = 0x282;
                    }
                } else {
                    messageId = 0x25E;
                }
                break;
            }
            if (messageId != -1) {
                RequestMessageWindow(2, messageId);
                return 1;
            }
        }
        Ovl11DispatchInteractionHitAction((Recon_ovl_11_func_800CCFF8_A0View *)object, 1);
    }
    return 0;
}
