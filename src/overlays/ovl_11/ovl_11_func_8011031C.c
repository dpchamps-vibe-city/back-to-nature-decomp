#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"

s32 ovl_11_func_800D0408(u16 arg0, Recon800D0408A1View *arg1, s32 arg2);
u16 ovl_11_func_800D85F8(Recon_ovl_11_func_800D85F8_A0View *arg0);

s32 ovl_11_func_8011031C(Ovl11ActionProbeObject *arg0) {
    Recon800D0408A1View sp20;
    void *sp30;
    s32 sp34;
    s32 result;

    ovl_11_func_800D0408(arg0->unk22, &sp20, 0x64);
    result = 0;
    arg0->unkDC = 0;
    sp34 = 0;
    sp20.unk0 += arg0->unk38;
    sp20.unk8 += arg0->unk40;
    if (Ovl11QueryCollisionUnbudgeted(&sp20.unk0, arg0->collision, 0, 0,
                           &sp30, &sp34, 0, 0x8026) == 0) {
        sp34 = 0;
    }
    if (sp34 & 0x20) {
        result = ovl_11_func_800D85F8(sp30);
    }
    return result;
}
