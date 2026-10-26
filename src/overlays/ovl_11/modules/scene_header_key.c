/*
 * scene_header_key — reconstructed translation unit.
 *
 * Compare a two-halfword key with the bound scene header. Return zero for an
 * exact pair match and one for a mismatch; no state is changed.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_scene_header_key.h"

s32 Ovl11SceneHeaderKeyDiffers(s16 *key) {
    char *work = (char *)&D_8007AFF0;
    s16 *header = *(s16 **)(work + 0x25388);

    if (header[0] == key[0] && header[1] == key[1])
        return 0;
    return 1;
}
