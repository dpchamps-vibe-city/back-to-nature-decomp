#ifndef OVL11_NEARBY_RECORD_SCANS_H
#define OVL11_NEARBY_RECORD_SCANS_H

#include "common.h"

/* Both searches use signed-halfword world coordinates {x,y,z} and strict
 * open bounds: points exactly 150 units away in X/Z are excluded. They return
 * the first match in pool order, not the nearest record; no state is changed. */

/* Scan 37 actor slots, also requiring |dy| < 20. Flag 0x1000 selects the
 * alternate position and active bit 0x800, otherwise bit 0x100 is required.
 * Return the slot or NULL. unusedContext is retained by the original ABI. */
struct_80076220 *Ovl11FindNearbyActiveActor(s16 *position, void *unusedContext);

/* Scan 197 item records at the current work-area pool pointer. Match the
 * record region to regionTag[1]; ignore Y. Return its u16 identifier, or 0
 * when none matches (not a separate out-of-band sentinel). */
u16 Ovl11FindNearbyItemId(s16 *position, s16 *regionTag);

#endif
