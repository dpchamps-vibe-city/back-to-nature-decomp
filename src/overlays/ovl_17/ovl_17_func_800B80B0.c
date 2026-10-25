#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "screen_fade.h"


void SsSeqClose (short);
void SsSeqSetVol (short, short, short);
void SsVabClose (short);
void ovl_17_func_800BA808 (s32 arg0);
s32 func_8001FE6C (void);
void func_8001FAE8 (s32 arg0);
s32 func_8001FBBC (s16 arg0);
s32 func_80020B80 (s32 arg0, s32 arg1);
void ovl_17_func_800BB394 (s16 arg0);
void ovl_17_func_800BB3F4 (s16 arg0);
void func_80011EF0 (s32 arg0);
s32 func_80021484 (s32 arg0);
s32 func_80020414 (s32 arg0, s32 arg1);
void func_80015840(ObjectState *obj, u8 arg1);
void func_8001585C(ObjectState *obj, u8 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80015F80 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);

void ovl_17_func_800B80B0(void) {
    u8 var_s0 = 0;

    ovl_17_func_800BA808(0x800000);
    if (IsScreenFadeComplete() == 1) {
        var_s0++;
    }
    if (func_8001FE6C() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        if (D_800BDB10 != -1) {
            func_8001FAE8((s32) D_800BDB10);
        }
        func_8001FBBC(0);
        func_80020B80(2, 0);
        func_80020B80(1, 0);
        if (((Ovl17CompletionView *) D_8006C838)->mode == 1) {
            ovl_17_func_800BB394(1);
            ovl_17_func_800BB394(2);
            ovl_17_func_800BB394(4);
            ovl_17_func_800BB394(6);
            ovl_17_func_800BB394(0xF);
            ovl_17_func_800BB394(0x10);
            ovl_17_func_800BB394(0x11);
            ovl_17_func_800BB394(0x13);
            ovl_17_func_800BB394(0x15);
            ovl_17_func_800BB394(0x16);
            ovl_17_func_800BB394(0x1A);
            ovl_17_func_800BB394(0x1C);
            ovl_17_func_800BB3F4(3);
            ovl_17_func_800BB3F4(7);
            ovl_17_func_800BB3F4(0x14);
        }
        ((Ovl17CompletionView *) D_8006C838)->state = 0xFF;
        ((Ovl17CompletionView *) D_8006C838)->flags |= 0x80000;
        func_80011EF0(6);
    }
}
