#ifndef GTE_TRANSFORM_H
#define GTE_TRANSFORM_H

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* A single shared transform snapshot, not a stack. SDK rotation entries use
 * 0x1000 fixed-point units; translation uses the caller's coordinate units. */

/* exe:0x8001BB88. Capture the current GTE rotation AND translation. */
void CaptureGteTransform(void);

/* exe:0x8001BBD8. Replace the captured transform C with C * local: rotate
 * each local column, then rotate/translate its translation. Load the result
 * into the GTE. Modifies both the shared snapshot and GTE registers; callers
 * must capture before the first composition. No implicit restore occurs. */
void ComposeCapturedGteTransform(MATRIX *local);

#endif
