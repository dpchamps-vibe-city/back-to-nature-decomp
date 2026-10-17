#include "common.h"
#include "game_types.h"

void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);
void ovl_19_func_800BAFAC (s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3);

void ovl_19_func_800BA770(void) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 2; i++) {
        ovl_19_func_800BA834(&D_800BF4D0[i * 0x24]);
    }
    for (j = 0; j < 3; j++) {
        ovl_19_func_800BAB4C(&((Ovl19Func800BAB4CArg *) D_800BF560)[j]);
    }
    for (k = 0; k < 9; k++) {
        ovl_19_func_800BAFAC(D_8005E3C0->field_120, (ObjectState *) &D_800BF6D0,
                           &D_800BCF7C[k * 6], D_800BCF7C[k * 6 - 2]);
    }
}
