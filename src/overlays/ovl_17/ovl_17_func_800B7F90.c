#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "ovl17_record_sprite_animation.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl17_record_backdrop.h"

void ovl_17_func_800B7F90(void) {
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_s1;

    Ovl17DrawRecordBackdrop(0x40000);
    Ovl17SelectAllRecordAnimations2();
    var_s1 = gOvl17AnimatedRecordBase;
    for (var_s0 = 0; var_s0 < 6; var_s0++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *) var_s1, 0x40000);
        var_s1 += 0x50;
    }
    var_s0_2 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0_2 = (var_s0_2 + 1) & 0xFF;
    }
    if (var_s0_2 == 2) {
        ((Ovl21MainStateView *) D_8006C838)->state++;
    }
}
