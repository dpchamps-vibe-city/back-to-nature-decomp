/* Code-banded halfword accumulation used by cell-action scoring.
 * Original entry: ovl_11:800CD5BC. The shared object view remains partial;
 * no gameplay meaning is inferred for its code or progress slots.
 * Membership and boundaries: notes/file-groupings.md. */
#include "ovl11_indexed_progress.h"

void Ovl11AddIndexedProgress(Ovl11CD5BCObject *object, s32 delta) {
    s32 index;
    s32 sum;

    if (!(object->flags & 8)) {
        index = (object->value - 1) / 5;
        sum = delta + object->progress[index];
        if (sum > 0xFFFF) {
            sum = 0xFFFF;
        }
        object->progress[index] = (u16) sum;
    }
}
