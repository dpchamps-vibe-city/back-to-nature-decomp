#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

void ovl_19_func_800BA770(void);

void ovl_19_func_800B8150(void) {
    BeginScreenFadeFeedback(0xA);
    BeginMasterVolumeFadeOut(0xA);
    ovl_19_func_800BA770();
    ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
}
