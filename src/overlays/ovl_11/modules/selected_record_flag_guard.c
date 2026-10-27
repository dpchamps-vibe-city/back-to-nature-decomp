/* Keep the selected item record active while deactivating other records.
 * Selection is external state; this unit owns neither the record nor item. */
#include "common.h"
#include "ovl11_selected_record_flag_guard.h"

void Ovl11DeactivateUnselectedRecord(s32 *record) {
    s32 flags = record[0xD];

    if ((flags & 0x20000) && (Ovl11Item_80071A8A.id != *(u16 *)record)) {
        record[0xD] = flags & 0xFFFDFFFF;
    }
}
