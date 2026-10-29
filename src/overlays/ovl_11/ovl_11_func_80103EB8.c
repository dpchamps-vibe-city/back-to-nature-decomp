#include "common.h"
#include "globals_override.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void ovl_11_func_80103EB8(void) {
    s32 temp_s3;
    s32 temp_s3_2;

    temp_s3 = gActiveRenderContext->field_D8;
    temp_s3_2 = temp_s3 + 0x58;
    DrawMenuPanel((u32 *)(temp_s3 + 0x5C), 0, 0xE3, 0x10, 0x47, 0x1E);
    DrawTextLine(temp_s3_2, (s32)((u8 *)D_800517C6 + gTextBankOffsets[1]), 0xE5, 0x12);
    CopyTextString(WriteDecimalText(D_8012CF20, D_8012CF30, 7),
                  (u16 *)((u8 *)D_800517C6 + 0x42 + gTextBankOffsets[1]));
    DrawTextLine(temp_s3_2, (s32)D_8012CF30, 0xE5, 0x20);
}
