#include "common.h"
#include "game_flags.h"
#include "ovl11_default_area_selection.h"

s32 ovl_11_func_800E5E98(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_a2;

    switch (arg0) {
    case -1:
        var_a2 = Ovl11ResolveDefaultAreaSelection() - ((struct Ovl11MessageSelectState *) D_8006C838)->base;
        break;
    case -2:
        var_a2 = (((struct Ovl11MessageSelectState *) D_8006C838)->variant * 4) + 0x50;
        break;
    case -4:
        if (((struct Ovl11MessageSelectState *) D_8006C838)->status & 0x100000) {
            var_a2 = 0x78;
        } else if (GetGameFlag(0x3AU) == 1) {
            var_a2 = 0x74;
        } else {
            var_a2 = 0x70;
        }
        break;
    case -3:
        if (((struct Ovl11MessageSelectState *) D_8006C838)->status & 0x100) {
            var_a2 = 0x6C;
        } else if (((struct Ovl11MessageSelectState *) D_8006C838)->status & 0x200) {
            var_a2 = 0x68;
        } else if (GetGameFlag(0x3CU) == 1) {
            var_a2 = 0x64;
        } else {
            var_a2 = 0x60;
        }
        break;
    default:
        var_a2 = arg0;
        break;
    }
    if (arg1 == 0) {
        ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_25480 = (s32) (var_a2 + ((struct Ovl11MessageSelectState *) D_8006C838)->base);
    } else {
        ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_25480 = (s32) var_a2;
    }
    ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_254A0 = 0x18;
    if (arg2 == 0) {
        ((struct Ovl11MessageSelectState *) D_8006C838)->flags = (s32) (((struct Ovl11MessageSelectState *) D_8006C838)->flags & ~0x2000);
    } else {
        ((struct Ovl11MessageSelectState *) D_8006C838)->flags = (s32) (((struct Ovl11MessageSelectState *) D_8006C838)->flags | 0x2000);
    }
    if (arg3 != 0) {
        ((struct Ovl11MessageSelectState *) D_8006C838)->flags = (s32) (((struct Ovl11MessageSelectState *) D_8006C838)->flags | 0x4000);
    } else {
        ((struct Ovl11MessageSelectState *) D_8006C838)->flags = (s32) (((struct Ovl11MessageSelectState *) D_8006C838)->flags & ~0x4000);
    }
    return 1;
}
