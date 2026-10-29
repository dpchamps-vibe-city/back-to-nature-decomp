#ifndef OVL17_RECORD_SCENE_H
#define OVL17_RECORD_SCENE_H

/* Per-frame entry for scene 3, after its overlay and shared engine state load.
 * gSceneSubphase selects five setup/run/exit callbacks; the run callback
 * dispatches eleven stages over six 0x50-byte rows and a 90-node trail ring.
 * Updates may change selection, Q12 row positions, scroll, sound/volume,
 * messages, fades and scene state. The exit path requests scene 6.
 * Indices, resource availability and call order are caller prerequisites;
 * counters measure eligible frame calls, not elapsed milliseconds.
 * The scene owns shared scratch and is not reentrant. No return value.
 */
void Ovl17DispatchRecordSceneSubphase(void);

#endif
