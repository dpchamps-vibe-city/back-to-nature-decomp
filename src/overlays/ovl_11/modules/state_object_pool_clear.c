/*
 * state_object_pool_clear — reconstructed translation unit.
 *
 * Clear eligible records in the six-entry state-object pool, preserving the
 * two independent record/status protection gates.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_state_object_pool_clear.h"
#include "game_types.h"
#include "ovl11_object_record_clear.h"

void Ovl11ClearEligibleStateObjects(s32 includeFlag20000) {
    UnkStruct80075BC4 *object;
    s32 recordProtection;
    char *work;
    s32 lastIndex;
    s32 statusProtection;

    object = gStateObjectPool;
    recordProtection = 0x20000;
    work = (char *)&D_8006C838;
    lastIndex = 5;
    statusProtection = 0x01000000;

    do {
        /* Both protection gates apply; the argument bypasses only the first. */
        if ((!(*(s32 *)((u8 *)object + 0x34) & recordProtection) ||
             (includeFlag20000 != 0)) &&
            (!(*(u16 *)((u8 *)object + 0x00) & 0x15B) ||
             !(*(s32 *)(work + 0x44F8) & statusProtection))) {
            Ovl11ClearB0ObjectRecord((s32)object);
        }
        lastIndex -= 1;
        object += 1;
    } while (lastIndex >= 0);
}
