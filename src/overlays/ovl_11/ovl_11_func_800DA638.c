#include "common.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800DA638(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    Ovl11GridBounds *sp20;
    s32 sp24;
    s32 sp28;
    s16 *var_s2;
    Ovl11D124Entry *temp_v0;
    s16 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    u32 var_s1;
    s32 var_s4;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    s32 var_v1_2;
    s32 bit;
    s32 base;
    s32 outerX;
    s32 outerY;
    s32 (**setter)(s16, s16, s16);

    sp10 = (s32) arg0;
    sp14 = (s32) arg1;
    sp18 = (s32) arg3;
    sp1C = (s32) arg4;
    sp20 = &D_80123E0C[arg2];
    if ((arg3 == 0) && (sp1C == 0)) {
        D_80123DFC[arg2](arg1, sp10, 5);
    }
    temp_v0 = ovl_11_func_800DAF60((s32) arg2, (s16) sp14, (s16) sp10);
    if (temp_v0 != NULL) {
        sp24 = arg2 * 4;
        temp_v0->unk6 = (u16) temp_v0->unk6 & 0xFFF0;
        var_v1 = 0;
        do {
            outerX = sp14 + D_80123E14[(var_v1 * 2) | 1];
            D_80128DAC = (u16) outerX;
            outerY = sp10 + D_80123E14[var_v1 * 2];
            D_80128DAE = (u16) outerY;
            var_s1 = 0;
            for (var_s4 = 0; var_s4 < 9; var_s4++) {
            temp_v1 = (s16) (D_80128DAC + D_80123E14[var_s4 * 2 + 1]);
            temp_v0_2 = D_80128DAE + D_80123E14[var_s4 * 2];
            if ((temp_v1 >= 0) && (temp_v0_2 >= 0) && (temp_v1 < sp20->width) && (temp_v0_2 < sp20->height)) {
                temp_v0 = ovl_11_func_800DAF60((s32) arg2, temp_v1, temp_v0_2);
                if ((temp_v0 != NULL) && ((u16) temp_v0->unk0 == 0x36) && (temp_v0->unk4 != 0)) {
                    var_s1 |= 1 << var_s4;
                }
            }
            }
            D_80128DB0 = -1;
            var_a0 = -1;
            if ((var_s1 & 0x1B8) == 0x1B8) {
                temp_v0_2 = 6;
                D_80128DB0 = temp_v0_2;
            }
            if (D_80128DB0 == var_a0) {
                if (((var_s1 & 0xDB) == 0xDB) && ((var_s1 & 0x24) != 0x24) && ((var_s1 & 0x120) != 0x120)) {
                    D_80128DB0 = 2;
                }
                if (D_80128DB0 == -1) {
                    if ((var_s1 & 0x1BB) == 0x1BB) {
                        D_80128DB0 = 6;
                    }
                    if (D_80128DB0 == -1) {
                        var_v1_2 = var_s1 & 0xFE;
                        if (var_v1_2 == 0xFE) {
                            var_v1_2 = 6;
                            D_80128DB0 = var_v1_2;
                        }
                        if (D_80128DB0 == -1) {
                            if (((var_s1 & 0x3F) == 0x3F) && ((var_s1 & 0xC0) != 0xC0) && ((var_s1 & 0x180) != 0x180)) {
                                D_80128DB0 = 4;
                            }
                            if (D_80128DB0 == -1) {
                                if (((var_s1 & 0x1B6) == 0x1B6) && ((var_s1 & 9) != 9) && ((var_s1 & 0x48) != 0x48)) {
                                    var_a0 = 6;
                                    D_80128DB0 = var_a0;
                                }
                                if (D_80128DB0 == -1) {
                                    if (((var_s1 & 0x1B) == 0x1B) && ((var_s1 & 0xC0) != 0xC0) && ((var_s1 & 0x24) != 0x24)) {
                                        D_80128DB0 = 3;
                                    }
                                    if (D_80128DB0 == -1) {
                                        if (((var_s1 & 0xD8) == 0xD8) && ((var_s1 & 3) != 3) && ((var_s1 & 0x120) != 0x120)) {
                                            D_80128DB0 = 1;
                                        }
                                        if (D_80128DB0 == -1) {
                                            if (((var_s1 & 0x36) == 0x36) && ((var_s1 & 9) != 9) && ((var_s1 & 0x180) != 0x180)) {
                                                D_80128DB0 = 5;
                                            }
                                            if (D_80128DB0 == -1) {
                                                if ((var_s1 & 0x1B0) == 0x1B0) {
                                                    if (((var_s1 & 0x48) != 0x48) && ((var_s1 & 6) != 6)) {
                                                        D_80128DB0 = 7;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ((var_s1 & 0x10) != 0) {
                if (sp18 != 0) {
                    var_a0 = 0;
                    bit = 0;
                    for (; bit < 9; bit++) {
                        temp_v1 = 1;
                        var_v0_2 = temp_v1 << bit;
                        if (var_s1 & var_v0_2) {
                            var_a0++;
                        }
                    }
                    if (D_80128DB0 == -1) {
                        if (var_a0 == 2) {
                            D_80128DB0 = 0;
                        }
                        if (D_80128DB0 == -1) {
                            if (var_a0 == 3) {
                                D_80128DB0 = 0;
                            }
                            if (D_80128DB0 == -1) {
                                if ((u32) (var_a0 - 4) < 3U) {
                                    if ((var_s1 & 0x1A0) != 0x1A0) {
                                        D_80128DB0 = 0;
                                    }
                                    if (D_80128DB0 == -1) {
                                        if ((var_s1 & 0x26) != 0x1A0) {
                                            D_80128DB0 = 0;
                                        }
                                        if (D_80128DB0 == -1) {
                                            if ((var_s1 & 0x4A) != 0x4A) {
                                                D_80128DB0 = 0;
                                            }
                                            if (D_80128DB0 == -1) {
                                                if ((var_s1 & 0xB) != 0x1A) {
                                                    D_80128DB0 = 0;
                                                }
                                                goto block_84;
                                            }
                                            goto block_85;
                                        }
                                        goto block_85;
                                    }
                                    goto block_85;
                                }
                                goto block_84;
                            }
                            goto block_85;
                        }
                        goto block_85;
                    }
                    goto block_85;
                }
            }
block_84:
            if (D_80128DB0 != -1) {
block_85:
                temp_v0 = ovl_11_func_800DAF60((s32) arg2, (s16) D_80128DAC, (s16) D_80128DAE);
                if (temp_v0 != NULL) {
                    if (sp1C == 0) {
                        setter = &D_80123DFC[arg2];
                        base = ((temp_v0->unk4 - 1) * 8) + 5;
                        temp_v0_2 = base + (u16) D_80128DB0;
                        (*setter)((s16) D_80128DAC, (s16) D_80128DAE, temp_v0_2);
                    }
                    var_v0_2 = (u16) temp_v0->unk6 & 0xFFF0;
                    var_v0_2 |= (u16) D_80128DB0;
                    temp_v0->unk6 = var_v0_2;
                }
            }
            var_v1++;
        } while (var_v1 < 9);
    }
}
