/* Three projected object renderers and their private marker operation.
 * Scratch slots are separate for the default and source-offset variants.
 * Public contracts: ovl11_object_render.h. Unit evidence: file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl11_object_render.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

s32 ovl_11_func_800F5888(u16 *position, s32 *screen);
s32 ovl_11_func_800D2950(s32 objectId);
void ovl_11_func_80107E2C(u8 *part, s32 ot, s16 x, s16 y);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                  s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
s32 func_8001DFD4(s32 *screen, SVECTOR *position);
s32 func_80015DD4(s32 *arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3,
                 s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void ovl_11_func_800C9E90(s32 ot, s32 x, s32 y, s32 variant);
s32 Ovl11SubmitObjectMarker(s32 flags, s32 ot, s32 x, s32 y);

void Ovl11DrawObjectWithMarker(u8 *object) {
    s32 screen[4];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        gOvl11ObjectDrawPosition[0] = *(u16 *)(object + 0x38) + 0x96;
        gOvl11ObjectDrawPosition[1] = *(u16 *)(object + 0x3C) - 0x64;
        gOvl11ObjectDrawPosition[2] = *(u16 *)(object + 0x40) - 0x96;
        if (ovl_11_func_800F5888((u16 *)gOvl11ObjectDrawPosition, screen) != 0) {
            ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
            ovl_11_func_80107E2C(object + 0xA8, ot, (s16)screen[0],
                (s16)(screen[1] + ovl_11_func_800D2950(*(s16 *)object)));
            func_80016054(ot, gActiveRenderContext->field_118, (s32)(object + 0x78),
                object[0x7C], object[0x7D], (s16)screen[0], (s16)screen[1],
                *(s32 *)(object + 0x80), 0, 0x40);
            /* The marker may use a fixed-height projection without moving
             * the sprite. Keep the two ordering-table selections distinct. */
            if (*(s32 *)(object + 0x34) & 0x01000000) {
                gOvl11ObjectDrawPosition[0] = *(u16 *)(object + 0x38) + 0x96;
                gOvl11ObjectDrawPosition[1] = -0x64;
                gOvl11ObjectDrawPosition[2] = *(u16 *)(object + 0x40) - 0x96;
                ot = gActiveRenderContext->field_120 +
                    ((func_8001DFD4(gOvl11ObjectDrawMarkerScreen, (SVECTOR *)gOvl11ObjectDrawPosition) >> 2) * 4);
            } else {
                gOvl11ObjectDrawMarkerScreen[0] = screen[0];
                gOvl11ObjectDrawMarkerScreen[1] = screen[1];
            }
            Ovl11SubmitObjectMarker(*(s32 *)(object + 0x34), ot,
                gOvl11ObjectDrawMarkerScreen[0], gOvl11ObjectDrawMarkerScreen[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            UpdateSpriteAnimation((SpriteSourceData *)(object + 0x78));
        }
    }
}

void Ovl11DrawObjectWithExtraSprite(u8 *object) {
    u16 position[3];
    s32 projected[3];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        position[0] = *(u16 *)(object + 0x38) + 0x96;
        position[1] = *(u16 *)(object + 0x3C) - 0x64;
        position[2] = *(u16 *)(object + 0x40) - 0x96;
        if (ovl_11_func_800F5888(position, projected) != 0) {
            ot = gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4);
            ovl_11_func_80107E2C(object + 0xA8, ot, (s16)projected[0], (s16)(projected[1] - 0x14));
            if (*(u16 *)(object + 0xB6) != 0) {
                DrawSpriteFrameRects(ot, (s32)(object + 0xC8), 0, 0, (s16)projected[0], (s16)(projected[1] - 0x10));
            }
            func_80016054(ot, gActiveRenderContext->field_118, (s32)(object + 0x78), object[0x7C], object[0x7D], (s16)projected[0], (s16)projected[1], *(s32 *)(object + 0x80), 0, 0x40);
            Ovl11SubmitObjectMarker(*(s32 *)(object + 0x34), ot, projected[0], projected[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            UpdateSpriteAnimation((SpriteSourceData *)(object + 0x78));
        }
    }
}

void Ovl11DrawObjectWithSourceOffsets(u8 *object, s32 textureBase, s32 sourceBias) {
    s32 screen[4];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        gOvl11OffsetObjectDrawPosition[0] = *(u16 *)(object + 0x38) + 0x96;
        gOvl11OffsetObjectDrawPosition[1] = *(u16 *)(object + 0x3C) - 0x64;
        gOvl11OffsetObjectDrawPosition[2] = *(u16 *)(object + 0x40) - 0x96;
        if (ovl_11_func_800F5888((u16 *)gOvl11OffsetObjectDrawPosition, screen) != 0) {
            ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
            func_80015DD4((s32 *)ot, (SpriteSourceData *)(object + 0x78), (s16)screen[0], (s16)screen[1], 0x1000, 0, 0, D_8005E3BC + sourceBias, textureBase);
            ovl_11_func_80107E2C(object + 0xA8, ot, (s16)screen[0],
                (s16)(screen[1] + ovl_11_func_800D2950(*(s16 *)object)));
            if (*(s32 *)(object + 0x34) & 0x01000000) {
                gOvl11OffsetObjectDrawPosition[0] = *(u16 *)(object + 0x38) + 0x96;
                gOvl11OffsetObjectDrawPosition[1] = -0x64;
                gOvl11OffsetObjectDrawPosition[2] = *(u16 *)(object + 0x40) - 0x96;
                ot = gActiveRenderContext->field_120 +
                    ((func_8001DFD4(gOvl11OffsetObjectDrawMarkerScreen, (SVECTOR *)gOvl11OffsetObjectDrawPosition) >> 2) * 4);
            } else {
                gOvl11OffsetObjectDrawMarkerScreen[0] = screen[0];
                gOvl11OffsetObjectDrawMarkerScreen[1] = screen[1];
            }
            Ovl11SubmitObjectMarker(*(s32 *)(object + 0x34), ot,
                gOvl11OffsetObjectDrawMarkerScreen[0], gOvl11OffsetObjectDrawMarkerScreen[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            UpdateSpriteAnimation((SpriteSourceData *)(object + 0x78));
        }
    }
}

/* All three callers ignore the legacy result; retain its existing ABI. */
s32 Ovl11SubmitObjectMarker(s32 flags, s32 ot, s32 x, s32 y) {
    if ((flags & 0x1000) == 0) {
        ovl_11_func_800C9E90(ot, x, y, ((u32)flags) >> 0x1F);
    } else {
        return flags & 0x1000;
    }
}
