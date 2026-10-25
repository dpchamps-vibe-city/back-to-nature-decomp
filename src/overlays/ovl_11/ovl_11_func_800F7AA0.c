#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

s32 func_800226B0(void);
void func_80022738(void);
void ovl_11_func_800F9BE4(void);
s32 ovl_11_func_800F7D34(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F8224(s16 *arg0, s16 arg1);
s16 *ovl_11_func_800F82FC(s32 arg0, s32 arg1);

s32 ovl_11_func_800F7AA0(void) {
    s16 temp_s0;
    s32 temp_s0_2;
    s32 temp_v1;

    temp_s0 = D_80126E4E;
    if ((temp_s0 == 0xFF) && (func_800226B0() != 0)) {
        temp_v1 = *(s32 *) ((u8 *) gPad0Input + 8);
        if (temp_v1 & 0x10) {
            if (*ovl_11_func_800F82FC((s32) D_80126E44, (s32) D_80126E4A) != 0) {
                func_80022738();
                PlaySoundEffect(0);
                D_80126E4E = 0;
                goto block_32;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x20) {
            if (D_80126E48 == temp_s0) {
                func_80022738();
                D_80126E40 = 0xFE;
                PlaySoundEffect(3);
                return 0;
            }
            PlaySoundEffect(1);
            D_80126E48 = temp_s0;
            goto block_32;
        }
        if (temp_v1 & 0x40) {
            if (D_80126E48 == temp_s0) {
                PlaySoundEffect(0);
                D_80126E48 = (s16) (u16) D_80126E44;
                D_80126E4C = (s16) (u16) D_80126E4A;
                goto block_32;
            }
            D_80126E46 = (u16) D_80126E44;
            if ((D_80126E48 == (s16) (u16) D_80126E44) && ((D_80126E4C == D_80126E4A) || (D_80126E48 < 9))) {
                D_80126E44 = 0;
            }
            ovl_11_func_800F9BE4();
            temp_s0_2 = ovl_11_func_800F7D34((s32) D_80126E48, (s32) D_80126E44, (s32) D_80126E4C, (s32) D_80126E4A);
            ovl_11_func_800F9BE4();
            D_80126E44 = (s16) D_80126E46;
            if (temp_s0_2 == 1) {
                PlaySoundEffect(0);
                D_80126E48 = 0xFF;
                return 1;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x100) {
            if (D_80126E48 == temp_s0) {
                PlaySoundEffect(0);
                switch (D_80126E40) {
                case 0:
                    ovl_11_func_800F8224((s16 *) &D_80070EC2, 0x40);
                    break;
                case 2:
                    ovl_11_func_800F8224((s16 *) D_80070D42_slots, 0x40);
                    break;
                case 1:
                    ovl_11_func_800F8224((s16 *) D_80071042, 0x40);
                    break;
                }
                goto block_32;
            }
            PlaySoundEffect(2);
            goto block_32;
        }
        goto block_32;
    }
block_32:
    return 0;
}
