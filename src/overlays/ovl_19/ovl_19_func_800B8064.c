#include "common.h"
#include "screen_fade.h"

void ovl_19_func_800BB358(void);
void ovl_19_func_800BB470(void);
void ovl_19_func_800BB4D4(void);
void ovl_19_func_800BB108();
void func_8001B5DC(void);
s32 func_8001FE6C(void);

void ovl_19_func_800B8064(void) {
    u8 var_s0;

    ovl_19_func_800BB358();
    ovl_19_func_800BB470();
    func_8001B5DC();
    ovl_19_func_800BB4D4();
    ovl_19_func_800BB108();
    var_s0 = IsScreenFadeComplete() == 1;
    if (func_8001FE6C() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
    }
}
