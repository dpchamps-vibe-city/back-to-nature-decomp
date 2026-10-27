/* Current-record sprite-offset decay (ovl_11:800F4A58).
 * The frame service draws with the old index before calling this operation.
 * The index/offset table is shared with other record actions and the renderer;
 * this module neither owns it nor installs a current record.
 */
#include "ovl11_record_sprite_offset_decay.h"

void Ovl11DecayCurrentRecordSpriteOffset(void) {
    Ovl11CurrentRecordSpriteWork *work = (Ovl11CurrentRecordSpriteWork *)&D_8006C838;
    Ovl11PoolCompletionEntry *record;
    u16 flags;

    record = work->current;
    flags = record->flags;
    if ((flags & 0x100) && !(work->flags & 0x08000000)) {
        if (gOvl11RecordSpriteYOffsetIndex == 0) {
            record->flags = (u16)(flags & 0xFEFF);
            return;
        }
        gOvl11RecordSpriteYOffsetIndex -= 1;
    }
}
