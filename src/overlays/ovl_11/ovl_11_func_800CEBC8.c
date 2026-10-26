#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800DE9C8 (UnkStruct800DF4F0 *arg0);
s32 ovl_11_func_800E05A8 (UnkStruct800E109C *arg0);
s32 ovl_11_func_800D3250 (u16 *arg0);
s32 ovl_11_func_800E20F0(u16 *arg0);

void ovl_11_func_800CEBC8(void) {
    s32 var_s0;
    u16 *var_s1;
    u16 *records;

    if (!(((struct Ovl11PoolResetView *) D_8006C838)->flags & 8)) {
        for (var_s0 = 0; var_s0 < 10; var_s0++) {
            var_s1 = &((struct Ovl11PoolResetView *) D_8006C838)->records_7AB4[var_s0].type;
            if (*var_s1 != 0) {
                ovl_11_func_800E20F0(var_s1);
            }
        }
        for (var_s0 = 0; var_s0 < 20; var_s0++) {
            records = (u16 *) ((u8 *) &gTaggedRecordBase + var_s0 * 0xB8);
            if (*records != 0) {
                ovl_11_func_800DE9C8((UnkStruct800DF4F0 *) records);
            }
        }
        for (var_s0 = 0; var_s0 < 20; var_s0++) {
            records = (u16 *) ((u8 *) &gTaggedRecordBase + var_s0 * 0xB8);
            if (*records != 0) {
                ovl_11_func_800E05A8((UnkStruct800E109C *) records);
            }
        }
        for (var_s0 = 0; var_s0 < 6; var_s0++) {
            var_s1 = (u16 *) (D_80075BC4_reset + var_s0 * 0xB0);
            ovl_11_func_800D3250(var_s1);
        }
    }
}
