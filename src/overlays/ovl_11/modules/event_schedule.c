/*
 * Flag-selected schedules for the 37 resident object-event slots.
 * Initialization and time selection share the thirty-entry +E4 table.
 * See ovl11_event_schedule.h for the stream/base and clock contracts.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "game_flags.h"
#include "ovl11_event_schedule.h"

void *Ovl11ResolveScheduleStream(u16 *stream, s32 blobBase);

void Ovl11InitEventSchedules(s32 scriptBlobBase) {
    struct_80076220 *slot;
    u16 *source;
    u16 *stream;
    u16 *attributes;
    u32 slotIndex;
    u32 entryIndex;
    u32 value;

    slot = &gObjectEventSlotBase;
    for (slotIndex = 0; slotIndex < 0x25U; slotIndex++, slot++) {
        *(u16 *)slot->unk0 = slotIndex;
        slot->unk2C = 0x36;
        slot->unk2E = 0x36;
        *(s16 *)((u8 *)slot + 8) = 0;
        slot->unkA = 0;
        slot->unkC = 0;
        slot->unkE = 0;
        *(s16 *)((u8 *)slot + 0x10) = 0;
        *(s16 *)((u8 *)slot + 0x14) = 0;
        *(s16 *)((u8 *)slot + 0x16) = 0;
        *(s16 *)((u8 *)slot + 0x18) = 0;
        *(s16 *)((u8 *)slot + 0x1A) = 0;
        *(s16 *)((u8 *)slot + 0x1C) = 0;
        slot->unk22 = 0;
        *(s32 *)slot->unk28 = 0;
        slot->unk1E = (slot->unk1E | 0x8000) & 0x9E48;
        *(u16 *)slot->unk20 &= 0xDFFF;
        Ovl11ResetTimedSpriteEffect((s16 *)&slot->unk30[0xB0]);
        attributes = (u16 *)&slot->unkE4[0].unk2;
        for (entryIndex = 0; entryIndex < 0x1EU; entryIndex++) {
            slot->unkE4[entryIndex].unk0 = 0;
            attributes[entryIndex * 4] = 0;
            slot->unkE4[entryIndex].unk4 = 0x36;
            slot->unkE4[entryIndex].unk6 = 0x36;
        }
        stream = gOvl11EventScheduleStreams[slotIndex];
        if (stream != NULL) {
            source = Ovl11ResolveScheduleStream(stream, scriptBlobBase);
            if (source == NULL) {
                *(s32 *)slot->unk28 = 0;
            } else {
                *(s32 *)slot->unk28 = (s32)source - scriptBlobBase;
            }
            if (source != NULL) {
                for (entryIndex = 0; entryIndex < 0x1EU; entryIndex++, source += 4) {
                    slot->unkE4[entryIndex].unk0 = source[0];
                    attributes[entryIndex * 4] = source[1];
                    slot->unkE4[entryIndex].unk4 = source[2];
                    slot->unkE4[entryIndex].unk6 = source[3];
                }
            }
            value = *(u16 *)&slot->unkE4[0].unk2 >> 8;
            slot->unk2C = slot->unkE4[0].unk4;
            slot->unk2E = slot->unkE4[0].unk6;
            *(s16 *)((u8 *)slot + 0x14) = value;
            *(s16 *)((u8 *)slot + 0x16) = value;
        }
    }
}

/* FFFC escapes to a base-relative aligned offset; FFFB ends the stream.
 * Conditional arms test 15-bit game-flag IDs, with bit 15 inverting a test.
 * FFFE ends an arm. Keep the two distinct skip sites: one consumes an
 * alternate offset, the other advances from its terminator. */
void *Ovl11ResolveScheduleStream(u16 *stream, s32 blobBase) {
    s32 flag;
    s32 scanArm;
    s32 failed;
    u16 word;

    while (1) {
        scanArm = 1;
        if (*stream == 0xFFFC) {
            stream += 1;
            if (*stream == 0xFFFB) {
                return NULL;
            }
            word = *stream;
            return (void *) (blobBase + (word & 0xFFFC));
        }
        failed = 0;
        stream += 1;
        for (;;) {
            flag = GetGameFlag(*stream & 0x7FFF);
            if (*stream & 0x8000) {
                if (flag != 0) {
                    flag = 0;
                } else {
                    flag = 1;
                }
            }
            if (flag == 1) {
                stream += 1;
                word = *stream;
                if (word != 0xFFFE) {
                    goto nextTest;
                }
                stream += 1;
                if (failed != 0) {
                    goto skipTerminator;
                }
                word = *stream;
                if (word == 0xFFFB) {
                    return NULL;
                }
                return (void *) (blobBase + (word & 0xFFFC));
            }
            stream += 1;
            word = *stream;
            failed = 1;
            if (word == 0xFFFE) {
                stream += 2;
                scanArm = 0;
                word = *stream;
            }
            if (word != 0xFFFB) {
                goto nextTest;
            }
        skipTerminator:
            stream += 1;
            scanArm = 0;
        nextTest:
            if (scanArm != 0) {
                continue;
            }
            break;
        }
    }
}

void Ovl11SelectScheduleEntryForTime(s16 timeOfDay) {
    struct_80076220 *slot;
    struct_80076220_entry *entry;
    u8 *attributeBase;
    u16 *firstBase;
    u16 *secondBase;
    u8 *attribute;
    u16 *first;
    u16 *second;
    s16 key;
    s32 entryIndex;
    s32 slotIndex;

    if (timeOfDay != 0x168) {
        slot = &gObjectEventSlotBase;
        for (slotIndex = 0; slotIndex < 37; slotIndex++, slot++) {
            entry = (struct_80076220_entry *)((u8 *)slot + 0xE4);
            for (entryIndex = 0; entryIndex < 30; entryIndex++, entry++) {
                key = entry->unk0;
                if (key < timeOfDay) {
                    continue;
                }
                if (timeOfDay < key) {
                    slot->unk22 = (entryIndex < 0) ? 0 : entryIndex;
                } else {
                    slot->unk22 = entryIndex;
                }
                slot->unkC = 0;
                attributeBase = (u8 *)slot + 0xE6;
                attribute = (void *)((u8 *)attributeBase + (u32)slot->unk22 * 8);
                slot->unkE = *attribute;
                firstBase = (u16 *)((u8 *)slot + 0xE8);
                first = (void *)((u8 *)firstBase + (u32)slot->unk22 * 8);
                slot->unk2C = *first;
                secondBase = (u16 *)((u8 *)slot + 0xEA);
                second = (void *)((u8 *)secondBase + (u32)slot->unk22 * 8);
                slot->unk2E = *second;
                break;
            }
        }
    }
}
