/*
 * spawn_record_access — reconstructed translation unit.
 *
 * Initialize, clear, populate and read the two fixed spawn records and the
 * indexed spawn list. All entries share the resident 0x18-byte
 * dispatch-record protocol; position handoff retains the full SDK VECTOR ABI.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_record_pool_refresh.h"
#include "ovl11_record_row_values.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_dispatch_record_flags.h"

void Ovl11ClearSpawnRecords(Ovl11DispatchRecord *records, s32 count);

void ovl_11_func_800F40A4(void);

/* The partial-reset path retains the indexed list and both fixed records. */
void Ovl11PrepareSpawnRecordPools(u16 selector, s32 preserveIndexed) {
    Ovl11DispatchRecord *record;
    u8 *base;
    u8 *initialBase;

    ovl_11_func_800F40A4();
    Ovl11RefreshDispatchRecordPools();
    Ovl11UpdateList10RecordGates((s16) selector);
    Ovl11AssignPoolRowValues((s16) selector);
    initialBase = (u8 *) D_8006C838;
    base = initialBase + 0x8000;
    Ovl11ClearSpawnRecords(*(Ovl11DispatchRecord **) (base + 0x5DD0), *(s16 *) (base + 0x5DFA));
    if (preserveIndexed == 0) {
        Ovl11ClearSpawnRecords(*(Ovl11DispatchRecord **) (base + 0x5DCC), *(s16 *) (base + 0x5DF8));
        record = *(Ovl11DispatchRecord **) (base + 0x5DD4) + 1;
        record->flags &= 0xFFFE;
        record = *(Ovl11DispatchRecord **) (base + 0x5DD4);
        record->flags &= 0xFFFE;
    }
}

/* The dispatcher forwards a complete SDK VECTOR, including its unused pad
 * word. Both identifiers are passed as raw zero-extended halfwords. */
void Ovl11FillSpawnRecord(Ovl11DispatchRecord *record, u16 kind, u16 id, VECTOR position) {
    record->id = id;
    record->kind = kind;
    record->x = position.vx;
    record->y = position.vy;
    record->z = position.vz;
    record->flags = (u16)(record->flags | 1);
}

/* Preserve reserved words and every flag other than the active bit. */
void Ovl11ClearSpawnRecords(Ovl11DispatchRecord *records, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        records->id = 0;
        records->kind = 0;
        records->x = 0;
        records->y = 0;
        records->z = 0;
        records->flags = (u16)(records->flags & 0xFFFE);
        records++;
    }
}

void Ovl11SetSpawnRecord(s32 kind, VECTOR position, u16 id, s32 index) {
    Ovl11DispatchRecord *record;
    char *indexedState;
    char *secondState;
    char *firstState;
    u16 kind16;

    kind16 = kind & 0xFFFF;
    switch (kind16) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        indexedState = (char *)&D_8006C838;
        record = (Ovl11DispatchRecord *)(*(s32 *)(indexedState + 0x8000 + 0x5DCC) + (index * 0x18));
        break;
    case 0x65:
        secondState = (char *)&D_8006C838;
        record = (Ovl11DispatchRecord *)((u8 *)(*(Ovl11DispatchRecord **)(secondState + 0x8000 + 0x5DD4)) + 0x18);
        break;
    case 0x64:
        firstState = (char *)&D_8006C838;
        record = *(Ovl11DispatchRecord **)(firstState + 0x8000 + 0x5DD4);
        break;
    default:
        return;
    }
    Ovl11FillSpawnRecord(record, kind16, id, position);
}

/* Even an inactive selected slot has its stored position and ID copied. */
s32 Ovl11ReadSpawnRecord(u16 kind, s32 *position, s16 *id, s32 index) {
    Ovl11DispatchRecord *record;

    switch (kind) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        record = ((struct Ovl11DispatchState *) D_8006C838)->list10 + index;
        break;
    case 0x65:
        record = ((struct Ovl11DispatchState *) D_8006C838)->fixed + 1;
        break;
    case 0x64:
        record = ((struct Ovl11DispatchState *) D_8006C838)->fixed;
        break;
    default:
        return 0;
    }
    position[0] = record->x;
    position[1] = record->y;
    position[2] = record->z;
    *id = record->id;
    return record->flags & 1;
}
