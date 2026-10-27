#include "common.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_small_record_handlers.h"

void ovl_11_func_800CFD9C(void) {
    Ovl11CFD9CEntry *var_s0;
    s16 *temp_v0;
    s32 var_s1;
    s16 var_s3;
    s32 *var_s2;

    var_s1 = 0;
    var_s3 = 0x30;
    var_s0 = D_801239D0;
    var_s2 = (s32 *)((u8 *) D_801239D0 + 4);
    do {
        temp_v0 = Ovl11AllocateSmallRecord(0x109, -1, 0);
        if (temp_v0 != 0) {
            (*(s16 *) ((u8 *) temp_v0 + 0x30)) = var_s3;
            (*(s16 *) ((u8 *) temp_v0 + 0x22)) = var_s1;
            (*(s32 *) ((u8 *) temp_v0 + 0x38)) = var_s0->unk0;
            (*(s32 *) ((u8 *) temp_v0 + 0x3C)) = *var_s2;
            (*(s32 *) ((u8 *) temp_v0 + 0x40)) = var_s0->unk8;
        }
        var_s0 = (Ovl11CFD9CEntry *) ((u8 *) var_s0 + 0x10);
        var_s1 += 1;
        var_s2 += 4;
    } while (var_s1 < 3);
}
