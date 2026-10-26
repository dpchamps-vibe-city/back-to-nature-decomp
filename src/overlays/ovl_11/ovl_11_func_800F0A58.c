#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_11_func_800F0A58(s16 arg0) {
    RECT rect;
    s32 row;
    u32 i;
    s16 x;
    Ovl11A04B8Entry *record;
    Ovl11ActiveRecordView *state;
    s16 *first;
    s16 *second;

    row = -1;
    first = gSlotSpriteIds;
    second = first + 9;
    for (i = 0; i < 9; i++) {
        if (*first == arg0) {
            row = 0;
            break;
        }
        if (*second == arg0) {
            row = 1;
            break;
        }
        first++;
        second++;
    }
    if (row == -1) {
        FntPrint(D_800B9F58);
        return 0;
    }
    state = (Ovl11ActiveRecordView *)D_8006C838;
    record = &state->record;
    state->record =
        ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][i];
    x = 0x340;
    if (record->unk18 >= 0x3F0) {
        x = 0x3C0;
    }
    rect.x = x + record->unk0;
    rect.y = (u16)record->unk2;
    rect.w = record->unk8 / 4;
    rect.h = (u16)record->unkA;
    MoveImage2(&rect, 0x2C0, 0x1C0);
    record->unk0 = 0x2C0;
    record->unk2 = 0x1C0;
    rect.w = 0x10;
    rect.h = 1;
    rect.x = (u16)record->unk18;
    rect.y = record->unk1A;
    MoveImage2(&rect, 0x2C0, 0x1BF);
    record->unk18 = 0x2C0;
    record->unk1A = 0x1BF;
    *(s16 *)record->pad = 0;
    return 1;
}
