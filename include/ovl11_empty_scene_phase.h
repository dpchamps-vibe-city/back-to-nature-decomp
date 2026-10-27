#ifndef OVL11_EMPTY_SCENE_PHASE_H
#define OVL11_EMPTY_SCENE_PHASE_H

/* Retail no-op callback in slot 3 of gOvl11ScenePhaseCallbacks.
 * Ovl11DispatchScenePhase supplies the table base as an unused extra input;
 * retain the entry's no-parameter C contract. Returns immediately: no state
 * changes, drawing, waits, or automatic phase advancement. */
void Ovl11EmptyScenePhase(void);

#endif
