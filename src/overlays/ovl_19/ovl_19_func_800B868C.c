#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_19_func_800BA770 (void);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);
void ovl_19_func_800BAFAC (s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3);
void func_80015840 (ObjectState *obj, s8 arg1);
void func_8001585C (ObjectState *obj, s8 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
s32 ovl_19_func_800BADAC (s16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BB08C (s32 arg0, s32 arg1);

void ovl_19_func_800BB108();                        /* static */

void ovl_19_func_800B868C(void) {
    ovl_19_func_800BA770();
    ovl_19_func_800BB108();
    if ((*(s32 *) ((u8 *) D_8005E3A8 + 8)) & 0x60) {
        D_800BF4C4 = 1;
    }
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
