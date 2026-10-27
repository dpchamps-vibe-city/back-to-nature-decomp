/* Sprite-slot lifetime adapter for the shared six-byte triplet operations.
 * The tail helper's post-truncation zero test is intentional: a negative
 * countdown is not a release request. Original entry: ovl_11:800CCF14.
 * Evidence and conservative unit boundaries: notes/file-groupings.md. */
#include "ovl11_triplet_sprite_release.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_slot_sprite_cache.h"

void Ovl11StepTripletSpriteRelease(Ovl11TripletTailView *record) {
    if (Ovl11AddTripletTail(record, -1) != 0) {
        Ovl11RemoveSlotSprite(*(s16 *) ((u8 *) record + 0));
        Ovl11ClearHalfwordTriplet((s16 *) record);
    }
}
