/* Shared reset for caller-owned 0xB0-byte object records. No global state.
 * The explicit leading-halfword store is the original constructor idiom;
 * retain it even though the preceding clear already zeroes that field. */

#include "common.h"
#include "ovl11_object_record_clear.h"
#include "psyq/memory.h"

void Ovl11ClearB0ObjectRecord(s32 recordAddress) {
    memset(recordAddress, 0, 0xB0);
    *(s16 *)recordAddress = 0;
}
