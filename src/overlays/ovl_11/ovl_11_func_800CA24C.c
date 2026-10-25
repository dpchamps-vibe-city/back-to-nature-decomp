#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

s32 ovl_11_func_800DD1D0 (s16 *arg0);
s32 func_8001DFD4 (s32 *arg0, SVECTOR *arg1);
u32 ovl_11_func_800CDEEC (u8 *arg0);
void ovl_11_func_800C9F20 (Ovl11C7010Object *arg0, s32 arg1, s32 *arg2);
void ovl_11_func_800CA044 (Recon_ovl_11_func_800CD08C_A0View *arg0, s32 arg1, s32 *arg2);
void ovl_11_func_800C9E90 (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800CA580 (Ovl11DrawState *arg0);
void ovl_11_func_8010C1C0 (Ovl11DrawState *arg0);
void ovl_11_func_8010C2EC (u8 *arg0);
u32 ovl_11_func_800CDFAC (u8 *arg0);
void ovl_11_func_80107E2C(s16 *arg0, s32 arg1, s16 arg2, s16 arg3);

void ovl_11_func_800CA24C(Ovl11SpritePositionView *arg0) {
    s32 var_a0;
    s32 var_s3;
    u8 *object;

    object = (u8 *) &D_80071A00;

    if (ovl_11_func_800DD1D0((s16 *) (object + 0xFC)) == 0) {
        D_80128B80[0] = arg0->field_0 + 0x96;
        D_80128B80[1] = arg0->field_4 - 0x64;
        D_80128B80[2] = arg0->field_8 - 0x96;
        var_s3 = func_8001DFD4(D_80128B88, (SVECTOR *) D_80128B80);
        if ((*(s16 *) (object + 0x32)) != 0) {
            ovl_11_func_800CDEEC(object);
        }
        if (var_s3 != 0) {
            if ((*(u16 *) (object + 0x36)) & 4) {
                DrawSpriteFrameRects(gActiveRenderContext->field_120 + 0xA0, (s32) &gSharedSpriteSource, 0xE, ((u32) (*(u32 *) (object + -0x51C0)) >> 2) & 1, (s16) (s32) (*(s16 *) ((u8 *) D_80128B88 + 0)), (s16) (s32) (s16) ((*(u16 *) ((u8 *) D_80128B88 + 4)) - 0x32));
            }
            ovl_11_func_80107E2C((s16 *) (object + 0x2CC), gActiveRenderContext->field_120 + ((var_s3 >> 2) * 4), *(s16 *) ((u8 *) D_80128B88 + 0), (s16) ((*(u16 *) ((u8 *) D_80128B88 + 4)) - 0x28));
            if ((*(u16 *) (object + 0x38) == 0) ||
                (*(u16 *) (object + 0x38) == 3) ||
                (*(s32 *) (object + 0x6C) & 0x10000000) ||
                (*(u16 *) (object + 0x3C) == 0x15) ||
                (*(u16 *) (object + 0x3C) == 4) ||
                (*(u16 *) (object + 0x3C) == 0x32) ||
                (*(u8 *) (object + 0x264) == 5)) {
                ovl_11_func_800C9F20((Ovl11C7010Object *) object, var_s3, D_80128B88);
                ovl_11_func_800CA044((Recon_ovl_11_func_800CD08C_A0View *) object, var_s3, D_80128B88);
            } else {
                ovl_11_func_800CA044((Recon_ovl_11_func_800CD08C_A0View *) object, var_s3, D_80128B88);
                ovl_11_func_800C9F20((Ovl11C7010Object *) object, var_s3, D_80128B88);
            }
            var_a0 = 0;
            if ((*(u16 *) (object + 0x3C)) == 0x4A) {
                var_a0 = (*(u8 *) (object + 0x264)) == 1;
            }
            if ((*(s32 *) (object + 0x6C)) & 0x40000000) {
                var_a0 = 1;
            }
            if (var_a0 != 0) {
                D_80128B80[0] = arg0->field_0 + 0x96;
                D_80128B80[1] = -0x64;
                D_80128B80[2] = arg0->field_8 - 0x96;
                var_s3 = func_8001DFD4(D_80128B88, (SVECTOR *) D_80128B80);
                if (var_s3 == 0) {
                    return;
                }
            }
            {
                if (!(*(s32 *) (object + 0x6C) & 0x100)) {
                    ovl_11_func_800C9E90(gActiveRenderContext->field_120 + ((var_s3 >> 2) * 4),
                        D_80128B88[0], D_80128B88[1], 0);
                }
                if (*(s32 *) (object + 0x6C) & 0x100) {
                    if ((*(s32 *) (object + 0x6C)) & 0x80) {
                        ovl_11_func_800CA580((Ovl11DrawState *) object);
                    } else {
                        ovl_11_func_8010C1C0((Ovl11DrawState *) object);
                        ovl_11_func_8010C2EC((u8 *) &D_80075AD4);
                    }
                }
                if ((D_80123154 != 0) && (*((s16 *) ((u8 *) &D_80125F24 + ((*(s16 *) (object + 0xFE)) * 2))) == 0)) {
                    ovl_11_func_800CDFAC(object);
                }
            }
        }
    }
}
