#include "common.h"
#include "game_types.h"
#include "random.h"

typedef struct {
    u16 field_0;
    char pad_02[0x14];
    s16 field_16;
    char pad_18[0x18];
    s16 field_30;
    char pad_32[2];
    s32 field_34;
} Ovl11B994Object;

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
void ovl_11_func_800D12A0(s16 arg0);
s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

s32 ovl_11_func_8010B994(Ovl11B994Object *arg0) {
    char *far_base;
    Ovl11B994PositionView *base;

    if (arg0->field_0 == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) != arg0->field_30) {
        return -1;
    }
    if (arg0->field_34 & 0x400) {
        return -1;
    }
    if ((s32)Rand(0xC8) < arg0->field_16 + 0x64) {
        base = (Ovl11B994PositionView *)D_8006C838;
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0, *(Ovl11PaddedVec3 *)base->position);
        arg0->field_34 |= 2;
        if (*(s16 *)(far_base + 0x25476) == arg0->field_30) {
            ovl_11_func_800D12A0(0xE);
        }
        ovl_11_func_8010B57C((u16 *)arg0, 0x11);
        return 0;
    }
    return -1;
}
