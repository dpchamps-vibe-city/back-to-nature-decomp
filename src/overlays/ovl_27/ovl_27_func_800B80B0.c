#include "common.h"
#include "psyq/memory.h"
#include "screen_fade.h"
#include "random.h"

s32 ovl_27_func_800BA230(void);
void ovl_27_func_800B9324(void);

void ovl_27_func_800B80B0(void) {
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;
    Ovl27StateCounterView *state2;

    if (ovl_27_func_800BA230() != 0) {
        memcpy(D_800605E8, D_800C4A40[Rand(3)], 0x1800);
        if (D_800C4A28 & 0x4000) {
            D_800C4A18 = 7;
            state = (Ovl27StateCounterView *)D_8006C838;
            state->counter++;
        } else {
            gRenderContexts->unk19 = 0xFF;
            gRenderContexts->unk1A = 0xFF;
            gRenderContexts->unk1B = 0xFF;
            second = &gRenderContexts[1];
            second->unk19 = 0xFF;
            second->unk1A = 0xFF;
            second->unk1B = 0xFF;
            gRenderContexts->unk18 = 1;
            gRenderContexts[1].unk18 = 1;
            ovl_27_func_800B9324();
            BeginScreenFadeDecrease(0x1E, 0, 2);
            D_800C4A18 = 0;
            state2 = (Ovl27StateCounterView *)D_8006C838;
            state2->counter++;
        }
    }
}
