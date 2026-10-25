#include "common.h"
#include "scene_selection.h"
#include "random.h"

void ovl_27_func_800BA914(s32 arg0);

extern s16 D_800C4A26;
extern s16 D_800C4A1A;

void ovl_27_func_800B8D28(void) {
    u8 *base;
    u8 *p;
    s32 i;

    base = (u8 *)&D_8006C838;
    *(s32 *)(base + 0x4450) &= -2;
    if (D_800C4A26 != 0) {
        *(s32 *)(base + 0xC) |= 0x8000;
        *(s32 *)(*(u8 **)(base + 0x1C)) = 0;
        *(s32 *)(base + 0x4450) |= 1;
        ovl_27_func_800BA914(3);
    }
    *(s32 *)(base + 0x4488) = 0;
    if (D_800C4A1A == 0) {
        if (*(s32 *)(base + 0xC) & 0x4000) {
            *(s32 *)(base + 0xC) &= ~0x4000;
        }
        p = base;
        for (i = 3; i >= 0; i--) {
            *(s16 *)p = Rand(0xFFFF);
            p += 2;
        }
        RequestSceneChange(2);
    } else {
        if (*(s32 *)(base + 0xC) & 0x8000) {
            ovl_27_func_800BA914(3);
            RequestSceneChange(2);
        } else {
            *(s32 *)(base + 0xC) |= 0x4000;
            RequestSceneChange(8);
        }
    }
}
