#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

extern u8 D_8005346A[];
extern s8 gOvl15MenuState;
extern s16 gOvl15SelectedCard;
extern s16 gOvl15SelectedSlot;
extern s16 gOvl15MenuStateTimer;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012F8DC(void) {
    s32 var_v0;

    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_8005346A);
    if (gOvl15MenuStateTimer >= 2) {
        var_v0 = Ovl15CreateSaveFile(gOvl15SelectedCard, gOvl15SelectedSlot);
        if (var_v0 == 0) {
            gOvl15MenuState = 0x13;
        } else if (var_v0 == 0xB) {
            gOvl15MenuState = 0x11;
        } else if (var_v0 == 4) {
            gOvl15MenuState = 0x15;
        } else {
            gOvl15MenuState = 0x10;
        }
    }
}
