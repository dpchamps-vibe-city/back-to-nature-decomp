#include "common.h"

s32 ovl_11_func_800BCB5C(void);
void func_8001FE34(s32 arg0);
void func_8001FD10(void);
s32 func_8001FE6C(void);
s32 func_8001FBBC(s16 arg0);
s32 func_80020818(void);
s32 func_80020B80(s32 arg0, s32 arg1);
void func_8001FBF0(s16 arg0, s16 arg1);

void ovl_11_func_800BC868(void) {
    s32 temp_s1;

    temp_s1 = ovl_11_func_800BCB5C();
    if (!(((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x4000) && (((struct Ovl11SoundWork *) &D_8007AFF0)->selected != temp_s1)) {
        func_8001FE34(0x14);
        while (func_8001FE6C() != 0) {
            func_8001FD10();
        }
        func_8001FBBC(0);
        func_80020818();
        func_80020B80(2, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->previous = ((struct Ovl11SoundWork *) &D_8007AFF0)->header[7];
        func_8001FBF0((s16) temp_s1, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->selected = temp_s1;
        if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0xF80) {
            D_801217A4 = 0;
        } else {
            D_801217A4 = 1;
        }
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x80;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x800;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x100;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x200;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x400;
    }
}
