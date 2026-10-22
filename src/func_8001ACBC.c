#include "common.h"
void func_8001ACA0(void);

s16 GetPairedTpage(s32 arg0);

void func_8001A018(s32 arg0, s32 arg1);

void func_80019FC4(s16 arg0);

void func_8001ACBC(u16 arg0) {
    s16 callRet3;
    func_8001ACA0();
    callRet3 = GetPairedTpage(arg0);
    func_8001A018(callRet3, 0);
    func_80019FC4(callRet3);
}
