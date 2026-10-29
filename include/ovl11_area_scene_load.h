#ifndef OVL11_AREA_SCENE_LOAD_H
#define OVL11_AREA_SCENE_LOAD_H

#include "common.h"
#include "game_types.h"

/* Original ovl_11:801044E4-80104EC8. These services require initialized
 * main/far work buffers, area metadata and graphics heap. They mutate shared
 * scene, resource, weather, object, message and event state; not reentrant.
 * Callback entries below remain phase-table slots 4/5/6/7 respectively. */

/* Ordered full preparation: resets slot/row records and subsystem state,
 * stages sprite banks/events, initializes horse-race entries, and advances day-condition
 * setup. Main-work flag 8000 also clears the pointed reset value, seeds RNG
 * to 1 and creates the fixed initial object set. The three empty lifecycle
 * hooks are still called at their original positions. */
void Ovl11PrepareAreaScene(void);

/* Clear exactly 0x680 words (0x1A00 bytes) at gAreaSpriteWorkBase.
 * The single-word extern is an anchor, not the full buffer's allocation type. */
void Ovl11ClearAreaSpriteWork(void);

/* Slot 4: reset transient state without the full sprite reload; set entry
 * step to 10 and mode to 1, queue sound load command 1000, and derive arrival
 * flags/tint for previous scene IDs 3/4/5/B/C. */
void Ovl11BeginLightAreaEntry(void);

/* Slot 5: rebind static tables while preserving pools, reload sprite/record
 * resources, reset transient subsystems and events, and publish mode 1/step 10.
 * The 16-byte setup buffer is cleared; no argument or return status. */
void Ovl11LoadAreaScene(void);

/* Slot 6: construct projected geometry, install records/clamp bounds and
 * animations, wait for sound transfers (no timeout), then start the sound
 * sequences and ten-update volume/screen fades. Can create a random initial
 * object when the work/game flags allow it. Publishes scene mode 2. */
void Ovl11InstallAreaScene(void);

/* Slot 7: delegates to LoadAreaScene, with all the same effects. */
void Ovl11ReloadAreaScene(void);

#endif
