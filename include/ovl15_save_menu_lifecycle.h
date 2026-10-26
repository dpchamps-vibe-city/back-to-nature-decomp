#ifndef OVL15_SAVE_MENU_LIFECYCLE_H
#define OVL15_SAVE_MENU_LIFECYCLE_H

#include "common.h"

/* Blocking reset: wait for the outstanding card command, clear menu controls,
 * cursor/confirmation, three 0x3C-byte summaries and the shared 0x9690-byte
 * transfer workspace. Card states become 4, save-slot states 2, selected card
 * 2 and selected slot 0. Selected summary fields become 0xFFFF. Rebind the
 * icon buffer and save-work pointer (transfer workspace +0x280); initialize
 * card support and clear the 0x6378-byte save image. Requires resident ovl_15
 * data and valid shared work storage; this destroys any pending image. */
void Ovl15ResetSaveMenuState(void);

/* Per-frame controllers using shared scene state at D_8006C838 and menu
 * controls. Call only the controller for the active scene, once per update.
 * State 0 resets card/menu, sound and message state and starts 60-update
 * screen/volume fades. State 1 waits for the screen fade, state 2 dispatches
 * the menu, and states 3/4 wait for a 10-update exit fade. These calls draw
 * the scrolling grid and text; they require initialized render/audio state.
 *
 * In state 2, gOvl15MenuStateTimer resets on a menu-state change, otherwise
 * saturates at 255 updates. The sub-dispatcher writes gOvl15MenuExitRequest:
 * 2 continues, -1 selects scene state 4, 1 selects state 3. Other values do
 * nothing. Text pen offset is (0,2) during dispatch and (0,0) afterwards.
 * The routines do not service the fade/volume counters themselves. */
void Ovl15UpdateCardWriteScene(void); /* dispatch 8012EA18; exit scene 15 */

/* Dispatch 801301A0: state 4 exits to 13 if scene flag 0x4000 is set, 19
 * otherwise. State 3 clears that flag if set and exits to 14. */
void Ovl15UpdateCardReadScene(void);

/* Dispatch 80131094, load the extra menu sprite resources, exit to scene 19.
 * The two read flows retain separate dispatchers and saved-image policies. */
void Ovl15UpdateCardReadSceneWithSprites(void);

#endif
