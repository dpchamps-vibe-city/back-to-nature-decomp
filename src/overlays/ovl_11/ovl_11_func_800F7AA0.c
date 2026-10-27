#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_selection_texture_transfer.h"

s32 ovl_11_func_800F7D34(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F8224(s16 *arg0, s16 arg1);
s16 *ovl_11_func_800F82FC(s32 arg0, s32 arg1);

s32 ovl_11_func_800F7AA0(void) {
    s16 temp_s0;
    s32 temp_s0_2;
    s32 temp_v1;

    temp_s0 = gOvl11MenuGridMessageState;
    if ((temp_s0 == 0xFF) && (IsMessageWindowStatusFinal() != 0)) {
        temp_v1 = *(s32 *) ((u8 *) gPad0Input + 8);
        if (temp_v1 & 0x10) {
            if (*ovl_11_func_800F82FC((s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridPageIndex) != 0) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                gOvl11MenuGridMessageState = 0;
                goto block_32;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x20) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                CloseMessageWindow();
                gOvl11InteractionPhase = 0xFE;
                PlaySoundEffect(3);
                return 0;
            }
            PlaySoundEffect(1);
            gOvl11MenuGridSelectedSlot = temp_s0;
            goto block_32;
        }
        if (temp_v1 & 0x40) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = (s16) (u16) gOvl11MenuGridCursorSlot;
                gOvl11MenuGridSelectedPageIndex = (s16) (u16) gOvl11MenuGridPageIndex;
                goto block_32;
            }
            gOvl11MenuGridSavedCursorSlot = (u16) gOvl11MenuGridCursorSlot;
            if ((gOvl11MenuGridSelectedSlot == (s16) (u16) gOvl11MenuGridCursorSlot) && ((gOvl11MenuGridSelectedPageIndex == gOvl11MenuGridPageIndex) || (gOvl11MenuGridSelectedSlot < 9))) {
                gOvl11MenuGridCursorSlot = 0;
            }
            Ovl11ReleaseUnusedSelectionTextures();
            temp_s0_2 = ovl_11_func_800F7D34((s32) gOvl11MenuGridSelectedSlot, (s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridSelectedPageIndex, (s32) gOvl11MenuGridPageIndex);
            Ovl11ReleaseUnusedSelectionTextures();
            gOvl11MenuGridCursorSlot = (s16) gOvl11MenuGridSavedCursorSlot;
            if (temp_s0_2 == 1) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = 0xFF;
                return 1;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x100) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                switch (gOvl11InteractionPhase) {
                case 0:
                    ovl_11_func_800F8224((s16 *) &gItemOverflowSlotBase, 0x40);
                    break;
                case 2:
                    ovl_11_func_800F8224((s16 *) D_80070D42_slots, 0x40);
                    break;
                case 1:
                    ovl_11_func_800F8224((s16 *) D_80071042, 0x40);
                    break;
                }
                goto block_32;
            }
            PlaySoundEffect(2);
            goto block_32;
        }
        goto block_32;
    }
block_32:
    return 0;
}
