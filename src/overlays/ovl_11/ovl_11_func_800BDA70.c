#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

void func_80014BCC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8001719C(u8 *arg0);

extern s32 D_8009CBF8;

void ovl_11_func_800BDA70(void) {
    s32 s0;

    DrawSync(0);
    if ((s32)gActiveRenderContext == (s32)&gRenderContexts) {
        s0 = *(s32 *)((u8 *)&gRenderContexts + 0x258);
    } else {
        s0 = *(s32 *)((u8 *)&gRenderContexts + 0x124);
    }
    func_80014BCC(0, 0x7F000, 0x4800, 0, s0);
    memcpy(&D_8009CBF8, (void *)s0, 0x770);
    func_8001719C((u8 *)s0 + 0x770);
}
