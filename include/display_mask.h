#ifndef DISPLAY_MASK_H
#define DISPLAY_MASK_H

/* GPU display-output gate; neither operation waits for VBlank or GPU work,
 * changes the draw/display environments, nor clears VRAM. Disabling output
 * hides scanout, not rendering. Auxiliary-display setup brackets its staging
 * with these calls; callers are responsible for synchronization.
 * Original identities: exe:0x800129F4 / exe:0x80012A14. */
void EnableDisplayOutput(void);
void DisableDisplayOutput(void);

#endif
