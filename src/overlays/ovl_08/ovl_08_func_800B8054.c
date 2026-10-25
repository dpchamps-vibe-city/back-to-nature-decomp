#include "common.h"
#include "screen_fade.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "card_deal.h"

void ovl_08_func_800B8054(void) {
    u8 *fnt;

    D_80070CC4 = 0;
    KanjiFntClose();
    KanjiFntOpen(8, 0, 0x140, 0xF0, 0x1C0, 0, 0x140, 0xFF, 0, 0xFF);
    BeginScreenFadeDecrease(1, 0, 2);
    fnt = (u8 *)&gRenderContexts[0];
    fnt[0x19] = 0x40;
    fnt[0x1A] = 0x40;
    fnt[0x1B] = 0x80;
    fnt += 0x134;
    fnt[0x19] = 0x40;
    fnt[0x1A] = 0x40;
    fnt[0x1B] = 0x80;
    DealCardGameTable();
    D_800B8500 += 1;
}
