#include "common.h"
#include "game_types.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80136288(s16 arg0, s16 arg1, Ovl15SaveSummary *arg2);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void ovl_15_func_8012F158(void) {
    s16 *slotState;
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = D_80137594;
    slotState = gOvl15SaveSlotStatus;
    for (slot = 0; slot < 3; slot++, slotState++) {
        *slotState = 2;
        status = ovl_15_func_80136288(D_8013758C, (s16) slot, (Ovl15SaveSummary *) &D_801376E0_headers[slot]);
        if (status == 8) {
            D_80137584 = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            D_80137584 = 0x15;
            return;
        }
        if (status == 9) {
            fileStatus = ovl_15_func_80135FF0(D_8013758C, (s16) slot);
            if (fileStatus == 0) {
                *slotState = 1;
            } else if (fileStatus == 0xF) {
                *slotState = 3;
            } else {
                *slotState = 2;
            }
        } else if (status == 0) {
            *slotState = 0;
        }
        if ((u32) ((u16) cardStates[D_8013758C] - 6) < 2U) {
            *slotState = 1;
        }
    }
    gOvl15MenuCursor = 0;
    D_80137584 = 8;
}
