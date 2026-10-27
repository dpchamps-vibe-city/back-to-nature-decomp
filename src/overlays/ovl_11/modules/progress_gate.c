/* Shared read-only progress gate (ovl_11:800EFA1C).
 * See ovl11_progress_gate.h for the exact predicate and units left unknown.
 * The condition-record engine is one client, not owner of this service.
 * Membership and boundary evidence: notes/file-groupings.md. */

#include "common.h"
#include "game_flags.h"
#include "ovl11_progress_gate.h"

s32 Ovl11TestProgressGate(s16 includeFlag20) {
    s32 flag25;
    char *work;

    flag25 = GetGameFlag(0x25U);
    if (flag25 == 1) {
        if (GetGameFlag(0xC9U) == flag25) {
            return 1;
        }
        work = (char *) &D_8006C838;
        if (*(u8 *) (work + 0x8000 + 0x6642) >= 0x5AU) {
            return 1;
        }
    }
    if (includeFlag20 != 0) {
        if (GetGameFlag(0x20U) == 1) {
            return 1;
        }
    }
    return 0;
}
