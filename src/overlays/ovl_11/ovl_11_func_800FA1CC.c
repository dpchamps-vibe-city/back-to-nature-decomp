#include "common.h"
#include "game_types.h"

void func_80017A38(s16 arg0, s16 arg1);
void ovl_11_func_800FA9B8(u32 *arg0, s32 arg1);
void ovl_11_func_800FA6CC(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800FA7D4(s32 arg0, s16 arg1);
void ovl_11_func_800FA558(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800FA31C(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800FA3A0(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_11_func_800FA1CC(Ovl11FA1CCArg *arg0) {
    s16 temp_s0;
    s32 start;
    s32 temp_a1;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s4;

    start = (s16) ((s16) arg0->unk4 % 7);
    start -= 7;
    temp_s0 = (arg0->unk6 - start) % 7;
    temp_s1 = gActiveRenderContext->field_D8;
    temp_s4 = temp_s1 + 0x60;
    temp_s1_2 = temp_s1 + 0x54;
    func_80017A38(1, 2);
    temp_a1 = gActiveRenderContext->field_D8;
    ovl_11_func_800FA9B8((u32 *) (temp_a1 + 0x64), temp_a1 + 0x60);
    ovl_11_func_800FA6CC(temp_s4, arg0->unk0, arg0->unk2);
    ovl_11_func_800FA7D4(temp_s4, temp_s0);
    ovl_11_func_800FA558(temp_s1_2, (s16) arg0->unk4, temp_s0);
    ovl_11_func_800FA31C(temp_s1_2, arg0->unk2, temp_s0);
    ovl_11_func_800FA3A0(temp_s1_2, arg0->unk0, arg0->unk2, temp_s0);
    func_80017A38(0, 0);
}
