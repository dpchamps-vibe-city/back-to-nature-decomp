#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

s32 ovl_25_func_800B9480 (s16 *arg0);
void ovl_25_func_800B97E4 (s16 *arg0);
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);

void ovl_25_func_800B8DF4(s16 *arg0) {
    s16 temp_a0;
    s16 temp_v0;
    s16 temp_v0_2;

    temp_v0 = (*(u16 *) ((u8 *) arg0 + 2)) - 2;
    switch (temp_v0) {                              /* switch 1 */
    case 5:                                         /* switch 1 */
        if ((arg0[0] != 0) && ((*(s16 *) ((u8 *) arg0 + 4)) <= 0)) {
            ovl_25_func_800B9480(arg0);
        }
        break;
    case 3:                                         /* switch 1 */
        (*(s32 *) ((u8 *) arg0 + 0xC)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0xC)) + ((s32) (D_800BFE44_view.unk4D6 << 0xC) / (s16) D_800BFE44_view.unk4D4));
    default:                                        /* switch 1 */
        break;
    case 0:                                         /* switch 1 */
        (*(s32 *) ((u8 *) arg0 + 0xC)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0xC)) - ((s32) (D_800BFE44_view.unk4DA << 0xC) / (s16) D_800BFE44_view.unk4D4));
        break;
    case 4:                                         /* switch 1 */
        (*(s32 *) ((u8 *) arg0 + 0xC)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0xC)) + ((s32) (D_800BFE44_view.unk4D6 * (D_800BFE44_view.unk4D0 << 0xC)) / (s32) (D_800BFE44_view.unk4D4 * 0x64)));
        break;
    case 2:                                         /* switch 1 */
        ovl_25_func_800B97E4(arg0);
        break;
    case 1:                                         /* switch 1 */
        (*(s32 *) ((u8 *) arg0 + 0xC)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0xC)) - ((s32) (D_800BFE44_view.unk4DA << 0xC) / (s16) D_800BFE44_view.unk4D4));
        if ((*(s16 *) ((u8 *) arg0 + 4)) <= 0) {
            ovl_25_func_800B9B00(arg0, 6, *(s16 *) ((u8 *) arg0 + 6));
            (*(s16 *) ((u8 *) arg0 + 6)) = 0;
        }
        break;
    }
    if ((*(s32 *) ((u8 *) arg0 + 0xC)) > 0x31FFF) {
        (*(s32 *) ((u8 *) arg0 + 0xC)) = 0x32000;
    }
    if ((*(s32 *) ((u8 *) arg0 + 0xC)) < 0) {
        (*(s32 *) ((u8 *) arg0 + 0xC)) = 0;
    }
    if (arg0[0] == 0) {
        temp_v0_2 = (*(u16 *) ((u8 *) arg0 + 2)) - 2;
        switch (temp_v0_2) {                        /* switch 2 */
        case 0:                                     /* switch 2 */
            if (D_800BFE44_view.unkE >= ((D_800BFE44_view.unk4CE * 0x1E) / 100)) {
                ovl_25_func_800B9B00(arg0, 6, *(s16 *) ((u8 *) arg0 + 4));
            } else {
                if ((*(s32 *) ((u8 *) arg0 + 0xC)) <= 0x9FFF) {
                    ovl_25_func_800B9B00(arg0, 7, 0x3C);
                }
            }
        case 2:                             /* switch 2 */
        case 3:                             /* switch 2 */
            if (D_800BFE44_view.unk4CA == -1) {
                D_800BFE44_view.unk4CA = PlaySoundEffect(0x37);
            }
            break;
        case 4:                                     /* switch 2 */
            if (D_800BFE44_view.unkE >= (D_800BFE44_view.unk4DC * 0x1E)) {
                D_800BFE44_view.unk4CC = 1;
            }
        case 5:                                 /* switch 2 */
            temp_a0 = D_800BFE44_view.unk4CA;
            if (temp_a0 != -1) {
                StopSoundEffectVoice((s32) temp_a0);
                D_800BFE44_view.unk4CA = -1;
            }
            if ((*(s16 *) ((u8 *) arg0 + 4)) == 0) {
                ovl_25_func_800B9B00(arg0, 6, 0);
            }
            break;
        case 6:                                     /* switch 2 */
            if (D_800BFE44_view.unk4CA != -1) {
                StopSoundEffectVoice((s32) D_800BFE44_view.unk4CA);
                D_800BFE44_view.unk4CA = -1;
            }
            break;
        }
        if ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 5) {
            if (D_800BFE44_view.unk4CA != -1) {
                SetSoundVoiceVolume(D_800BFE44_view.unk4CA, 0x46, 0x46);
            }
        } else if (D_800BFE44_view.unk4CA != -1) {
            SetSoundVoiceVolume(D_800BFE44_view.unk4CA, 0x7F, 0x7F);
        }
        if ((D_800BFE44_view.unk8 == 1) && (D_800BFE44_view.unk20 == D_800BCC40)) {
            D_800BFE44_view.unk4CC = -1;
        }
    }
}
