/* Reset one event slot's transient fields, timed effect and schedule entries.
 * Membership/boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_event_slot_reset_command.h"

s32 Ovl11ResetObjectEventSlotCommand(s16 slot, s32 unused1, s32 unused2, s32 unused3) {
    struct_80076220 *record;
    u16 *entryEnd;
    u32 scheduleIndex;
    s16 emptyCode;

    record = &gObjectEventSlotBase + slot;
    record->unk2C = 0x36;
    record->unk2E = 0x36;
    (*(s16 *) ((u8 *) record + 8)) = 0;
    record->unkA = 0;
    record->unkC = 0;
    record->unkE = 0;
    (*(s16 *) ((u8 *) record + 0x10)) = 0;
    (*(s16 *) ((u8 *) record + 0x14)) = 0;
    (*(s16 *) ((u8 *) record + 0x16)) = 0;
    (*(s16 *) ((u8 *) record + 0x18)) = 0;
    (*(s16 *) ((u8 *) record + 0x1A)) = 0;
    (*(s16 *) ((u8 *) record + 0x1C)) = 0;
    record->unk22 = 0;
    record->unk1E = (record->unk1E | 0x8000) & 0x9E48;
    Ovl11ResetTimedSpriteEffect((s16 *) &record->unk30[0xB0]);
    scheduleIndex = 0;
    emptyCode = 0x36;
    entryEnd = &record->unkE4[0].unk6;
    do {
        (*(s16 *) ((u8 *) entryEnd + -6)) = 0;
        (*(s16 *) ((u8 *) entryEnd + -4)) = 0;
        (*(s16 *) ((u8 *) entryEnd + -2)) = emptyCode;
        *entryEnd = emptyCode;
        entryEnd += 4;
        scheduleIndex += 1;
    } while (scheduleIndex < 0x1EU);
    return 1;
}
