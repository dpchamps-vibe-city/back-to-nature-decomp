#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"


void func_80017A38 (s16 arg0, s16 arg1);
void ovl_11_func_800FED0C (void);
s32 GetVal8005E5B4 (void);
void func_800226F0 (void);
void ovl_11_func_800BCF28 (void);
void ovl_11_func_800FF96C (void);
s32 func_800226B0 (void);
void ovl_11_func_800FEFEC (void);
void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SetVal8005E334 (s32 arg0);
void SetVal8005E2BC (s32 arg0);
s32 func_8002261C (s32 arg0, s32 arg1);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);
void func_80022738 (void);
s32 func_8001FABC (s16 arg0);

void ovl_11_func_800FECB8(void) {
    if (D_801273B0 != 0) {
        DrawScreenSubtractOverlay(0xA000U, 0xA000U, 0xA000U);
        func_80017A38(0, 2);
        ovl_11_func_800FED0C();
        func_80017A38(0, 0);
    }
}
