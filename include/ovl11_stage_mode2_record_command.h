#ifndef OVL11_STAGE_MODE2_RECORD_COMMAND_H
#define OVL11_STAGE_MODE2_RECORD_COMMAND_H

#include "common.h"

/* Low halfword zero: request ID 15E through the item-record allocator.
 * Nonzero: clear the singleton's F0-byte record. Always return 1, including
 * an unsuccessful acquisition (e.g. an already occupied slot). This is the
 * object-program command adapter, not an allocation-success query.
 */
s32 Ovl11LoadOrClearStageMode2Record(s32 control);

#endif
