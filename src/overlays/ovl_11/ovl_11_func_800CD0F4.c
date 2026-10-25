#include "common.h"
#include "game_types.h"
#include "cd_audio.h"
#include "sound_commands.h"

s32 ovl_11_func_800DF010(Ovl11StageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11StageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_800E2824(u16 *arg0, s32 arg1);
s32 ovl_11_func_80109188(Ovl11PrimaryStageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);
/* Both parameters are stored with sh; this caller zero-extends them. */
void ovl_11_func_800D31EC(u16 arg0, u16 arg1);
s32 ovl_11_func_800D3104(u16 *arg0, s32 arg1);

s32 ovl_11_func_800CD0F4(Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1) {
    s16 action;

    if (D_80128C54 & 0x80) {
        return 1;
    }
    if (D_80128C54 & 0x400) {
        return 1;
    }
    if (D_80128C54 & 2) {
        action = D_80123290[arg1];
        if (arg1 == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(10);
        }
        ovl_11_func_800DF010(D_80128C4C_dispatch, action);
        return 1;
    }
    if (D_80128C54 & 4) {
        action = D_80123290[arg1];
        if (arg1 == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(12);
        }
        ovl_11_func_800E0C24(D_80128C4C_dispatch, action);
        return 1;
    }
    if (D_80128C54 & 8) {
        if (arg1 == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            if (*(u16 *) D_80128C4C_dispatch == 0x109) {
                ovl_11_func_800E2824(D_80128C4C_dispatch, 4);
                PlaySoundEffect(24);
            }
        } else if (arg1 == 4) {
            ovl_11_func_800E2824(D_80128C4C_dispatch, 9);
        }
        return 1;
    }
    if (D_80128C54 & 0x100) {
        if (arg1 == 0) {
            ovl_11_func_80109188(D_80128C4C_dispatch, 3);
            PlaySoundEffect(26);
        }
        return 1;
    }
    if (D_80128C54 & 0x200) {
        if (arg1 == 0) {
            if (((struct Ovl11ActionCounter *) D_8006C838)->count < 15) {
                ((struct Ovl11ActionCounter *) D_8006C838)->count++;
            }
            StartCdAudioTrack(14);
        } else {
            ovl_11_func_8010B57C(D_80128C4C_dispatch, D_80123290[arg1]);
        }
        return 1;
    }
    if (D_80128C54 & 0x800) {
        if (arg1 == 0 && *(u16 *) D_80128C4C_dispatch == 0x15B) {
            ovl_11_func_800D31EC(((arg0->unk3C - 0x48) % 5) * 10 & 0xFFFE, D_80071A38);
            ovl_11_func_800D3104(D_80128C4C_dispatch, 6);
            return 1;
        }
        if (arg1 == 1) {
            ovl_11_func_800D3104(D_80128C4C_dispatch, D_80123292);
            return 1;
        }
    }
    return 0;
}
