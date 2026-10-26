#include "common.h"
#include "ovl19_actor_behavior_states.h"
#include "ovl19_projected_actor_render.h"

void ovl_19_func_800B87C4(void) {
    void (*temp_a0)(void);

    if (gOvl19SceneWork[3] == 7) {
        gOvl19SceneWork[3] = 0;
        gOvl19SceneWork[2] = 3;
        return;
    }
    temp_a0 = D_800BCEC8[gOvl19SceneWork[3]];
    temp_a0();
    Ovl19UpdateActorBehaviors();
    Ovl19DrawProjectedActorSprites();
}
