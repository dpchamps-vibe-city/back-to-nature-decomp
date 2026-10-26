#ifndef OVL11_BLOCK_COPY_H
#define OVL11_BLOCK_COPY_H

#include "common.h"

/*
 * Forward copies with unsigned ELEMENT counts, not byte sizes. Word buffers
 * require 4-byte alignment; halfword buffers require 2-byte alignment. Zero
 * count touches neither pointer. No bounds checks or overlap adjustment:
 * overlapping destinations above the source can overwrite unread elements.
 */
void Ovl11CopyWordBlock(s32 *destination, s32 *source, u32 count);
void Ovl11CopyHalfwordBlock(u16 *destination, u16 *source, u32 count);

#endif
