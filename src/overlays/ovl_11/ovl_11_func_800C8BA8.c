#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_item_transfer_menu.h"
#include "ovl11_interaction_volume_query.h"
#include "selection_panel.h"

s32 ovl_11_func_800FBE4C(void);
s32 ovl_11_func_800C7AC0(Recon_ovl_11_func_800C7270_A0View *arg0);

s32 ovl_11_func_800C8BA8(Recon_ovl_11_func_800C7270_A0View *arg0, u16 arg1) {
    struct struct_8006C838_view *state;

    if (((struct Ovl11SelectionResetMode *) &D_8007AFF0)->mode != 0) {
        return 1;
    }
    state = (struct struct_8006C838_view *) D_8006C838;
    if (state->field_0C & 0x10000000) {
        return 0;
    }
    if (arg1 & 0x100) {
        Ovl11OpenSelectionPanel();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg1 & 0x800) {
        ovl_11_func_800FBE4C();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg0->unk6C & 0x40000108) {
        return 0;
    }
    if (arg1 & 0x10) {
        Ovl11OpenItemTransferMenu();
        state->field_0C |= 0x10000000;
        state->field_0C |= 0x08000000;
        return 1;
    }
    if (arg1 & 0x40) {
        if (Ovl11ProbeInteractionVolume(D_8009F78C, (Ovl11Func800BDCE8Arg1 *) &((Ovl11C7010Object *) arg0)->unk100, 1) != NULL) {
            return 1;
        }
        return ovl_11_func_800C7AC0(arg0);
    }
    return 0;
}
