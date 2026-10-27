#ifndef OVL15_CARD_WRITE_STATES_H
#define OVL15_CARD_WRITE_STATES_H

/* Card-write menu frame step (ovl_15:8012EA18), after scene initialization.
 * Polls/chooses a card and one of three save slots, confirms overwrite or
 * formatting, builds the checksummed save image/card icons and polls writing.
 * Draws through the active render context; updates the shared menu stage,
 * choices, card cache and exit request. The completion prompt requests exit
 * after acknowledgement or a 91-tick timeout on the scene-maintained timer.
 *
 * No arguments/return value. Overlay 15 must remain loaded. Step only one of
 * write/read/transfer each frame: their staged workflows share menu/card state.
 * Internal stages keep their linkage but are not external entry points. */
void Ovl15StepCardWriteMenu(void);

#endif
