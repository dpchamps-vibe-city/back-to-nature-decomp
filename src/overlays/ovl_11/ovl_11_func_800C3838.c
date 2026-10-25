#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_event_script.h"

s32 ovl_11_func_800C3828(void);
void ovl_11_func_800C3B78(Ovl11C3B78Arg0 *arg0, Ovl11C3B78Arg1 *arg1);
void ovl_11_func_800C32C8(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C3EE8(Ovl11AnimationObject *arg0, u16 *arg1);
void ovl_11_func_800C3F6C(Ovl11C3F6CObjectView *arg0, u16 *arg1);

s32 ovl_11_func_800C3838(Ovl11C3B78Arg0 *arg0, s32 arg1, s32 arg2) {
    s32 temp_a2;
    s32 temp_a3;
    s32 var_a0;
    s32 var_v0;
    u16 temp_t0;
    u16 temp_v0;
    u16 temp_v1_2;
    u16 var_v1;
    u8 temp_a0;
    Ovl11C3B78Arg1 *temp_s0;
    void *temp_v1;

    temp_a3 = ovl_11_func_800C3828();
    if (arg2 == 0) {
        temp_v0 = *(u16 *)((u8 *)arg0 + 0x22);
        temp_a2 = temp_v0 * 8;
        if (temp_v0 != 0) {
            if (((s16)((Ovl11EventObject *)arg0)->entries[temp_v0].data[2] ==
                 (s16)((Ovl11EventObject *)arg0)->entries[temp_v0 - 1].data[2]) &&
                (*((u8 *)arg0 + temp_a2 + 0xE6) == *(s16 *)((u8 *)arg0 + 0xE)) &&
                (*(u16 *)((u8 *)arg0 + 0x1E) & 0x100)) {
                return 1;
            }
        }
    }
    temp_v1 = (u8 *)arg0 + (*(u16 *)((u8 *)arg0 + 0x22) * 8);
    temp_a0 = *((u8 *)temp_v1 + 0xE6);
    temp_s0 = (Ovl11C3B78Arg1 *)(temp_a3 + (temp_a0 * 0xCC));
    *(s16 *)((u8 *)arg0 + 0xE) = (s16)temp_a0;
    arg0->unk6 = temp_s0->unk4;
    var_a0 = 0;
    if ((temp_s0->unk0 != 7) || !(*(u16 *)((u8 *)arg0 + 0x20) & 0x2000)) {
        var_a0 = 1;
    }
    if (var_a0 != 0) {
        arg0->unk30 = temp_s0->unk6;
        arg0->unk34 = temp_s0->unk8;
        arg0->unk38 = temp_s0->unkA;
        *(s32 *)((u8 *)arg0 + 0x40) = temp_s0->unk6 << 16;
        *(s32 *)((u8 *)arg0 + 0x44) = temp_s0->unk8 << 16;
        *(s32 *)((u8 *)arg0 + 0x48) = temp_s0->unkA << 16;
        if (arg2 == 0) {
            arg0->unkC = 1;
        }
    }
    if (temp_s0->unk0 == 7) {
        arg0->unkC = 0;
        Ovl11MoveTowardScriptCommand(arg0);
    }
    ovl_11_func_800C3B78(arg0, (Ovl11C3B78Arg1 *)((u8 *)temp_s0 + (arg0->unkC * 0xC)));
    ovl_11_func_800C32C8((Ovl11AnimationObject *)arg0, 1, 0, -2);
    temp_t0 = *(u16 *)((u8 *)arg0 + 0x1E) & 0xBFFF;
    *(u16 *)((u8 *)arg0 + 0x1E) = temp_t0;
    temp_v1_2 = temp_s0->unk0;
    if (temp_v1_2 == 5) {
        *(u16 *)((u8 *)arg0 + 0x1E) = temp_t0 | 0x4000;
        ovl_11_func_800C32C8((Ovl11AnimationObject *)arg0, 2, (s16)arg0->unk6 + 1, -2);
        if (gOvl11ObjectScriptLiveMode != 0) {
            *(u16 *)((u8 *)arg0 + 0x10) = temp_s0->unk2;
        }
        return 1;
    }
    temp_s0++;
    if (temp_v1_2 == 7) {
        ovl_11_func_800C32C8((Ovl11AnimationObject *)arg0, 2, (s16)arg0->unk6 + 1, -2);
        return 0;
    }
    var_v1 = temp_s0->unk0;
    if (var_v1 == 3 || var_v1 == 17 || var_v1 == 18) {
        if (temp_s0->unk0 == 0x12) {
            *(u16 *)((u8 *)arg0 + 0x20) |= 0x1000;
        }
        arg0->unk6 = temp_s0->unk4;
        if ((arg1 != 0) && !(*(u16 *)((u8 *)arg0 + 0x1E) & 0x1000)) {
            ovl_11_func_800C3EE8((Ovl11AnimationObject *)arg0, (u16 *)temp_s0);
            return 0;
        }
        temp_s0++;
        arg0->unkC = (u16)arg0->unkC + 1;
        ovl_11_func_800C3B78(arg0, temp_s0);
        var_v1 = temp_s0->unk0;
    }
    if (var_v1 == 9) {
        ovl_11_func_800C3F6C((Ovl11C3F6CObjectView *)arg0, (u16 *)temp_s0);
        temp_s0++;
        arg0->unkC = (u16)arg0->unkC + 1;
        ovl_11_func_800C3B78(arg0, temp_s0);
    }
    var_v0 = 0;
    if (temp_s0->unk0 == 1) {
        ovl_11_func_800C32C8((Ovl11AnimationObject *)arg0, 2, (s16)arg0->unk6 + 1, -2);
        var_v0 = 0;
    }
    return var_v0;
}
