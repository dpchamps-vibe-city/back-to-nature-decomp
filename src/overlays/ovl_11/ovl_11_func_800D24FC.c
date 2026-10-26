#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_directional_tile_probe.h"

s32 ovl_11_func_800D24FC(Recon_ovl_11_func_800D2594_A0View *arg0,
                       VECTOR position, s16 arg5, s32 arg6) {
    s32 result;

    result = Ovl11ProbeDirectionalTile(position,
                                *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E),
                                arg5, 0);
    if (result == 2 && arg6 == 1) {
        arg0->unk34 |= 0x01000000;
    }
    return result;
}
