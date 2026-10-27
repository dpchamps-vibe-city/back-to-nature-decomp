/*
 * Resident scene-record reset and ordered two-pass service.
 * Pool layout and pass order are shared across all three entry points;
 * ovl11_record_pool_lifecycle.h documents the skip latch and reset side effects.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_record_pool_lifecycle.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "aged_item_pool.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_item_spawn_placement.h"
#include "ovl11_tagged_record_choice.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_small_record_handlers.h"

u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
void ovl_11_func_800CFD9C(void);
s32 ovl_11_func_800DE9C8(UnkStruct800DF4F0 *arg0);
s32 ovl_11_func_800DEB00(Ovl11Deb00Object *arg0);
s32 ovl_11_func_80108D84(s32 *arg0);
s32 ovl_11_func_8010B264(Ovl11B264Object *arg0);
void ovl_11_func_800D0FB4(void);

void Ovl11ResetRecordPools(void) {
    s32 index;
    u32 counterIndex;
    u16 *seedRecord;
    struct Ovl11PoolResetView *smallBase;
    struct Ovl11PoolResetView *smallRecord;
    struct Ovl11PoolResetView *largeRecord;
    struct Ovl11PoolResetView *largeRecordAgain;
    struct Ovl11PoolResetView *slotRecord;
    struct Ovl11PoolResetView *tailRecord;

    memset(&gSmallTaggedRecordBase, 0, 0x708);
    smallBase = (struct Ovl11PoolResetView *) ((u8 *) &gSmallTaggedRecordBase - 0x7AB4);
    for (index = 0; index < 10; index++) {
        smallRecord = (struct Ovl11PoolResetView *) ((u8 *) smallBase + index * 0xB4);
        smallRecord->records_7AB4[0].type = 0;
        smallRecord->records_7AB4[0].id = 0xFFFF;
    }
    memset(&gTaggedRecordBase, 0, 0xE60);
    for (index = 0; index < 20; index++) {
        largeRecord = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + index * 0xB8);
        largeRecord->records_81BC[0].type = 0;
        largeRecord->records_81BC[0].id = 0xFFFF;
    }
    /* The original repeats these sentinel writes to the same pool. */
    for (index = 0; index < 20; index++) {
        largeRecordAgain = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + index * 0xB8);
        largeRecordAgain->records_81BC[0].type = 0;
        largeRecordAgain->records_81BC[0].id = 0xFFFF;
    }
    memset(gAgedItemCellsResetSpan, 0, 0x18E);
    for (index = 98; index >= 0; index--) {
        ((struct Ovl11PoolResetView *) D_8006C838)->cells_901C[index][0] = 0;
    }
    memset(gStateObjectPoolBytes, 0, 0x420);
    for (index = 0; index < 6; index++) {
        slotRecord = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + index * 0xB0);
        slotRecord->records_938C[0].type = 0;
        slotRecord->records_938C[0].id = 0xFFFF;
    }
    for (index = 0; index < 3; index++) {
        tailRecord = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + index * 0xB4);
        tailRecord->records_97AC[0].type = 0;
        tailRecord->records_97AC[0].id = 0xFFFF;
    }
    ((struct Ovl11PoolResetView *) D_8006C838)->field_91AC = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_91E0 = 0;
    seedRecord = ovl_11_func_800CE744(0x107, 0);
    *(s32 *) ((u8 *) seedRecord + 0x38) = -0x708;
    *(s32 *) ((u8 *) seedRecord + 0x3C) = 0;
    *(s32 *) ((u8 *) seedRecord + 0x40) = 0xC1C;
    *(s16 *) ((u8 *) seedRecord + 0x2E) = 0;
    *(s16 *) ((u8 *) seedRecord + 0x30) = 1;
    memset(&D_80075AD4, 0, 0xF0);
    ((struct Ovl11PoolResetView *) D_8006C838)->field_929C = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_92A0 = 0xFFFF;
    ovl_11_func_800CFD9C();
    for (counterIndex = 0; counterIndex < 3U; counterIndex++) {
        (&gOvl11Mode0DispatchCounter)[counterIndex] = 0x12C;
    }
    gOvl11PendingStageMode = -1;
    for (index = 0; index < 4; index++) {
        ((struct Ovl11PoolResetView *) D_8006C838)->pairs_99C8[index][0] = -1;
        ((struct Ovl11PoolResetView *) D_8006C838)->pairs_99C8[index][1] = -1;
    }
    Ovl11ClearAgedItemPool();
    ((struct Ovl11PoolResetView *) D_8006C838)->field_99D8 = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_99E6 = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags &= ~8;
}

void Ovl11RunRecordPoolFirstPass(void) {
    s32 index;
    u16 *record;
    u16 *largeRecord;

    if (!(((struct Ovl11PoolResetView *) D_8006C838)->flags & 8)) {
        for (index = 0; index < 10; index++) {
            record = &((struct Ovl11PoolResetView *) D_8006C838)->records_7AB4[index].type;
            if (*record != 0) {
                Ovl11UpdateSmallRecordKind1(record);
            }
        }
        for (index = 0; index < 20; index++) {
            largeRecord = (u16 *) ((u8 *) &gTaggedRecordBase + index * 0xB8);
            if (*largeRecord != 0) {
                ovl_11_func_800DE9C8((UnkStruct800DF4F0 *) largeRecord);
            }
        }
        for (index = 0; index < 20; index++) {
            largeRecord = (u16 *) ((u8 *) &gTaggedRecordBase + index * 0xB8);
            if (*largeRecord != 0) {
                Ovl11UpdateTaggedRecordActivity((UnkStruct800E109C *) largeRecord);
            }
        }
        for (index = 0; index < 6; index++) {
            record = (u16 *) (gStateObjectPoolBytes + index * 0xB0);
            Ovl11ApplyTaggedRecordValueAdjustment(record);
        }
    }
}

void Ovl11RunRecordPoolSecondPass(void) {
    E21E4Object *smallRecord;
    s32 *largeRecord;
    s32 *recordWords;
    s32 index;
    struct Ovl11PoolResetView *tailRecord;

    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 = (s32) (((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 & 0xF7FFFFFF);
    if (!(((struct Ovl11PoolResetView *) D_8006C838)->flags & 8)) {
        for (index = 0; index < 10; index++) {
            smallRecord = (E21E4Object *) &((struct Ovl11PoolResetView *) D_8006C838)->records_7AB4[index];
            if ((smallRecord->type != 0) || (smallRecord->flags & 0x02000000)) {
                Ovl11UpdateSmallRecordState(smallRecord);
            }
        }
        for (index = 0; index < 20; index++) {
            largeRecord = (s32 *) ((u8 *) &gTaggedRecordBase + index * 0xB8);
            if ((*(u16 *) ((u8 *) largeRecord + 0)) != 0) {
                ovl_11_func_800DEB00((Ovl11Deb00Object *) largeRecord);
            }
        }
        for (index = 0; index < 20; index++) {
            recordWords = (s32 *) ((u8 *) &gTaggedRecordBase + index * 0xB8);
            if ((*(u16 *) ((u8 *) recordWords + 0)) != 0) {
                Ovl11UpdateTaggedRecordCounters((Ovl11Deb00Object *) recordWords);
            }
        }
        recordWords = (s32 *) &D_800759E4;
        if (*(u16 *) recordWords != 0) {
            ovl_11_func_80108D84(recordWords);
        }
        recordWords = (s32 *) ((u8 *) recordWords + 0xF0);
        if (*(u16 *) recordWords != 0) {
            ovl_11_func_8010B264((Ovl11B264Object *) recordWords);
        }
        Ovl11UpdateAgedItemPool();
    }
    if (((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 & 0x08000000) {
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C++;
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C =
            ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C < 0x10 ?
            ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C : 0xF;
    } else {
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C = 0U;
    }
    gOvl11ItemSpawnRuleMask = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x8000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x800000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x1000000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags = (s32) (((struct Ovl11PoolResetView *) D_8006C838)->flags & ~8);
    Ovl11RollSpawnWindowLatch();
    Ovl11ResyncRecordRowPlacements(1);
    Ovl11ResyncRecordRowPlacements(0);
    ovl_11_func_800D0FB4();
    if (((struct Ovl11PoolResetView *) D_8006C838)->field_E4CA == 0) {
        ovl_11_func_800CFD9C();
    } else {
        for (index = 0; index < 3; index++) {
            tailRecord = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + index * 0xB4);
            tailRecord->records_97AC[0].type = 0;
            tailRecord->records_97AC[0].id = 0xFFFF;
        }
    }
}
