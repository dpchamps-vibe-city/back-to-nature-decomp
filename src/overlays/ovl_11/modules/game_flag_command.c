/*
 * game_flag_command — reconstructed translation unit.
 *
 * Set or test one resident game flag through the object-program command
 * interface.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_flags.h"
#include "ovl11_game_flag_command.h"

s32 Ovl11GameFlagCommand(s16 testOnly, s16 enabled, s32 flagId) {
    if (testOnly == 0) {
        SetGameFlag(flagId & 0xFFFF, enabled != 0);
        return 1;
    }
    return GetGameFlag(flagId & 0xFFFF);
}
