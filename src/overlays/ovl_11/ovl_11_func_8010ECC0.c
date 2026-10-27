#include "common.h"
#include "game_types.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_action_sequences.h"

void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);

s32 ovl_11_func_8010ECC0(Ovl11E644Object *arg0) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 sp2A;
    s16 temp_v1;
    s32 temp_a0;
    s32 var_s1_2;

    var_s1_2 = 1;
    temp_a0 = arg0->unk34 | 0x800;
    arg0->unk34 = temp_a0;
    if ((*(s16 *) ((u8 *) arg0 + 0x28)) == -1) {
        Ovl11AdvanceActionMovement(arg0, 0xE6);
        if ((*(s16 *) ((u8 *) arg0 + 0x30)) == 6) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
    } else {
        temp_v1 = *(s16 *) ((u8 *) arg0 + 0x30);
        if (temp_v1 == 1) {
            arg0->unk34 = temp_a0 & 0xC3FFFFFF;
            if (Ovl11StepGridTargetActionAlternate((Ovl11SetFieldsView *) arg0, 1) != 0) {
                if (Ovl11FindCandidateGridCell(&sp28, &sp2A, 0, 0, 1) == 0) {
                    Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 3, &sp18.field_0, *(s16 *) ((u8 *) arg0 + 0x30));
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, sp18);
                    (*(s16 *) ((u8 *) arg0 + 0x28)) = -1;
                    arg0->unk34 |= 0x10000000;
                } else {
                    goto block_9;
                }
            }
        } else if ((temp_v1 != 6) || (arg0->unk34 = temp_a0 & 0xC3FFFFFF, (Ovl11StepGridTargetActionAlternate((Ovl11SetFieldsView *) arg0, 0) != 0))) {
block_9:
            var_s1_2 = 0;
        }
    }
    if (var_s1_2 == 1) {
        var_s1_2 = 0;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x26));
        (*(u16 *) ((u8 *) arg0 + 0xC0)) = (u16) (*(s16 *) ((u8 *) arg0 + 0x28));
        (*(u16 *) ((u8 *) arg0 + 0xC2)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x2A));
    } else {
        var_s1_2 = 1;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = 0U;
        arg0->unk34 &= ~0x800;
        (*(u16 *) ((u8 *) arg0 + 0xB8)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xB8)) | 0x40);
        ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 7);
        arg0->unk34 |= 0x3C000000;
    }
    return var_s1_2;
}
