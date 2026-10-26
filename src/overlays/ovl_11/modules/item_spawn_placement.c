/*
 * item_spawn_placement — reconstructed translation unit.
 *
 * Item placement from area rules and clock windows, positioned random
 * attempts, and the shared spawn-window latch.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_item_spawn_placement.h"
#include "random.h"
#include "ovl11_tagged_record_choice.h"

u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
void Ovl11SpawnArea108Item(void);
void Ovl11SpawnGatedAreaItems(void);
s32 ovl_11_func_800D2E20(void);

void Ovl11SpawnTimedItemPlacements(void) {
    char *base;
    char *initial_base;
    char *final_base;
    char *far_base;
    char *special_base;
    char *last_base;
    char *object;
    s16 *entry;
    s32 i;
    s32 hour;
    s16 id;
    s16 mask;
    s32 unused[2];

    Ovl11SpawnArea108Item();
    Ovl11SpawnGatedAreaItems();
    initial_base = (char *)D_8006C838;
    if (*(s16 *)(initial_base + 0xE4C8) != 0) {
        return;
    }
    mask = ovl_11_func_800D2E20();
    entry = (s16 *)gOvl11ItemPlacementRules;
    i = 0;
    for (; i < 17; i++) {
        if (!((gOvl11ItemSpawnRuleMask >> i) & 1)) {
            far_base = (char *)&D_8007AFF0;
            if (*(s16 *)(far_base + 0x25476) == entry[1] &&
                (mask & (u16)entry[2])) {
                id = entry[0];
                if (id != 0x163 && id != 0x166 && Rand(10) != 0) {
                    entry += 12;
                    continue;
                }
                object = (char *)ovl_11_func_800CE744(entry[0], -1);
                if (object != 0) {
                    *(s32 *)(object + 0x38) = *(s32 *)(entry + 4);
                    *(s32 *)(object + 0x3C) = *(s32 *)(entry + 6);
                    *(s32 *)(object + 0x40) = *(s32 *)((char *)entry + 0x10);
                    if (Rand(2) != 0) {
                        *(s16 *)(object + 0x22) = 0;
                    } else {
                        *(s16 *)(object + 0x22) = 3;
                    }
                    id = entry[0];
                    switch (id) {
                    case 0x163:
                    case 0x166:
                        break;
                    default:
                        gOvl11ItemSpawnRuleMask = gOvl11ItemSpawnRuleMask | (1 << i);
                        break;
                    }
                }
            }
        }
        entry += 12;
    }
    /* Keep the separate work-base reads and shared exit: allocation and RNG
     * calls separate these phases, and each gate observes the current state. */
    special_base = (char *)&D_8007AFF0;
    if (*(s16 *)(special_base + 0x25476) != 1) {
        return;
    }
    if (*(s16 *)(*(char **)(special_base + 0x2538C) + 2) == 2) {
        object = (char *)D_8006C838;
        hour = *(s16 *)(object + 0x44C0);
        if (hour < 6) {
            goto special_done;
        }
        if (hour >= 9) {
            goto special_done;
        }
        if (Rand(10) == 0 && *(s16 *)(object + 0x44BA) != 3 &&
            !((u16)gOvl11ItemSpawnRuleMask >> 15)) {
            i = 0;
            base = object;
            for (; i < 3; i++) {
                object = (char *)ovl_11_func_800CE744(0x154, -1);
                if (object != 0) {
                    *(s32 *)(object + 0x38) = gOvl11Timed154SpawnPositions[i][0];
                    *(s32 *)(object + 0x3C) = gOvl11Timed154SpawnPositions[i][1];
                    *(s32 *)(object + 0x40) = gOvl11Timed154SpawnPositions[i][2];
                    *(s16 *)(object + 0x22) = i;
                }
                *(s32 *)(base + 0x44F8) |= 0x8000;
            }
        }
    }
special_done:
    last_base = (char *)&D_8007AFF0;
    if (*(s16 *)(last_base + 0x25476) == 1 &&
        *(s16 *)(*(char **)(last_base + 0x2538C) + 2) != 12) {
        final_base = (char *)D_8006C838;
        hour = *(s16 *)(final_base + 0x44C0);
        if (hour < 18) {
            return;
        }
        if (hour >= 23) {
            return;
        }
        if ((*(s32 *)(final_base + 0x44F8) & 0x1800000) == 0x800000) {
            ovl_11_func_800CE744(0x15B, -1);
        }
    }
}

void Ovl11SpawnArea108Item(void) {
    char *far_base;
    char *s0;
    s32 buf[4];

    far_base = (char *)&D_8007AFF0;
    switch (*(s16 *)(far_base + 0x25476)) {
    case 0x1F:
        buf[0] = gOvl11Area108SpawnPositions[0];
        buf[1] = gOvl11Area108SpawnPositions[1];
        buf[2] = gOvl11Area108SpawnPositions[2];
        buf[3] = gOvl11Area108SpawnPositions[3];
        break;
    case 0x1B:
        buf[0] = gOvl11Area108SpawnPositions[4];
        buf[1] = gOvl11Area108SpawnPositions[5];
        buf[2] = gOvl11Area108SpawnPositions[6];
        buf[3] = gOvl11Area108SpawnPositions[7];
        break;
    case 0x1C:
    case 0x1E:
    default:
        return;
    }
    s0 = (char *)ovl_11_func_800CE744(0x108, -1);
    if (s0 == 0) {
        return;
    }
    *(s32 *)(s0 + 0x38) = buf[0];
    *(s32 *)(s0 + 0x3C) = buf[1];
    *(s32 *)(s0 + 0x40) = buf[2];
    if (Rand(2) != 0) {
        *(u16 *)(s0 + 0x22) = 0;
    } else {
        *(u16 *)(s0 + 0x22) = 3;
    }
}

void Ovl11SpawnGatedAreaItems(void) {
    char *base;
    char *far_base;
    s16 *entry;
    char *s0;
    s32 i;
    s16 a2;
    u16 v;

    entry = gOvl11AreaItemSpawnRules;
    far_base = (char *)D_8009AFF0;
    base = (char *)D_8006C838;
    for (i = 5; i >= 0; i--) {
        a2 = *(s16 *)(far_base + 0x5476);
        if (a2 != *(s16 *)(entry + 1)) {
            entry += 0xC;
            continue;
        }
        if (*entry != 0x17A || ((v = *(u16 *)(base + 0x99D8)) != 0 &&
            (entry[2] == 0 || *(u16 *)(base + 0x99D8) == a2))) {
            s0 = (char *)ovl_11_func_800CE744((s16)*(u16 *)entry, -1);
            if (s0 != 0) {
                *(s32 *)(s0 + 0x38) = *(s32 *)(entry + 4);
                *(s32 *)(s0 + 0x3C) = *(s32 *)(entry + 6);
                *(s32 *)(s0 + 0x40) = *(s32 *)(entry + 8);
                if (Rand(2) != 0) {
                    *(u16 *)(s0 + 0x22) = 0;
                } else {
                    *(u16 *)(s0 + 0x22) = 2;
                }
            }
        }
        entry += 0xC;
    }
}

void Ovl11TrySpawnPositionedItem(Ovl11ItemSpawnPosition position, s32 mode) {
    char *work;
    char *spawned;

    work = (char *)D_8006C838;
    if (*(s16 *)(work + 0xE4C8) != 0) {
        return;
    }
    if (Rand(0x32) != 0) {
        return;
    }
    if (*(s16 *)(work + 0x44BA) == 3) {
        return;
    }
    spawned = (char *)ovl_11_func_800CE744(mode == 1 ? 0x156 : 0x157, -1);
    if (spawned == 0) {
        return;
    }
    *(s32 *)(spawned + 0x38) = position.x;
    *(s32 *)(spawned + 0x3C) = position.y;
    *(s32 *)(spawned + 0x40) = position.z;
    if (mode == 1) {
        *(u16 *)(spawned + 0x22) = *(u16 *)(work + 0x5200);
    } else {
        *(u16 *)(spawned + 0x22) = Rand(2) != 0 ? 0 : 3;
    }
}

/* This producer only latches the bit; the coordinator owns its time-window
 * test. Keep the two record queries short-circuited in their original order. */
void Ovl11RollSpawnWindowLatch(void) {
    char *base;

    if ((Ovl11ChooseTaggedRecordIndex(0) >= 0) || (Ovl11ChooseTaggedRecordIndex(1) >= 0)) {
        base = (char *)D_8006C838;
        if ((*(s16 *)(base + 0xE4C8) == 0) && (*(s16 *)(base + 0x44BA) != 3) && (Rand(0xA) == 0)) {
            *(s32 *)(base + 0x44F8) |= 0x800000;
        }
    }
}
