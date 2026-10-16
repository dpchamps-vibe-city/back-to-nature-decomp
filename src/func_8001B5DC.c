#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"

void func_8001B6A0(void);

/* GP-relative fog and color state shared with the adjacent setters. */
s32 D_8005E2D8;
u8 D_8005E2DC;
u8 D_8005E2DD;
u8 D_8005E2DE;
u8 D_8005E2E0;
u8 D_8005E2E1;
u8 D_8005E2E2;

void func_8001B5DC(void) {
    func_8001B6A0();
    SetFogNear(D_8005E2D8, 0x3E8);
    gte_SetBackColor(D_8005E2DC, D_8005E2DD, D_8005E2DE);
    SetFarColor(D_8005E2E0, D_8005E2E1, D_8005E2E2);
    gte_SetColorMatrix(&D_80061E68);
    gte_SetLightMatrix(&D_80061E48);
}
