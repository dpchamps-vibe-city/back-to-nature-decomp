#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "master_volume.h"
#include "random.h"
#include "game_flags.h"
#include "ovl11_weather_effect.h"

void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
void ovl_11_func_800DBAB0(void);
/* 800CBB9C consumes argument 1 with sh; its upper bits are immaterial. */
void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2);

void ovl_11_func_800DE4C0(void) {
    char *base;
    struct Ovl11UpdateWork *work;

    Ovl11SelectCurrentHourTint();
    if (GetGameFlag(0x48) == 1) {
        SetGameFlag(0x48, 0);
        SetGameFlag(0x47, 0);
        RequestSceneChange(7);
    } else if (GetGameFlag(0x9D) == 1) {
        SetGameFlag(0x47, 0);
        SetGameFlag(0x9D, 0);
        RequestSceneChange(0x10);
    } else if (GetGameFlag(0x48) != 1) {
        SetGameFlag(0x47, 0);
        if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == 2) {
            ovl_11_func_800F0FB0(8);
            ovl_11_func_800F0F50();
            ovl_11_func_800DBAB0();
            if (GetGameFlag(2) == 1) {
                for (;;) {
                    if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                        ovl_11_func_800DBAB0();
                        break;
                    }
                    ovl_11_func_800DBAB0();
                    if (GetGameFlag(2) == 0) {
                        break;
                    }
                }
            }
        }
        work = (struct Ovl11UpdateWork *)&D_8007AFF0;
        if (((struct Ovl11SelectionResetMode *)work)->mode != 0x18 && ((struct Ovl11SelectionResetMode *)work)->mode != 0x19) {
            BeginScreenFadeDecrease(0x2D, 0, 2);
            BeginMasterVolumeFadeIn(0xA);
        }
        base = (char *)D_8006C838;
        if ((*(s32 *)(base + 0x4450) & 4) == 0 && GetGameFlag(2) == 0) {
            ovl_11_func_800CBB9C((Ovl11C7010Object *)(base + 0x51C8), Rand(4) + 0x11B, 0);
            if (*(s16 *)(base + 0x44C0) >= 7) {
                SetGameFlag(0xAD, 1);
            }
        }
        ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x800000;
    }
}
