#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_id_state.h"
#include "ovl11_object_record_clear.h"
#include "ovl11_direction_vector.h"

void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
void ovl_11_func_800D1960 (s16 arg0);
s32 ovl_11_func_800D425C (u16 *arg0);
void ovl_11_func_800D12B8 (Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800D506C(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_s2;
    Ovl11PaddedVec3 position;

    var_s2 = 0;
    if ((u16) (*(u16 *) ((u8 *) arg0 + 0xAA)) < 2U) {
        Ovl11StartTimedSpriteEffect((M2C_729a6ccf4038_UnkStruct80107DE0 *) ((u8 *) arg0 + 0xA8), 0x21, 0x5A);
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        ((Ovl11ProgressState *) D_8006C838)->flags &= ~0x1000000;
        ((Ovl11ProgressState *) D_8006C838)->flags &= ~0x800000;
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 1;
        arg0->field_34 |= 0x800;
        position.field_0 = -0x1324;
        position.field_4 = 0;
        position.field_8 = -0x22C4;
        ovl_11_func_800D05D0(arg0, position);
        break;
    case 1:
        ovl_11_func_800D1960(0xC8);
        ovl_11_func_800D425C((u16 *) arg0);
        if (!(arg0->field_34 & 0x2000)) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        }
        arg0->field_34 |= 0x800;
        break;
    case 2:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = 0;
        arg0->field_34 |= 0x200000;
        ovl_11_func_800D425C((u16 *) arg0);
        if (!(arg0->field_34 & 0x200000)) {
            Ovl11SetDirectionVector(0, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0xC);
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x40)) < -0x2EDF) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
        }
        arg0->field_34 |= 0x800;
        break;
    case 3:
        arg0->field_34 &= ~0x800;
        Ovl11ClearB0ObjectRecord((s32) arg0);
        var_s2 = 1;
        break;
    }
    return var_s2;
}
