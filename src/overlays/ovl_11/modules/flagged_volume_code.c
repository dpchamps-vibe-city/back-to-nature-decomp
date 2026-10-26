/*
 * flagged_volume_code — reconstructed translation unit.
 *
 * Query a flagged interaction volume at a narrowed three-word position,
 * retain its address, and expose only the four recognized volume codes.
 *
 * Original ovl_11:800C79F8-800C7AC0. This is the three-word position
 * adapter for the halfword-coordinate volume scanner, not a new volume
 * owner. See notes/file-groupings.md for the conservative unit boundaries.
 */

#include "common.h"
#include "ovl11_flagged_volume_code.h"
#include "ovl11_interaction_volume_query.h"

typedef struct {
    u16 x;
    u16 y;
    u16 z;
} FlaggedVolumePosition16;

s32 Ovl11QueryFlaggedVolumeCode(Vec3 *position) {
    FlaggedVolumePosition16 narrowedPosition;
    char *farState;
    void *volume;
    u16 code;

    farState = (char *) &D_8007AFF0;
    /* Keep the low halfword of each coordinate, without fixed-point scaling. */
    narrowedPosition.x = *(u16 *) ((u8 *) position + 0);
    narrowedPosition.y = *(u16 *) ((u8 *) position + 4);
    narrowedPosition.z = *(u16 *) ((u8 *) position + 8);
    gOvl11LastFlaggedVolumeAddress = 0;
    volume = Ovl11FindFlaggedVolume(*(u8 **) (farState + 0x25388), (s16 *) &narrowedPosition);
    gOvl11LastFlaggedVolumeAddress = (s32) volume;
    if (volume != 0) {
        code = *(u16 *) ((u8 *) volume + 2);
        switch (code) {
        case 0x26:
            return 0x26;
        case 0x2B:
            return 0x2B;
        case 0x2A:
            return 0x2A;
        case 0x2C:
            return 0x2C;
        default:
            return 0;
        }
    }
    return 0;
}
