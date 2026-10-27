/*
 * save_image — reconstructed translation unit.
 *
 * Serialize and restore persistent state, insert/remove per-record checksum
 * slots, and write/verify the blocked save image.
 *
 * Public ordering/lifetime contracts are in ovl15_save_image.h. Byte-layout
 * views retain neutral names where gameplay meaning is not established.
 * The evidence for membership and boundaries is in notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/memory.h"
#include "random.h"
#include "ovl15_save_image.h"

/* Aggregate assignment preserves each block's witnessed copy width/alignment;
 * replacing these typed transfers with one bulk copy would also copy holes. */
#define SAVE(type, source, offset) \
    (*(type *)(gOvl15SaveWorkBase + (offset)) = *(type *)(source))

void Ovl15SerializeSaveImage(void)
{
    s32 i;
    u8 *record;

    gOvl15SaveWorkBase = gOvl15SavePayload;
    SAVE(Ovl15SaveBlock1F34, &gSmallTaggedRecordBase, 0);
    SAVE(Ovl15SaveBlock18, D_8006C838, 0x225C);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2274 = *(s32 *)((u8 *)D_8006C838 + 0x20);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2278 = *(s32 *)((u8 *)D_8006C838 + 0x24);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_227C = *(s32 *)((u8 *)D_8006C838 + 0x28);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2280 = *(s32 *)((u8 *)D_8006C838 + 0x2C);
    SAVE(Ovl15SaveBlock8, (u8 *)D_8006C838 + 0x30, 0x2284);
    SAVE(Ovl15SaveBlock94, (u8 *)D_8006C838 + 0x38, 0x228C);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2320 = ((s32 *)&gScenePhase)[0];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2324 = ((s32 *)&gScenePhase)[1];
    SAVE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 8, 0x2328);
    SAVE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 0x1C, 0x233C);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2350 = ((s32 *)&gScenePhase)[12];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2354 = ((s32 *)&gScenePhase)[13];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2358 = ((s32 *)&gScenePhase)[14];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_235C = ((s32 *)&gScenePhase)[15];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_2360 = ((s32 *)&gScenePhase)[16];
    SAVE(Ovl15SaveBlockCFC, (u8 *)&gScenePhase + 0x44, 0x2364);
    SAVE(Ovl15SaveBlock3FC, &gPrimaryObjectBase, 0x3060);
    SAVE(Ovl15SaveBlock2328, gMainAreaGrid, 0x345C);
    SAVE(Ovl15SaveBlock188, gAlternateAreaGrid, 0x5784);
    SAVE(Ovl15SaveBlock40, &gRecordSlotPhase, 0x590C);
    SAVE(Ovl15SaveBlock74, D_8007A5C4, 0x594C);
    SAVE(Ovl15SaveBlock6C8, gObjectRecordPool, 0x59C0);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_6088 = D_8007AD00[0];
    SAVE(Ovl15SaveBlockC, (u8 *)D_8007AD00 + 4, 0x608C);
    SAVE(Ovl15SaveBlock3C, (u8 *)D_8007AD00 + 0x10, 0x6098);
    SAVE(Ovl15SaveBlock108, gTrackedRecordCounters, 0x6200);
    SAVE(Ovl15SaveBlock24, &gScreenColorRamp, 0x6308);
    SAVE(Ovl15SaveBlock34, D_8007AFA4, 0x632C);
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_6360 = D_8007AFD8[0];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_6364 = D_8007AFD8[1];
    ((Ovl15SaveBufferWords *)gOvl15SaveWorkBase)->field_6368 = D_8007AFD8[2];
    SAVE(Ovl15SaveBlockC, (u8 *)D_8007AFD8 + 0xC, 0x636C);
    for (i = 0; i < 37; i++) {
        record = (u8 *)D_8006C838 + i * 0x1D4;
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].bytes = *(Ovl15SaveBytes4 *)(record + 0x9A0C);
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_4 = *(u16 *)(record + 0x99EA);
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_6 = *(u16 *)(record + 0x99EC);
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_8 = *(u16 *)(record + 0x9A06);
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_A = *(u16 *)(record + 0x9A08);
        ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_C = *(u16 *)(record + 0x9A0A);
    }
    /* The serialized image deliberately discards transient slot activation. */
    SAVE(Ovl15SaveBlockF0, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDCC, 0x213C);
    SAVE(Ovl15SaveBlock30, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDD4, 0x222C);
    for (i = 0; i < 10; i++) {
        ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B4[i].state = 0;
    }
    for (i = 0; i < 20; i++) {
        ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B8[i].state = 0;
    }
    memset(gOvl15SaveWorkBase + 0x1CF8, 0, 0x21C);
}

#undef SAVE

#define RESTORE(type, destination, offset) \
    (*(type *)(destination) = *(type *)(gOvl15SaveWorkBase + (offset)))

void Ovl15RestoreSaveImage(void)
{
    s32 i;
    u8 *record;

    gOvl15SaveWorkBase = gOvl15SavePayload;
    RESTORE(Ovl15SaveBlockF0, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDCC, 0x213C);
    RESTORE(Ovl15SaveBlock30, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDD4, 0x222C);
    for (i = 0; i < 37; i++) {
        record = (u8 *)D_8006C838 + i * 0x1D4;
        *(Ovl15SaveBytes4 *)(record + 0x9A0C) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].bytes;
        *(u16 *)(record + 0x99EA) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_4;
        *(u16 *)(record + 0x99EC) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_6;
        *(u16 *)(record + 0x9A06) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_8;
        *(u16 *)(record + 0x9A08) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_A;
        *(u16 *)(record + 0x9A0A) = ((Ovl15SavedRecordList *)gOvl15SaveWorkBase)->records[i].field_C;
    }
    RESTORE(Ovl15SaveBlock1F34, &gSmallTaggedRecordBase, 0);
    RESTORE(Ovl15SaveBlock18, D_8006C838, 0x225C);
    RESTORE(Ovl15SaveBlock8, (u8 *)D_8006C838 + 0x30, 0x2284);
    RESTORE(Ovl15SaveBlock94, (u8 *)D_8006C838 + 0x38, 0x228C);
    ((s32 *)&gScenePhase)[0] = *(s32 *)(gOvl15SaveWorkBase + 0x2320);
    ((s32 *)&gScenePhase)[1] = *(s32 *)(gOvl15SaveWorkBase + 0x2324);
    RESTORE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 8, 0x2328);
    RESTORE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 0x1C, 0x233C);
    ((s32 *)&gScenePhase)[12] = *(s32 *)(gOvl15SaveWorkBase + 0x2350);
    ((s32 *)&gScenePhase)[13] = *(s32 *)(gOvl15SaveWorkBase + 0x2354);
    ((s32 *)&gScenePhase)[14] = *(s32 *)(gOvl15SaveWorkBase + 0x2358);
    ((s32 *)&gScenePhase)[15] = *(s32 *)(gOvl15SaveWorkBase + 0x235C);
    ((s32 *)&gScenePhase)[16] = *(s32 *)(gOvl15SaveWorkBase + 0x2360);
    RESTORE(Ovl15SaveBlockCFC, (u8 *)&gScenePhase + 0x44, 0x2364);
    RESTORE(Ovl15SaveBlock3FC, &gPrimaryObjectBase, 0x3060);
    RESTORE(Ovl15SaveBlock2328, gMainAreaGrid, 0x345C);
    RESTORE(Ovl15SaveBlock188, gAlternateAreaGrid, 0x5784);
    RESTORE(Ovl15SaveBlock40, &gRecordSlotPhase, 0x590C);
    RESTORE(Ovl15SaveBlock6C8, gObjectRecordPool, 0x59C0);
    D_8007AD00[0] = *(s32 *)(gOvl15SaveWorkBase + 0x6088);
    RESTORE(Ovl15SaveBlockC, (u8 *)D_8007AD00 + 4, 0x608C);
    RESTORE(Ovl15SaveBlock3C, (u8 *)D_8007AD00 + 0x10, 0x6098);
    RESTORE(Ovl15SaveBlock108, gTrackedRecordCounters, 0x6200);
    RESTORE(Ovl15SaveBlock24, &gScreenColorRamp, 0x6308);
    RESTORE(Ovl15SaveBlock34, D_8007AFA4, 0x632C);
    D_8007AFD8[0] = *(s32 *)(gOvl15SaveWorkBase + 0x6360);
    D_8007AFD8[1] = *(s32 *)(gOvl15SaveWorkBase + 0x6364);
    D_8007AFD8[2] = *(s32 *)(gOvl15SaveWorkBase + 0x6368);
    RESTORE(Ovl15SaveBlockC, (u8 *)D_8007AFD8 + 0xC, 0x636C);
}

#undef RESTORE

void Ovl15InsertSaveChecksumSlots(void) {
    u8 *record;
    s32 remaining;
    s32 copyBytes;

    gOvl15SaveWorkBase = gOvl15SavePayload;
    record = gOvl15SavePayload;
    copyBytes = 0x6441;
    for (remaining = 0xC8; remaining >= 0; remaining--) {
        memmove(record + 0x80, record + 0x7F, copyBytes);
        record += 0x80;
        copyBytes -= 0x80;
    }
    gOvl15TransferBuffer[0x7FFE] = Rand(0x100);
}

void Ovl15RemoveSaveChecksumSlots(void) {
    u8 *record;
    s32 remaining;
    s32 copyBytes;

    gOvl15SaveWorkBase = gOvl15SavePayload;
    record = gOvl15SavePayload;
    copyBytes = 0x6441;
    for (remaining = 0xC8; remaining >= 0; remaining--) {
        memmove(record + 0x7F, record + 0x80, copyBytes);
        record += 0x7F;
        copyBytes -= 0x7F;
    }
}

/*
 * Save-buffer checksums in the save/load overlay (ovl_15).
 *
 * The 0x8000-byte save image has 256 records of 0x80 bytes. Records 5..255
 * each store the XOR of their first 127 bytes in their last byte. Record 4
 * (gOvl15SaveMetadata, transfer-buffer +0x200) stores their combined XOR
 * at +0x38, then its own first-127-byte XOR at +0x7F. Records 0..3 (card
 * header and icons) are not covered here.
 *
 * The payload base is record 5: the packers above create its final-byte
 * checksum slots. Card I/O remains a separate service. Transfer storage is
 * externally allocated, absolutely addressed and reused for sprite loading.
 */

enum {
    SAVE_RECORD_BYTES = 0x80,
    SAVE_RECORD_PAYLOAD_BYTES = 0x7F,
    SAVE_FIRST_DATA_RECORD = 5,
    SAVE_RECORD_COUNT = 0x100,
    SAVE_DATA_XOR_OFFSET = 0x38,
    SAVE_METADATA_XOR_OFFSET = 0x27F
};

/* ovl_15:0x80135AE0 */
void Ovl15WriteSaveChecksums(void) {
    u8 dataChecksum;
    u8 checksum;
    s32 index;
    s32 byteIndex;
    u8 *metadata;

    dataChecksum = 0;
    for (index = SAVE_FIRST_DATA_RECORD; index < SAVE_RECORD_COUNT; index++) {
        checksum = 0;
        for (byteIndex = 0; byteIndex < SAVE_RECORD_PAYLOAD_BYTES; byteIndex++) {
            checksum ^= gOvl15TransferBuffer[index * SAVE_RECORD_BYTES + byteIndex];
        }
        gOvl15TransferBuffer[index * SAVE_RECORD_BYTES + byteIndex] = checksum;
        dataChecksum ^= checksum;
    }
    gOvl15SaveMetadata[SAVE_DATA_XOR_OFFSET] = dataChecksum;
    checksum = 0;
    metadata = gOvl15SaveMetadata;
    for (index = 0; index < SAVE_RECORD_PAYLOAD_BYTES; index++) {
        checksum ^= *metadata++;
    }
    gOvl15TransferBuffer[SAVE_METADATA_XOR_OFFSET] = checksum;
}

/* ovl_15:0x80135B68 */
s32 Ovl15VerifySaveChecksums(void) {
    u8 dataChecksum;
    u8 checksum;
    s32 index;
    s32 byteIndex;
    s32 status;
    u8 *metadata;

    dataChecksum = 0;
    for (index = SAVE_FIRST_DATA_RECORD; index < SAVE_RECORD_COUNT; index++) {
        checksum = 0;
        for (byteIndex = 0; byteIndex < SAVE_RECORD_PAYLOAD_BYTES; byteIndex++) {
            checksum ^= gOvl15TransferBuffer[index * SAVE_RECORD_BYTES + byteIndex];
        }
        dataChecksum ^= checksum;
        if (gOvl15TransferBuffer[index * SAVE_RECORD_BYTES + byteIndex] != checksum) {
            return -1;
        }
    }
    if (gOvl15SaveMetadata[SAVE_DATA_XOR_OFFSET] != dataChecksum) {
        return -1;
    }
    checksum = 0;
    metadata = gOvl15SaveMetadata;
    for (index = 0; index < SAVE_RECORD_PAYLOAD_BYTES; index++) {
        checksum ^= *metadata++;
    }
    status = -1;
    if (gOvl15TransferBuffer[SAVE_METADATA_XOR_OFFSET] == checksum) {
        status = 0;
    }
    return status;
}
