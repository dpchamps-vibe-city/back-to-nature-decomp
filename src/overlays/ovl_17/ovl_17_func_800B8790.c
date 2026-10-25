#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl17_record_sprite_animation.h"
#include "ovl17_timer_hud.h"

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
/* Original overlay imports for SetVal8005E2BC and SetMessagePortraitEnabled. */
void SetVal8005E2BC(s32 arg0);
s16 ovl_17_func_800B9C48(s16 arg0);

void ovl_17_func_800B8790(void) {
    s32 i;
    u8 *base;
    u8 *work;

    Ovl17SelectAllRecordAnimations2();
    base = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        base += 0x50;
    }
    ovl_17_func_800BA808(0x40000);
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 5);
    if (GetMessageWindowStatus() == 5) {
        u8 *counter = D_800BD848;
        (*(s16 *)(counter + 0x2CE))++;
        if (*(s16 *)(counter + 0x2CE) >= 60) {
            for (i = 0; i < 6; i++) {
                base = D_800BD848;
                *(s16 *)(base + i * 0x50 + 0x2E) = *(s16 *)(base + i * 0x50 + 0x2C) =
                    ovl_17_func_800B9C48(*(s16 *)(base + i * 0x50 + 0x34));
            }
            work = D_800BD848;
            CloseMessageWindow();
            *(s16 *)(work + 0x2C8) = PlaySoundEffect(0x38);
            Ovl17SelectAllRecordAnimations0();
            work[4] = 7;
            *(s16 *)(work + 0x2CC) = -1;
            Ovl17UpdateTimerHud(-1);
        }
    }
}
