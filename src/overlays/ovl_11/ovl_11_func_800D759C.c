#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_11_func_800D759C(s16 arg0, s16 arg1, s16 arg2) {
    s16 var_t0;
    u32 temp_a2;
    u32 temp_v1;
    Ovl11GridTextureEntry *temp_a1;
    Ovl11TextureCoordinates *temp_a3;
    Ovl11TextureStateView *state;

    var_t0 = arg2;
    state = (Ovl11TextureStateView *)&D_8007AFF0;
    if (state->mode != 1) {
        return 0;
    }
    temp_a1 = state->entries[arg1 * 45 + arg0];
    if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 3) {
        var_t0 = 0x1D;
    }
    temp_a2 = temp_a1->header;
    if (temp_a2 & 8) {
        temp_a3 = &((Ovl11TextureCoordinates *)D_80123CA0)[var_t0];
        temp_v1 = temp_a2 & 0xFDFF0000;
        if (temp_v1 == 0x3D010000) {
            if (D_80123DCC[arg0] != 0) {
                setUV4(temp_a1, temp_a3->u0, temp_a3->v0,
                       temp_a3->u1, temp_a3->v1, temp_a3->u2,
                       temp_a3->v2, temp_a3->u3, temp_a3->v3);
            } else {
                setUV4(temp_a1, temp_a3->u3, temp_a3->v3,
                       temp_a3->u2, temp_a3->v2, temp_a3->u1,
                       temp_a3->v1, temp_a3->u0, temp_a3->v0);
            }
            temp_a1->clut = temp_a3->clut;
            if (var_t0 == 0x1D) {
                temp_a1->tpage = 7;
                return 0;
            }
            if ((var_t0 >= 5) && (var_t0 != 0x1D)) {
                temp_a1->tpage = 0x1D;
                if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 2) {
                    temp_a1->clut = (temp_a3->clut & 0xFFC0) | 0x35;
                }
            } else {
                temp_a1->tpage = 8;
            }
            return 0;
        }
        if (temp_v1 == 0x2D010000) {
            if (D_80123DCC[arg0] != 0) {
                setUV4(temp_a1, temp_a3->u0, temp_a3->v0,
                       temp_a3->u1, temp_a3->v1, temp_a3->u2,
                       temp_a3->v2, temp_a3->u3, temp_a3->v3);
            } else {
                setUV4(temp_a1, temp_a3->u3, temp_a3->v3,
                       temp_a3->u2, temp_a3->v2, temp_a3->u1,
                       temp_a3->v1, temp_a3->u0, temp_a3->v0);
            }
            temp_a1->clut = temp_a3->clut;
            if (var_t0 == 0x1D) {
                temp_a1->tpage = 7;
                return 0;
            }
            if (var_t0 >= 5) {
                temp_a1->tpage = 0x1D;
                if (((struct struct_8006C838_button *)D_8006C838)->field_44BA == 2) {
                    temp_a1->clut = (temp_a3->clut & 0xFFC0) | 0x35;
                }
            } else {
                temp_a1->tpage = 8;
            }
            return 0;
        }
    }
    return 1;
}
