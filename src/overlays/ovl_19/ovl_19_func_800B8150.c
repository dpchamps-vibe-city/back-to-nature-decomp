#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "ovl19_projected_actor_render.h"

void ovl_19_func_800B8150(void) {
    BeginScreenFadeFeedback(0xA);
    BeginMasterVolumeFadeOut(0xA);
    Ovl19DrawProjectedActorSprites();
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}
