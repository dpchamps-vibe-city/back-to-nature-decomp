#ifndef SELECTION_PANEL_H
#define SELECTION_PANEL_H

/* Farm-overlay selection panel. Open resets phase/lane and starts the
 * frame-driven lifecycle; Update advances it and returns no completion code.
 * It shares the engine message window, fades, sprite source state, active
 * ordering tables, graphics scratch and input, so only one instance may run.
 *
 * Setup saves VRAM rectangle (896,1,64,256), with x/w in VRAM words, into
 * graphics scratch +0x12A90. Exit restores it before resuming the scene.
 * Sprites load synchronously from the seasonal archive (season clamped 0..3)
 * through scratch +0x4290; the shared buffers and render context must already
 * exist. The byte-pair ID table must contain both scene IDs used for drawing:
 * a missing ID returns -1 and the original draw path does not guard that slot.
 * Direction input follows four links in the current eight-byte lane record;
 * confirm/cancel and the map-camera detour retain their original message and
 * fade gates. These functions do not validate or normalize scene state.
 */
void Ovl11OpenSelectionPanel(void);
void Ovl11UpdateSelectionPanel(void);

#endif
