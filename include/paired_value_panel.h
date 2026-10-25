#ifndef PAIRED_VALUE_PANEL_H
#define PAIRED_VALUE_PANEL_H

#include "common.h"

/* Queue a four-row panel in the active render context. Reads four signed
 * word pairs at work-area +49C4 (eight-byte stride), the icon cycle at +44BA
 * and the current text bank. Does not advance or modify the pair values.
 * Requires initialized packet space, ordering tables and shared sprite data.
 *
 * First-pair numeric totals clamp to +/-9999999. Each row's left/right bar
 * uses pair[1]/pair[0] respectively: signed remainder modulo 10000 scaled to
 * 64 pixels, with optional two-digit higher-order labels. The bars are NOT
 * clamped; normal callers must supply nonnegative values for useful widths.
 * Row labels classify each value as >=5000, >=2000 or below 2000, with a
 * special label when both are zero. Coordinates are screen pixels.
 *
 * Reuses gOvl11NumberTextScratch (a base label, not a two-byte buffer limit),
 * so rendering is non-reentrant with other formatted-number panels. */
void Ovl11DrawPairedValuePanel(void);

#endif
