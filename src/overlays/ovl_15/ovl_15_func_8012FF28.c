#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

extern u8 D_8005346A[];
extern s16 gOvl15SelectedSlot;

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012FF28(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32) D_8005346A);
    temp_v0 = Ovl15WriteSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 2:
    case 5:
        gOvl15MenuState = 0x10;
        break;
    case 4:
        gOvl15MenuState = 0x15;
        break;
    case 0:
        gOvl15MenuState = 0x18;
        break;
    }
}
