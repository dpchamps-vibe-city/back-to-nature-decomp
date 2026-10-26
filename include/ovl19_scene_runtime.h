#ifndef OVL19_SCENE_RUNTIME_H
#define OVL19_SCENE_RUNTIME_H

#include "common.h"

/* Frame entry for overlay 19, loaded by EXE scene mode 4 from archive [19,20).
 * ovl_19 must be resident and gSceneSubphase must index its five-entry phase
 * table. Dispatches load, fade-in, update, exit-start or exit-finish; it does
 * not block until the scene completes. Saves/restores the shared text setting.
 *
 * Owns the scene's mode/stage protocol, actor pairs and auxiliary records,
 * camera/projection scratch, sprite resources and value-table UI. Loading
 * steps wait for sound transfer before initializing work and fading in.
 * Exit waits for both fades, stops sound, releases resource slots, requests
 * scene 6 and conditionally raises persistent record values. Frame drawing
 * appends packets to the active render context. No arguments or return value.
 *
 * No concurrent stepping with another overlay or mutation of its scene work.
 * Other ovl19_* headers describe the geometry, actor and draw subinterfaces;
 * those functions retain linkage but now compile in this same translation unit.
 * Original entry: ovl_19:0x800B7ED8. */
void Ovl19StepScene(void);

#endif
