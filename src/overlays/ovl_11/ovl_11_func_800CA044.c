#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
void func_800158E4(SpriteSourceData *src);
POLY_FT4 *func_80016C08(s32 *ot, POLY_FT4 *poly, SpriteSourceData *src, s16 ox, s16 oy, u16 flags, s32 total, s32 texBase, s16 subst, s16 substFrom, s16 substTo);

void ovl_11_func_800CA044(Recon_ovl_11_func_800CD08C_A0View *arg0, s32 arg1, s32 *arg2) {
    SpriteSourceData *temp_s0;
    s16 var_t2;
    s16 var_t4;
    s16 var_t5;
    s32 temp_a3;
    s32 temp_v1;

    temp_a3 = gActiveRenderContext->field_120 + ((arg1 >> 2) * 4);
    if (arg0->unk6C & 0x100000) {
        temp_s0 = (SpriteSourceData *) ((u8 *) arg0 + 0x290);
        func_80016054(temp_a3, gActiveRenderContext->field_118, (s32) temp_s0, (s32) (*(u8 *) ((u8 *) arg0 + 0x294)), (u8) (s32) (*(u8 *) ((u8 *) arg0 + 0x295)), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 0)), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 4)), *(s32 *) ((u8 *) arg0 + 0x298), 0, 0x40U);
        func_800158E4(temp_s0);
        return;
    }
    var_t2 = -1;
    var_t4 = -1;
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x3C);
    var_t5 = -1;
    if (((u16) temp_v1 == 0x17) || ((u16) temp_v1 == 0x19) || ((u16) temp_v1 == 0x1B) || ((u16) temp_v1 == 0x1D) || ((u16) temp_v1 == 0x1F)) {
        if ((u32) ((*(u8 *) ((u8 *) arg0 + 0x264)) - 0x48) < 0x64U) {
            if (temp_v1 == 0x1F) {
                temp_v1 = (s32) D_801230A4;
                var_t2 = ((s16 *) temp_v1)[*(s16 *) ((u8 *) arg0 + 0x86)];
            } else {
                var_t2 = D_8012308C[*(s16 *) ((u8 *) arg0 + 0x86)];
                var_t5 = D_80123098[*(s16 *) ((u8 *) arg0 + 0x86)];
                temp_v1 = (s16) (*(u16 *) ((u8 *) arg0 + 0x18)) / 20;
                var_t4 = D_80123098[(s16) temp_v1];
            }
        }
    }
    func_80016C08((s32 *) temp_a3, (POLY_FT4 *) gActiveRenderContext->field_118, (SpriteSourceData *) ((u8 *) arg0 + 0x260), *(s16 *) ((u8 *) arg2 + 0), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 4)), 0x40U, D_8005E3BC, (s32) ((void *) ((u8 *) &D_80122F9C + (D_8005E3A4 * 0x78))), (s16) (s32) var_t2, (s16) (s32) var_t4, (s16) (s32) var_t5);
    if (!(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
        func_800158E4((SpriteSourceData *) ((u8 *) D_8006C838 + 0x5428));
    }
}
