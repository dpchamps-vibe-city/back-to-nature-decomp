#include "common.h"
#include "sprite_grid.h"

s32 ovl_11_func_800DD9F0(void);
s32 ovl_11_func_800DDB4C(void);
s32 func_8001FABC(s16 arg0);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226B0(void);
s32 ovl_11_func_800F77F0(s32 arg0);
void func_80022738(void);
s32 GetVal8005E5B4(void);
void func_800226F0(void);
void ovl_11_func_800BCF28(void);
void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_800F69FC(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_800F787C(void);

void ovl_11_func_800F67E0(void) {
    s32 (*check)(void);
    s32 id;
    s32 next;
    u32 saved;

    if (D_80126E40 != 0xFF) {
        if (D_80126E40 == 6 || D_80126E40 == 7) {
            if (D_80126E40 == 6) {
                check = ovl_11_func_800DD9F0;
                id = 0x378;
                next = 3;
            } else {
                check = ovl_11_func_800DDB4C;
                id = 0x379;
                next = 5;
            }
            if (check()) {
                ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
                func_8001FABC(3);
                SetVal8005E334(0);
                SetVal8005E2BC(0);
                func_8002261C(3, id);
                D_80126E40 = next;
            }
        } else {
            DrawScrollingSpriteGrid(gActiveRenderContext->field_D8 + 0x6C);
            if (D_80126E40 == 3 || D_80126E40 == 5 || D_80126E40 == 4) {
                if (D_80126E40 == 3) {
                    id = 0;
                    next = 0;
                } else if (D_80126E40 == 5) {
                    id = 1;
                    next = 2;
                } else {
                    id = 2;
                    next = 1;
                }
                if (func_800226B0()) {
                    if (ovl_11_func_800F77F0(id) == 0) {
                        func_80022738();
                        D_80126E40 = next;
                    }
                }
            } else if (D_80126E40 == 0xFE) {
                if (GetVal8005E5B4() == 2) {
                    func_800226F0();
                    D_80126E40 = 0xFF;
                    ovl_11_func_800BCF28();
                    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x40000;
                    ((struct struct_8006C838_800E8F78 *)&D_8006C838)->field_5234 |= 0x2000;
                    func_8001AF70(0x71, 0);
                    func_8001AF70(0x70, 0);
                }
            } else {
                ovl_11_func_800F69FC();
                saved = func_80017A64();
                func_80017A48(3);
                ovl_11_func_800F787C();
                func_80017A48(saved);
            }
        }
    }
}
