#include "common.h"

s32 GetVal8005E5B4(void);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_80117674(void) {
    if (GetVal8005E5B4() == 5) {
        return ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x28, 0x83, 0x48);
    }
    return 0;
}
