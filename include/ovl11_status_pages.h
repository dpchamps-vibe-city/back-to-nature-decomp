#ifndef OVL11_STATUS_PAGES_H
#define OVL11_STATUS_PAGES_H

#include "common.h"

/* Open the eight-page status display when idle; repeated requests while
 * active leave it alone. The legacy s32 result is the current state only
 * when already active; callers must ignore the result of a new request.
 * Opening begins a ten-update screen fade, not synchronous resource loading. */
s32 Ovl11OpenStatusPages(void);

/* Advance fade/setup/display/exit once per scene update. Setup blocks on an
 * archive-14 read, overwrites graphics scratch at heap+4290, uploads textures
 * and initializes five sprite sources. It also snapshots counts, names and
 * the heading; the pages combine that snapshot with live work-area values.
 *
 * Page input uses the primary repeated-button stream; close uses new presses
 * (mask 0x860). Eight pages wrap, skipping empty record pages 2..4. Lists
 * display seven rows and wrap their first-row index. Arrow animation runs
 * through four frames, 30 updates each (not milliseconds).
 *
 * State, text scratch, active render context, fade and CD resources are
 * shared: this service is non-reentrant and must not run concurrently with
 * another menu owning those resources. Work flags 0x20000/0x40000 bracket
 * scene suspension/restoration. The state returns to zero before the scene
 * restoration callback. */
void Ovl11UpdateStatusPages(void);

/* Live, read-only counts used by page navigation and event eligibility;
 * neither requires the display to be open or its resources initialized.
 * Scan 20 records at work-area +0x81BC, stride 0xB8, for tags 0x160..0x163
 * (A) or 0x164..0x166 (B). Each result is a count in 0..20, not a Boolean. */
s32 Ovl11CountStatusTaggedRecordsA(void);
s32 Ovl11CountStatusTaggedRecordsB(void);

#endif
