/*
 * scene_mode_query — reconstructed translation unit.
 *
 * Read-only object-command predicate for scene mode 3 in shared far-work
 * state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_scene_mode_query.h"

s32 Ovl11IsSceneMode3(void) {
    char *farWork = (char *)&D_8007AFF0;

    return *(s32 *)(farWork + 0x254A0) == 3;
}
