/* Shared request-handler effect gate; this neither subtracts the quantity
 * nor sets the caller's operation flags. See the public header for offsets
 * and update-count units, notes/file-groupings.md for the separate service. */
#include "ovl11_record_effect_threshold.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

void Ovl11StartRecordEffectAboveThreshold(Ovl11DF72CResetView *record) {
    if (record->unk16 >= 10) {
        Ovl11StartTimedSpriteEffect(&record->unkA8, 0x25, 0x2D);
    }
}
