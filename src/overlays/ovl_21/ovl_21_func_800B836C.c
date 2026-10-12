#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s16 ovl_21_func_800BABF8(s16 arg0);
s32 ovl_21_func_800BACB0(s16 arg0);
u8 *ovl_21_func_800BAD18(s16 arg0);

void ovl_21_func_800B836C(void) {
    s32 index;
    s32 j;
    s32 nextIndex;
    s32 offset;
    UnkStruct800C0448 *origin;
    UnkStruct800C0448 *record;
    UnkStruct800C0448 *state;
    SVECTOR *position;
    Ovl21InitSelectorView *selector;

    selector = (Ovl21InitSelectorView *) D_8006C838;
    if (selector->selector == 7) {
        index = 0;
    } else if (selector->selector == 9) {
        index = 1;
    } else if (selector->selector == 0x14) {
        index = 2;
    } else {
        index = 3;
    }
    D_800C0448_init.tableIndex = index;
    D_800C0448_init.variantIndex = D_800BCC04[index][((s16 *) &D_800C0448_init)[1]];
    D_800C0448_init.records[0].unk10 = 0;
    D_800C0448_init.records[1].unk10 = D_800BCBBC[index][1];
    D_800C0448_init.records[2].unk10 = D_800BCBBC[index][2];
    D_800C0448_init.records[3].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][0];
    D_800C0448_init.records[4].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][1];
    D_800C0448_init.records[5].unk10 = D_800BCBD4[index][((s16 *) &D_800C0448_init)[1]][2];
    for (index = 0; index < 6; index = nextIndex) {
        offset = index * sizeof(UnkStruct800C0448);
        record = (UnkStruct800C0448 *) (offset + (s32) &D_800C0448_init);
        record->unk12 = index;
        D_800C0448_init.records[index].unk14 = 1;
        record->unk18 = 0;
        position = (SVECTOR *) &D_800C0448_init.records[index].unk1A;
        position->vy = 0;
        position->vx = D_800BCC48[index].unk0;
        position->vz = D_800BCC48[index].unk2;
        record->unk22 = ovl_21_func_800BABF8(record->unk10);
        record->unk24 = ovl_21_func_800BACB0(record->unk10);
        D_800C0448_init.records[index].unk28 = 0;
        record->unk2C = 4;
        record->unk2E = 0;
        D_800C0448_init.records[index].unk30 = ovl_21_func_800BAD18(record->unk10);
        nextIndex = index + 1;
        /* Walk the three state slices while retaining the position source. */
        state = record;
        origin = state;
        for (j = 0; j < 3; j++) {
            state->unk34 = 0;
            state->unk38 = 0;
            state->unk3A = 0;
            state->unk3C = 0;
            state->unk48 = 0;
            state->unk4A = 0;
            state->unk4C = 0;
            state->unk40 = origin->unk1A;
            state->unk42 = origin->unk1C;
            state->unk44 = origin->unk1E;
            state = (UnkStruct800C0448 *) ((u8 *) state + sizeof(Ovl21InitSubstate));
        }
    }
}
