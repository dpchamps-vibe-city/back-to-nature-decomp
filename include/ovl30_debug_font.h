#ifndef OVL30_DEBUG_FONT_H
#define OVL30_DEBUG_FONT_H

/* Load the SDK font at VRAM (704,256), open a transparent 256x200-pixel
 * console at screen (24,0) with a 512-character buffer, and select it for
 * dump output. Set the shared custom-text origin to (24,24), clearing its
 * secondary offsets. Caller must arrange GPU/display initialization and
 * reserve the font's VRAM area. No allocation/error status is returned. */
void Ovl30InitDebugFont(void);

#endif
