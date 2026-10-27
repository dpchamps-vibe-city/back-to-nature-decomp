#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl25_sprite_set_service.h"


void Ovl25DrawBiasedProjectedSprite (SpriteSourceData *arg0, u16 *arg1);
void Ovl25DrawProjectedSprite (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B941C(void) {
    Ovl25ObjectRecord *var_s1;
    Ovl25ObjectRecord *var_s1_2;
    s32 var_s0;
    s32 var_s0_2;

    var_s1 = &D_800BFE54;
    var_s0 = 5;
    do {
        Ovl25DrawObjectRecordPair(var_s1);
        var_s0 -= 1;
        var_s1 += 1;
    } while (var_s0 >= 0);
    var_s1_2 = &D_800C0124;
    var_s0_2 = 1;
    do {
        Ovl25DrawAuxiliaryRecord(var_s1_2);
        var_s0_2 -= 1;
        var_s1_2 += 1;
    } while (var_s0_2 >= 0);
}
