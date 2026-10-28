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
#include "ovl11_object_program_install.h"
#include "game_types.h"
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
#include "ovl11_object_event_script.h"

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
            source = (SpriteSourceData *)(D_800762D0 + (selector * 0x1D4));
            InitSpriteSourceData(source, (SpriteDataHeader *)spriteData);
            if (selector == 0x21) {
                objectId = ((struct Ovl11CurrentObjectView *)(gPrimaryObjectSpriteSource - 0x5458))->current[1];
                source = (SpriteSourceData *)D_8006C838;
                if ((objectId == 0xB6) || (objectId == 0x131)) {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *)(D_800762D0 + 0x3C54),
                                                 gOvl11ArchiveTextureX, gOvl11ArchiveTextureY, 0, 0);
                } else {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *)(D_800762D0 + 0x3C54),
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
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)&D_8007AFF8);
            ((Ovl11EF488FlagView *)(gPrimaryObjectSpriteSource - 0x5458 + selector * 0x1D4))->flags |= 0x1000;
        } else if (tag == 0x405) {
            maskOrAddress = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[2];
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)maskOrAddress);
            ((Ovl11EF488FlagView *)(gPrimaryObjectSpriteSource - 0x5458 + selector * 0x1D4))->flags |= 0x1000;
        } else if (tag == 0x406) {
            maskOrAddress = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[6];
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)maskOrAddress);
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
        *(SpriteSourceData *)(D_800762D0 + index * 0x1D4) =
            *(SpriteSourceData *)(D_800762D0 - 0x30 + index * 0x1D4);
        if (((struct Ovl11CurrentObjectView *)D_8006C838)->current[1] == 0x82) {
            Ovl11SelectScriptSpriteSource(index, (SpriteSourceData *)(D_800762D0 + index * 0x1D4));
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
