#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void ovl_17_func_800BAFDC(void);
void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
/* Original overlay imports for SetVal8005E2BC and SetVal8005E334. */
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
/* Original overlay import for GetVal8005E5B4. */
s32 GetVal8005E5B4(void);
s16 ovl_17_func_800B9C48(s16 arg0);
void func_80022738(void);
/* The original 8001FAD0 jal forwards func_800212A8's s32 voice result. */
void ovl_17_func_800BB050(void);
void ovl_17_func_800BB0C4(s16 arg0);

void ovl_17_func_800B8790(void) {
    s32 i;
    u8 *base;
    u8 *work;

    ovl_17_func_800BAFDC();
    base = D_800BD870;
    for (i = 0; i < 6; i++) {
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        base += 0x50;
    }
    ovl_17_func_800BA808(0x40000);
    SetVal8005E2BC(0);
    SetVal8005E334(0);
    func_8002261C(4, 5);
    if (GetVal8005E5B4() == 5) {
        u8 *counter = D_800BD848;
        (*(s16 *)(counter + 0x2CE))++;
        if (*(s16 *)(counter + 0x2CE) >= 60) {
            for (i = 0; i < 6; i++) {
                base = D_800BD848;
                *(s16 *)(base + i * 0x50 + 0x2E) = *(s16 *)(base + i * 0x50 + 0x2C) =
                    ovl_17_func_800B9C48(*(s16 *)(base + i * 0x50 + 0x34));
            }
            work = D_800BD848;
            func_80022738();
            *(s16 *)(work + 0x2C8) = PlaySoundEffect(0x38);
            ovl_17_func_800BB050();
            work[4] = 7;
            *(s16 *)(work + 0x2CC) = -1;
            ovl_17_func_800BB0C4(-1);
        }
    }
}
