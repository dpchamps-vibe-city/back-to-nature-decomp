#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Globals accessed GP-relatively (this TU defines them) */
struct_8005E3C0 *gActiveRenderContext;

void func_800121D4(void) {
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
