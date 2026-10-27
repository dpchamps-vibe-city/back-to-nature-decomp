#ifndef OVL11_OBJECT_SPRITE_ANIMATION_COMMAND_H
#define OVL11_OBJECT_SPRITE_ANIMATION_COMMAND_H

#include "common.h"

/* ovl_11 primary sprite adapter; source/work storage must be initialized.
 * Frame is narrowed to s16, or read from source-0x258 when frameOrReuse == -1.
 * mode -1: clear animation index and work+0x5234 bit 0x100000, return 1.
 * mode 1: set that bit. With operand3 == 0, select a different animation and
 * return 0; on later calls with the same index, return 1/clear bit/index only
 * when sprite result flags & 0x300 != 0, otherwise return 0. With operand3
 * nonzero, select animation if needed and force the frame, returning 1.
 * Comparisons use the full signed animationId; animation/frame selection
 * masks to a byte, so callers should use valid byte-range indices.
 * Other modes install the shared deferred record at work+0x5214, writing
 * signed {animationId, frame, operand3-10, 9999} at +6/+8/+A/+C, clear bit
 * 0x100000, set auxiliary bit 0x10 and return 0. Its timing fields are not
 * range-checked. Does not step animation; shared record is overwritten and
 * non-reentrant. Original identity: ovl_11:800E9104. */
s32 Ovl11ObjectSpriteAnimationCommand(s16 mode, s16 animationId, s32 frameOrReuse, s32 operand3);

#endif
