#ifndef OVL11_OBJECT_RECORD_CLEAR_H
#define OVL11_OBJECT_RECORD_CLEAR_H

#include "common.h"

/* Clear exactly 0xB0 bytes and leave the leading signed-halfword state zero.
 * recordAddress must name writable, halfword-aligned storage of that size.
 * The integer-address ABI is retained for the existing record views. No
 * allocation, sprite release or other resource teardown is performed. */
void Ovl11ClearB0ObjectRecord(s32 recordAddress);

#endif
