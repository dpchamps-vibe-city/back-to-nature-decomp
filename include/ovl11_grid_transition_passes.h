#ifndef OVL11_GRID_TRANSITION_PASSES_H
#define OVL11_GRID_TRANSITION_PASSES_H

#include "common.h"

/* Sweep all 25x45 main-grid cells through the kind-3F -> 175 random
 * transition. mode==0 uses Rand bound 32; all other modes use bound 16.
 * Only eligible cells consume RNG. Does not touch the alternate grid. */
void Ovl11PassGridKind3FTransitions(s32 mode);

/* At each main-grid cell, in order: eligible reset-to-168 (bound 16),
 * active-kind-36 refresh (bound 8), kind-3F transition (bound 16).
 * Later tests see earlier mutations. Both services require initialized grid
 * and lookup tables, mutate shared RNG/state and are not reentrant. */
void Ovl11PassGridEntryResets(void);

#endif
