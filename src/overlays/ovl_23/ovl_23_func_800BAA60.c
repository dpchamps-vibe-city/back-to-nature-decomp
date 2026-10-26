#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl23_record_trails.h"

extern Ovl23D87CViewBAA60 gOvl23RecordRunWork;
void func_800248E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_23_func_800BB040(s32 arg0);
s32 ovl_23_func_800BAFFC(s32 arg0);

void ovl_23_func_800BAA60(Ovl23BAA60Arg *arg0) {
    ObjectState *obj;
    s16 temp_s3;
    s32 temp_s0;
    s32 temp_s1;
    u8 var_a1;

    if (arg0->unk4 == 1) {
        var_a1 = 0;
    } else if (arg0->unk4 == 2) {
        var_a1 = 2;
    } else if (arg0->unk4 == 3) {
        var_a1 = 4;
    } else {
        return;
    }
    if (arg0->unk6 == 6) {
        var_a1 = (var_a1 + 1) & 0xFF;
    }
    obj = (ObjectState *) ((u8 *) arg0 + 0x10);
    if (arg0->unk14 != var_a1) {
        SelectSpriteAnimation(obj, var_a1);
    }
    temp_s1 = ovl_23_func_800BB040(arg0->unkC);
    temp_s3 = *((s16 *) ((u8 *) &gOvl23RecordAnchorY + (arg0->unk0 * 2)));
    temp_s0 = ovl_23_func_800BAFFC(temp_s3 << 0xC);
    SetSpriteSourceTextureOffsets((Struct_800154CC *) obj, 0, 0, 0, arg0->unk40);
    DrawSpriteAnimationQuadsAndUpdate(temp_s0, (SpriteSourceData *) obj, temp_s1, temp_s3);
    func_800248E8(temp_s0, temp_s1, temp_s3, 0);
    if ((gOvl23RecordRunWork.unk4 != 1) && (gOvl23RecordRunWork.unk14 == ((((gOvl23RecordRunWork.unk14 + 1) / 10) * 10) - 1))) {
        Ovl23PushDecisionTrailNode(arg0->unkC, temp_s3, arg0->unk6);
    }
}
