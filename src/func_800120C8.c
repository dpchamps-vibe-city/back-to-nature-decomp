#include "common.h"
#include "psyq/types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"

/* Globals accessed GP-relatively (this TU defines them) */
s32 D_8005E3A4;
s32 D_8005E3B0;
s32 *gFrameOrderingTable;
s32 D_8005E3BC;
struct_8005E3C0 *gActiveRenderContext;

void func_800120C8(void) {
    GsSetWorkBase((PACKET *)gActiveRenderContext->packetWorkBase);
    __asm__ volatile("" ::: "memory");
    {
        s32 temp = D_8005E3A4 * 0x3000;
        temp += 0x38E50;
        D_8005E3BC = D_8005E3B0 + temp;
    }
    ClearOTagR(gFrameOrderingTable, 2);

    if (gActiveRenderContext->field_D8) {
        ClearOTagR(gActiveRenderContext->field_D8, gActiveRenderContext->field_EC);
    }
    if (gActiveRenderContext->field_DC) {
        ClearOTagR(gActiveRenderContext->field_DC, gActiveRenderContext->field_F0);
    }
    if (gActiveRenderContext->field_E0) {
        ClearOTagR(gActiveRenderContext->field_E0, gActiveRenderContext->field_F4);
    }
    if (gActiveRenderContext->field_E4) {
        ClearOTagR(gActiveRenderContext->field_E4, gActiveRenderContext->field_F8);
    }
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    gActiveRenderContext->field_118 = gActiveRenderContext->field_124;
}
