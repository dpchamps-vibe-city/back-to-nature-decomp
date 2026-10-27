/*
 * primary_object_flag29 — reconstructed translation unit.
 *
 * Read-only query for the absence of primary-object flag 0x20000000. It
 * exposes a shared status bit without changing the object or claiming private
 * ownership.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_primary_object_flag29.h"

s32 Ovl11IsPrimaryObjectFlag29Clear(void) {
    s32 maskedFlag = gPrimaryObjectFlags & 0x20000000;
    return maskedFlag == 0;
}
