#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl25_record_value_increments.h"


extern s16 D_800BFE44[];

void ovl_25_func_800B9B00 (s16 *arg0, s16 arg1, s16 arg2);

void ovl_25_func_800B9208(void) {
    char *base;
    s16 *rec;
    s32 var_s2;
    s32 var_s6;

    var_s6 = 1;
    var_s2 = 0;
    base = (char *) D_80074838;
    rec = (s16 *) ((u8 *) &D_800BFE44);
    do {
        rec = (s16 *) ((u8 *) &D_800BFE44 + var_s2 * 0x78);
        if (rec[9] != 8) {
            if (rec[0x10] >= (*(s16 *) ((u8 *) &D_800BFE44 + 0x4D2) - 0x140)) {
                ovl_25_func_800B9B00((s16 *) ((u8 *) &D_800BFE44 + 0x10 + var_s2 * 0x78), 8, 0x1E);
                if (*(s32 *) ((u8 *) &D_800BFE44 + 8) == 0) {
                    *(s32 *) ((u8 *) &D_800BFE44 + 8) = 1;
                    *(s16 *) ((u8 *) &D_800BFE44 + 0x4C8) = PlaySoundEffect(0x39);
                    ovl_25_func_800B9B00((s16 *) ((u8 *) &D_800BFE44 + 0x358), 0xC, 0x1E);
                    if (var_s2 == 0) {
                        *(s32 *) (base + 0x6638) = 1;
                        *(s16 *) (base + 0x663C) = 0;
                        Ovl25RaiseRecordByteRangeValue(1);
                        Ovl25RaiseRecordByteRangeValue(2);
                        Ovl25RaiseRecordByteRangeValue(4);
                        Ovl25RaiseRecordByteRangeValue(5);
                        Ovl25RaiseRecordByteRangeValue(8);
                        Ovl25RaiseRecordByteRangeValue(0xB);
                        Ovl25RaiseRecordByteRangeValue(0x10);
                        Ovl25RaiseRecordByteRangeValue(0x11);
                        Ovl25RaiseRecordByteRangeValue(0x13);
                        Ovl25RaiseRecordByteRangeValue(0x19);
                        Ovl25RaiseRecordByteRangeValue(0x1C);
                        Ovl25RaiseRecordByteRangeValue(0x22);
                        Ovl25RaiseRecordHalfwordRangeValue(3);
                        Ovl25RaiseRecordHalfwordRangeValue(7);
                        Ovl25RaiseRecordHalfwordRangeValue(9);
                        Ovl25RaiseRecordHalfwordRangeValue(0xE);
                        Ovl25RaiseRecordHalfwordRangeValue(0x14);
                    } else {
                        *(s32 *) (base + 0x6638) = 0;
                        *(s16 *) (base + 0x663C) = var_s2;
                        goto block_8;
                    }
                } else {
                    goto block_8;
                }
            } else {
                var_s6 = 0;
                goto block_8;
            }
        } else {
block_8:
            ;
        }
        var_s2 += 1;
    } while (var_s2 < 6);
    if (var_s6 == 1) {
        char *p;

        p = (char *) D_8006C838;
        *(s32 *) (p + 0x448C) = *(s32 *) (p + 0x448C) + 1;
    }
}
