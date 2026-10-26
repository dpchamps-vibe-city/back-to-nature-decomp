#include "common.h"
#include "game_types.h"
#include "ovl11_record_pair_slots.h"

s32 ovl_11_func_800F0380(s16 arg0);

void ovl_11_func_800F05C0(void) {
    s32 i;
    s32 *flags;
    s32 index;
    s32 offset;
    s16 secondIndex;
    s16 *pair;
    s32 pairOffset;
    u8 *dataBase;

    for (i = 0; i < 4; i++) {
        dataBase = (u8 *)D_8006C838;
        pairOffset = 0x99C8;
        if (ovl_11_func_800F0380((s16)i) == 1) {
            if ((u32)i < 2U) {
                index = ((Ovl11ClearPairWorkView *)D_80074838)->pairs[i][1];
                offset = index * -0xB8;
                flags = (s32 *)((u8 *)((Ovl11ClearFlagWorkView *)D_80074838)->records - offset);
                index = ~0x200;
                *flags &= index;
                pair = (s16 *)(dataBase + i * 4 + pairOffset);
                secondIndex = *pair;
                index = secondIndex;
                offset = index * -0xB8;
                flags = (s32 *)((u8 *)((Ovl11ClearFlagWorkView *)D_80074838)->records - offset);
                *flags &= 0xFDFFFFFF;
            }
            Ovl11ClearRecordPairSlot((Ovl11RecordPairSlot *)
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[i]);
        }
    }
    Ovl11ResyncRecordRowPlacements(1);
    Ovl11ResyncRecordRowPlacements(0);
}
