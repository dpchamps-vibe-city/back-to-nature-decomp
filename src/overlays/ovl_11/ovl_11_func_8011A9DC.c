#include "common.h"

extern s32 D_8005182A;
extern s32 gOvl11InteractionDrawMode;

typedef struct {
    /* 0x00 */ s32 field_0;
    /* 0x04 */ s32 field_4;
    /* 0x08 */ s16 field_8;
    /* 0x0A */ s16 field_A;
} ovl_11_A9DC_arg0;

s32 ovl_11_func_800D6014(u16 arg0);

s32 ovl_11_func_8011A9DC(void) {
    ovl_11_A9DC_arg0 *p;

    p = (ovl_11_A9DC_arg0 *)&gOvl11QuantityPrompt;
    p->field_0 = D_80054BC0[0] + (s32)&D_8005182A;
    p->field_4 = D_80054BC0[0] + (s32)&D_8005182A - 0x10;
    p->field_A = ovl_11_func_800D6014(0x6C);
    p->field_8 = 1;
    gOvl11InteractionDrawMode = 0;
    return 1;
}
