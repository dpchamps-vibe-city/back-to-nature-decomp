#include "common.h"

void func_80014BCC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_8001AF44(u32 arg0);
void ovl_11_func_800BD938(void);
void func_8001719C(u8 *arg0);
void ovl_11_func_800F0E00(void *arg0);
void ovl_11_func_800E3CDC(u8 *arg0);
void *memcpy(void *dest, const void *src, u32 n);

void ovl_11_func_800BCF54(s32 arg0, s32 arg1) {
    s32 *record;
    s32 start;
    s32 start1;
    s32 start2;
    u8 *base;

    arg0 *= 4;
    record = &D_801217A8 + arg0;
    D_8005E328 = record[1] - record[0];
    start = record[0];
    if (arg1 == 0) {
        func_80014BCC(0, start, D_8005E328, 0, D_8007AFF0);
    }
    memcpy(D_8009F78C, (void *)D_8007AFF0, 0xBDC);
    base = (u8 *)&D_8007AFF0;
    *(Ovl25SetupStatePair *)(base + 0x25474) =
        *(Ovl25SetupStatePair *)(base + 0x2479C);
    *(u8 **)(base + 0x25388) = D_8009F78C;
    if (func_8001AF44(0x9B) == 1) {
        ovl_11_func_800BD938();
    }
    start1 = record[1];
    func_80014BCC(0, start1, record[2] - start1, 0, D_8005E3B0 + 0x4290);
    func_8001719C((u8 *)(D_8005E3B0 + 0x4290));
    start2 = record[2];
    func_80014BCC(0, start2, record[4] - start2, 0, D_8005E3B0 + 0x4290);
    ovl_11_func_800F0E00((void *)(D_8005E3B0 + 0x4290));
    ovl_11_func_800E3CDC((u8 *)D_8005E3B0 + ((record[3] - record[2]) + 0x4290));
}
