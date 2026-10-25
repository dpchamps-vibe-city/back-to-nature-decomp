#include "common.h"
#include "game_types.h"
#include "ovl11_collision_query_budget.h"

s32 ovl_11_func_800F3BCC(u16 arg0);
s32 func_8001FABC(s16 arg0);
void ovl_11_func_800CCBE8(Ovl11CC4E0Object *arg0);

s32 ovl_11_func_800CBE2C(Ovl11CC4E0Object *arg0) {
    void *object;
    s32 result;

    if (*((struct Ovl11SpriteWork *)&D_8007AFF0)->header != 0) {
        goto failed;
    }
    if (((Ovl11CBE2CState *)D_8006C838)->selectedRecord != 0x15F) {
        return 0;
    }
    if (Ovl11QueryCollisionBudgeted((s32 *)((u8 *)arg0 + 0x24),
            ((Ovl11CBE2CState *)D_8006C838)->collision,
            0, 0, &object, &result, 0, 0x200) != 0) {
        goto success;
    }
failed:
    return 0;
success:
    ovl_11_func_800F3BCC(*(u16 *)((u8 *)arg0 + 2));
    func_8001FABC(0x13);
    ovl_11_func_800CCBE8(arg0);
    return 1;
}
