#include "common.h"

void func_80013328(s16 arg0);
void func_8001FE34(s32 arg0);
void ovl_19_func_800BA770(void);

void ovl_19_func_800B8150(void) {
    func_80013328(0xA);
    func_8001FE34(0xA);
    ovl_19_func_800BA770();
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}
