#ifndef BEVELED_PANEL_H
#define BEVELED_PANEL_H

#include "common.h"

/* exe:0x800136D4. Draw an opaque beige beveled panel at (x,y), with width
 * and height in pixels. The outer border ends at x+width-1/y+height-1;
 * the gradient begins at (x+2,y+2), with extent (width-4,height-4).
 * No clipping, size checks or overflow checks: provide sensible positive
 * dimensions and signed-16-bit coordinates, including derived endpoints.
 *
 * Prepends six points, eight lines and a gradient quad (236 packet bytes)
 * to the supplied ordering-table slot. The active render context and packet
 * cursor must be initialized, and storage kept until the GPU has consumed it.
 * Does not submit commands, synchronize the GPU or change persistent state.
 */
void DrawBeveledPanel(u32 *ot, s16 x, s16 y, s16 width, s16 height);

#endif
