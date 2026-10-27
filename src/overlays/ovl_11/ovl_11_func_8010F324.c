#include "common.h"
#include "game_types.h"
#include "ovl11_action_sequences.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_8010E644(Ovl11E644Object *arg0);
s32 ovl_11_func_8010CE80(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);

s32 ovl_11_func_8010F324(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_s3;
    u16 temp_a0;
    Ovl11PaddedVec3 position;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    var_s3 = 0;
    switch (temp_v1) {
    case 0:
        *(s16 *) ((u8 *) arg0 + 0x28) = 1;
        *(s16 *) ((u8 *) arg0 + 0x22) = 0;
        arg0->field_34 |= 0x800;
        position.field_0 = -0xCE4;
        position.field_4 = 0;
        position.field_8 = 0x960;
        ovl_11_func_800D05D0(arg0, position);
        *(s32 *) ((u8 *) arg0 + 0x38) = -0xCE4;
        *(s32 *) ((u8 *) arg0 + 0x3C) = 0;
        arg0->field_30 = 1;
        *(s32 *) ((u8 *) arg0 + 0x40) = 0x16A8 - (*(u16 *) ((u8 *) arg0 + 0xAC) * 0xC8);
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0xE6) == 0) {
            break;
        }
        *(s16 *) ((u8 *) arg0 + 0x28) = 2;
        /* fall through */
    case 2:
        temp_a0 = *(u16 *) ((u8 *) arg0 + 0xB8);
        arg0->field_34 &= ~0x800;
        if (temp_a0 & 1) {
            ovl_11_func_8010E644((Ovl11E644Object *) arg0);
            ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 8);
        } else if (temp_a0 & 2) {
            ovl_11_func_8010E644((Ovl11E644Object *) arg0);
            ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 9);
        } else if (temp_a0 & 4) {
            *(s16 *) ((u8 *) arg0 + 0x22) = 3;
            ovl_11_func_8010E644((Ovl11E644Object *) arg0);
            ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 0xA);
        } else {
            ovl_11_func_8010E644((Ovl11E644Object *) arg0);
            ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 7);
        }
        var_s3 = 1;
        break;
    }
    return var_s3;
}
