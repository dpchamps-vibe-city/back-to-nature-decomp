#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012F158(void) {
    s16 *slotState;
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, gTextBankOffsets[0] + (s32) D_80052FC4);
    cardStates = gOvl15CardStates;
    slotState = gOvl15SaveSlotStatus;
    for (slot = 0; slot < 3; slot++, slotState++) {
        *slotState = 2;
        status = Ovl15ReadSaveSummary(gOvl15SelectedCard, (s16) slot, (Ovl15SaveSummary *) &gOvl15SaveSlotHeaders[slot]);
        if (status == 8) {
            gOvl15MenuState = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            gOvl15MenuState = 0x15;
            return;
        }
        if (status == 9) {
            fileStatus = Ovl15CheckSaveCapacity(gOvl15SelectedCard, (s16) slot);
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
        if ((u32) ((u16) cardStates[gOvl15SelectedCard] - 6) < 2U) {
            *slotState = 1;
        }
    }
    gOvl15MenuCursor = 0;
    gOvl15MenuState = 8;
}
