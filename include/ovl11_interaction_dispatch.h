#ifndef OVL11_INTERACTION_DISPATCH_H
#define OVL11_INTERACTION_DISPATCH_H

/* ovl_11 must be resident. The shared interaction ID indexes 29 begin/update
 * slots (0..28); draw mode indexes eight draw slots (0..7). Neither entry point
 * checks bounds. Callback banks and interaction state are shared, not owned
 * allocations. No arguments or return types of the callbacks are changed. */

/* If an interaction is selected, call its update callback, then draw using
 * the resulting draw mode. A nonzero update result invokes begin[1], the
 * common reset callback, before drawing. The draw callback receives the draw
 * bank's base address. Returns 0 only while a present update callback reports
 * pending; returns 1 for idle, absent-update and completed cases. Completion
 * does not cause a second interaction-ID check before the draw step. */
s32 Ovl11UpdateInteractionDispatch(void);

/* Admit a selection only when idle, except ID 1 may replace a busy selection.
 * Store the ID and invoke its optional begin callback, ignoring its result.
 * Returns 1 when busy/rejected, 0 when admitted. Special ID 0x16 also runs the
 * existing flag-2 preflight loop; its deferred exit returns 0 WITHOUT installing
 * the ID. Calls may mutate message/object/scene state through callbacks;
 * serialize them with the per-frame updater. */
s32 Ovl11BeginInteraction(s32 interactionId);

#endif
