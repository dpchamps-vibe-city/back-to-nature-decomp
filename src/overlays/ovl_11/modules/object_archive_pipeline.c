/*
 * Object-archive pipeline: dispatch/load, execute 16-byte records, release.
 * The same unit provides the program cursor, guarded sprite-bank transfer
 * and blocking object sound-bank refresh. These share the current-object
 * and graphics/sound work areas; they are not reentrant services.
 *
 * Public contracts: ovl11_object_archive_pipeline.h and
 * ovl11_guarded_sprite_bank.h. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "asset_scratch.h"
#include "screen_fade.h"
#include "cd_loader.h"
#include "game_flags.h"
#include "ovl11_guarded_sprite_bank.h"
#include "psyq/libapi.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_scene_value_commands.h"
#include "psyq/memory.h"
#include "ovl11_sprite_block_bank.h"
#include "ovl11_object_archive_pipeline.h"
#include "ovl11_active_record_sprites.h"

s32 Ovl11DispatchObjectLoadPhase(void) {
    char *work;
    s32 phase;
    s32 result;
    s32 (**handler)(void);

    work = (char *)&D_8006C838;
    *(s32 *)(work + 0x522C) = 0;
    *(s32 *)(work + 0x5230) = 0;
    phase = *(s32 *)(work + 0x7A74);
    handler = &gOvl11ObjectLoadHandlers[phase];
    result = (*handler)();
    *(s32 *)(work + 0x4450) &= ~0x20;
    return result;
}

s32 Ovl11GetObjectProgramCursor(void) {
    return (s32)Ovl11RecordCursor;
}

void ClearActiveOrderingTables(void);

void Ovl11ReloadObjectSoundBank(void) {
    s16 bankId;
    u8 savedRed;
    u8 savedGreen;
    u8 savedBlue;
    u8 *farWork;
    env_struct_0x134 *context;
    env_struct_0x134 *otherContext;

    if (GetGameFlag(2U) == 1) {
        bankId = ((struct Ovl11ObjectSoundRequestView *)D_8006C838)->bankId;
        farWork = (u8 *)&D_8007AFF0;
        if ((*(s32 *)(farWork + 0x2549C) != bankId) && (bankId != -1)) {
            context = gRenderContexts;
            savedRed = context->unk19;
            savedGreen = context->unk1A;
            savedBlue = context->unk1B;
            context->unk19 = 0;
            context->unk1A = 0;
            context->unk1B = 0;
            otherContext = context + 1;
            otherContext->unk19 = 0;
            otherContext->unk1A = 0;
            otherContext->unk1B = 0;
            DrawSync(0);
            ClearActiveOrderingTables();
            SetSoundCommandReadBuffer(0, gGraphicsHeapBase + 0x4290);
            StopSoundSequence(0);
            RewindSoundLoadSlots(2, 0);
            *(s32 *)(farWork + 0x25498) = (s32)((Ovl11ObjectSoundDescriptorView *)(*(u8 **)(farWork + 0x25388)))->valueE;
            QueueSoundLoadCommand(bankId, 0);
            *(s32 *)(farWork + 0x2549C) = (s32)bankId;
            ((struct Ovl11ObjectSoundRequestView *)D_8006C838)->bankId = -1;
            do {
                UpdateMasterVolumeAndSoundTransfer();
            } while (IsSoundTransferComplete() == 0);
            /* Both contexts receive context zero's saved color, not their
             * separate pre-transfer colors. */
            context = gRenderContexts;
            context->unk19 = savedRed;
            context->unk1A = savedGreen;
            context->unk1B = savedBlue;
            context++;
            context->unk19 = savedRed;
            context->unk1A = savedGreen;
            context->unk1B = savedBlue;
        }
    } else {
        gObjectSoundBankRequest = -1;
    }
}

s32 ovl_11_func_800E7798 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
void Ovl11RequestObjectProgramRelease(void) {
    u8 *base = (u8 *)D_8006C838;
    (*(s32 *)(base + 0x7A74)) = 2;
    ovl_11_func_800E7798(0, 0, 0, 0);
    (*(s32 *)(base + 0x522C)) = 0;
    (*(s32 *)(base + 0x5230)) = 0;
}

/* The sprite path is complementary to sound refresh: it is suppressed
 * when game flag 2 is exactly 1, rather than enabled by that value. */

s32 Ovl11ReloadGuardedSpriteBank(void) {
    s32 bufferAddress;

    if (GetGameFlag(2U) != 1) {
        if (D_8006C844 & 0x18000) {
            return 0;
        }
        DrawSync(0);
        ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
        bufferAddress = gSpriteBankArchiveBuffer;
        CdLoadArchiveRangeBlocking(0x1D, 0x1E, bufferAddress, bufferAddress);
        Ovl11UploadGuardedSpriteBank((u8 *) bufferAddress);
    }
    return 0;
}

void Ovl11UploadGuardedSpriteBank(u8 *buffer) {
    char *stageWork;
    s16 xOffset;

    if ((GetGameFlag(2U) != 1) && !(D_8006C844 & 0x18000)) {
        stageWork = (char *)&D_8007AFF0;
        if (*(s16 *)(stageWork + 0x25476) != 0x2C) {
            xOffset = ((*(u16 *)(*(s32 *)(stageWork + 0x25388) + 8) & 0x8000) == 0) << 8;
            if (*(s16 *)(stageWork + 0x25476) == 0x1B) {
                xOffset = 0;
            }
            Ovl11UploadSpriteBlockBank(buffer, xOffset, 0);
            gOvl11SpriteBankVramXOffset = xOffset;
        }
    }
}

s32 Ovl11GetCurrentObjectId(void) {
    s32 enabled;

    enabled = GetGameFlag(2);
    if (enabled == 0) {
        return 0;
    } else {
        return Ovl11CurrentObjectId[1];
    }
}

/* The four handlers retain original table/function order, NOT chronological
 * phase order: 0 start, 1 run, 2 release, 3 poll. A deferred start selects 3,
 * completion selects 1, opcode FF selects 2, and release selects 0. */

void ovl_11_func_800E5230(void);
void ovl_11_func_800E5294(void);
u8 *ovl_11_func_800E54F8(u16 *arg0, s32 arg1);
void ovl_11_func_800E499C(s32 arg0);
void ovl_11_func_800E5524(u8 *arg0, u8 *arg1, s32 arg2);
void ovl_11_func_800E5078(void);
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
        ovl_11_func_800E5524((u8 *)D_8006C838 + 0x20D8, data, Ovl11GetSpriteBlockSize(id));
        if (Ovl11GetSpriteBlockSize(id) >= 0xCE1U) {
            SystemError(0x53, ((struct Ovl11CurrentObjectView *)D_8006C838)->current[1]);
        }
        ovl_11_func_800E5078();
    }
    gOvl11ObjectProgramBase = Ovl11RecordCursor = D_8006E910;
    gOvl11ObjectProgramIndex = 0;
    Ovl11ClearValueSlotsCommand(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    ((struct Ovl11ObjectSoundRequestView *)base)->bankId = -1;
    ((struct Ovl11E4280State *)base)->selector = 1;
    return 0;
}

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
                    /* Bookkeeping reads the NEW record's branch byte. */
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
    Ovl11TickActiveRecordSprites();
    Ovl11DrawAttachmentRecordSprites();
    ovl_11_func_800E4428();
    return 0;
}

void ovl_11_func_800E549C(void);
void ovl_11_func_800E537C(void);

s32 Ovl11ReleaseObjectLoad(void) {
    ((struct Ovl11E4280State *)D_8006C838)->flags_C &= ~0x400000;
    Ovl11DrawAttachmentRecordSprites();
    ovl_11_func_800E4428();
    ovl_11_func_800E54C8();
    ovl_11_func_800E549C();
    ((struct Ovl11E4280State *)D_8006C838)->field_5214 = 0;
    ((struct Ovl11E4280State *)D_8006C838)->field_448C = 0;
    ((struct Ovl11E4280State *)D_8006C838)->flags_4450 &= ~0x10;
    ovl_11_func_800E5230();
    if (Ovl11HasQueuedRecordSlots() == 1) {
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
    Ovl11ClearValueSlotsCommand(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    *(s16 *)(base + 0x4438) = -1;
    *(s32 *)(base + 0x7A74) = 1;
    return 0;
}
