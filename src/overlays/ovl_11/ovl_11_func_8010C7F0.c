#include "common.h"
#include "game_types.h"

void ovl_11_func_8010CF58(Ovl11CF58Object *arg0);
s32 ovl_11_func_8010CD80(Recon_ovl_11_func_8010D250_A0View *arg0);
void ovl_11_func_8010CAA8(s32 arg0);
s32 ovl_11_func_80107DEC(Ov11Timer *arg0);
s16 ovl_11_func_8010D0EC(M2C_90bbdd1382f9_Struct_8010D0EC *arg0);
void ovl_11_func_8010CA0C(M2C_ececd3ff98bf_Struct_801110CC *arg0);

void ovl_11_func_8010C7F0(void) {
    s32 var_s2;
    u8 *var_s0;
    struct Ovl11C7F0NearState *nearState;
    struct Ovl11UpdateWork *farState;
    s32 farOffset;
    struct Ovl11C7F0Flags *flags;

    if (!(((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x100)) {
        var_s0 = D_8007A638;
        for (var_s2 = 0; var_s2 < 7; var_s2++) {
            farState = (struct Ovl11UpdateWork *)&D_8007AFF0;
            farOffset = 0x20000;
            flags = (struct Ovl11C7F0Flags *)D_8006C838;
            nearState = (struct Ovl11C7F0NearState *)D_8009AFF0;
            if (!((*(u16 *) ((u8 *) var_s0 + 0xB8)) & 0x20)) {
                if (((struct Ovl11C7F0NearState *)((u8 *)farState + farOffset))->area != 0x28) {
                    (*(s16 *) ((u8 *) var_s0 + 0xBC)) = 1;
                }
                ovl_11_func_8010CF58((Ovl11CF58Object *) var_s0);
                if (!(flags->flagsC & 0x08000000)) {
                    if (!(flags->flagsC & 0x40000000)) {
                        if (((struct Ovl11C7F0NearState *)((u8 *)farState + farOffset))->area == (*(s16 *) ((u8 *) var_s0 + 0x30))) {
                            ovl_11_func_8010CD80((Recon_ovl_11_func_8010D250_A0View *) var_s0);
                            ovl_11_func_8010CAA8((s32) var_s0);
                            goto block_10;
                        }
                    } else {
                        ovl_11_func_8010CD80((Recon_ovl_11_func_8010D250_A0View *) var_s0);
                        ovl_11_func_8010CAA8((s32) var_s0);
                        goto block_10;
                    }
                } else {
block_10:
                    if (nearState->area == (*(s16 *) ((u8 *) var_s0 + 0x30))) {
                        if (!(flags->flagsC & 0x08000000)) {
                            ovl_11_func_80107DEC((Ov11Timer *) &var_s0[0xA8]);
                        }
                        ovl_11_func_8010D0EC((M2C_90bbdd1382f9_Struct_8010D0EC *) var_s0);
                        ovl_11_func_8010CA0C((M2C_ececd3ff98bf_Struct_801110CC *) var_s0);
                    }
                }
            }
            var_s0 += 0xF8;
        }
    }
}
