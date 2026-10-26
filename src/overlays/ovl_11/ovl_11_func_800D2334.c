#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

s32 ovl_11_func_801090A4(Ovl11HandlerObjectView *arg0);
s32 ovl_11_func_8010B4D8(u16 *arg0);
void ovl_11_func_800D266C(Ov11D266CFields *arg0, u16 *arg1, s32 arg2);
void ovl_11_func_80108CA4(s32 arg0);
void ovl_11_func_80108BE8(u8 *arg0);
void ovl_11_func_8010B198(s32 arg0);
void ovl_11_func_8010B0E8(u8 *arg0);

void ovl_11_func_800D2334(void) {
    s32 var_s1;
    struct_800759E4 *var_s0;
    u8 *base;
    u8 *far_base;
    u8 *base2;
    s32 update_mask;

    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if (var_s1 == 0) {
            var_s0 = &D_800759E4;
            if ((D_800759E4.f00 != 0) && (!(D_800759E4.f34 & 0x400) || (Ovl11FindCalendarDate(*(s16 *) (base + 0x44BA), *(s16 *) (base + 0x44BC)) != 8))) {
                ovl_11_func_801090A4((Ovl11HandlerObjectView *) &D_800759E4);
                goto block_9;
            }
            goto block_20;
        }
        var_s0 = (struct_800759E4 *) &D_80075AD4;
        if ((D_80075AD4 != 0) && !(((struct_800759E4 *) &D_80075AD4)->f34 & 0x400)) {
            ovl_11_func_8010B4D8(&((struct_800759E4 *) &D_80075AD4)->f00);
block_9:
            update_mask = 0x20000;
            if ((*(s16 *) (far_base + 0x5476)) != (s16) var_s0->f30) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((u8 *) var_s0 + 0xA8));
            } else if (!(var_s0->f34 & update_mask)) {
                if (var_s1 == 0) {
                    if (!((*(s32 *) (base + 0xC)) & 0x08000000)) {
                        ovl_11_func_800D266C((Ov11D266CFields *) var_s0, (u16 *) &gOvl11Mode1DispatchCounter, 0x11);
                        ovl_11_func_80108CA4((s32) var_s0);
                    }
                    ovl_11_func_80108BE8((u8 *) var_s0);
                    Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) var_s0 + 0xA8));
                } else {
                    base2 = (u8 *)D_8006C838;
                    if (!((*(s32 *) (base2 + 0x5234)) & 0x100)) {
                        if (!((*(s32 *) (base2 + 0xC)) & 0x08000000)) {
                            ovl_11_func_800D266C((Ov11D266CFields *) var_s0, (u16 *) &gOvl11Mode2DispatchCounter, 0x11);
                            ovl_11_func_8010B198((s32) var_s0);
                        }
                        ovl_11_func_8010B0E8((u8 *) var_s0);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) var_s0 + 0xA8));
                    }
                    goto block_20;
                }
            } else {
                goto block_20;
            }
        } else {
block_20:
            ;
        }
    }
}
