#include "common.h"

void func_8001AF70(u16 arg0, u16 arg1);
s32 func_8001AF44(u32 arg0);

#define DATE ((struct Ovl11FlagDateView *)D_8006C838)

void ovl_11_func_800C0FC0(s16 arg0) {
    u32 i;
    s16 *pair;
    struct Ovl11FlagDatePair *last_pair;

    for (i = 0; i < 7; i++) {
        func_8001AF70((u16)(i + 0xB), 0);
    }
    func_8001AF70((u16)(arg0 + 0xB), 1);
    for (i = 0x78; i < 0x96; i++) {
        func_8001AF70((u16)i, 0);
    }
    func_8001AF70((u16)(DATE->field_44BC + 0x78), 1);
    func_8001AF70(0x97, 0);
    if (DATE->field_44BA == DATE->field_5246 && DATE->field_44BC == DATE->field_5248) {
        func_8001AF70(0x97, 1);
    }
    if (func_8001AF44(0x1B) != 1) {
        func_8001AF70(0x99, 0);
        if (DATE->field_44BA == DATE->field_E798) {
            if (DATE->field_44BC == DATE->field_E79A) {
                func_8001AF70(0x99, 1);
            }
        }
        func_8001AF70(0x98, 0);
        pair = (s16 *)((u8 *)D_80076244 + DATE->field_E778 * 0x1D4);
        if (DATE->field_44BA == pair[0] && DATE->field_44BC == pair[1]) {
            func_8001AF70(0x98, 1);
        }
        if (func_8001AF44(0x4F) != 1) {
            func_8001AF70(0x9A, 0);
            last_pair = &D_8007A414;
            if (DATE->field_44BA == last_pair->field_0 && DATE->field_44BC == last_pair->field_2) {
                func_8001AF70(0x9A, 1);
            }
        }
    }
}
