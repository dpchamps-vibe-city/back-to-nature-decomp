#include "common.h"
#include "screen_fade.h"
#include "message_window.h"
#include "random.h"
#include "state_memory_init.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B7F2C(void);
void ovl_27_func_800BA914(s32 arg0);
void ovl_27_func_800BAA34(void);
void ovl_27_func_800BABD4(void);
void ovl_27_func_800BAC14(void);

void ovl_27_func_800B7FE8(void) {
    u8 *base;
    s32 flags;
    s32 sel;

    ovl_27_func_800B7F2C();
    base = (u8 *)&D_8006C838;
    flags = *(s32 *)(base + 0xC);
    D_800C4A28 = flags;
    *(s32 *)(base + 0xC) = flags & 0xFFFF7FFF;
    if (!(flags & 0x4000)) {
        sel = *(s32 *)(base + 0x18);
        InitStateMemory();
        *(s32 *)(base + 0x18) = sel;
        ovl_27_func_800BA914(Rand(3));
        ovl_27_func_800BAA34();
        ovl_27_func_800BABD4();
        ovl_27_func_800BAC14();
    }
    BeginScreenFadeIncrease(0, 0, 2);
    ResetMessageWindow();
    Ovl27BeginBackdropStageLoad();
    *(s32 *)(base + 0xC) &= -0x1001;
    *(s32 *)(base + 0x4488) += 1;
}
