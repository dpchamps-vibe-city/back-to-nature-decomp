#ifndef OVL27_MENU_SCENE_H
#define OVL27_MENU_SCENE_H
#include "common.h"

/* Resident phase-table callbacks, following title setup. The menu maintains
 * its own signed stage halfword plus an independently self-installing backdrop/
 * panel animation. Updates consume pad state and service text entry, indexed
 * selection, message windows, fades and packet drawing. Timers count update
 * calls; no elapsed-time conversion or reentrancy is provided. */
void Ovl27UpdateTitleMenu(void);

/* Begin ten-update screen/master-volume fades, advance gScenePhase and
 * return the new phase word (not a Boolean). */
s32 Ovl27BeginTitleExitFade(void);

/* Return 2 while either fade is pending. When both finish, stop sequence 0,
 * reopen/start sound sequences, rewind sequence slots, advance gScenePhase
 * and return its new value. Call through the phase protocol, not repeatedly
 * after completion. */
s32 Ovl27PollTitleExitFade(void);

/* Apply exit flags, optionally restore ID-table variant 3, reset gScenePhase
 * and request scene 2 or 8 according to selection/work flags. The scene-2
 * selection path also replaces four work halfwords with random values. */
void Ovl27FinishTitleMenu(void);
#endif
