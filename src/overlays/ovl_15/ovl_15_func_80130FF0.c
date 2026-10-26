#include "common.h"
#include "ovl15_save_menu_ui.h"
#include "psyq/memory.h"
#include "ovl15_save_image.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_15_func_8012E15C(void);

void ovl_15_func_80130FF0(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_800533C8 + gTextBankOffsets[0]));
    D_80137584 = Ovl15WaitForMenuButton(0x13, 0);
    if (D_8013759A >= 0x5B) {
        D_80137584 = 0;
    }
    if (D_80137584 != 0x13) {
        Ovl15RemoveSaveChecksumSlots();
        Ovl15RestoreSaveImage();
        ovl_15_func_8012E15C();
        D_80137586 = 1;
    }
}
