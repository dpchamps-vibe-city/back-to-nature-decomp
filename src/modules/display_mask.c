/*
 * display_mask — reconstructed translation unit.
 *
 * GPU display-output gate. These complementary wrappers bracket auxiliary
 * sprite-display setup; see display_mask.h for the public contract.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "display_mask.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void EnableDisplayOutput(void) {
    SetDispMask(1);
}

void DisableDisplayOutput(void) {
    SetDispMask(0);
}
