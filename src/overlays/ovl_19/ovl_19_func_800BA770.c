#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_projected_sprite.h"

void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);

void ovl_19_func_800BA770(void) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 2; i++) {
        ovl_19_func_800BA834(&gOvl19ActorPairState[i * 0x24]);
    }
    for (j = 0; j < 3; j++) {
        ovl_19_func_800BAB4C(&((Ovl19Func800BAB4CArg *) D_800BF560)[j]);
    }
    for (k = 0; k < 9; k++) {
        Ovl19DrawProjectedSpriteState(gActiveRenderContext->field_120, (ObjectState *) &gOvl19AuxSceneSpriteSource,
                           &D_800BCF7C[k * 6], D_800BCF7C[k * 6 - 2]);
    }
}
