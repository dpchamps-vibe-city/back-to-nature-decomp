#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_surface_height.h"

s32 ovl_11_func_800D2DB8(Ovl11Func800BDCE8Arg1 *arg0) {
    if (D_80128D6C == 1) {
        if (Ovl11ProbeInteractionVolume((u8 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header, arg0, 0) != NULL) {
            return 1;
        }
    }
    return Ovl11ResolveSurfaceHeight(&arg0->unk0, 0xC000) != 0;
}
