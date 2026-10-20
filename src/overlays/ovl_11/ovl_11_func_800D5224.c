#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D3104(u16 *arg0, s32 arg1);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_800D425C(u16 *arg0);
/* 800D3E40 reads these eight ABI slots and writes both output pointers. */
s32 ovl_11_func_800D3E40(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
    s16 arg4, s32 arg5, s32 *arg6, void **arg7);

s32 ovl_11_func_800D5224(u16 *arg0) {
    s32 kind;
    void *object;
    s32 index;

    if (((struct struct_8006C838_time *)D_8006C838)->field_44C0 < 7) {
        ovl_11_func_800D3104(arg0, 0x13);
    }
    index = ovl_11_func_800D3E40(*(s32 *)((u8 *)arg0 + 0x38),
        *(s32 *)((u8 *)arg0 + 0x3C), *(s32 *)((u8 *)arg0 + 0x40),
        *(s32 *)((u8 *)arg0 + 0x44), 1, 0x320, &kind, &object);
    if (index != -1) {
        arg0[1] = index;
        if (kind == 2) {
            *(s16 *)((u8 *)arg0 + 0x16) = 1;
        } else {
            *(s16 *)((u8 *)arg0 + 0x16) = 0;
        }
    }
    if (*(s16 *)((u8 *)arg0 + 0x16) == 0) {
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0,
            ((Ovl11D5224LargeView *)((u8 *)D_8006C838 + arg0[1] * 0xB8))->position);
    } else {
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0,
            ((Ovl11D5224SmallView *)((u8 *)D_8006C838 + arg0[1] * 0xB4))->position);
    }
    ovl_11_func_800D425C(arg0);
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x2000)) {
        ovl_11_func_800D3104(arg0, 7);
    }
    return 1;
}
