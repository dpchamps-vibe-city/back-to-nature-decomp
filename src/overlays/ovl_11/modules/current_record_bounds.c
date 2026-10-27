/* Current-record contact probe (ovl_11:800F4994).
 * The moving-item client uses the returned record as its spawn/contact target.
 * A hit arms the shared sprite Y-offset index; it does not set record flags.
 * See ovl11_current_record_bounds.h for bounds and initialization constraints.
 */
#include "ovl11_current_record_bounds.h"
#include "game_types.h"

Ovl11CurrentRecordBoundsEntry *Ovl11ProbeCurrentRecordBounds(Vec3 *position) {
    Ovl11CurrentRecordBoundsWork *work = (Ovl11CurrentRecordBoundsWork *)&D_8006C838;
    Ovl11CurrentRecordBoundsEntry *record;
    char *selectorWork;
    s16 selector;

    record = work->current;
    selectorWork = (char *)&D_8007AFF0;
    selector = *(s16 *)(selectorWork + 0x25476);
    if (selector == record->selector) {
        if (record->y - 20 < position->y && position->y < record->y + 20) {
            if (record->x - 200 < position->x && position->x < record->x + 200) {
                if (record->z - 200 < position->z && position->z < record->z + 200) {
                    gOvl11RecordSpriteYOffsetIndex = 3;
                    return record;
                }
            }
        }
    }
    return 0;
}
