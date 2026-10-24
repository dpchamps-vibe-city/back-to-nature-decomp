/*
 * Save-buffer checksums in the save/load overlay (ovl_15).
 *
 * The 0x8000-byte save image has 256 records of 0x80 bytes. Records 5..255
 * each store the XOR of their first 127 bytes in their last byte. Record 4
 * (D_80137A30, at D_80137830 + 0x200) stores the XOR of those 251 checksums
 * at +0x38, then its own first-127-byte XOR at +0x7F. Records 0..3 (card
 * header and icons) are not covered here.
 *
 * Serialization, packing/unpacking and card I/O remain separate. This pair
 * is a reconstructed module, not a proven original TU boundary; see
 * notes/file-groupings.md. The data stays externally allocated and absolutely
 * addressed. D_80137830 is also reused for sprite-data loading, so it is not
 * private checksum state.
 */

#include "common.h"
#include "ovl15_save_checksums.h"

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
            checksum ^= D_80137830[index * SAVE_RECORD_BYTES + byteIndex];
        }
        D_80137830[index * SAVE_RECORD_BYTES + byteIndex] = checksum;
        dataChecksum ^= checksum;
    }
    D_80137A30[SAVE_DATA_XOR_OFFSET] = dataChecksum;
    checksum = 0;
    metadata = D_80137A30;
    for (index = 0; index < SAVE_RECORD_PAYLOAD_BYTES; index++) {
        checksum ^= *metadata++;
    }
    D_80137830[SAVE_METADATA_XOR_OFFSET] = checksum;
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
            checksum ^= D_80137830[index * SAVE_RECORD_BYTES + byteIndex];
        }
        dataChecksum ^= checksum;
        if (D_80137830[index * SAVE_RECORD_BYTES + byteIndex] != checksum) {
            return -1;
        }
    }
    if (D_80137A30[SAVE_DATA_XOR_OFFSET] != dataChecksum) {
        return -1;
    }
    checksum = 0;
    metadata = D_80137A30;
    for (index = 0; index < SAVE_RECORD_PAYLOAD_BYTES; index++) {
        checksum ^= *metadata++;
    }
    status = -1;
    if (D_80137830[SAVE_METADATA_XOR_OFFSET] == checksum) {
        status = 0;
    }
    return status;
}
