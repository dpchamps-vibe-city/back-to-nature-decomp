/*
 * Five code-0x109 records loaded from text-bank payloads, or removal of
 * small records marked 0x40000. The private five-offset table belongs to
 * this service; the allocator and ten-slot pool are shared engine services.
 * See ovl11_flagged_code109_records.h for the asymmetric load/clear contract.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_flagged_code109_records.h"

/* The bank stores exactly 18 unaligned bytes, not a padded 20-byte record.
 * Its contents are copied opaquely; field meanings are not established. */
typedef struct __attribute__((packed)) {
    s32 word0;
    s32 word4;
    s32 word8;
    s32 wordC;
    s8 byte10;
    s8 byte11;
} Ovl11Code109Payload;

s32 Ovl11LoadOrClearCode109Records(s32 control) {
    s32 *flags;
    s32 remaining;
    s32 index;
    s32 flagMask;
    u16 *record;
    u8 *payload;
    u8 *poolRecord;
    char *work;

    if ((control << 0x10) == 0) {
        for (index = 0; index < 5; index++) {
            record = Ovl11AllocateItemRecord(0x109, -1);
            if (record != NULL) {
                *(s32 *)((u8 *)record + 0x34) |= 0x40000;
                payload = (u8 *)D_8005175C + (gTextBankOffsets[1] + gOvl11Code109PayloadOffsets[index]);
                *(Ovl11Code109Payload *)((u8 *)record + 4) = *(Ovl11Code109Payload *)payload;
            }
        }
    } else {
        flagMask = 0x40000;
        work = (char *)&D_8006C838;
        poolRecord = (u8 *)(work + 0x7AB4);
        flags = (s32 *)(work + 0x7AE8);
        remaining = 9;
        do {
            if (*flags & flagMask) {
                Ovl11ClearSmallRecord((s32)poolRecord);
            }
            poolRecord += 0xB4;
            remaining -= 1;
            flags += 0x2D;
        } while (remaining >= 0);
    }
    return 1;
}
