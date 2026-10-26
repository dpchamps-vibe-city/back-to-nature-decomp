/*
 * Ten-slot selection-record input and access (ovl_11:8011D734–8011D9DC).
 * The leading ten words mark occupied entries; signed index +5C selects the
 * ID table at +28 or message table at +46. Callers also use the occupied-row
 * count to place a cursor beside the compressed display list.
 *
 * Preserve the two established, word-aligned 0x60-byte by-value views. The
 * navigation direction is a separate stack argument after the copied record,
 * not an extra field in that record. No storage or private linkage is added.
 */

#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_selection_record_access.h"

s32 Ovl11ProcessSelectionRecordInput(Ovl11D548Arg *record) {
    s16 *index;
    s32 action;
    s32 repeatButtons;
    s32 pressedButtons;

    action = 0;
    index = &record->index;
    repeatButtons = ((PadInputState *)gPad0Input)->repeatButtons;
    pressedButtons = ((PadInputState *)gPad0Input)->pressedButtons;
    if (repeatButtons & 0x1000) {
        *index = Ovl11FindOccupiedSelectionIndex(*record, 0);
        PlaySoundEffect(5);
    } else if (repeatButtons & 0x4000) {
        *index = Ovl11FindOccupiedSelectionIndex(*record, 1);
        PlaySoundEffect(5);
    } else {
        if (pressedButtons & 0x40) {
            action = 1;
            PlaySoundEffect(0);
        } else if (pressedButtons & 0x10) {
            if (gOvl11InteractionDrawMode != 7) {
                action = 2;
                PlaySoundEffect(0);
            }
        } else if (pressedButtons & 0x20) {
            action = 3;
            PlaySoundEffect(1);
        }
    }
    return action;
}

s16 Ovl11FindOccupiedSelectionIndex(Ovl11D548Arg record, s32 direction) {
    s16 candidate;
    s32 distance;

    distance = 1;
    for (; distance < 10; distance++) {
        if (direction == 1) {
            candidate = record.index + distance;
        } else {
            candidate = record.index - distance;
        }
        candidate = (candidate >= 10) ? candidate - 10 : candidate;
        candidate = (candidate < 0) ? candidate + 10 : candidate;
        if (*(s32 *)((char *)&record + ((s32)candidate << 2)) != 0) {
            return candidate;
        }
    }
    return record.index;
}

s32 Ovl11CountPrecedingSelectionEntries(StructD548 record) {
    s32 count = 0;
    s32 limit = (s32)record.index;
    s32 nextCountQ16;
    s32 oneQ16;
    s32 countQ16;
    s32 i;
    u32 *entry;

    /* The original accumulates occupied rows in the high halfword. Keep
     * that arithmetic and its signed shift rather than replacing it with
     * a plain increment, including behavior outside the normal 0..9 index. */
    if (limit > 0) {
        entry = (u32 *)&record;
        nextCountQ16 = 0x10000;
        oneQ16 = 0x10000;
        for (i = 0; i < limit; i++) {
            if (*entry != 0) {
                countQ16 = nextCountQ16;
                nextCountQ16 += oneQ16;
                count = countQ16 >> 16;
            }
            entry++;
        }
    }
    return count;
}

s16 Ovl11GetSelectionEntryId(StructD548 record) {
    return record.data[record.index];
}

s16 Ovl11GetSelectionMessageId(StructD548 record) {
    s16 *messages = &record.data[15];
    return messages[record.index];
}
