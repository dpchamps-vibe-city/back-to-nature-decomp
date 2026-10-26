/* Capture the UI/save summary separately from payload serialization and
 * Shift-JIS card-title formatting. The published record is consumed by both
 * save-slot display and the metadata area of the transfer image.
 * Original ovl_15:80134A34–80134C48; contract: ovl15_save_slot_header.h. */
#include "common.h"
#include "psyq/memory.h"
#include "ovl15_save_slot_header.h"

void Ovl15BuildSaveSlotHeader(s16 slot) {
    gOvl15SaveSlotHeaders[slot].words = D_80070CF0_headerSource.words;
    memcpy(&gOvl15SaveSlotHeaders[slot].name_8,
           &D_80070CF0_headerSource.name_28,
           sizeof(D_80070CF0_headerSource.name_28));
    memcpy(&gOvl15SaveSlotHeaders[slot].name_1A,
           &D_80070CF0_headerSource.name_D10,
           sizeof(D_80070CF0_headerSource.name_D10));
    /* The source's far-base spelling addresses the eight bytes at work+0. */
    memcpy(&gOvl15SaveSlotHeaders[slot].fields_2C,
           (u8 *)&D_80070CF0_headerSource - 0x44B8,
           sizeof(gOvl15SaveSlotHeaders[slot].fields_2C));
    gOvl15SaveSlotHeaders[slot].field_34 = 0x14;
    memset(gOvl15TransferBuffer, 0, 0x9690);
    memcpy(gOvl15TransferBuffer + 0x200, &gOvl15SaveSlotHeaders[slot],
           sizeof(gOvl15SaveSlotHeaders[slot]));
}
