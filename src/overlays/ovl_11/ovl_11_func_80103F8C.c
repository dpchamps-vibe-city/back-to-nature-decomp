#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void ovl_11_func_80104148 (s32 arg0);

void ovl_11_func_80103F8C(void) {
    s32 var_s2;
    s32 temp_s3;
    s32 temp_t0;
    u16 var_a1;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s3 = temp_t0 + 0x58;
    DrawMenuPanel((u32 *)(temp_t0 + 0x5C), 0, 0x20, 0x30, 0x101, 0x72);
    var_a1 = 2;
    if (((Ovl11CardView *) D_8006C838)->selector != 3) {
        var_a1 = (u16) ((Ovl11CardView *) D_8006C838)->selector;
    }
    *(u16 *)WriteDecimalText(var_a1 + 1, D_8012CF30, 1) = 0xFFFF;
    SetTextSourcePointer(0, (s32) D_8012CF30);
    DrawTextLine(temp_s3, (s32)(D_80051C0E + gTextBankOffsets[0]), 0x22, 0x32);
    DrawTextLine(temp_s3, (s32)(D_80051C0E + 0x18 + gTextBankOffsets[0]), 0xDF, 0x94);
    var_s2 = 0;
    do {
        ovl_11_func_80104148((s16) var_s2);
        var_s2 += 1;
    } while (var_s2 < 6);
}
