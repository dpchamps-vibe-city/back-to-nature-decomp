/*
 * object_load_states — reconstructed translation unit.
 *
 * Four table-selected handlers for object-archive load, record-program
 * execution, release and asynchronous load completion. The selector at
 * primary state +0x7A74 and shared program cursor bind the handlers; table
 * order is original function order. Selector 0 starts a load (possibly
 * synchronous), 3 polls the scratch-buffer read, 1 runs the record program,
 * and 2 releases it. Start/poll/run return zero; release returns one.
 *
 * Requires valid current-object id, archive-offset pairs and record/opcode
 * indices. No record bound or instruction budget is imposed: execution runs
 * to opcode 0xFF or a record yield bit. Load completion resets the shared
 * program base/cursor/index and publishes selector 1. GPU/OT clearing and
 * asset-scratch reuse are observable effects, not private allocations.
 * Branch bookkeeping deliberately reads the new cursor's branch byte after
 * the cursor changes; it is not replaced by a computed array index.
 * The table is the external interface, so no new callback header is needed.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"
#include "asset_scratch.h"
#include "screen_fade.h"
#include "cd_loader.h"
#include "game_flags.h"

void ClearActiveOrderingTables(void);
/* The callee's four-byte flag aggregates use the scalar word calling ABI. */
void ovl_11_func_800E5230(void);
void ovl_11_func_800E5294(void);
u8 *ovl_11_func_800E54F8(u16 *arg0, s32 arg1);
void ovl_11_func_800E499C(s32 arg0);
s32 ovl_11_func_800E3978(s32 arg0);
void ovl_11_func_800E5524(u8 *arg0, u8 *arg1, s32 arg2);
void ovl_11_func_800E5078(void);
s32 ovl_11_func_800EEBC8(s32 unused0, s32 unused1, s32 unused2, s32 unused3);
void ovl_11_func_800E54C8(void);

s32 Ovl11StartObjectArchiveLoad(void) {
    s16 id;
    s32 size;
    s32 offset;
    u8 *result;
    s32 needsRead;
    s32 useAsync;
    u8 *data;
    u8 *base;

    data = NULL;
    if (((struct Ovl11CurrentObjectView *)D_8006C838)->current[3] != 1) {
        SetGameFlag((u16)(((struct Ovl11CurrentObjectView *)D_8006C838)->current[1] + 0xFB), 1);
    }
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[2] = 0;
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[4] = 0;
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[5] = 0;
    id = ((struct Ovl11CurrentObjectView *)D_8006C838)->current[1];
    offset = gOvl11ObjectArchiveOffsets[id];
    needsRead = 1;
    size = gOvl11ObjectArchiveOffsets[id + 1] - offset;
    ovl_11_func_800E5230();
    ovl_11_func_800E5294();
    if ((u32)(id - 0x15F) < 0x32U) {
        data = ovl_11_func_800E54F8((u16 *)((u8 *)D_8006C838 + 0xD8), id);
        needsRead = data == NULL;
    }
    useAsync = 0;
    if (needsRead != 0) {
        if ((size < GetAssetScratchCapacity()) && (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0xF)) {
            useAsync = id != 0x12D;
        }
        if (useAsync != 0) {
            CdReadArchiveBytes(0xC, offset, size, (u8 *)GetAssetScratchStart(), 0, 1);
            ((struct Ovl11E4280State *)D_8006C838)->selector = 3;
            return 0;
        }
        DrawSync(0);
        if (gRenderContexts[0].field_11C < size) {
            ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
            data = (u8 *)gRenderContexts[0].field_124;
            if (gRenderContexts[0].field_11C * 2 < size) {
                ClearActiveOrderingTables();
            }
        } else if ((void *)gActiveRenderContext == (void *)gRenderContexts) {
            data = (u8 *)gRenderContexts[1].field_124;
        } else {
            data = (u8 *)gRenderContexts[0].field_124;
        }
        CdReadArchiveBytes(0xC, offset, size, data, 0, 1);
        do {
            result = CdReadArchiveBytes(0xC, offset, size, data, 0, 0);
        } while (result == NULL);
        ovl_11_func_800E499C((s32)result);
    } else {
        ovl_11_func_800E5524((u8 *)D_8006C838 + 0x20D8, data, ovl_11_func_800E3978(id));
        if (ovl_11_func_800E3978(id) >= 0xCE1U) {
            SystemError(0x53, ((struct Ovl11CurrentObjectView *)D_8006C838)->current[1]);
        }
        ovl_11_func_800E5078();
    }
    gOvl11ObjectProgramBase = Ovl11RecordCursor = D_8006E910;
    gOvl11ObjectProgramIndex = 0;
    ovl_11_func_800EEBC8(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    ((struct struct_8006C838_800E3AA4 *)base)->field_4438 = -1;
    ((struct Ovl11E4280State *)base)->selector = 1;
    return 0;
}

void ovl_11_func_800E44D4(void);
void ovl_11_func_800E4568(void);
void ovl_11_func_800E4428(void);

s32 Ovl11RunObjectRecordProgram(void) {
    Ovl11E910Entry *record;

    if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 == 0x18) {
        return 0;
    }
    if (!(((struct Ovl11E4280State *)D_8006C838)->flags_C & 0x10000000)) {
        for (;;) {
            if (Ovl11RecordCursor->unk0 == 0xFF) {
                ((struct Ovl11E4280State *)D_8006C838)->selector = 2;
                break;
            }
            record = Ovl11RecordCursor;
            if (gOvl11ObjectOpcodeHandlers[record->unk0](
                    *(s16 *)((u8 *)record + 4),
                    *(s16 *)((u8 *)record + 6),
                    *(s32 *)((u8 *)record + 8),
                    *(s32 *)((u8 *)record + 0xC)) == 1) {
                if (Ovl11RecordCursor->unk1 == 0xFF) {
                    Ovl11RecordCursor++;
                    gOvl11ObjectProgramIndex++;
                } else {
                    Ovl11RecordCursor = &gOvl11ObjectProgramBase[Ovl11RecordCursor->unk1];
                    gOvl11ObjectProgramIndex = Ovl11RecordCursor->unk1;
                }
            } else {
                if (Ovl11RecordCursor->unk2 == 0xFF) {
                    Ovl11RecordCursor++;
                    gOvl11ObjectProgramIndex++;
                } else {
                    Ovl11RecordCursor = &gOvl11ObjectProgramBase[Ovl11RecordCursor->unk2];
                    gOvl11ObjectProgramIndex = Ovl11RecordCursor->unk2;
                }
            }
            if (record->unk3[0] & 1) {
                break;
            }
        }
    }
    ovl_11_func_800E44D4();
    ovl_11_func_800E4568();
    ovl_11_func_800E4428();
    return 0;
}

void ovl_11_func_800E549C(void);
s32 ovl_11_func_800DBF60(void);
void ovl_11_func_800E537C(void);

s32 Ovl11ReleaseObjectLoad(void) {
    ((struct Ovl11E4280State *)D_8006C838)->flags_C &= ~0x400000;
    ovl_11_func_800E4568();
    ovl_11_func_800E4428();
    ovl_11_func_800E54C8();
    ovl_11_func_800E549C();
    ((struct Ovl11E4280State *)D_8006C838)->field_5214 = 0;
    ((struct Ovl11E4280State *)D_8006C838)->field_448C = 0;
    ((struct Ovl11E4280State *)D_8006C838)->flags_4450 &= ~0x10;
    ovl_11_func_800E5230();
    if (ovl_11_func_800DBF60() == 1) {
        ovl_11_func_800E5294();
    } else {
        ovl_11_func_800E537C();
    }
    ((struct Ovl11E4280State *)D_8006C838)->selector = 0;
    return 1;
}

/* Poll returns early while the fade is complete and the read is pending;
 * otherwise it waits here until the archive bytes are available. */
s32 Ovl11PollObjectArchiveLoad(void) {
    s16 index;
    s32 offset;
    s32 size;
    u8 *data;
    u8 *base;

    index = Ovl11CurrentObjectId[1];
    offset = gOvl11ObjectArchiveOffsets[index];
    size = gOvl11ObjectArchiveOffsets[index + 1] - offset;
    do {
        data = CdReadArchiveBytes(12, offset, size, (u8 *)GetAssetScratchStart(), 0, 0);
        if (data == 0 && IsScreenFadeComplete() != 0) {
            return 0;
        }
    } while (data == 0);
    ovl_11_func_800E499C((s32)data);
    Ovl11RecordCursor = D_8006E910;
    gOvl11ObjectProgramBase = D_8006E910;
    gOvl11ObjectProgramIndex = 0;
    ovl_11_func_800EEBC8(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    *(s16 *)(base + 0x4438) = -1;
    *(s32 *)(base + 0x7A74) = 1;
    return 0;
}
