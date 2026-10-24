#include "common.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);
s32 func_80011F5C(s32 arg0);
void func_80011FD8(s32 arg0);
void *ovl_11_func_800D7144(s16 arg0, s16 arg1, s16 arg2, s32 *arg3, void *, s16 arg5, s16 arg6, s32 arg7);

void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5) {
    s16 *first;
    s16 *second;
    s16 row;
    u32 i;

    row = -1;
    first = D_800A0494;
    second = first + 9;
    for (i = 0; i < 9;) {
        if (*first == arg0) {
            row = 0;
            break;
        }
        if (*second == arg0) {
            row = 1;
            break;
        }
        i++;
        first++;
        second++;
    }
    if (row != -1) {
        if (arg5 & 0xD) {
            ovl_11_func_800F7010((s32)(arg2 + 1), arg3, arg4);
        }
        if (arg5 & 8) {
            arg3 += (32 - ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][i].unk8) / 2;
            arg4 += (32 - ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][i].unkA) / 2;
        }
        if (arg5 & 4) {
            arg3 += 16;
            arg4 += 16;
            arg5 |= 2;
        }
        func_80011FD8(ovl_11_func_800D7144((s16)i, row, arg1, arg2, func_80011F5C(0), arg3, arg4, arg5));
    }
}
