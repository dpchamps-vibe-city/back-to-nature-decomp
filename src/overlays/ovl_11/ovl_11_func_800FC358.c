#include "common.h"
#include "game_types.h"

void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void ovl_11_func_800FC544(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s16 arg4);
void ovl_11_func_800FC5F4(void);
void ovl_11_func_800FC6A4(void);
void ovl_11_func_800FC754(void);
void ovl_11_func_800FC8C8(void);
void ovl_11_func_800FE614(u16 *arg0, s32 arg1);
void ovl_11_func_800FE7FC(s16 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800FC358(void) {
    s32 value;

    func_80015EE8(gActiveRenderContext->field_D8 + 0x68, (s32) D_8012CDF8, 0, 0, 0x18, 0x30);
    ovl_11_func_800FE7FC(((struct struct_8006C838_800FC358 *) D_8006C838)->field_4A84, 0x78, 0x3C);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &((struct struct_8006C838_800FC358 *) D_8006C838)->field_51C8, 0x30, 0x32);
    ovl_11_func_800FE614((u16 *) func_8001A970(D_8012CDE6, &D_8012A028, 3), 0x1E74);
    ovl_11_func_800FC544(3, 0xB, (s32) &D_8012A028, 0x28, 0x48);
    ovl_11_func_800FE614((u16 *) func_8001A970(D_8012CDE8, &D_8012A028, 3), 0x1E78);
    ovl_11_func_800FC544(3, 0xC, (s32) &D_8012A028, 0x28, 0x60);
    ovl_11_func_800FC754();
    value = ((struct struct_8006C838_800FC358 *) D_8006C838)->field_5224;
    if (value >= -9999999) {
        if (value > 9999999) {
            value = 9999999;
        }
    } else {
        value = -9999999;
    }
    ovl_11_func_800FE614((u16 *) func_8001A970(value, &D_8012A028, 8), 0x1E86);
    ovl_11_func_800FC544(3, 0xA, (s32) &D_8012A028, 0x8C, 0x30);
    ovl_11_func_800FC6A4();
    ovl_11_func_800FC8C8();
    ovl_11_func_800FC5F4();
    ovl_11_func_800FE614((u16 *) func_8001A970(D_8012CDE4, &D_8012A028, 3), 0x1E74);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &D_8012A028, 0xBA, 0x1A);
}
