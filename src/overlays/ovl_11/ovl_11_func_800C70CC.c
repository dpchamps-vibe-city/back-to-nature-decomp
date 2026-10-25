#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

/* This state-switch family passes unsigned animation bytes; the callee
 * stores only the low byte (as in 800C7010 and 800C7270). */
void ovl_11_func_800C7010(Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
/* The target forwards the full incoming words in a2/a3 unchanged. */
void ovl_11_func_800CCFF8(Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800C6E0C(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6E8C(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F0C(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F6C(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800CA5D4(Ovl11C7010Object *arg0);
void ovl_11_func_8010C1FC(Ovl11C7010Object *arg0);

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u16 previousState;

    arg0->unk6C = (arg0->unk6C | 0x20000000) & ~0x04000000;
    previousState = arg0->unk3C;
    arg0->unk3C = arg1;
    *(s16 *)((u8 *)arg0 + 0x1C) = (s16)arg3;
    *(u16 *)((u8 *)arg0 + 0x3E) = previousState;
    if (arg0->unk6C & 0x40000000) {
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        ovl_11_func_800C7010(arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        return;
    }
    if (arg0->unk6C & 0x800) {
        ovl_11_func_800CCFF8((Recon_ovl_11_func_800CCFF8_A0View *)arg0, 1, arg2, arg3);
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        arg0->unk6C &= ~0x800;
        return;
    }
    if (!(arg0->unk6C & 0x1000)) {
        SelectSpriteAnimation((ObjectState *)&arg0->sprite, (u8)arg2);
        if (arg0->unk6C & 0x10000000) {
            ovl_11_func_800C6E0C((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        }
        if (arg0->unk6C & 0x08000000) {
            ovl_11_func_800C6E8C((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
        }
        if (!(arg0->unk6C & 0x18000000)) {
            if (arg0->unk6C & 0x200000) {
                ovl_11_func_800C6F0C((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
            }
        }
        if ((arg0->unk6C & 0x100) && (arg0->unk3C != 0x39)) {
            ovl_11_func_800C6F6C((Recon_ovl_11_func_800C6F0C_A0View *)arg0, (Recon_ovl_11_func_800C6F0C_A1View *)&arg2);
            if (!(arg0->unk6C & 0x80)) {
                ovl_11_func_8010C1FC(arg0);
            } else {
                ovl_11_func_800CA5D4(arg0);
            }
        }
        if (GetSpriteFrameControlHighBit(&arg0->sprite) != 0) {
            arg0->sprite.field_5 = *(u8 *)((u8 *)arg0 + 0x38);
        }
    }
}
