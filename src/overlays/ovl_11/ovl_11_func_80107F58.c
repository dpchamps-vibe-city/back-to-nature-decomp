#include "common.h"

s32 GetVal8005E5B4(void);
void func_80022738(void);
void ovl_11_func_800BCF28(void);
void ovl_11_func_80108104(void);
void ovl_11_func_80108118(void);
s32 ovl_11_func_80108214(void);
void ovl_11_func_8010860C(void);
s32 ovl_11_func_801086CC(void);
s32 ovl_11_func_80108738(void);
s32 ovl_11_func_80108828(void);
void ovl_11_func_80108864(void);
void ovl_11_func_801088E4(void);
void ovl_11_func_80108930(void);
void ovl_11_func_801089DC(void);
void ovl_11_func_80108B8C(void);

void ovl_11_func_80107F58(void) {
    char *far_base;
    s16 *p;
    s32 s0;
    s32 v;

    far_base = (char *)&D_8007AFF0;
    p = *(s16 **)(far_base + 0x25388);
    if (p[1] != 2) {
        return;
    }
    switch (D_8012D044) {
    case 0:
        break;
    case 1:
        ovl_11_func_80108118();
        ovl_11_func_80108864();
        if (ovl_11_func_80108214() == 1) {
            v = 3;
        } else {
            ovl_11_func_80108738();
            v = 2;
        }
        D_8012D044 = v;
        break;
    case 2:
        s0 = GetVal8005E5B4();
        if (s0 == 5) {
            if (ovl_11_func_801086CC() == 1) {
                func_80022738();
            }
        } else if (s0 == 2) {
            D_8012D044 = 3;
        }
        break;
    case 3:
        func_80022738();
        ovl_11_func_801088E4();
        ovl_11_func_80108828();
        v = 4;
        D_8012D044 = v;
        break;
    case 4:
        s0 = GetVal8005E5B4();
        if (s0 == 5) {
            ovl_11_func_80108B8C();
        }
        if (s0 == 2) {
            D_8012D044 = 5;
        }
        if (s0 != 14) {
            ovl_11_func_8010860C();
        }
        ovl_11_func_80108930();
        break;
    case 5:
        ovl_11_func_801089DC();
        ovl_11_func_80108104();
        ovl_11_func_800BCF28();
        break;
    case 6:
        if (GetVal8005E5B4() == 2) {
            v = 5;
            D_8012D044 = v;
        }
        break;
    }
}
