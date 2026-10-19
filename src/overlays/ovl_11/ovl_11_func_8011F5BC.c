#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void ovl_11_func_8011F608 (void);
s32 func_8001FBBC (s16 arg0);
void func_800132F0 (s32 arg0, s32 arg1, s32 arg2);
void func_8001FE34 (s32 arg0);
s32 func_80013394 (void);
s32 func_8001FE6C (void);
void ovl_11_func_8011F9EC (s32 *arg0);
void func_8001FE00 (s32 arg0);
void func_80017A08 (s32 arg0, s32 arg1);
s32 func_8002261C (s32 arg0, s32 arg1);
s32 GetVal8005E5B4 (void);
s32 GetVal8005E5B8 (void);
s32 ovl_11_func_8011FB80 (s16 arg0);
s32 func_8001FB30 (s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800BC9AC (void);
void func_800132B8 (s32 arg0, s32 arg1, s32 arg2);
void func_80015114 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void func_80021B90 (s32 arg0);
s32 func_80021B64 (void);
void func_8001328C (void);
void ovl_11_func_8011F958 (void);

void ovl_11_func_8011F5BC(void) {
    u32 temp_s0;

    if (D_80128540 != 0) {
        temp_s0 = func_80017A64();
        func_80017A48(3U);
        ovl_11_func_8011F608();
        func_80017A48(temp_s0);
    }
}
