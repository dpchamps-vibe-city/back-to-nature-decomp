#ifndef OVL11_MASTER_VOLUME_RAMP_COMMAND_H
#define OVL11_MASTER_VOLUME_RAMP_COMMAND_H

#include "common.h"

/* ovl_11 command adapter for the resident master-volume ramp.
 * begin != 0 starts fade-in when fadeIn != 0, otherwise fade-out, and returns
 * 1 immediately. ticks is signed and forwarded without validation; see
 * master_volume.h for ramp units/range constraints. begin == 0 only polls,
 * returning 1 iff the shared ramp state is zero; other operands are ignored.
 * Does not advance the ramp. unused retains the fourth operand supplied by
 * the interpreter and both original EC52C calls. Original: ovl_11:800E9AFC. */
s32 Ovl11MasterVolumeRampCommand(s16 fadeIn, s16 ticks, s32 begin, s32 unused);

#endif
