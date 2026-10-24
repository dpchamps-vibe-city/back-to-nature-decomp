#include "common.h"
#include "psyq/memory.h"

u32 Rand(s32 arg0);
s32 func_80022014(s32 arg0, s16 arg1);

void func_8002206C(void) {
    CardGameState *state = (CardGameState *)&D_8007AD4C;
    s16 *walk;
    s16 *pW;
    u32 rows;
    u32 n;
    s16 v;
    s32 saved;
    s32 max;
    s32 modeY;

    saved = state->mode;
    memset(state, 0, 0x108);
    for (rows = 0; rows < 3U; rows++) {
        s16 *clear;
        n = 0;
        clear = &D_8007AD4E;
        clear = (s16 *)((u8 *)clear + rows * 0x54);
        do {
            *clear = -1;
            clear += 7;
            n++;
        } while (n < 6U);
    }
    for (rows = 0; rows < 3U; rows++) {
        for (n = 0; n < 6U; n++) {
            do {
                v = Rand(40);
            } while (func_80022014((s32)state, v));
            *(s16 *)((u8 *)&D_8007AD4E + rows * 84 + n * 14) = v;
            *(u16 *)((u8 *)&D_8007AD54 + rows * 84 + n * 14) = D_800558A4[v * 2];
            *(u16 *)((u8 *)&D_8007AD56 + rows * 84 + n * 14) = D_800558A4[(v * 2) | 1];
            *(s16 *)((u8 *)&D_8007AD50 + rows * 84 + n * 14) = D_80055944[n * 4] +
                Rand((u16)(D_80055944[n * 4 + 1] - D_80055944[n * 4]));
            *(s16 *)((u8 *)&D_8007AD52 + rows * 84 + n * 14) = D_80055944[n * 4 + 2] +
                Rand((u16)(D_80055944[n * 4 + 3] - D_80055944[n * 4 + 2]));
        }
    }
    if (saved == 3) {
        Ovl17ModeView *q = (Ovl17ModeView *)D_8006C838;
        u32 modeX = q->unk9348;
        CardGameState *p = state;
        modeY = q->unk92B2;
        p->cards[2][2].value = -1;
        p->cards[2][2].x = modeX;
        p->cards[2][2].y = modeY;
        p->cards[2][2].attr[0] = 0;
        p->cards[2][2].attr[1] = 0;
    }
    rows = 0;
    do {
        max = 0;
        for (n = 0; n < 6U; n++) {
            if ((u32)max < (u32)state->cards[rows][n].x) {
                max = state->cards[rows][n].x;
            }
        }
        n = 0;
        pW = (s16 *)((char *)&D_8007AD58 + rows * 0x54);
        walk = (s16 *)((u32)&D_8007AD50 + rows * 84);
        do {
            *pW = (s16)(Rand(4) +
                ((u32)((max - *walk) * (max - *walk)) / 1000 + 1));
            walk += 7;
            n++;
            pW += 7;
        } while (n < 6U);
        rows++;
    } while (rows < 3U);
}
