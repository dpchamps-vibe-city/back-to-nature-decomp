/*
 * Selection from the paired tagged-record families: twenty 0xB8-byte records
 * and ten 0xB4-byte records. Both scans use the same ID/key/flags/position
 * prefix; they differ in eligibility and in how they select a winner.
 *
 * The storage belongs to the engine, not this unit. No linkage or ownership
 * is inferred from these two complementary readers. Membership and the
 * evidence-limited outer boundaries are recorded in notes/file-groupings.md.
 * Single-iteration blocks preserve this compiler's loop/control-flow shape;
 * flattening them changes the emitted code despite identical C semantics.
 */
#include "common.h"
#include "psyq/memory.h"
#include "random.h"
#include "game_types.h"
#include "ovl11_record_selection.h"

s32 Ovl11ChooseEligibleTaggedRecord(u16 family) {
    u8 eligibleIndices[20];
    s32 count;
    s32 index;
    s32 limit;
    u8 *record;
    u8 *largeBase;
    u8 *smallBase;
    s32 smallOffset;
    s32 largeOffset;
    u32 excludedFlags;
    s32 requiredKey;

    memset(eligibleIndices, 0xFF, 20);
    count = 0;
    limit = 10;
    requiredKey = 1;
    if (family == 0) {
        limit = 20;
    }
    index = 0;
    if (limit != 0) {
        excludedFlags = 0x20400;
        largeBase = (u8 *)&gTaggedRecordBase;
        smallBase = largeBase - 0x708;
        smallOffset = 0;
        largeOffset = 0;
        /* Keep the two offsets separate: each family has its own stride. */
        do {
            if (family == 0) {
                record = (u8 *)(largeOffset + (u32)largeBase);
            } else {
                record = (u8 *)(smallOffset + (u32)smallBase);
            }
            do {
                do {
                    if (*(u32 *)(record + 0x34) & excludedFlags) {
                        goto next_record;
                    }
                    if (*(s16 *)(record + 0x30) != requiredKey) {
                        goto next_record;
                    }
                } while (0);
                if (*(u16 *)record == 0) {
                    goto next_record;
                }
            } while (0);
            eligibleIndices[count++] = index;
next_record:
            smallOffset += 0xB4;
            largeOffset += 0xB8;
            index++;
        } while (index < limit);
    }
    if (count == 0) {
        return -1;
    }
    return eligibleIndices[Rand(count & 0xFFFF)];
}

s32 Ovl11FindTaggedRecordInBox(Ovl11PaddedVec3 position, s16 key,
                              s32 radius, s32 *category, void **outRecord) {
    s32 result;
    u8 *record;
    s32 minZ;
    s32 minX;
    s16 requiredKey;
    s32 index;
    u8 *largeBase;
    s32 extent;
    s32 maxX;
    s32 maxZ;

    result = -1;
    index = 0;
    largeBase = (u8 *)&gTaggedRecordBase;
    requiredKey = key;
    extent = radius;
    minX = position.vx - extent;
    maxX = position.vx + extent;
    minZ = position.vz - extent;
    maxZ = position.vz + extent;
    do {
        do {
            record = largeBase + index * 0xB8;
            if (*(u16 *)record == 0 || requiredKey != *(s16 *)(record + 0x30)) {
                goto next_large;
            }
        } while (0);
        if ((*(s32 *)(record + 0x34) & 0x20000) != 0) {
            goto next_large;
        }
        do {
            if (!(minX < *(s32 *)(record + 0x38) && *(s32 *)(record + 0x38) < maxX)) {
                goto next_large;
            }
            if (!(minZ < *(s32 *)(record + 0x40) && *(s32 *)(record + 0x40) < maxZ)) {
                goto next_large;
            }
            do {
                do {
                    result = index;
                } while (0);
            } while (0);
        } while (0);
        if (*(s32 *)(record + 0x34) & 0x10000) {
            *category = 0;
            *outRecord = record;
        } else {
            *category = 1;
            *outRecord = record;
        }
        break;
next_large:
        index++;
    } while (index < 0x14);
    if (result == -1) {
        record = (u8 *)&gSmallTaggedRecordBase;
        index = 0;
        do {
            do {
                if (*(u16 *)record == 0 || requiredKey != *(s16 *)(record + 0x30)) {
                    goto next_small;
                }
            } while (0);
            if ((*(s32 *)(record + 0x34) & 0x20000) != 0) {
                goto next_small;
            }
            do {
                if (!(minX < *(s32 *)(record + 0x38) && *(s32 *)(record + 0x38) < maxX)) {
                    goto next_small;
                }
                if (!(minZ < *(s32 *)(record + 0x40) && *(s32 *)(record + 0x40) < maxZ)) {
                    goto next_small;
                }
                do {
                    do {
                        result = index;
                    } while (0);
                } while (0);
            } while (0);
            *category = 2;
            *outRecord = record;
            break;
next_small:
            index++;
            record += 0xB4;
        } while (index < 0xA);
    }
    return result;
}
