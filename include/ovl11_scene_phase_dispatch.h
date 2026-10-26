#ifndef OVL11_SCENE_PHASE_DISPATCH_H
#define OVL11_SCENE_PHASE_DISPATCH_H

/* GF_FARM overlay entry callback. Dispatch gScenePhase (unchecked, 0..7)
 * through the overlay's phase table. The selected callback receives the table
 * base in a0; its return value is ignored. Callbacks may change scene phase.
 * This entry belongs to ovl_11, not other overlays at the same RAM address. */
void Ovl11DispatchScenePhase(void);

#endif
