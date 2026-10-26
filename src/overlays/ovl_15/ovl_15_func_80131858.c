#include "common.h"
#include "game_types.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80136288(s16 arg0, s16 arg1, Ovl15SaveSummary *arg2);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void ovl_15_func_80131858(void) {
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    slot = 0;
    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    for (; slot < 3; slot++) {
        gOvl15SaveSlotStatus[slot] = 2;
        status = ovl_15_func_80136288(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x1E;
            return;
        }
        if (status == 9) {
            fileStatus = ovl_15_func_80135FF0(gOvl15SelectedCard, (s16) slot);
            if (fileStatus == 0) {
                gOvl15SaveSlotStatus[slot] = 1;
            } else if (fileStatus == 0xF) {
                gOvl15SaveSlotStatus[slot] = 3;
            } else {
                gOvl15SaveSlotStatus[slot] = 2;
            }
        } else if (status == 0) {
            gOvl15SaveSlotStatus[slot] = 0;
        }
        if ((u32) ((u16) cardStates[gOvl15SelectedCard] - 6) < 2U) {
            gOvl15SaveSlotStatus[slot] = 1;
        }
    }
    if (gOvl15SaveSlotStatus[0] != 0 && gOvl15SaveSlotStatus[1] != 0 && gOvl15SaveSlotStatus[2] != 0) {
        gOvl15MenuState = 8;
    } else {
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 9;
    }
}
