#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"


void ovl_11_func_800CD08C (Recon_ovl_11_func_800CD08C_A0View *arg0);
void ovl_11_func_800C9D38 (void);
s32 ovl_11_func_800CB4E0 (void);
void func_8001AF70 (u16 arg0, u16 arg1);
s32 func_8001AF44 (u32 arg0);
s32 ovl_11_func_800D622C (s8 arg0, s8 arg1, s16 arg2, s32 arg3);
s32 ovl_11_func_800CD578 (s32 arg0);
s32 ovl_11_func_800CD534 (s32 arg0);
s32 ovl_11_func_800CD734 (Ovl11C7010Object *arg0, s32 arg1);
s32 ovl_11_func_800CDE3C (u8 *arg0);
s32 ovl_11_func_800CDF4C (u8 *arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800BD168 (void);
void ovl_11_func_800DE3A0 (void);

void ovl_11_func_800CCCC0(void) {
    struct Ovl11CCCC0State *temp_s0;
    Ovl11C7010Object *player = (Ovl11C7010Object *)&D_80071A00;
    s32 temp_s1;
    s8 temp_s0_2;

    if (player->unk6C & 0x1000) {
        ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *)player);
    }
    player->unk22 = -1;
    temp_s0 = (struct Ovl11CCCC0State *)((u8 *)player - 0x51C8);
    player->unk1A = 0;
    player->unk18 = 0;
    player->unk24 = 0;
    player->unk38 = 0;
    player->unk6C &= 0xBD9B22A4;
    temp_s0->status = (s32) (temp_s0->status & 0xFFFE3FFF);
    ovl_11_func_800C9D38();
    ovl_11_func_800CB4E0();
    func_8001AF70(0x3FU, 0U);
    func_8001AF70(0x40U, 0U);
    if (!(temp_s0->flags & 4)) {
        if (func_8001AF44(0x47U) != 1) {
            ovl_11_func_800D622C(0, 0xA, 0, 0);
        } else {
            temp_s1 = temp_s0->angle + 0x5A0;
            temp_s1 -= player->unk2C;
            temp_s0_2 = ovl_11_func_800CD578(temp_s1);
            ovl_11_func_800D622C(temp_s0_2, ovl_11_func_800CD534(temp_s1), 0, 0);
        }
    } else {
        if ((u32) ((u16) player->unk2C - 0x168) < 0x2D0U) {
            ovl_11_func_800D622C(0x64, -0x19, 0, 0);
            ovl_11_func_800D622C(0x64, 0, 0, 0);
        } else {
            ovl_11_func_800D622C(0x32, -0x16, 0, 0);
        }
        ((struct Ovl11CCCC0State *)D_8006C838)->flags = (s32) (((struct Ovl11CCCC0State *)D_8006C838)->flags & ~4);
    }
    player->unk36 = 0;
    ovl_11_func_800CD734(player, 1);
    player->unk2A = (u16) ((struct Ovl11CCCC0State *)D_8006C838)->angle;
    if (((struct Ovl11CCCC0State *)D_8006C838)->time >= 8) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter = (u16) ((u16) ((struct Ovl11CCCC0State *)D_8006C838)->time + (((struct Ovl11CCCC0State *)D_8006C838)->counter - 8));
    }
    if ((s16) ((struct Ovl11CCCC0State *)D_8006C838)->counter >= 0x10) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter = 0xFU;
    }
    ((struct Ovl11CCCC0State *)D_8006C838)->pending = 0;
    func_8001AF70(0xAU, 0U);
    func_8001AF70(0x76U, 0U);
    ovl_11_func_800CDE3C((u8 *)player);
    ovl_11_func_800CDF4C((u8 *)player);
    func_8001AF70(0xA0U, 0U);
}
