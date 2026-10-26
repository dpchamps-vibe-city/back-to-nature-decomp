/*
 * selection_candidate — reconstructed translation unit.
 *
 * Transactional candidate validation, result messages and selection-record
 * commit. Validation consumes shared flags and IDs only inside a saved-state
 * trial; commit re-runs that trial and publishes its result. Outcome handlers
 * belong here with the validators because their commit calls back to the
 * scanner. See ovl11_selection_candidate.h for sentinels and capacity rules.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_selection_candidate.h"
#include "game_types.h"
#include "screen_fade.h"
#include "message_window.h"
#include "game_flags.h"

s32 Ovl11ConsumeRequiredFlagBits(void *arg0);
s32 Ovl11ConsumeRequiredObjectIds(Ovl11IdAdjustmentObject *arg0);
s32 Ovl11AccumulateIdAdjustments(Ovl11IdAdjustmentObject *arg0, u16 *arg1);

s16 Ovl11ChooseEligibleSelection(u16 *outId, u16 *outValue) {
    u16 savedIds[8];
    u16 value;
    Ovl11IdAdjustmentObject *object;
    s32 index;
    s32 i;
    u16 savedFlags;
    u16 id;

    for (i = 0; i < 8; i++) {
        savedIds[i] = D_8012CF00_ids[i];
    }
    value = 0;
    savedFlags = D_801273EE_flags;
    id = 0;
    for (index = 0; ((Ovl11IdAdjustmentObject *) D_80050B5C_objects)[index].id != 0xE2; index++) {
        for (i = 0; i < 8; i++) {
            D_8012CF00_ids[i] = savedIds[i];
        }
        object = (Ovl11IdAdjustmentObject *) D_80050B5C_objects + index;
        D_801273EE_flags = savedFlags;
        value = object->value;
        if (Ovl11ConsumeRequiredFlagBits(object) &&
            Ovl11ConsumeRequiredObjectIds(object) &&
            Ovl11AccumulateIdAdjustments(object, &value)) {
            id = object->id;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        D_8012CF00_ids[i] = savedIds[i];
    }
    D_801273EE_flags = savedFlags;
    if (id == 0) {
        *outId = 0xE2;
        *outValue = 1;
        return 0xFE;
    }
    *outId = id;
    *outValue = value;
    for (index = 0; index < 0x50; index++) {
        if (((Ovl11SelectionRecords *) D_8006C838)->records[index][0] == id) {
            break;
        }
    }
    if (index == 0x50) {
        return 0xFF;
    }
    return (u8) index;
}

s32 Ovl11ConsumeRequiredFlagBits(void *object) {
    s32 bit = 0;
    s32 mask = 1;
    u16 requiredFlags = *(u16 *)((char *)object + 8);

    for (; bit < 12; bit++, mask = (mask << 1) & 0xFFFE) {
        if (requiredFlags & mask) {
            s32 availableFlags = D_801273EE_flags;
            if (availableFlags & mask) {
                D_801273EE_flags = mask ^ availableFlags;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

s32 Ovl11ClassifyRequiredId(u16 arg0);

s32 Ovl11ConsumeRequiredObjectIds(Ovl11IdAdjustmentObject *arg0) {
    s32 i;
    s32 j;
    u16 id;

    for (i = 0; i < 8; i++) {
        id = arg0->ids[i];
        if (id != 0) {
            switch (Ovl11ClassifyRequiredId(id)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; j < 8; j++) {
                    if (id == D_8012CF00_ids[j]) {
                        break;
                    }
                }
                if (j == 8) {
                    return 0;
                }
                D_8012CF00_ids[j] = 0;
                break;
            }
        }
    }
    return 1;
}

s32 Ovl11ApplyMappedIdAdjustment(Ovl11IdAdjustmentObject *arg0, s32 arg1, u16 *arg2);

s32 Ovl11AccumulateIdAdjustments(Ovl11IdAdjustmentObject *arg0, u16 *arg1) {
    s16 sum;
    s32 i;
    s32 j;
    u16 id;
    u16 mask;
    u16 *entry;
    u16 key;

    i = 0;
    mask = 1;
    sum = 0;
    for (; i < 12; i++, mask <<= 1) {
        id = 0;
        if (D_801273EE_flags & mask) {
            switch (mask) {
            case 1: id = 0x72; break;
            case 2: id = 0x73; break;
            case 4: id = 0x74; break;
            case 8: id = 0x75; break;
            case 0x10: id = 0x76; break;
            case 0x20: id = 0x77; break;
            case 0x40: id = 0x78; break;
            case 0x80: id = 0x17C; break;
            case 0x100: id = 0x17D; break;
            case 0x200: id = 0x17E; break;
            case 0x400: id = 0x17F; break;
            case 0x800: id = 0x180; break;
            }
            for (j = 0; arg0->adjustments[j].id != 0; j++) {
                if (arg0->adjustments[j].id == id) {
                    sum += (s8)arg0->adjustments[j].adjustment;
                    break;
                }
            }
            if (arg0->adjustments[j].id == 0) {
                return 0;
            }
        }
    }
    i = 0;
    entry = D_8012CF00_ids;
    for (; i < 8; i++, entry++) {
        key = *entry;
        if (key != 0) {
            switch (Ovl11ApplyMappedIdAdjustment(arg0, key, (u16 *)&sum)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; arg0->adjustments[j].id != 0; j++) {
                    if (arg0->adjustments[j].id == key) {
                        sum = (u16)sum + (s8)arg0->adjustments[j].adjustment;
                        break;
                    }
                }
                if (arg0->adjustments[j].id == 0) {
                    return 0;
                }
                break;
            }
        }
    }
    *arg1 += (u16)sum;
    return 1;
}

s32 Ovl11ClassifyRequiredId(u16 arg0) {
    s32 row;
    s32 i;
    s32 j;

    switch (arg0) {
    case 0x181:
        row = 0;
        break;
    case 0x182:
        row = 1;
        break;
    case 0x183:
        row = 2;
        break;
    case 0x184:
        row = 3;
        break;
    default:
        return 0;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++) {
            if (D_8012CF00_ids[i] == gOvl11RequiredIdClasses[row][j]) {
                D_8012CF00_ids[i] = 0;
                return 1;
            }
        }
    }
    return 2;
}

s32 Ovl11ApplyMappedIdAdjustment(Ovl11IdAdjustmentObject *object, s32 rawId, u16 *sum) {
    s32 index;
    s32 mappedId;
    s32 result;

    switch (rawId & 0xFFFF) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        mappedId = 0x181;
        break;
    case 0x8E:
    case 0x8F:
    case 0x90:
        mappedId = 0x182;
        break;
    case 0xE4:
        mappedId = 0x182;
        break;
    case 0xE3:
        mappedId = 0x183;
        break;
    case 0x8B:
    case 0x8C:
    case 0x8D:
        mappedId = 0x183;
        break;
    case 0x88:
    case 0x89:
    case 0x8A:
        mappedId = 0x184;
        break;
    case 0xE5:
        mappedId = 0x184;
        break;
    default:
        return 0;
    }

    for (index = 0; object->adjustments[index].id != 0; index++) {
        if (object->adjustments[index].id == mappedId) {
            *sum += (s8)object->adjustments[index].adjustment;
            break;
        }
    }
    result = 2;
    if (object->adjustments[index].id != 0) {
        result = 1;
    }
    return result;
}

extern void ovl_11_func_80103714(void);

void Ovl11SelectionUnchangedHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C0);
        if (GetMessageWindowStatus() == 2) {
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}

void Ovl11CommitSelection(void);
void ovl_11_func_80103714(void);

void Ovl11SelectionSameRecordHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C1);
        if (GetMessageWindowStatus() == 2) {
            Ovl11CommitSelection();
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}

void Ovl11SelectionNoCandidateHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C2);
        if (GetMessageWindowStatus() == 2) {
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}

/* Executable imports GetMessageWindowStatus and GetMessageWindowChoice. */

void Ovl11SelectionIncreaseHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C3);
        if (GetMessageWindowStatus() == 2) {
            if ((s16)GetMessageWindowChoice() == 1) {
                Ovl11CommitSelection();
            }
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}

/* Executable imports GetMessageWindowStatus and GetMessageWindowChoice. */

void Ovl11SelectionDecreaseHandler(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C4);
        if (GetMessageWindowStatus() == 2) {
            if ((s16)GetMessageWindowChoice() == 1) {
                Ovl11CommitSelection();
            }
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}

s16 Ovl11ChooseEligibleSelection(u16 *outId, u16 *outValue);

void Ovl11CommitSelection(void) {
    u16 id;
    u16 value;
    s32 index;
    s32 i;
    u8 *base;
    s32 offset;
    u16 *dest;

    index = Ovl11ChooseEligibleSelection(&id, &value);
    if (index == 0xFF) {
        for (i = 0; i < 0x50; i++) {
            if (((Ovl11SelectionRecords *)D_8006C838)->records[i][0] == 0) {
                index = (s16)i;
                break;
            }
        }
    }
    base = (u8 *)D_8006C838;
    offset = index * 0x16;
    dest = (u16 *)((u8 *)((Ovl11SelectionRecordArray *)D_8006C838)->records[0].ids + offset);
    i = 0;
    ((Ovl11SelectionRecordArray *)base)->records[index].id = id;
    ((Ovl11SelectionRecordArray *)base)->records[index].value = value;
    ((Ovl11SelectionRecordArray *)base)->records[index].flags = D_801273EE_flags;
    do {
        *dest++ = D_8012CF00_ids[i];
        i++;
    } while (i < 8);
    switch ((s16)id) {
    case 0xD6:
        SetGameFlag(0x5B, 1);
        break;
    case 0xD7:
        SetGameFlag(0x5C, 1);
        break;
    case 0xD1:
        SetGameFlag(0x5D, 1);
        break;
    }
}
