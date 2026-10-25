#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_id_predicates.h"

s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C83FC(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 temp_a2;

    if (Ovl11IsInteractionItemSetA(arg0->unk8A) != 0) {
        temp_a2 = arg0->unk38;
        if (temp_a2 == arg1[4]) {
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x5B, temp_a2 + 0x17, -2);
        }
    } else {
        RequestMessageWindow(2, 0x1A2);
    }
}
