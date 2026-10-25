#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "message_window.h"


void func_80017A38 (s16 arg0, s16 arg1);
void ovl_11_func_800FED0C (void);
void ovl_11_func_800BCF28 (void);
void ovl_11_func_800FF96C (void);
void ovl_11_func_800FEFEC (void);
void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SetVal8005E2BC (s32 arg0);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800FECB8(void) {
    if (D_801273B0 != 0) {
        DrawScreenSubtractOverlay(0xA000U, 0xA000U, 0xA000U);
        func_80017A38(0, 2);
        ovl_11_func_800FED0C();
        func_80017A38(0, 0);
    }
}
