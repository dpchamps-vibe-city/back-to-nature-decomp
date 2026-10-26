#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_halfword_triplets.h"
#include "map_texture_camera.h"

s32 ovl_11_func_800CDD7C(Ovl11C7010Object *arg0);
/* No complete prototype: this call supplies only the object and zero.
 * The callee's trailing parameters are unused in its matched C; the
 * transitive arity bound also includes FntPrint's variadic argument saves. */
void ovl_11_func_800CCFF8();
/* The callee stores only the low byte of the animation selector. */
void ovl_11_func_800C7650(void);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C7524(Ovl11C7010Object *arg0, u16 *arg1) {
    switch ((s16) *arg1) {
    case 0x38:
    case 0x39:
        ovl_11_func_800CDD7C(arg0);
        ovl_11_func_800CCFF8((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
        SelectSpriteAnimation((ObjectState *) &arg0->sprite, (u8) arg0->unk38);
        return 1;
    case 0x21:
        ovl_11_func_800C70CC(arg0, 0x32, 0x47, 0xA);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x1E:
        ovl_11_func_800C7650();
        arg0->unk6C |= 0x1800;
        ovl_11_func_800C70CC(arg0, 0x51, arg0->unk38, -2);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x1F:
    case 0x20:
        ovl_11_func_800C7650();
        arg0->unk6C |= 0x1800;
        ovl_11_func_800C70CC(arg0, 0x55, arg0->unk38, -2);
        Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + 0xF0), (u16 *) &arg0->unk84);
        arg0->unk6C |= 0x8000;
        return 1;
    case 0x12D:
        arg0->unk6C |= 0x400;
        Ovl11EnterMapCameraMode();
        return 1;
    default:
        return 0;
    }
}
