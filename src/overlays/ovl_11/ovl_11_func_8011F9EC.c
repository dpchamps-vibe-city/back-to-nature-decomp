#include "common.h"

u32 Rand(s32 arg0);

void ovl_11_func_8011F9EC(s32 *arg0) {
    s16 *tbl;
    s32 i;
    s16 max_i;
    s16 sec_i;
    s16 sel;
    s32 max;
    s32 sec;
    s16 tmp;

    tbl = D_80071A70;
    max = -1;
    max_i = -1;
    sec = -1;
    sec_i = -1;
    for (i = 0; i < 6; i++) {
        if (max < tbl[i]) {
            max_i = i;
            max = tbl[i];
        }
    }
    for (i = 0; i < 6; i++) {
        if (max_i != i && sec < tbl[i]) {
            sec_i = i;
            sec = tbl[i];
        }
    }
    if (max == sec) {
        if (Rand(2) != 0) {
            tmp = max_i;
            max_i = sec_i;
            sec_i = tmp;
        }
    }
    sel = Rand(0x13);
    arg0[0] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + D_8012855C[max_i].unk0));
    arg0[1] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + D_8012855C[sec_i].unk0));
    arg0[2] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + D_80128580[sel].unk0));
    D_80128550.unk0 = max_i;
    D_80128550.unk2 = sec_i;
    D_80128550.unk4 = sel;
}
