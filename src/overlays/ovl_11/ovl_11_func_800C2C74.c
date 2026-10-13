#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C3828(void);
void ovl_11_func_800C3B78(Ovl11C3B78Arg0 *arg0, Ovl11C3B78Arg1 *arg1);
void ovl_11_func_800C1F94(Ovl11AnimationObject *arg0, u16 *arg1);

void ovl_11_func_800C2C74(Ovl11C3B78Arg0 *arg0) {
    Ovl11C3B78Arg1 *event;

    event = (Ovl11C3B78Arg1 *)(ovl_11_func_800C3828() +
        (*(s16 *)((u8 *)arg0 + 0xE) * 0xCC) + (arg0->unkC * 0xC));
    ovl_11_func_800C3B78(arg0, event);
    ovl_11_func_800C1F94((Ovl11AnimationObject *)arg0, &event->unk0);
}
