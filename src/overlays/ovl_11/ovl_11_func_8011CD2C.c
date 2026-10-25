#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void func_800136D4(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u16 *func_80017E34(u16 *arg0, u16 *arg1);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_80024810(s32 arg0, s16 arg1, s16 arg2);
void func_80024860(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_8011CEE0(u16 *dst);

void ovl_11_func_8011CD2C(Ovl11D8012D538 s, s16 arg3, s32 arg4, s16 arg5, s16 arg6) {
    s32 temp_s0;
    s16 var_s1;
    s32 var_s4;
    s32 temp_s3;
    s32 var_t0;

    var_s1 = arg6 + 4;
    temp_s3 = gActiveRenderContext->field_D8 + 0x30;
    if (s.unk0 == 0) {
        var_t0 = -0xE;
        var_s4 = arg5 + 4;
    } else {
        temp_s0 = arg5 + 4;
        func_80017B3C(temp_s3, s.unk0, (s16) temp_s0, (s32) var_s1);
        var_t0 = 0;
        var_s1 += 0xE;
        var_s4 = temp_s0;
    }
    func_800136D4((u32 *) (gActiveRenderContext->field_D8 + 0x34), arg5, arg6, 0x74, (s16) (var_t0 + 0x30));
    *((u16 *) func_8001A970((s32) arg3, &D_8012D608, 3)) = 0xFFFF;
    func_80017E34((u16 *) &D_8012D608, (u16 *) s.unk4);
    func_80017B3C(temp_s3, (s32) &D_8012D608, (s32) (s16) (arg5 + 0x20), (s32) var_s1);
    func_80024810(temp_s3, var_s4, var_s1);
    func_80024860(temp_s3, (s16) (arg5 + 0x60), var_s1);
    var_s1 += 0xE;
    ovl_11_func_8011CEE0(func_8001A970(arg4, &D_8012D608, 5));
    func_80017B3C(gActiveRenderContext->field_D8 + 0x30, (s32) &D_8012D608, (s32) (s16) (arg5 + 0x10), (s32) var_s1);
}
