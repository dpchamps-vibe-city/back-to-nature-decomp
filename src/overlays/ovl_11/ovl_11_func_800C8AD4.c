#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_id_predicates.h"
#include "ovl11_primary_object_state_change.h"
s32 ovl_11_func_800D0F20(s32 arg0, s32 arg1);       /* static */

s32 ovl_11_func_800C8AD4(Recon_ovl_11_func_800C7270_A0View *arg0, s16 arg1, s16 arg2) {
    s16 temp_v1;
    s16 var_a1_2;
    s32 temp_v0;

    if (arg1 == 0) {
        RequestMessageWindow(2, 0x1E7);
        goto fail;
    }
    if (Ovl11IsInteractionItemSetA(*(s16 *) ((u8 *) arg0 + 0x8A)) == 0) {
fail:
        return 0;
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    var_a1_2 = 0;
    if (temp_v1 == 0x125) {
        var_a1_2 = 2;
    } else if (temp_v1 == 0xE8) {
        var_a1_2 = 1;
    }
    temp_v0 = ovl_11_func_800D0F20(arg2, var_a1_2);
    if (temp_v0 == 0) {
        arg0->unk6C &= 0xFFFF3FFF;
        Ovl11ChangeObjectStateDirect(arg0, 0x66, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return 1;
    } else {
        RequestMessageWindow(2, temp_v0);
        return 0;
    }
}
