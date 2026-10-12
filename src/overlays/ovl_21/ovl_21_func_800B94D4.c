#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_21_func_800BA4C0 (void);
s32 func_80013394 (void);
void ovl_21_func_800B8320 (void);
void func_800132B8 (s32 arg0, s32 arg1, s32 arg2);
s32 func_8002261C (s32 arg0, s32 arg1);
void ovl_21_func_800B836C (void);
void ovl_21_func_800BA510 (void);
void ovl_21_func_800BA5D0 (void);
void ovl_21_func_800BA670 (void);

void ovl_21_func_800B94D4(void) {
    ovl_21_func_800BA4C0();
    if (func_80013394() == 1) {
        ovl_21_func_800B8320();
        func_800132B8(0xA, 0, 2);
        func_8002261C(4, 0x28);
        *D_800C0448 = 0xE;
    }
}
