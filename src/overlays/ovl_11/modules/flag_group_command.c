/*
 * flag_group_command — reconstructed translation unit.
 *
 * Object-command selection of the three established game-flag combinations
 * for flags 4, 5 and B0.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_flags.h"
#include "ovl11_flag_group_command.h"

u16 Ovl11SetFlagGroupMode(s16 mode, s32 reserved1, s32 reserved2,
                       s32 reserved3) {
    switch (mode) {
    case 0:
        SetGameFlag(4, 0);
        SetGameFlag(5, 1);
        SetGameFlag(0xB0, 1);
        break;
    case 1:
        SetGameFlag(4, 1);
        SetGameFlag(5, 0);
        SetGameFlag(0xB0, 0);
        break;
    case 2:
    default:
        SetGameFlag(4, 0);
        SetGameFlag(5, 0);
        SetGameFlag(0xB0, 0);
        break;
    }
    return 1;
}
