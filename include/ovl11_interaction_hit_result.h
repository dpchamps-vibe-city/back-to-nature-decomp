#ifndef OVL11_INTERACTION_HIT_RESULT_H
#define OVL11_INTERACTION_HIT_RESULT_H

#include "common.h"
#include "game_types.h"

/* Copy the shared hit pointer/flags to object +50/+54, then dispatch on the
 * EXACT value flags & 0x8928. Supported categories are 8000 (seasonal record),
 * 100, 800/8 and 20 (cell records). Multiple masked bits do not select an arm.
 * Requires valid category-appropriate hit storage, loaded item tables and a
 * writable six-byte result; there is no null guard on a supported path.
 *
 * Seasonal success writes result id/valid but retains +2; rejection retains
 * the entire result. Other supported arms write id, zero +2, and valid=1.
 * Category 100 substitutes id 65 when the record resolver returns 2. Sound 7
 * plays on supported cell paths and successful seasonal consumption.
 * Unsupported categories leave result untouched. ALL paths clear both shared
 * hit words, leaving the snapshot in object; always returns zero. Non-reentrant.
 * A caller testing result->id must initialize it before calling. */
s32 Ovl11ResolveInteractionHitResult(Ovl11InteractionHitObject *object,
                                   Ovl11RecordPresentation *result);

#endif
