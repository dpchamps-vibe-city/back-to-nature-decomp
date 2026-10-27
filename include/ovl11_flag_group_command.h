#ifndef OVL11_FLAG_GROUP_COMMAND_H
#define OVL11_FLAG_GROUP_COMMAND_H

#include "common.h"

/* Assign game flags (4, 5, B0) as a group:
 *   mode 0: (0, 1, 1); mode 1: (1, 0, 0); every other mode: (0, 0, 0).
 * Always return 1. The remaining three object-command operands are ignored.
 * Other game flags are untouched. These combinations are used by message
 * and object-command clients; their gameplay meanings are not established.
 */
u16 Ovl11SetFlagGroupMode(s16 mode, s32 reserved1, s32 reserved2,
                         s32 reserved3);

#endif
