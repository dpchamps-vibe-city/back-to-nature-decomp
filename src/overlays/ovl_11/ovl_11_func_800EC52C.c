#include "common.h"

u32 func_80012A34(s32);
s32 ovl_11_func_800E5870(s16, s16, s32, s16);
s32 ovl_11_func_800E58DC(s16, s16, s32, s32);
s32 ovl_11_func_800E5ADC(s16, s16, s32, s32);
s32 ovl_11_func_800E5B84(s16, s16, s32, s32);
s32 ovl_11_func_800E60A0(s16, s32, s32, s32);
s32 ovl_11_func_800E6AB0();
s32 ovl_11_func_800E6B18();
s32 ovl_11_func_800E6BDC();
s32 ovl_11_func_800E6D2C(s16, s16, s32, s32);
s32 ovl_11_func_800E6EE0(s16, s16, s32, s32);
s32 ovl_11_func_800E880C(s16, s16, s32, s32);
s32 ovl_11_func_800E8960(s16, s16, s32, s32);
s32 ovl_11_func_800E8E78(s16, s16, s32, s32);
s32 ovl_11_func_800E8F78(s16, s16, s32, s32);
s32 ovl_11_func_800E9284();
s32 ovl_11_func_800E99EC(s16, s16, s32, s32);
s32 ovl_11_func_800E9AFC();
s32 ovl_11_func_800ED4A8(s16, s16, s32, s32);
s32 ovl_11_func_800EDF3C();
s32 ovl_11_func_800F0A58(s16);
s32 ovl_11_func_801000D4(s16);

s32 ovl_11_func_800EC52C(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    struct Ovl11SceneScores scores = D_800B9BD4;
    s32 state;
    s32 next;
    s32 result;
    s32 level;
    s32 best;
    s16 i;
    s32 x;
    s32 z;
    s32 message;
    s16 *item;

    if (arg0 == -1) {
        ovl_11_func_800E8960(0, -200, -1227, 1402);
        ovl_11_func_800E8960(2, -200, -653, 1402);
        ovl_11_func_800E8960(1, -200, -996, -999);
        ovl_11_func_800E8960(3, -200, 992, -1081);
        ovl_11_func_800E8960(4, -200, 992, -630);
        return 1;
    }
    state = *(s16 *)&D_80129560[arg0];
    result = 0;
    switch (state) {
    case 0:
        item = (s16 *)&Ovl11Item_80071A8A;
        if (ovl_11_func_801000D4(item[0]) == 1) {
            result = 1;
            D_80129560[arg2] = item[1];
        } else if (item[0] == 0x87) {
            D_80129560[arg2] = 4;
            result = 1;
        }
        if (result == 1) {
            ovl_11_func_800F0A58(item[0]);
            D_80070CB4 = 1000;
            D_80129560[arg1] = item[0];
            ovl_11_func_800E6EE0(item[0], 1, 0, 1);
        }
        return result;
    case 1:
        level = ((Ovl11SceneState *)D_8006C838)->level;
            ovl_11_func_800E5ADC(0x1E, 0, -0x4CB, 0x3F2);
            ovl_11_func_800E5B84(0x1E, 0, 2, 1);
            ovl_11_func_800E5ADC(0x16, 0, -0x524, 0x6FE);
            ovl_11_func_800E5B84(0x16, 0, 0, 1);
            ovl_11_func_800E5ADC(3, 0, -0x31D, 0x6FE);
            ovl_11_func_800E5B84(3, 0, 0, 1);
            ovl_11_func_800E5ADC(0xD, 0, -0x573, -0x3C9);
            ovl_11_func_800E5B84(0xD, 0, 3, 1);
            ovl_11_func_800E5ADC(6, 0, 0x57B, -0x54D);
            ovl_11_func_800E5B84(6, 0, 1, 1);
            ovl_11_func_800E5ADC(7, 0, 0x57B, -0x338);
            ovl_11_func_800E5B84(7, 0, 1, 1);
            ovl_11_func_800E5ADC(0xC, 0, -0x8D0, -0x6A6);
            ovl_11_func_800E5B84(0xC, 0, 3, 1);
            ovl_11_func_800E5ADC(0xE, 0, -0x855, -0x8E4);
            ovl_11_func_800E5B84(0xE, 0, 3, 1);
            ovl_11_func_800E5ADC(5, 0, -0x6BB, -0xA55);
            ovl_11_func_800E5B84(5, 0, 2, 1);
            ovl_11_func_800E5ADC(8, 0, 0x333, -0xA01);
            ovl_11_func_800E5B84(8, 0, 2, 1);
            ovl_11_func_800E5ADC(9, 0, 0x70B, -0xA01);
            ovl_11_func_800E5B84(9, 0, 2, 1);
            ovl_11_func_800E5ADC(0x18, 0, -0x9A3, 0x5BC);
            ovl_11_func_800E5B84(0x18, 0, 3, 1);
            ovl_11_func_800E5ADC(0xA, 0, -0x928, 0x809);
            ovl_11_func_800E5B84(0xA, 0, 3, 1);
            ovl_11_func_800E5ADC(0x11, 0, 0x541, 0x8C9);
            ovl_11_func_800E5B84(0x11, 0, 0, 1);
            ovl_11_func_800E5ADC(2, 0, -0x843, 0x327);
            ovl_11_func_800E5B84(2, 0, 3, 1);
            if (ovl_11_func_800EDF3C(0x14, 0, 0, 0) == 1) {
                ovl_11_func_800E5ADC(0x14, 0, 0x51F, -0xA01);
                ovl_11_func_800E5B84(0x14, 0, 2, 1);
            }

        best = 0;
        if (level >= 5) { level = 4; }
        for (i = 0; i < 5; i++) {
            scores.values[level][i] += func_80012A34(D_801249B8[level][i]);
            if (scores.values[level][best] < scores.values[level][i]) {
                best = i;
            }
        }
        if (D_80129560[arg1] != 0) {
            ovl_11_func_800E8E78(0, 0, 1001, 1790);
            ovl_11_func_800E9284(0, 0, 0, 0);
            ((Ovl11SceneState *)D_8006C838)->y = -400;
            ((Ovl11SceneState *)D_8006C838)->x = 887;
            ((Ovl11SceneState *)D_8006C838)->z = 1352;
            if (scores.values[level][best] <= D_80129560[arg2]) {
                best = -1;
                ovl_11_func_800ED4A8(32, 1, 20, 2000);
            }
        }
        switch ((s16)(best + 1)) {
        case 0: D_80129560[arg3] = 41; break;
        case 1: D_80129560[arg3] = 22; break;
        case 3: D_80129560[arg3] = 13; break;
        case 4: D_80129560[arg3] = 6; break;
        case 5: D_80129560[arg3] = 7; break;
        }
        ovl_11_func_800E880C(30, 0, 0, 0);
        result = 1;
        break;
        case 2:                                     
            result = ovl_11_func_800E6B18(0x13, 1, 0, 0);
            break;
        case 3:                                     
            result = ovl_11_func_800E6D2C(1, 0, 0, 0);
            if (result == 1) {
                ovl_11_func_800E6BDC(0, 0, 0, 0);
            }
            break;
        case 4:                                     
            ovl_11_func_800E5B84(0x1E, 7, 0, 0);
            result = ovl_11_func_800E58DC(1, 0x45C, -1, 0);
            break;
        case 5:                                     
            result = ovl_11_func_800E5870(0x1E, 3, -0x28D, 4);
            break;
        case 6:                                     
            ovl_11_func_800E5B84(0x1E, 7, 0, 0);
            result = ovl_11_func_800E58DC(1, 0x45E, -1, 0);
            break;
        case 7:                                     
            result = ovl_11_func_800E5870(0x1E, 0, -0x3E7, 1);
            break;
        case 8:                                     
            ovl_11_func_800E5B84(0x1E, 6, 0, 0);
            result = ovl_11_func_800E58DC(1, 0x45D, -1, 0);
            break;
        case 9:                                     
            result = ovl_11_func_800E5870(0x1E, 0, -0x439, 1);
            break;
        case 10:                                    
            result = ovl_11_func_800E5870(0x1E, 3, 0x26A, 4);
            break;
        case 11:                                    
            ovl_11_func_800E5B84(0x1E, 8, 0, 0);
            result = ovl_11_func_800E58DC(1, 0x45F, -1, 0);
            break;
        case 12:                                    
            result = ovl_11_func_800E5870(0x1E, 2, -0x276, 3);
            break;
        case 13:                                    
            ovl_11_func_800E5B84(0x1E, 8, 0, 0);
            result = ovl_11_func_800E58DC(1, 0x460, -1, 0);
            break;
        case 14:                                    
            result = ovl_11_func_800E5870(0x1E, 2, 0x3C9, 3);
            break;
        case 15:                                    
            result = ovl_11_func_800E5870(0x1E, 3, 0x3DB, 4);
            break;
        case 16:                                    
            ovl_11_func_800E5B84(0x1E, 7, 0, 0);
            if (D_80129560[arg3] == 0x29) {
                result = ovl_11_func_800E58DC(1, 0x461, -1, 0);
            } else {
                result = ovl_11_func_800E58DC(1, 0x462, -1, 0);
            }
            break;
        case 17:                                    
            ovl_11_func_800E5B84(0x1E, 0, 2, 1);
            ovl_11_func_800E9AFC(0, 0x3C, 1, 0);
            ovl_11_func_800E6D2C(0, 0x3C, 1, 0);
            result = 1;
            break;
        case 18:                                    
            result = ovl_11_func_800E6D2C(0, 0, 0, 0);
            break;
        case 19:                                    
            ovl_11_func_800E99EC(0xF, 0, 0, 0);
            ovl_11_func_800E5ADC(0x1E, 0, 0x4E, 0x8CF);
            ovl_11_func_800E5B84(0x1E, 0, 0, 1);
            ovl_11_func_800E5ADC(0x11, 0, -0x14C, 0x8CF);
            ovl_11_func_800E5B84(0x11, 0, 0, 1);
            if (D_80129560[arg1] != 0) {
                ovl_11_func_800E8E78(0, 0, -0x7F, 0xCD);
            } else {
                ovl_11_func_800E8E78(0, 0, 0x1E8, -0x35D);
            }
            ovl_11_func_800E9284(2, 0, 0, 0);
            ovl_11_func_800E5ADC(0x16, 0, -0x30F, 0x2E2);
            ovl_11_func_800E5B84(0x16, 0, 2, 1);
            ovl_11_func_800E5ADC(3, 0, 0x211, 0xCD);
            ovl_11_func_800E5B84(3, 0, 2, 1);
            switch (D_80129560[arg3]) {
            case 0x29: x = -0x7F; z = 0xCD; break;
            case 0x16: x = -0x30F; z = 0x2E2; break;
            case 6: x = -0x30F; z = 0xCD; break;
            case 7: x = 0x211; z = 0x2E2; break;
            default: x = -0x7F; z = 0x2E2; break;
            }
            ovl_11_func_800E5ADC(0xD, 0, x, z);
            ovl_11_func_800E5B84(0xD, 0, 2, 1);
            ovl_11_func_800E5ADC(6, 0, -0x30F, 0xCD);
            ovl_11_func_800E5B84(6, 0, 2, 1);
            ovl_11_func_800E5ADC(7, 0, 0x211, 0x2E2);
            ovl_11_func_800E5B84(7, 0, 2, 1);

            if (D_80129560[arg3] == 0x29) {
                ovl_11_func_800E8E78(0, 0, -0x7F, 0x2E2);
            } else {
                ovl_11_func_800E5ADC((s16) D_80129560[arg3], 0, -0x7F, 0x2E2);
            }
            ovl_11_func_800E60A0(-1, 0, 0x29, 0x47C);
            result = ovl_11_func_800E6B18(0xF, 0, 0, 0);
            break;
        case 20:                                    
            ovl_11_func_800E9AFC(1, 0x3C, 1, 0);
            ovl_11_func_800E6D2C(1, 0x3C, 1, 0);
            result = 1;
            break;
        case 21:                                    
            result = ovl_11_func_800E6D2C(1, 0, 0, 0);
            break;
        case 22:                                    
            ovl_11_func_800E6AB0(3, 0, 1, 0);
            result = 1;
            break;
        case 23:                                    
            result = ovl_11_func_800E6AB0(3, 0, 0, 0);
            if (result == 1) {
                ovl_11_func_800E6BDC(0, 0, 0, 0);
            }
            break;
        case 24:                                    
            switch (D_80129560[arg3]) {                    
            case 22:                                
                message = 0x474;
                break;
            case 13:                                
                message = 0x476;
                break;
            case 6:                                 
                message = 0x478;
                break;
            case 7:                                 
                message = 0x47A;
                break;
            case 41:
            default:
                message = 0x472;
                break;
            }
            result = ovl_11_func_800E58DC(1, message, -1, 0);
            break;
        case 25:                                    

            if (D_80129560[arg3] == 0x29) {
                result = ovl_11_func_800E8F78(0, 2, 0x5A4, 0);
            } else {
                result = ovl_11_func_800E5870((s16) D_80129560[arg3], 2, 0x5A4, 3);
            }
            break;
        case 26:                                    

            if (D_80129560[arg3] == 0x29) {
                ovl_11_func_800E9284(2, 0, 0, 0);
            } else {
                ovl_11_func_800E5B84((s16) D_80129560[arg3], 0, 2, 1);
            }
            switch (D_80129560[arg3]) {                   
            case 22:                                
                message = 0x475;
                break;
            case 13:                                
                message = 0x477;
                break;
            case 6:                                 
                message = 0x479;
                break;
            case 7:                                 
                message = 0x47B;
                break;
            case 41:
            default:
                message = 0x473;
                break;
            }
            result = ovl_11_func_800E58DC(1, message, -1, 0);
            break;

    case 27:
        result = ovl_11_func_800E880C(-2, 0, 1, 2);
        break;
    }
    if (result == 1) {
        if (state != 13 || (next = 17, D_80129560[arg1] != 0)) {
            next = (s16)(state + 1);
        }
        state = next;
    }
    D_80129560[arg0] = state;
    if (state != 20 && state < 28) {
        result = 0;
    }
    return result;
}
