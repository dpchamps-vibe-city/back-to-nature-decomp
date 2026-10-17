#include "common.h"
#include "game_types.h"

/* The callee stores only the low byte; callers in this state-switch run
 * pass the unsigned byte view of the animation selection. */
void func_80015840(ObjectState *obj, u8 arg1);
u32 func_8001589C(SpriteSourceData *src);
void ovl_11_func_8010C1FC(Ovl11C7010Object *arg0);

void ovl_11_func_800C7010(Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    SpriteSourceData *sprite;
    s32 flags;

    flags = arg0->unk6C;
    if (flags & 0x100) {
        if (arg0->unk3C != 0x39) {
            arg1->unk0.unk0_w = arg0->unk38 + 0x3F;
            func_80015840((ObjectState *)&arg0->sprite, arg1->unk0.unk0_b);
            ovl_11_func_8010C1FC(arg0);
        }
    } else {
        if (flags & 0x18000000) {
            arg1->unk0.unk0_w = 0x1C;
        } else {
            arg1->unk0.unk0_w = 0xA;
        }
        sprite = &arg0->sprite;
        func_80015840((ObjectState *)sprite, arg1->unk0.unk0_b);
        if (func_8001589C(sprite) != 0) {
            arg0->sprite.field_5 = arg0->unk38;
        }
    }
}
