#ifndef OVL08_SCENE_SELECT_MENU_H
#define OVL08_SCENE_SELECT_MENU_H

/* ovl_08's per-frame callback (original entry 0x800B8014).
 * Call only while this overlay is loaded. Original overlay data starts its
 * phase at 0: display/font setup -> sound-transfer wait -> interactive menu.
 * The phase is not range-checked or reset by this entry.
 *
 * Displays six rows of a 32-entry scene table. Pad-0 repeat input moves by
 * one row (wrap) or six rows (clamp); a newly pressed 0x20 button requests
 * the selected scene unless its ID is -1. Rows 13..16 also set one of bits
 * 19..16 in the game-state word at +0x10. Input and font flush wait for the
 * screen fade to complete. Initialization sets both backgrounds and deals
 * the card table; it does not just draw text. */
void Ovl08UpdateSceneSelectMenu(void);

#endif
