/*
 * Object sprite resources: archive installation, rendering and reset.
 * Original ovl_11:E44D4..E5294, with every entry in original link order.
 *
 * The archive installer binds the ten entity sources, the attachment renderer
 * consumes them in parallel with ten spawn records, and reset unbinds them.
 * The same unit installs the 37 event-record sources and renders those records.
 * Archive storage and relocated sprite data must outlive every bound source.
 *
 * Public contracts remain separated by caller responsibility:
 * ovl11_object_program_install.h, ovl11_active_record_sprites.h and the
 * entity-source reset in ovl11_object_load_resets.h. No storage is allocated
 * here; helpers retain their existing external linkage. Pause-bit differences,
 * unchecked indices and non-reentrant installation are documented there.
 */

#include "common.h"
#include "ovl11_object_program_install.h"
#include "game_types.h"
#include "ovl11_active_record_sprites.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "asset_scratch.h"
#include "ovl11_state_object_pool_clear.h"
#include "sprite_upload.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_projected_record_sprites.h"
#include "ovl11_surface_height.h"
#include "ovl11_shared_animation_icon.h"
#include "ovl11_active_record_sprite_packet.h"
#include "ovl11_object_event_script.h"
#include "ovl11_object_load_resets.h"
#include "ovl11_spawn_record_access.h"

void Ovl11UpdateRecordSprite(u8 *record);
void Ovl11DrawEntityRecordSprite(Ovl11EntitySpritePositionView *position,
                                SpriteSourceData *source);
void Ovl11DrawCurrentObjectSprite(void);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                  s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);

void Ovl11TickActiveRecordSprites(void) {
    s32 slotIndex;
    struct_80076220 *slot;

    if (!(D_8006C844 & 0x800000)) {
        for (slotIndex = 0; slotIndex < 37; slotIndex++) {
            slot = &(&gObjectEventSlotBase)[slotIndex];
            if ((*(s32 *)((u8 *)slot + 0x1C) & 0x18000000) == 0x18000000) {
                Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)slot + 0xE0));
                Ovl11UpdateRecordSprite(slot->unk0);
            }
        }
    }
}

/* Entity records and sprite sources are parallel arrays with different strides. */
void Ovl11DrawAttachmentRecordSprites(void) {
    u8 *work;
    u8 *entity;
    SpriteSourceData *source;
    s32 entityOffset;
    s32 remaining;

    if (!(D_8006C844 & 0x800000)) {
        work = D_80074838;
        source = (SpriteSourceData *)gOvl11EntitySpriteSources;
        entityOffset = 0;
        remaining = 9;
        do {
            entity = (u8 *)(*(s32 *)(work + 0x5DD0) + entityOffset);
            if (*(u16 *)(entity + 4) & 1) {
                Ovl11DrawEntityRecordSprite((Ovl11EntitySpritePositionView *)entity, source);
            }
            source += 1;
            remaining -= 1;
            entityOffset += 0x18;
        } while (remaining >= 0);
        Ovl11DrawCurrentObjectSprite();
    }
}

void Ovl11DrawCurrentObjectSprite(void) {
    struct Ovl11CurrentObjectSpriteWork *state = (struct Ovl11CurrentObjectSpriteWork *)D_8006C838;
    s32 screen[4];
    u16 box[3];

    if (!(state->flags & 0x800000) && (state->drawSuppressed == 0)) {
        box[0] = state->xLow + 0x96;
        box[1] = state->yLow - 0x64;
        box[2] = state->zLow - 0x96;
        if (Ovl11ProjectRecordSprite(box, screen) != 0) {
            Ovl11DrawActiveRecordSpritePacket(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4),
                               (s16)screen[0], (s16)screen[1]);
        }
    }
}

void Ovl11UpdateRecordSprite(u8 *record) {
    s32 screen[4];
    u16 box[3];
    s32 ot;
    s32 paused;

    paused = ((Ovl11RecordSpriteWork *)D_8006C838)->flags & 0x08000000;
    if (!paused) {
        if (*(s32 *)(record + 0x70) || *(s32 *)(record + 0x74) ||
            *(s32 *)(record + 0x78)) {
            *(s32 *)(record + 0x60) += *(s32 *)(record + 0x70);
            *(s32 *)(record + 0x64) += *(s32 *)(record + 0x74);
            *(s32 *)(record + 0x68) += *(s32 *)(record + 0x78);
            Ovl11ResolveSurfaceHeight((s32 *)(record + 0x60), 0xC000);
        }
    }
    box[0] = *(u16 *)(record + 0x60) + 0x96;
    box[1] = *(u16 *)(record + 0x64) - 0x64;
    box[2] = *(u16 *)(record + 0x68) - 0x96;
    if (Ovl11ProjectRecordSprite(box, screen)) {
        ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
        if (GetGameFlag(0x72) && *(s16 *)record == ((Ovl11RecordSpriteWork *)D_8006C838)->selectedId) {
            ot -= 0x14;
        }
        SetSpriteAnimationUpdateLatch(0);
        func_80016054(ot, gActiveRenderContext->field_118, (s32)(record + 0xB0),
                      record[0xB4], record[0xB5], (s16)screen[0], (s16)screen[1],
                      *(s32 *)(record + 0xB8), 0, 0x40);
        Ovl11DrawTimedSpriteEffect(record + 0xE0, ot, (s16)screen[0],
                                  (s16)(screen[1] - 0x2D));
        if (!(*(u16 *)(record + 0x20) & 0x8000)) {
            Ovl11DrawSharedAnimationIcon(ot, screen[0], screen[1],
                                        (*(u16 *)(record + 0x20) >> 14) & 1);
        }
    }
    /* These record codes keep animating even while movement is paused. */
    if (!paused || ((Ovl11RecordSpriteWork *)D_8006C838)->current[1] == 0x19D ||
        ((Ovl11RecordSpriteWork *)D_8006C838)->current[1] == 0x1BC ||
        ((Ovl11RecordSpriteWork *)D_8006C838)->current[1] == 0x1BD ||
        ((Ovl11RecordSpriteWork *)D_8006C838)->current[1] == 0x1BE ||
        ((Ovl11RecordSpriteWork *)D_8006C838)->current[1] == 0x1BF) {
        UpdateSpriteAnimation((SpriteSourceData *)(record + 0xB0));
    }
}

void Ovl11DrawEntityRecordSprite(Ovl11EntitySpritePositionView *position,
                                SpriteSourceData *source) {
    s32 screen[4];
    u16 box[3];

    box[0] = position->xLow + 0x96;
    box[1] = position->yLow - 0x64;
    box[2] = position->zLow - 0x96;
    if (Ovl11ProjectRecordSprite(box, screen) != 0) {
        func_80016054(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4),
                      gActiveRenderContext->field_118, (s32)source, (s32)source->field_4,
                      (u8)(s32)source->field_5, (s16)screen[0],
                      (s16)screen[1], source->field_8, 0, 0x40);
    }
    /* Animation advances even when projection culled the entity. */
    if (!(D_8006C844 & 0x08000000)) {
        UpdateSpriteAnimation(source);
    }
}

/*
 * Install object program archives and their sprite sources.
 *
 * The first two halfwords are flags and an offset count. Halfwords 2..25
 * tag up to 24 entries; signed byte offsets start at +0x34, and payloads
 * start at +0x38 + count*4. Program data is copied into the interpreter bank;
 * sprite data is relocated below asset-scratch end before source pointers
 * are installed. All helpers retain external linkage, not inferred static.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
s32 Ovl11GetObjectArchiveSpriteDataSize(s32 archiveAddress);
s32 Ovl11GetObjectArchiveEntryOffset(s32 archiveAddress, s32 entry);
s32 Ovl11GetObjectArchiveEntryAddress(s32 archiveAddress, s32 entry);
void Ovl11UploadObjectArchiveImages(s32 archiveAddress);
s32 Ovl11GetObjectArchiveProgramSize(s32 archiveAddress);
s32 Ovl11FindObjectArchiveProgramEntry(s32 archiveAddress);
void Ovl11CollectObjectArchiveSpriteOffsets(s32 archiveAddress);
void Ovl11InstallObjectArchiveSpriteSources(s32 archiveAddress);
void Ovl11InstallEntitySpriteSource(SpriteDataHeader *header, s32 selector);

void Ovl11InstallObjectProgramArchive(s32 archiveAddress) {
    s32 spriteBytes;
    u8 *destination;
    u32 programBytes;
    s32 entry;

    gOvl11ArchiveTextureX = 0x180;
    gOvl11ArchiveTextureY = 0;
    if (*(u16 *)archiveAddress & 0x20) {
        gOvl11ArchiveTextureX = 0;
        gOvl11ArchiveTextureY = 0;
        ((struct Ovl11PoolResetView *)D_8006C838)->flags |= 0x10;
    }
    spriteBytes = Ovl11GetObjectArchiveSpriteDataSize(archiveAddress);
    destination = (u8 *)(GetAssetScratchEnd() - spriteBytes);
    gOvl11InstalledSpriteDataBytes = spriteBytes;
    Ovl11CopyPreviousObjectSpriteSources();
    programBytes = Ovl11GetObjectArchiveProgramSize(archiveAddress);
    if (programBytes > 0xCE0) {
        SystemError(0x53, Ovl11CurrentObjectId[1]);
    }
    entry = Ovl11FindObjectArchiveProgramEntry(archiveAddress);
    memcpy(gOvl11ObjectProgramRecords,
           (void *)Ovl11GetObjectArchiveEntryAddress(archiveAddress, entry), programBytes);
    Ovl11UploadObjectArchiveImages(archiveAddress);
    if (spriteBytes != 0) {
        memmove(destination, (u8 *)Ovl11GetObjectArchiveEntryAddress(archiveAddress, 0), spriteBytes);
        gOvl11InstalledSpriteData = destination;
        Ovl11CollectObjectArchiveSpriteOffsets(archiveAddress);
        Ovl11InstallObjectArchiveSpriteSources(archiveAddress);
        Ovl11ClearEligibleStateObjects(1);
    } else {
        gOvl11InstalledSpriteData = 0;
    }
}

/* The first entry not tagged 0x3C00 marks the end of relocatable sprites. */
s32 Ovl11GetObjectArchiveSpriteDataSize(s32 archiveAddress) {
    s32 tagIndex;
    s16 tag;

    for (tagIndex = 2; tagIndex < 0x1A; tagIndex++) {
        tag = *(s16 *)(archiveAddress + tagIndex * 2);
        if ((tag & 0x3C00) == 0) {
            break;
        }
    }
    if (tagIndex == 0x1A) {
        return 0;
    }
    return Ovl11GetObjectArchiveEntryOffset(archiveAddress, tagIndex - 2);
}

s32 Ovl11GetObjectArchiveEntryOffset(s32 archiveAddress, s32 entry) {
    return *(s32 *)(entry * 4 + archiveAddress + 0x34);
}

s32 Ovl11GetObjectArchiveEntryAddress(s32 archiveAddress, s32 entry) {
    s32 payloadOffset;
    s32 payloadAddress;

    payloadOffset = (*(s16 *)(archiveAddress + 2) << 2) + 0x38;
    payloadAddress = archiveAddress + payloadOffset;
    return payloadAddress + Ovl11GetObjectArchiveEntryOffset(archiveAddress, entry);
}

void Ovl11UploadObjectArchiveImages(s32 archiveAddress) {
    s32 tagIndex;

    /* Index the header directly: a separate pointer induction variable changes
     * the compiler's two-pass invariant motion for the texture coordinates. */
    tagIndex = 2;
    do {
        if (((s16 *)archiveAddress)[tagIndex] & 0x8000) {
            UploadSpriteImagesKeepingWideType2Positions(
                (u8 *)Ovl11GetObjectArchiveEntryAddress(archiveAddress, tagIndex - 2),
                gOvl11ArchiveTextureX, gOvl11ArchiveTextureY,
                gOvl11ArchiveTextureX, (s16)(s32)gOvl11ArchiveTextureY);
        }
        tagIndex += 1;
    } while (tagIndex < 0x1A);
}

s32 Ovl11GetObjectArchiveProgramSize(s32 archiveAddress) {
    s32 entry;
    s32 end;
    s32 start;

    entry = Ovl11FindObjectArchiveProgramEntry(archiveAddress);
    end = Ovl11GetObjectArchiveEntryOffset(archiveAddress, ((s16)entry) + 1);
    start = Ovl11GetObjectArchiveEntryOffset(archiveAddress, ((s16)entry));
    return end - start;
}

s32 Ovl11FindObjectArchiveProgramEntry(s32 archiveAddress) {
    s32 tagIndex;
    s16 tag;

    for (tagIndex = 2; tagIndex < 0x1A; tagIndex++) {
        tag = *(s16 *)(archiveAddress + tagIndex * 2);
        if ((tag & 0x4000) != 0) {
            break;
        }
    }
    if (tagIndex == 0x1A) {
        SystemError(0x4E, Ovl11CurrentObjectId[1]);
    }
    return tagIndex - 2;
}

void Ovl11CollectObjectArchiveSpriteOffsets(s32 archiveAddress) {
    s16 *tag;
    s32 *offset;
    s32 tagIndex;
    s32 count;

    count = 0;
    tagIndex = 2;
    tag = (s16 *)(archiveAddress + 4);
    offset = gOvl11InstalledSpriteOffsets;
    do {
        s16 value = *tag;
        if (value & 0x3C00) {
            if (count >= 0x1A) {
                SystemError(0x41, 0x3E7);
            }
            *offset = Ovl11GetObjectArchiveEntryOffset(archiveAddress, tagIndex - 2);
            offset += 1;
            count += 1;
        }
        tagIndex += 1;
        tag += 1;
    } while (tagIndex < 0x1A);
}

void Ovl11InstallObjectArchiveSpriteSources(s32 archiveAddress) {
    s32 tagIndex;
    SpriteSourceData *source;
    s16 tag;
    s16 objectId;
    s32 *offsets;
    s32 selector;
    s32 entry;
    s32 maskOrAddress;
    u8 *spriteData;
    u8 *work;
    void *record;

    for (tagIndex = 2; tagIndex < 0x1A; tagIndex++) {
        tag = ((s16 *)archiveAddress)[tagIndex];
        maskOrAddress = 0xFFFF03FF;
        offsets = gOvl11InstalledSpriteOffsets;
        entry = tagIndex - 2;
        spriteData = gOvl11InstalledSpriteData + offsets[entry];
        selector = tag & maskOrAddress;
        if (tag == 0x401) {
            InitSpriteSourceData((SpriteSourceData *)gPrimaryObjectSpriteSource, (SpriteDataHeader *)spriteData);
            SetSpriteSourceTextureOffsets((Struct_800154CC *)gPrimaryObjectSpriteSource,
                                         gOvl11ArchiveTextureX, gOvl11ArchiveTextureY, 0, 0);
        } else if (tag & 0x2000) {
            source = (SpriteSourceData *)(gObjectEventSpriteSourceBase + (selector * 0x1D4));
            InitSpriteSourceData(source, (SpriteDataHeader *)spriteData);
            if (selector == 0x21) {
                objectId = ((struct Ovl11CurrentObjectView *)(gPrimaryObjectSpriteSource - 0x5458))->current[1];
                source = (SpriteSourceData *)D_8006C838;
                if ((objectId == 0xB6) || (objectId == 0x131)) {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *)(gObjectEventSpriteSourceBase + 0x3C54),
                                                 gOvl11ArchiveTextureX, gOvl11ArchiveTextureY, 0, 0);
                } else {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *)(gObjectEventSpriteSourceBase + 0x3C54),
                                                 gOvl11ArchiveTextureX, gOvl11ArchiveTextureY,
                                                 gOvl11ArchiveTextureX, (s16)(s32)gOvl11ArchiveTextureY);
                }
            } else {
                work = (u8 *)source;
                source = (SpriteSourceData *)D_8006C838;
                SetSpriteSourceTextureOffsets((Struct_800154CC *)work,
                                             gOvl11ArchiveTextureX, gOvl11ArchiveTextureY,
                                             gOvl11ArchiveTextureX, (s16)(s32)gOvl11ArchiveTextureY);
            }
            work = (u8 *)D_8006C838;
            if (((struct Ovl11CurrentObjectView *)work)->current[1] != 0xA1) {
                record = (void *)(work + (selector * 0x1D4));
                ((Ovl11EF488FlagView *)record)->flags |= 0x1000;
            }
        } else if (tag == 0x404) {
            InitSpriteSourceData(&gObjectArchiveSpecialSpriteSource, (SpriteDataHeader *)&D_8007AFF8);
            ((Ovl11EF488FlagView *)(gPrimaryObjectSpriteSource - 0x5458 + selector * 0x1D4))->flags |= 0x1000;
        } else if (tag == 0x405) {
            maskOrAddress = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[2];
            InitSpriteSourceData(&gObjectArchiveSpecialSpriteSource, (SpriteDataHeader *)maskOrAddress);
            ((Ovl11EF488FlagView *)(gPrimaryObjectSpriteSource - 0x5458 + selector * 0x1D4))->flags |= 0x1000;
        } else if (tag == 0x406) {
            maskOrAddress = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[6];
            InitSpriteSourceData(&gObjectArchiveSpecialSpriteSource, (SpriteDataHeader *)maskOrAddress);
            ((Ovl11EF488FlagView *)(gPrimaryObjectSpriteSource - 0x5458 + selector * 0x1D4))->flags |= 0x1000;
        } else if (tag & 0x1000) {
            /* Recognized but deliberately does not install a sprite source. */
        } else if (tag & 0x800) {
            Ovl11InstallEntitySpriteSource((SpriteDataHeader *)spriteData, selector);
        }
    }
}

void Ovl11CopyPreviousObjectSpriteSources(void) {
    s32 index;

    for (index = 0; index < 0x25; index++) {
        *(SpriteSourceData *)(gObjectEventSpriteSourceBase + index * 0x1D4) =
            *(SpriteSourceData *)(gObjectEventSpriteSourceBase - 0x30 + index * 0x1D4);
        if (((struct Ovl11CurrentObjectView *)D_8006C838)->current[1] == 0x82) {
            Ovl11SelectScriptSpriteSource(index, (SpriteSourceData *)(gObjectEventSpriteSourceBase + index * 0x1D4));
        }
    }
}

/* First free source among ten slots. Its parallel 0x18-byte side record
 * receives the selector at +2. A full pool is silently left unchanged.
 * Keep the byte and typed cursors separate for the original scan code. */
void Ovl11InstallEntitySpriteSource(SpriteDataHeader *header, s32 selector) {
    Ovl11EntitySpriteSlot *pool;
    Ovl11EntitySpriteSlot *slot;
    u8 *work;
    s32 sourceOffset;
    s32 nextIndex;
    s32 sideOffset;

    pool = gOvl11EntitySpriteSources;
    nextIndex = 0;
    work = D_80074838;
    sideOffset = 0;
    slot = pool;
    sourceOffset = 0;
scan_slot:
    nextIndex += 1;
    if (((Ovl11EntitySpriteSlot *)((u8 *)pool + sourceOffset))->sourceHeaderAddress == 0) {
        InitSpriteSourceData(slot, header);
        SetSpriteSourceTextureOffsets((Struct_800154CC *)slot,
                                     gOvl11ArchiveTextureX, gOvl11ArchiveTextureY,
                                     gOvl11ArchiveTextureX, (s16)(s32)gOvl11ArchiveTextureY);
        *(s16 *)((u8 *)(sideOffset + *(s32 *)(work + 0x5DD0)) + 2) = selector;
        return;
    }
    sideOffset += 0x18;
    slot += 1;
    sourceOffset += 0x30;
    if (nextIndex >= 0xA) {
        return;
    }
    goto scan_slot;
}

/* Unbind the source pool without clearing the rest of each source record.
 * The parallel spawn records are cleared separately, and current-object
 * drawing stays suppressed until its source is reinitialized. */

void Ovl11ResetEntitySpriteSources(void) {
    s32 *sourceHeader;
    s32 *work;
    s32 *farWork;
    s32 slot;

    slot = 9;
    sourceHeader = (s32 *)gOvl11EntitySpriteSources;
    /* Last slot's +14 header address: 9 * 30 + 14, in hexadecimal. */
    sourceHeader = (s32 *)((char *)sourceHeader + 0x1C4);
    while (slot >= 0) {
        *sourceHeader = 0;
        sourceHeader = (s32 *)((char *)sourceHeader - 0x30);
        slot--;
    }
    work = (s32 *)&D_8006C838;
    farWork = work + (0x8000 >> 2);
    Ovl11ClearSpawnRecords((Ovl11DispatchRecord *)farWork[0x5DD0 >> 2],
                          *(s16 *)((char *)farWork + 0x5DFA));
    *(s16 *)((char *)work + 0x4476) = 1;
}
