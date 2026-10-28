#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "packet_work.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "ui_sprites.h"


void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_15_func_80133B28 (s16 arg0);
void ovl_15_func_80134724 (void);
void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_801344E8 (u8 *arg0, s16 arg1);
void func_8001AAF4 (s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void ovl_15_func_801342A0 (u8 *arg0, s16 arg1, s16 arg2);
s32 ovl_15_func_801340B8 (s32 arg0, s16 arg1);
void ovl_15_func_80134000 (s32 arg0, s16 arg1);
void ovl_15_func_80133F4C (s32 arg0, s16 arg1);

s32 ovl_15_func_80133808(s16 arg0) {
    s16 *var_a0;
    s32 temp_a3;
    s16 temp_v1_2;
    s16 temp_v1_3;
    u16 *var_s0;
    s32 *var_v1;
    s32 temp_s2;
    s32 temp_s4;
    s32 temp_t0;
    s32 temp_v1;
    s32 var_a2;
    s32 temp_v0;
    u16 *temp_s0;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s2 = temp_t0 + 4;
    temp_s4 = temp_t0 + 0x14;
    DrawMenuPanel((u32 *) (temp_t0 + 0x28), 1, 0x18, 0x20, 0x110, 0x74);
    DrawMenuPanel((u32 *) (gActiveRenderContext->field_D8 + 0x24), 1, 0x50, 0x10, 0xA0, 0x20);
    func_80017B3C(temp_s4, (s32) ((void *) ((u8 *) &D_80053604 + *gTextBankOffsets)), 0xD0, 0x1A);
    DrawSpriteFrameRects(temp_s2, (s32)&gOvl15MenuSpriteSources[1], D_80137658[D_8013759E], 0, 0xB8, 0x12);
    StepUiSpriteFourFramePhase(0);
    DrawAnimatedUiSprite0D(temp_s2, 0x20, 0x18);
    DrawAnimatedUiSprite0C(temp_s2, 0x108, 0x18);
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_v1 & 0x2000) {
        PlaySoundEffect(5);
        D_8013759E++;
        if (D_8013759E >= 3) {
            D_8013759E = 0;
        }
        D_801375A0 = D_801375A2 = 0;
    } else if (temp_v1 & 0x8000) {
        PlaySoundEffect(5);
        D_8013759E--;
        if (D_8013759E < 0) {
            D_8013759E = 2;
        }
        D_801375A0 = 0;
        D_801375A2 = 0;
    }
    DrawUiSprite0BFrame1(temp_s2, 0x20, (s16) ((((s16) D_801375A2 - (s16) D_801375A0) * 0x18) + 0x3C));
    if ((s16) D_801375A0 > 0) {
        DrawAnimatedUiSprite29(temp_s2, 0x120, 0x30);
    }
    if (arg0 == 0) {
        var_s0 = (u16 *) &D_80070D18;
    } else {
        var_s0 = (u16 *) (gOvl15SaveWorkBase + 0x2378);
    }
    for (var_a2 = 0; var_a2 < 9; var_a2++) {
        temp_v0 = var_s0[var_a2];
        temp_a3 = (u16) temp_v0;
        if (temp_a3 == 0xFFFF) {
            var_s0 = (u16 *) (D_800535C6 + *gTextBankOffsets);
            gOvl15MenuTextScratch[var_a2] = var_s0[0];
            gOvl15MenuTextScratch[var_a2 + 1] = var_s0[1];
            gOvl15MenuTextScratch[var_a2 + 2] = var_s0[2];
            gOvl15MenuTextScratch[var_a2 + 3] = var_s0[3];
            gOvl15MenuTextScratch[var_a2 + 4] = temp_a3;
            break;
        }
        gOvl15MenuTextScratch[var_a2] = temp_v0;
    }
    func_80017B3C(temp_s4, (s32) gOvl15MenuTextScratch, 0x52, 0x1A);
    return ovl_15_func_80133B28(arg0);
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
