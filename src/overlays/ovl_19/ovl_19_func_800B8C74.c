#include "common.h"

void ovl_19_func_800BA770(void);
void func_80017A08(s32 arg0, s32 arg1);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
s32 ovl_19_func_800BADAC(s16 arg0);
void func_8001FAE8(s32 arg0);
void ovl_19_func_800B843C(void);

void ovl_19_func_800B8C74(void) {
    s32 flag;

    ovl_19_func_800BA770();
    flag = ((Ovl19SelectionState *)D_800BF4C0)->flag;
    if (flag == 0) {
        func_80017A08(0, ((Ovl19CompletionCounterView *)D_8006C838)->text);
        func_8002261C(4, 0x16);
    } else {
        func_80017A08(0, (s32)((u8 *)D_800BCD68 +
            ovl_19_func_800BADAC(((Ovl19SelectionState *)D_800BF4C0)->values[
                D_800BCF24[((Ovl19SelectionState *)D_800BF4C0)->selection]])));
        func_8002261C(4, 0x17);
    }
    if (func_800226A4() == 2) {
        ((Ovl19SelectionState *)D_800BF4C0)->selection++;
        func_8001FAE8(((Ovl19SelectionState *)D_800BF4C0)->handle);
        if (flag == 0) {
            if (((Ovl19SelectionState *)D_800BF4C0)->selection < 3) {
                ovl_19_func_800B843C();
                ((Ovl19SelectionState *)D_800BF4C0)->state = 0;
            } else {
                ((Ovl19SelectionState *)D_800BF4C0)->state = 7;
            }
        } else {
            ((Ovl19CompletionCounterView *)D_8006C838)->completed = 0;
            ((Ovl19CompletionCounterView *)D_8006C838)->counter++;
        }
    }
}
