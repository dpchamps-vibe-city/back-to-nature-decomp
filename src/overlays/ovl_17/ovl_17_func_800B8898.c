#include "common.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl17_timer_hud.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl17_record_backdrop.h"

/* 8001FAD0 forwards the s32 voice result returned by func_800212A8. */
void ovl_17_func_800B91B8(void);
void ovl_17_func_800B92B0(void);
void ovl_17_func_800B9324(void);
void ovl_17_func_800B946C(void);
void ovl_17_func_800B9594(void);
s32 ovl_17_func_800B986C(void);
s16 ovl_17_func_800B9C48(s16 arg0);
void ovl_17_func_800B9CAC(void);
void ovl_17_func_800B9628(void);
void ovl_17_func_800B9784(void);

void ovl_17_func_800B8898(void) {
    s16 temp_v0_2;
    s32 temp_v1;
    s16 var_a1_2;
    s32 temp_v0;
    s32 var_s1;
    u32 var_a0;
    u8 *var_a0_2;
    u8 *var_a1;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *base;
    u8 *threshold;
    u8 *motion;
    s32 displacement;

    ovl_17_func_800B91B8();
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x800) {
        var_s0 = gOvl17RecordWorkBase;
        temp_v0 = (*(s32 *) (var_s0 + 0)) != 1;
        (*(s32 *) (var_s0 + 0)) = temp_v0;
        if (temp_v0 == 1) {
            (*(s16 *) (var_s0 + 0x2C8)) = PlaySoundEffect(0x36);
            goto block_3;
        }
        goto block_6;
    }
block_3:
    if ((*(s32 *) gOvl17RecordWorkBase) == 1) {
        ovl_17_func_800B92B0();
        return;
    }
block_6:
    base = gOvl17RecordWorkBase;
    displacement = *(s16 *)(base + 0x2BC) * -0x50;
    if (*(s32 *)(base - displacement + 0x38) <= 0x800000) {
        Ovl17UpdateTimerHud(2);
    }
    (*(u32 *) (base + 0x1C))++;
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_s0 = gOvl17RecordWorkBase;
        temp_v0_2 = ovl_17_func_800B9C48(*(s16 *) (var_s0 + var_s1 * 0x50 + 0x34));
        (*(s16 *) (var_s0 + var_s1 * 0x50 + 0x2C)) = temp_v0_2;
        (*(s16 *) (var_s0 + var_s1 * 0x50 + 0x2E)) = temp_v0_2;
    }
    if ((((Ovl17ModeView *)D_8006C838)->mode == 3) && ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x40)) {
        u8 *work;
        work = gOvl17RecordWorkBase;
        (*(s32 *) (work + 0xD0)) = 1;
        RequestPadActuatorSequence(0, 0);
        (*(s16 *) (work + 0x2C8)) = PlaySoundEffect(0x3D);
    }
    var_s0 = gOvl17RecordWorkBase;
    if (*(u32 *) (var_s0 + 0x1C) == ((*(u32 *) (var_s0 + 0x1C) / (u32) (*(s16 *) (var_s0 + 6))) * (*(s16 *) (var_s0 + 6)))) {
        ovl_17_func_800B9CAC();
        ovl_17_func_800B9324();
        ovl_17_func_800B946C();
        ovl_17_func_800B9594();
        var_a0 = (u32) ((Ovl17ModeView *)D_8006C838)->mode;
        if (var_a0 == 3) {
            (*(s32 *) (var_s0 + 0xD0)) = 0;
        }
    }
    if (((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->scrollPosition <= 0x7FFFFF) {
        ovl_17_func_800B9784();
    }
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_a1 = gOvl17RecordWorkBase;
        (*(s32 *) (var_a1 + var_s1 * 0x50 + 0x38)) += (((*(s16 *) (var_a1 + var_s1 * 0x50 + 0x2E)) << 0xC) / 30);
    }
    ovl_17_func_800B9628();
    motion = gOvl17RecordWorkBase;
    (*(s32 *) (motion + 0x24)) += (((*(s16 *) (motion + 0x20)) << 0xC) / 30);
    for (var_s1 = 0; var_s1 < 6; var_s1++) {
        var_s0_2 = gOvl17RecordWorkBase;
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *) (var_s0_2 + 0x28 + var_s1 * 0x50), *(s32 *) (var_s0_2 + 0x24));
    }
    threshold = gOvl17RecordWorkBase;
    if ((((*(s16 *) (threshold + 0x2D0)) << 0x17) / 100) < (*(s32 *) (threshold + 0x24))) {
        if ((*(s16 *) (threshold + 0x2CC)) == -1) {
            (*(s16 *) (threshold + 0x2CA)) = PlaySoundEffect(0x43);
            (*(s16 *) (threshold + 0x2CC)) = 0;
        }
    }
    var_a0_2 = gOvl17RecordWorkBase;
    if ((*(s32 *) (var_a0_2 + 0x24)) > 0x7FFFFF) {
        (*(s32 *) (var_a0_2 + 0x24)) = 0x800000;
        (*(s16 *) (var_a0_2 + 0x20)) = 0;
        (*(s16 *) (var_a0_2 + 0x22)) = 0;
        temp_v1 = (u16) (*(s16 *) (var_a0_2 + 0x2CC)) + 1;
        var_a1_2 = 0x64 - temp_v1;
        (*(s16 *) (var_a0_2 + 0x2CC)) = temp_v1;
        if (var_a1_2 < 0x21) {
            var_a1_2 = 0x21;
        }
        SetSoundVoiceVolume(*(s16 *) (var_a0_2 + 0x2CA), var_a1_2, var_a1_2);
    }
    if (ovl_17_func_800B986C() == 1) {
        u8 *state;
        state = gOvl17RecordWorkBase;
        (*(s16 *) (state + 0x2CC)) = 0;
        (*(s8 *) (state + 4)) = 8;
    }
    Ovl17DrawRecordBackdrop(((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->scrollPosition);
}
