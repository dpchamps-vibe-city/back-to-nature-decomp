#include "common.h"

s32 ovl_11_func_800C0824(void);
s32 ovl_11_func_8010D314(u8 *arg0);

s32 ovl_11_func_8010CAD4(u8 *arg0) {
    if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_E4C8 - 3) >= 2U) {
        if (ovl_11_func_800C0824() != 1) {
            return 0;
        }
        if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
            return 0;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB8)) & 0x40) {
            return 0;
        }
        ovl_11_func_8010D314(arg0);
    }
    return 0;
}
