/*
 * Indexed object-record reset command and its all-record/global-flag modes.
 * The four-word copy and masked flags are intentionally kept distinct from
 * the separate full record constructor and pool-clearing services.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_record_reset_command.h"

s32 Ovl11ResetObjectRecordCommand(s16 firstId, s16 secondId,
                                 s32 thirdId, s32 fourthId) {
    s32 ids[4];
    s32 remaining;
    s32 *idCursor;
    s32 id;
    s32 secondIdHigh;
    struct_80076220 *record;
    struct_80076220 *base;

    switch (firstId) {
    case 42:
        remaining = 1;
        secondIdHigh = 0x20000;
        do {
            Ovl11ResetObjectRecordCommand(remaining, (s16)(secondIdHigh >> 16),
                                          remaining + 2, remaining + 3);
            remaining += 4;
            secondIdHigh += 0x40000;
        } while (remaining < 0x25);
        break;
    case 45:
        ((Ovl11AreaInitView *)D_8006C838)->field_4450 |= 2;
        break;
    default:
        base = &gObjectEventSlotBase;
        idCursor = ids;
        remaining = 3;
        ids[0] = (s32)firstId;
        ids[1] = (s32)secondId;
        ids[2] = thirdId;
        ids[3] = fourthId;
        do {
            id = *idCursor;
            if ((u32)(id - 1) < 0x24U) {
                record = (struct_80076220 *)(id * 468 + (u32)base);
                *(s32 *)((u8 *)record + 0x60) = *(s32 *)((u8 *)record + 0x30);
                *(s32 *)((u8 *)record + 0x64) = *(s32 *)((u8 *)record + 0x34);
                record->unk30[0x84] = 0;
                *(s32 *)((u8 *)record + 0x70) = 0;
                *(s32 *)((u8 *)record + 0x74) = 0;
                *(s32 *)((u8 *)record + 0x78) = 0;
                *(s32 *)((u8 *)record + 0x68) = *(s32 *)((u8 *)record + 0x38);
                *(s32 *)((u8 *)record + 0x6C) = *(s32 *)((u8 *)record + 0x3C);
                record->unk1E |= 0x1000;
                *(u16 *)((u8 *)record + 0x20) &= 0x3FFF;
                Ovl11ResetTimedSpriteEffect((s16 *)&record->unk30[0xB0]);
            }
            remaining -= 1;
            idCursor += 1;
        } while (remaining >= 0);
        break;
    }
    return 1;
}
