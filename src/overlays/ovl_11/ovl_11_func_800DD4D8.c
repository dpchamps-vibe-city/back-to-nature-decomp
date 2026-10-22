#include "common.h"

typedef struct {
    char pad_00[0x2E];
    u16 count;
} IncrementView;

typedef struct {
    char pad_00[0x2E];
    s16 count;
    s16 max;
} CompareView;

typedef struct {
    char pad_00[0x28];
    u16 field_28;
    char pad_2A[2];
    void *ptr;
} BufView;

s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_800DD4D8(void) {
    s32 ovl_11_func_800DD45C(BufView *arg0, BufView *arg1, s32 arg2) {
        IncrementView *iv;
        CompareView *cv;

        iv = arg0->ptr;
        iv->count = iv->count + 1;
        cv = arg0->ptr;
        if (cv->count < cv->max) {
            return 0;
        }
        *(s16 *)((u8 *)arg1->ptr + 0x2E) = 0;
        cv->count = 0;
        arg0->field_28 |= 0x1000;
        arg1->field_28 &= 0xEFFF;
        D_80129184 = arg2;
        return 1;
    }
    BufView *first;
    s32 one;

    first = (BufView *)D_80129178[0];
    if (first != 0) {
        one = 1;
        if (D_80129184 == one) goto state_1;
        if (D_80129184 == 0) goto state_0;
        if (D_80129184 == 2) goto state_2;
        if (D_80129184 == 3) goto state_3;
    }
    return;
state_0:
    if (func_8001AF44(7) == one) {
        D_80129184 = one;
    }
    return;
state_1:
    ovl_11_func_800DD45C(first, (BufView *)D_80129178[1], 2);
    return;
state_2:
    ovl_11_func_800DD45C((BufView *)D_80129178[1], (BufView *)D_80129178[2], 3);
    return;
state_3:
    if (ovl_11_func_800DD45C((BufView *)D_80129178[2], first, 0) == one) {
        func_8001AF70(7, 0);
    }
}
