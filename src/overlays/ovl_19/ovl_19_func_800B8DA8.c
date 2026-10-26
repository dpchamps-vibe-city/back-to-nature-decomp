#include "common.h"
#include "ovl19_projected_actor_render.h"

void ovl_19_func_800B8DA8(void) {
    Ovl19DrawProjectedActorSprites();
    ((Ovl19CompletionCounterView *) D_8006C838)->completed = 1;
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}
