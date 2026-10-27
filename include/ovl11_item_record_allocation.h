#ifndef OVL11_ITEM_RECORD_ALLOCATION_H
#define OVL11_ITEM_RECORD_ALLOCATION_H
#include "common.h"

/* Dispatch an item code to its record family, forwarding mode only to the
 * code160/tagged/small allocators. 163 aliases 162; 166 aliases 165. Supported
 * state-object codes are listed in the implementation. Codes A1..A3 enqueue
 * aged items but return NULL; unsupported codes and allocation failures also
 * return NULL. A non-NULL record is passed through the common post-allocation
 * initializer before return. Families use different record layouts: the u16*
 * return exposes only their leading ID. Requires initialized shared pools;
 * modifies those pools/queues and is not reentrant. */
u16 *Ovl11AllocateItemRecord(s32 itemCode, s32 mode);
#endif
