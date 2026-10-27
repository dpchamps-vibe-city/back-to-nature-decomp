/*
 * message_window_gate_command — reconstructed translation unit.
 *
 * Object-command message-window gate: control game flag 3 and optionally
 * request a mode.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_flags.h"
#include "ovl11_message_window_gate_command.h"
#include "ovl11_flag_group_command.h"

s32 Ovl11MessageWindowGateCommand(s16 release, s16 modeOverride,
                       s32 reserved2, s32 reserved3) {
    s32 complete;
    s16 requestedMode;

    complete = 0;
    requestedMode = 0;
    if (release == 0) {
        SetGameFlag(3U, 1U);
    } else if (gMessageWindowPhase == 0) {
        SetGameFlag(3U, 0U);
        complete = 1;
        requestedMode = 2;
    }
    if (modeOverride != -1) {
        if (modeOverride == 2) {
            requestedMode = 2;
        }
        Ovl11SetFlagGroupMode(requestedMode, 0, 0, 0);
    }
    return complete;
}
