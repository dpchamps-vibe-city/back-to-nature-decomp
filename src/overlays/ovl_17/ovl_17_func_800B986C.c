#include "common.h"
#include "ovl17_record_sprite_animation.h"

s32 ovl_17_func_800B986C(void) {
    s32 temp_a2;
    s32 i;
    s32 count;
    u8 *base;
    u8 *temp_s0;
    u8 *p;
    u8 *obj;
    u8 *arg;

    base = gOvl17RecordWorkBase;
    temp_a2 = (*(s16 *) (base + 0x2BC)) * 0x50;
    temp_s0 = base + 0x38;
    if (*(s32 *)(temp_s0 + temp_a2) > 0x800000) {
        arg = base + 0x28;
        Ovl17SelectRecordAnimation1(arg + temp_a2);
    }
    for (i = 0; i < 6; i++) {
        p = temp_s0 + i * 0x50;
        if ((*(s32 *) p) > 0x93FFFF) {
            (*(s32 *) p) = 0x940000;
        }
    }
    count = 0;
    for (i = 0; i < 6; i++) {
        obj = gOvl17RecordWorkBase;
        p = obj + 0x38 + i * 0x50;
        if ((*(s32 *) p) <= 0x93FFFF) {
            count = (count + 1) & 0xFF;
        }
    }
    return count == 0;
}
