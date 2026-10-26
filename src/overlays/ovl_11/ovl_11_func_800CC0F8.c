#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "point_projection.h"

unsigned long long ovl_11_func_800BF450(void);
s32 ovl_11_func_800CC7CC(Ovl11CC7CCObject *arg0);
s32 ovl_11_func_800CBE2C(Ovl11CC4E0Object *arg0);
void ovl_11_func_800CCBE8(Ovl11CC4E0Object *arg0);
s32 ovl_11_func_800CB540(u16 *arg0);
void ovl_11_func_800D6F78(s16, s16, s32 *, s16, s16, s32);
s32 ovl_11_func_800CE550(Vec3 *arg0);
void ovl_11_func_800C9E90(s32, s32, s32, s32);

void ovl_11_func_800CC0F8(Ovl11CC7CCObject *arg0) {
    VECTOR next;
    Ovl11CC7CCRequest request;
    s16 temp_a0;
    s16 temp_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 depth;
    s32 temp_v1_2;
    s32 var_a1;
    s32 var_s3;
    s32 var_v0;
    s32 var_v1;
    u16 temp_v0_3;
    u16 temp_v1;

    var_s3 = 0;
    D_80128BA0[0] = (*(u16 *) ((u8 *) arg0 + 0x24)) + 0x96;
    D_80128BA0[1] = ((*(u16 *) ((u8 *) arg0 + 0x28)) + (*(u16 *) ((u8 *) arg0 + 0xC))) - 0x64;
    D_80128BA0[2] = (*(u16 *) ((u8 *) arg0 + 0x2C)) - 0x96;
    if (!(D_8006C844 & 0x08000000) && IsScreenFadeComplete() == 1) {
            temp_v1 = arg0->unk6;
            switch (temp_v1) {
            default:
                break;
            case 0:
                arg0->position.vx += arg0->velocity.vx / 4096;
                arg0->position.vz += arg0->velocity.vz / 4096;
                arg0->position.vy += arg0->unkC;
                arg0->unkC += 9;
                if (arg0->unkC >= 0x2E) {
                    arg0->unkC = 0x2D;
                }
                var_s3 = ovl_11_func_800CC7CC(arg0);
                if (var_s3 == 1 || ovl_11_func_800CBE2C((Ovl11CC4E0Object *)arg0) != 0) return;
                break;
            case 1:
                if (--*(s16 *)&arg0->unk4 == 0) {
                    arg0->unk6 = 2;
                    arg0->unk4 = 4;
                    arg0->unk0 = 1;
                }
                break;
            case 2:
                arg0->unk4--;
                if ((s16)arg0->unk4 >= 4) {
                    if (arg0->unk4 & 4) return;
                } else if ((s16)arg0->unk4 >= 3) {
                    if (arg0->unk4 & 2) return;
                } else if ((s16)arg0->unk4 >= 2) {
                    if (arg0->unk4 & 1) return;
                }
                if ((s16)arg0->unk4 == 0) {
                    ovl_11_func_800CCBE8((Ovl11CC4E0Object *)arg0);
                    return;
                }
                break;
            case 3:
            case 4:
                if (D_80128C82 & 0x100) {
                    ovl_11_func_800CCBE8((Ovl11CC4E0Object *) arg0);
                    if (arg0->unk6 == 4) {
                        request.id = 0x87;
                        request.count = 1;
                        ovl_11_func_800CB540(&request.id);
                        return;
                    }
                    return;
                }
                break;
            }
    }

        depth = ProjectPointToScreen(D_80128B98, (SVECTOR *)D_80128BA0);
        if (depth != 0) {
            temp_v1_2 = gActiveRenderContext->field_120 + ((depth >> 2) * 4);
            if ((u32) (arg0->unk6 - 3) < 2U) {
                DrawSpriteAnimationRectsAndUpdate(temp_v1_2, &D_80128C80, *(s16 *) ((u8 *) D_80128B98 + 0), *(s16 *) ((u8 *) D_80128B98 + 4));
                return;
            }
            temp_a0 = arg0->unk2;
            if (temp_a0 == 0x186) {
                DrawSpriteFrameRects(temp_v1_2, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gOvl11HeldActionSpriteHeader + 0)), 0, (s16) (s32) (*(s16 *) ((u8 *) D_80128B98 + 0)), (s16) (s32) (*(s16 *) ((u8 *) D_80128B98 + 4)));
            } else {
                ovl_11_func_800D6F78(temp_a0, arg0->unk0, (s32 *) temp_v1_2, *(s16 *) ((u8 *) D_80128B98 + 0), (s16) (s32) (*(s16 *) ((u8 *) D_80128B98 + 4)), 2);
            }
            next.vx = arg0->position.vx;
            next.vy = arg0->height;
            next.vz = arg0->position.vz;
            ovl_11_func_800CE550((Vec3 *) &next);
            if ((var_s3 == 2) && ((s32)ovl_11_func_800BF450() == 0)) {
                arg0->height = next.vy;
            }
            if (arg0->unk2 != 0x186) {
                D_80128BA0[0] = (*(u16 *) ((u8 *) arg0 + 0x24)) + 0x96;
                D_80128BA0[1] = (u16) (*(s32 *) ((u8 *) arg0 + 0x10)) - 0x64;
                D_80128BA0[2] = (*(u16 *) ((u8 *) arg0 + 0x2C)) - 0x96;
                depth = ProjectPointToScreen(D_80128B98, (SVECTOR *)D_80128BA0);
                if (depth != 0) {
                    ovl_11_func_800C9E90(gActiveRenderContext->field_120 + ((depth >> 2) * 4), D_80128B98[0], *(s32 *) ((u8 *) D_80128B98 + 4), 0);
                }
            }
        }
}
