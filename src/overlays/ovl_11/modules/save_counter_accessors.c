/*
 * Four persistent save counters, identified by the EXE script accessor IDs.
 * Each add/take pair is kept in original order. The intervening save cell
 * at 80070D3C belongs to the aged-item service, not this counter family.
 * See ovl11_save_counters.h for kind mappings and wrap-before-cap behavior.
 * Membership and boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_save_counters.h"

void Ovl11AddSaveCounter19(u16 kind, u16 amount) {
    u16 *counter = &gSaveCounter19;
    u16 value;

    switch (kind) {
    case 0:
        gSaveCounter19 = gSaveCounter19 + 1;
        break;
    case 1:
        gSaveCounter19 = gSaveCounter19 + 1;
        break;
    case 2:
        gSaveCounter19 = gSaveCounter19 + 6;
        break;
    case 3:
        gSaveCounter19 = amount + gSaveCounter19;
        break;
    default:
        *counter = *counter + 1;
        break;
    }
    value = *counter;
    if (value > 999) {
        value = 999;
    }
    *counter = value;
}

s32 Ovl11TakeSaveCounter19(u16 amount) {
    u16 current;

    current = gSaveCounter19;
    if (current < amount)
        return 0;
    gSaveCounter19 = current - amount;
    return 1;
}

void Ovl11AddSaveCounter18(u16 kind, u16 amount) {
    u16 *counter = &gSaveCounter18;
    u16 value;

    switch (kind) {
    case 0:
        gSaveCounter18 = gSaveCounter18 + 1;
        break;
    case 1:
        gSaveCounter18 = gSaveCounter18 + 1;
        break;
    case 2:
        gSaveCounter18 = amount + gSaveCounter18;
        break;
    default:
        *counter = *counter + 1;
        break;
    }
    value = *counter;
    if (value > 999) {
        value = 999;
    }
    *counter = value;
}

s32 Ovl11TakeSaveCounter18(u16 amount) {
    u16 current;

    current = gSaveCounter18;
    if (current < amount)
        return 0;
    gSaveCounter18 = current - amount;
    return 1;
}

void Ovl11AddSaveCounter21(u16 kind, u16 amount) {
    u16 *counter = &gSaveCounter21;
    u16 value;

    switch (kind) {
    case 0:
        gSaveCounter21 = gSaveCounter21 + 1;
        break;
    case 1:
        gSaveCounter21 = amount + gSaveCounter21;
        break;
    default:
        {
            u16 previous;
            previous = *counter;
            *counter = previous + 1;
        }
        break;
    }
    value = *counter;
    if (value > 999) {
        value = 999;
    }
    *counter = value;
}

s32 Ovl11TakeSaveCounter21(u16 amount) {
    u16 current;

    current = gSaveCounter21;
    if (current < amount)
        return 0;
    gSaveCounter21 = current - amount;
    return 1;
}

void Ovl11AddSaveCounter20(u16 kind, u16 amount) {
    u16 *counter = &gSaveCounter20;
    u16 value;

    switch (kind) {
    case 0:
        gSaveCounter20 = gSaveCounter20 + 1;
        break;
    case 1:
        gSaveCounter20 = gSaveCounter20 + 10;
        break;
    case 2:
        gSaveCounter20 = amount + gSaveCounter20;
        break;
    default:
        *counter = *counter + 1;
        break;
    }
    value = *counter;
    if (value > 999) {
        value = 999;
    }
    *counter = value;
}

s32 Ovl11TakeSaveCounter20(u16 amount) {
    u16 current;

    current = gSaveCounter20;
    if (current < amount)
        return 0;
    gSaveCounter20 = current - amount;
    return 1;
}
