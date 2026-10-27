/*
 * master_volume_ramp_command — reconstructed translation unit.
 *
 * Begin a shared master-volume fade or poll the ramp state through the
 * object-command interface.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "master_volume.h"
#include "ovl11_master_volume_ramp_command.h"

s32 Ovl11MasterVolumeRampCommand(s16 fadeIn, s16 ticks, s32 begin, s32 unused) {
    s32 result;

    result = 1;
    if (begin != 0) {
        if (fadeIn != 0) {
            BeginMasterVolumeFadeIn((s32) ticks);
        } else {
            BeginMasterVolumeFadeOut((s32) ticks);
        }
    } else {
        result = GetMasterVolumeRampState() == 0;
    }
    return result;
}
