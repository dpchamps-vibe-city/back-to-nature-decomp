/*
 * Refresh flagged records' collision tag and position by ID.
 * The coordinator selects among the B4/B8 pools, one singleton and the
 * six-record B0 pool. Its two adjacent helpers have no other live callers.
 * No pool, lookup table or mask storage becomes private to this module.
 *
 * Public contract: ovl11_record_placement.h. Membership/boundary evidence:
 * notes/file-groupings.md (D_80075BC4 record-table consumers).
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_record_placement.h"

s32 ovl_11_func_800BDB88(s32 *position, s32 mode);
s32 ovl_11_func_80109188(Ovl11D04D4Obj *record, s32 mode);
s32 ovl_11_func_800D3104(u16 *record, s32 mode);
void ovl_11_func_800D3200(s32 record);
s32 ovl_11_func_800D2E20(void);
s32 Ovl11IsItemIdEnabled(s32 itemId);

s32 Ovl11UpdateRecordPlacement(Ovl11PlacedRecordView *record,
                               Ovl11CollisionTag *tag, VECTOR *position) {
    record->flags &= ~0x20000;
    record->collision = *tag;
    *(VECTOR *)record->position = *position;
    ovl_11_func_800BDB88((s32 *)&record->position, 0);
    return 1;
}

s32 Ovl11RefreshRecordById(s16 itemId, Ovl11CollisionTag *tag, VECTOR *position) {
    Ovl11PlacedRecordView *record;
    s32 *recordWords;
    s32 recordIndex;
    s32 smallIndex;
    s32 taggedIndex;
    u16 recordId;

    /* These two IDs select the first flagged B4 record, without an ID test. */
    if ((u32) (itemId - 0x109) < 2U) {
        recordWords = &gSmallTaggedRecordBase;
        for (smallIndex = 0; smallIndex < 10; smallIndex++, recordWords += 0x2D) {
            if ((*(s32 *) ((u8 *) recordWords + 0x34)) & 0x20000) {
                goto place_selected;
            }
        }
        return 1;
    }
    if ((u32) (itemId - 0x161) < 2U) {
        recordWords = &gTaggedRecordBase;
        for (taggedIndex = 0; taggedIndex < 20; taggedIndex++, recordWords += 0x2E) {
            if ((*(u16 *) ((u8 *) recordWords + 0)) == itemId &&
                ((*(s32 *) ((u8 *) recordWords + 0x34)) & 0x20000)) {
                goto place_selected;
            }
        }
        return 1;
    }
    if ((u32) (itemId - 0x106) < 2U) {
        if (D_800759E4.f34 & 0x20000) {
            ovl_11_func_80109188((Ovl11D04D4Obj *) &D_800759E4, 9);
            recordWords = (s32 *) &D_800759E4;
place_selected:
            return Ovl11UpdateRecordPlacement((Ovl11PlacedRecordView *) recordWords, tag, position);
        }
        return 1;
    }
    /* Unlike the first three paths, update every flagged matching B0 record. */
    record = (Ovl11PlacedRecordView *) D_80075BC4_reset;
    recordIndex = 0;
    do {
        if (((*(u16 *) ((u8 *) record + 0)) == itemId) && (record->flags & 0x20000)) {
            (*(s32 *) ((u8 *) record + 0x8C)) = 0;
            recordId = *(u16 *) ((u8 *) record + 0);
            switch (recordId) {
            case 0x118:
            case 0x119:
            case 0x11A:
                ovl_11_func_800D3104((u16 *) record, 0x10);
                (*(u16 *) ((u8 *) record + 0x22)) = (u16) D_80071A38;
                break;
            case 0x114:
            case 0x115:
                ovl_11_func_800D3104((u16 *) record, 0xF);
                (*(u16 *) ((u8 *) record + 0x22)) = (u16) D_80071A38;
                break;
            case 0x116:
            case 0x117:
                ovl_11_func_800D3104((u16 *) record, 0xE);
                (*(u16 *) ((u8 *) record + 0x22)) = (u16) D_80071A38;
                break;
            }
            Ovl11UpdateRecordPlacement(record, tag, position);
            if (Ovl11IsItemIdEnabled((s32) (*(u16 *) ((u8 *) record + 0))) == 0) {
                ovl_11_func_800D3200((s32) record);
            }
        }
        recordIndex += 1;
        record = (Ovl11PlacedRecordView *) ((u8 *) record + 0xB0);
    } while (recordIndex < 6);
    return 1;
}

s32 Ovl11IsItemIdEnabled(s32 itemId) {
    Ovl11ItemPlacementRule *entry;
    s16 mask;
    s32 i;

    mask = ovl_11_func_800D2E20();
    entry = gOvl11ItemPlacementRules;
    for (i = 0; i < 0x11; i++) {
        if (entry->itemId == itemId) {
            return (mask & entry->enabledMask) != 0;
        }
        entry++;
    }
    return 0;
}
