/*
 * Interaction-hit action dispatch, independent of hit detection itself.
 *
 * The first recognized category wins. A handled category may deliberately
 * do nothing for this mode; callers must not interpret 1 as "state changed".
 * The private table maps modes to the shared record-handler action codes.
 * Membership and conservative -G0 boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_interaction_hit_action.h"
#include "cd_audio.h"
#include "sound_commands.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_object_state_callbacks.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

s32 ovl_11_func_80109188(Ovl11PrimaryStageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

s32 Ovl11DispatchInteractionHitAction(
    Recon_ovl_11_func_800CCFF8_A0View *object, s32 mode) {
    s16 action;

    if (gOvl11InteractionHitFlags & 0x80) {
        return 1;
    }
    if (gOvl11InteractionHitFlags & 0x400) {
        return 1;
    }
    if (gOvl11InteractionHitFlags & 2) {
        action = gOvl11InteractionHitActions[mode];
        if (mode == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(10);
        }
        Ovl11DispatchCode160Handler(gOvl11InteractionHitRecordView, action);
        return 1;
    }
    if (gOvl11InteractionHitFlags & 4) {
        action = gOvl11InteractionHitActions[mode];
        if (mode == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(12);
        }
        Ovl11DispatchTaggedRecordHandler(gOvl11InteractionHitRecordView, action);
        return 1;
    }
    if (gOvl11InteractionHitFlags & 8) {
        if (mode == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            if (*(u16 *) gOvl11InteractionHitRecordView == 0x109) {
                Ovl11DispatchSmallRecordHandler(gOvl11InteractionHitRecordView, 4);
                PlaySoundEffect(24);
            }
        } else if (mode == 4) {
            Ovl11DispatchSmallRecordHandler(gOvl11InteractionHitRecordView, 9);
        }
        return 1;
    }
    if (gOvl11InteractionHitFlags & 0x100) {
        if (mode == 0) {
            ovl_11_func_80109188(gOvl11InteractionHitRecordView, 3);
            PlaySoundEffect(26);
        }
        return 1;
    }
    if (gOvl11InteractionHitFlags & 0x200) {
        if (mode == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(14);
        } else {
            ovl_11_func_8010B57C(gOvl11InteractionHitRecordView, gOvl11InteractionHitActions[mode]);
        }
        return 1;
    }
    if (gOvl11InteractionHitFlags & 0x800) {
        if (mode == 0 && *(u16 *) gOvl11InteractionHitRecordView == 0x15B) {
            Ovl11SetStateDebitAndDirection(((object->objectCode - 0x48) % 5) * 10 & 0xFFFE, gPrimaryObjectFacing);
            Ovl11EnterObjectState(gOvl11InteractionHitRecordView, 6);
            return 1;
        }
        if (mode == 1) {
            Ovl11EnterObjectState(gOvl11InteractionHitRecordView, gOvl11InteractionHitSecondaryAction);
            return 1;
        }
    }
    return 0;
}
