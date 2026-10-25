/*
 * Unique card dealing, exe:80022014..8002238C.
 *
 * The dealer and its sole called value-membership helper share the 3x6,
 * stride-14 table protocol. Private tables supply two attributes per value
 * and four coordinate bounds per column. card_deal.h describes the old-mode
 * special slot, randomization and weight arithmetic. The card state also has
 * overlay consumers; that shared storage is not evidence of a shared TU.
 * Membership and both boundaries are recorded in notes/file-groupings.md.
 */
#include "common.h"
#include "card_deal.h"
#include "psyq/memory.h"

u32 Rand(s32 limit);

s32 CardTableContainsValue(s32 tableAddress, s16 value) {
    u32 row;
    u32 column;

    row = 0;
    tableAddress += 2;
    for (row = 0; row < 3; row++) {
        for (column = 0; column < 6; column++) {
            if (*(s16 *)((char *)tableAddress + row * 84 + column * 14) == value) {
                return 1;
            }
        }
    }
    return 0;
}

void DealCardGameTable(void) {
    CardGameState *state = (CardGameState *)&gCardGameState;
    s16 *xCursor;
    s16 *weightCursor;
    u32 row;
    u32 column;
    s16 value;
    s32 savedMode;
    s32 maxX;
    s32 specialY;

    savedMode = state->mode;
    memset(state, 0, 0x108);
    for (row = 0; row < 3U; row++) {
        s16 *valueCursor;
        column = 0;
        valueCursor = &gCardFirstValue;
        valueCursor = (s16 *)((u8 *)valueCursor + row * 84);
        do {
            *valueCursor = -1;
            valueCursor += 7;
            column++;
        } while (column < 6U);
    }
    /* Keep the field-anchor views: each expression addresses the same
     * stride-14 record but preserves the original symbolic base formation. */
    for (row = 0; row < 3U; row++) {
        for (column = 0; column < 6U; column++) {
            do {
                value = Rand(40);
            } while (CardTableContainsValue((s32)state, value));
            *(s16 *)((u8 *)&gCardFirstValue + row * 84 + column * 14) = value;
            *(u16 *)((u8 *)&gCardFirstAttribute0 + row * 84 + column * 14) =
                gCardValueAttributes[value * 2];
            *(u16 *)((u8 *)&gCardFirstAttribute1 + row * 84 + column * 14) =
                gCardValueAttributes[(value * 2) | 1];
            *(s16 *)((u8 *)&gCardFirstX + row * 84 + column * 14) =
                gCardColumnPositionRanges[column * 4] +
                Rand((u16)(gCardColumnPositionRanges[column * 4 + 1] -
                           gCardColumnPositionRanges[column * 4]));
            *(s16 *)((u8 *)&gCardFirstY + row * 84 + column * 14) =
                gCardColumnPositionRanges[column * 4 + 2] +
                Rand((u16)(gCardColumnPositionRanges[column * 4 + 3] -
                           gCardColumnPositionRanges[column * 4 + 2]));
        }
    }
    if (savedMode == 3) {
        Ovl17ModeView *gameState = (Ovl17ModeView *)D_8006C838;
        u32 specialX = gameState->unk9348;
        CardGameState *specialState = state;
        specialY = gameState->unk92B2;
        specialState->cards[2][2].value = -1;
        specialState->cards[2][2].x = specialX;
        specialState->cards[2][2].y = specialY;
        specialState->cards[2][2].attr[0] = 0;
        specialState->cards[2][2].attr[1] = 0;
    }
    row = 0;
    do {
        maxX = 0;
        for (column = 0; column < 6U; column++) {
            if ((u32)maxX < (u32)state->cards[row][column].x) {
                maxX = state->cards[row][column].x;
            }
        }
        column = 0;
        weightCursor = (s16 *)((char *)&gCardFirstWeight + row * 84);
        xCursor = (s16 *)((u32)&gCardFirstX + row * 84);
        do {
            *weightCursor = (s16)(Rand(4) +
                ((u32)((maxX - *xCursor) * (maxX - *xCursor)) / 1000 + 1));
            xCursor += 7;
            column++;
            weightCursor += 7;
        } while (column < 6U);
        row++;
    } while (row < 3U);
}
