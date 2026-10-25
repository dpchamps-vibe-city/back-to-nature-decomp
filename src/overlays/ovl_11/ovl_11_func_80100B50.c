#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "screen_fade.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "texture_sprite_draw.h"
#include "integer_power.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F6D14 (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_80100E68 (u16 arg0);
void ovl_11_func_800F6F7C (s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F7010 (s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80100EB8 (s32 arg0);
void SetVal8005E2BC (s32 arg0);
void ovl_11_func_800F6A24 (u16 arg0);
void ovl_11_func_800F6C78 (s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_80100F9C (s32 arg0);


void ovl_11_func_80100B50(u8 arg0, u8 arg1) {
    s32 sp18;
    s32 *sp1C;
    s32 sp20;
    s32 sp24;
    u16 var_s4;
    u16 var_s5;
    s32 temp_s3;
    u8 *temp_v1;
    s32 var_s6;

    u16 *var_s2;

    u16 var_s7;
    u32 *temp_s0;
    u8 *temp_fp;

    sp18 = arg1;
    temp_v1 = (u8 *)gActiveRenderContext->field_D8;
    var_s6 = 0;
    temp_s0 = (u32 *)(temp_v1 + 0x74);
    temp_s3 = (s32)(temp_v1 + 0x70);
    sp1C = (s32 *)(temp_v1 + 0x68);
    sp20 = (s32)(temp_v1 + 0x6C);
    sp24 = (s32)(temp_v1 + 0x60);
    temp_fp = (void *) ((u8 *) D_80071318 + ((arg0 & 0xFF) * 0x16));
    func_80022580(temp_s0, 1, 0x16, 0x10, 0x10E, 0x4A);
    var_s4 = 0x84;
    var_s5 = 0x14;
    var_s7 = 1;
    func_80022580(temp_s0, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(temp_s3, (s32) ((void *) ((u8 *) &D_8005396A + *gTextBankOffsets)), 0x21, 0x20);
    func_80017B3C(temp_s3, (s32) ((void *) ((u8 *) ((void *) ((u8 *) &D_8005396A + 0x12)) + *gTextBankOffsets)), 0x21, 0x6A);
    var_s2 = D_801273F4;
    for (; var_s6 < 7; var_s6++) {
        if (*(u16 *)(temp_fp + 4) & var_s7) {
            ovl_11_func_800F6D14(sp1C, *var_s2, var_s4, var_s5);
            if (sp18 == 1 && ovl_11_func_80100E68(var_s7) == 0) {
                ovl_11_func_800F6F7C(sp24, var_s4, var_s5);
            }
        } else {
            ovl_11_func_800F7010(sp20, var_s4, var_s5);
        }
        var_s4 += 0x22;
        if ((u32) var_s4 >= 0xEBU) {
            var_s4 = 0x84;
            var_s5 += 0x22;
        }
        var_s7 *= 2;
        var_s2++;
    }
    if (*(u16 *)(temp_fp + 4) & 0xF80) {
        ovl_11_func_800F6D14(sp1C, 0x79U, var_s4, var_s5);
        if (sp18 == 1 && ovl_11_func_80100E68(var_s7) == 0) {
            ovl_11_func_800F6F7C(sp24, var_s4, var_s5);
        }
    } else {
        ovl_11_func_800F7010(sp20, var_s4, var_s5);
    }
    var_s4 = 0x84;
    var_s5 = 0x5E;
    var_s2 = (u16 *)(temp_fp + 6);
    for (var_s6 = 0; var_s6 < 8; var_s6++) {
        ovl_11_func_800F6D14(sp1C, *var_s2, var_s4, var_s5);
        if (sp18 == 1 && ovl_11_func_80100EB8(*var_s2) == 0) {
            ovl_11_func_800F6F7C(sp24, var_s4, var_s5);
        }
        var_s4 += 0x22;
        if ((u32)var_s4 >= 0xEBU) {
            var_s4 = 0x84;
            var_s5 += 0x22;
        }
        var_s2++;
    }
}
