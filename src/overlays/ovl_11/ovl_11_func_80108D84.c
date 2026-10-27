#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_selected_record_flag_guard.h"
#include "ovl11_fixed_spawn_record_handoff.h"

s32 ovl_11_func_80109310(s32 *arg0);
s32 ovl_11_func_80109188(Ovl11D04D4Obj *arg0, s32 arg1);

s32 ovl_11_func_80108D84(s32 *arg0) {
    s16 temp_v0_5;
    s16 temp_v1;
    s32 var_a0;
    s32 var_v0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_6;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;

    if (*(u16 *)arg0 == 0) {
        return -1;
    }
    if ((*(s32 *)((u8 *)arg0 + 0x34) & 0x400) &&
        Ovl11FindCalendarDate(((struct Ovl11B264StateView *)D_8006C838)->field_44BA,
                            ((struct Ovl11B264StateView *)D_8006C838)->field_44BC) == 8) {
        return 0;
    }
    {
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 2) {
            temp_v0 = (*(u16 *) ((u8 *) arg0 + 0x16)) + 2;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0;
            if ((s16) temp_v0 >= 0x100) {
                var_v1 = 0xFF;
            } else {
                var_v1 = temp_v0;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1;
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 4) {
            temp_v0_2 = (*(u16 *) ((u8 *) arg0 + 0x16)) + 2;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_2;
            if ((s16) temp_v0_2 >= 0x100) {
                var_v1_2 = 0xFF;
            } else {
                var_v1_2 = temp_v0_2;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_2;
        }
        if (!((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x26)) {
            temp_v0_3 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 1;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_3;
            if ((s16)temp_v0_3 < 0) {
                var_v1_3 = 0;
            } else {
                var_v1_3 = temp_v0_3;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_3;
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x20) {
            temp_v1 = (s16) (*(u16 *) ((u8 *) arg0 + 0x16));
            var_a0 = 1;
            if (temp_v1 >= 0xA) {
                var_a0 = 3;
                if (temp_v1 < 0x14) {
                    var_a0 = 2;
                }
            }
            temp_v0_4 = (*(u16 *) ((u8 *) arg0 + 0xAC)) + var_a0;
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = temp_v0_4;
            if ((s16) temp_v0_4 >= 0x100) {
                var_v1_4 = 0xFF;
            } else {
                var_v1_4 = temp_v0_4;
            }
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = var_v1_4;
        }
        if (((struct Ovl11B264StateView *)D_8006C838)->field_E4C8 != 0) {
            temp_v0_5 = (*(u16 *) ((u8 *) arg0 + 0x30)) - 1;
            switch (temp_v0_5) {
            case 0x0:
            case 0x7:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0x13:
            case 0x19:
            case 0x1C:
            case 0x21:
            case 0x26:
            case 0x29:
            case 0x2B:
            case 0x2F:
            case 0x33:
                temp_v0_6 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 5;
                (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_6;
                if ((s16)temp_v0_6 < 0) {
                    var_v1_5 = 0;
                } else {
                    var_v1_5 = temp_v0_6;
                }
                (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_5;
                break;
            }
        }
        if ((((struct Ovl11B264StateView *)D_8006C838)->field_44BA == 2) && (((struct Ovl11B264StateView *)D_8006C838)->field_44BC >= 0) && (((struct Ovl11B264StateView *)D_8006C838)->field_44B8 == 0) && ((*(u16 *) ((u8 *) arg0 + 0)) == 0x107)) {
            (*(u16 *) ((u8 *) arg0 + 0)) = 0x106U;
            (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) | 0x8000);
            ovl_11_func_80109310(arg0);
        }
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x26);
        Ovl11DeactivateUnselectedRecord(arg0);
        Ovl11ResetTimedSpriteEffect((s16 *) ((arg0 + 0x2A)));
        var_v0 = 0;
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x100000) {
            Ovl11PublishObjectToFixedSpawnRecord((Ovl11SpawnHandoffObject *)arg0);
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
            (*(s16 *) ((u8 *) arg0 + 0x26)) = 0;
            (*(s32 *) ((u8 *) arg0 + 0x34)) &= ~0x100000;
            (*(s32 *) ((u8 *) arg0 + 0x34)) &= ~0x2000;
            ovl_11_func_80109188((Ovl11D04D4Obj *) arg0, 0);
            var_v0 = 0;
        }
    }
    return var_v0;
}
