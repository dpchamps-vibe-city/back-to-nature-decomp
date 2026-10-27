#ifndef OVL11_QUANTITY_INPUT_H
#define OVL11_QUANTITY_INPUT_H

#include "common.h"

/* Service pad 0 once for a caller-owned quantity. Requires initialized pad
 * state, a valid quantity pointer and a positive signed-halfword limit.
 * Repeated directions edit by +1, -1, +10, -10 in that priority order;
 * edits truncate to u16, then signed values above limit wrap to 1 and values
 * <=0 wrap to limit. This is wrapping, not clamping (no overshoot remainder).
 *
 * Return 0 while editing, 1 on pressed bit 0x40 (confirm), 2 on bit 0x10
 * (alternate action), 3 on bit 0x20 (cancel). Directions suppress button
 * actions. All paths normalize the quantity, even when returning an action.
 * Emits edit/confirm/cancel sounds; does not draw, advance interaction steps
 * or retain a private quantity. Timings come from the pad repeat service. */
s32 Ovl11ProcessQuantityInput(s16 limit, u16 *quantity);

#endif
