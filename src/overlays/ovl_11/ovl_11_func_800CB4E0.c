#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_11_func_800C70CC (Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
void func_80015840 (ObjectState *obj, s8 arg1);
void ovl_11_func_800C7010 (Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800CCFF8 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1, u8 arg2, s32 arg3);
void ovl_11_func_800C6E0C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6E8C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F0C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F6C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
u32 func_8001589C (SpriteSourceData *src);

s32 ovl_11_func_800CB4E0(void) {
    s32 *temp_s0;

    ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 1U, 0, -2);
    temp_s0 = &D_80071A00 - 0x1472;
    (*(s16 *) ((u8 *) temp_s0 + 0x51EE)) = 0;
    (*(s32 *) ((u8 *) temp_s0 + 0x5234)) = (s32) (((*(s32 *) ((u8 *) temp_s0 + 0x5234)) & 0xDFFFFFFF) | 0x04000000);
    return 0;
}
