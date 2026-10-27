/*
 * Stateless delimiter-based byte-run decoder used for sprite-block readback.
 * The public contract is in ovl11_byte_run_decode.h. No storage or private
 * linkage is inferred from its move (ovl_11:800E5524); see file-groupings.md.
 */
#include "common.h"
#include "ovl11_byte_run_decode.h"

void Ovl11DecodeByteRuns(u8 *destination, u8 *encoded, s32 recordCount) {
    u8 delimiter;
    s32 recordIndex;
    u8 value;
    u8 repeatedByte;
    s32 repeats;

    delimiter = *encoded++;
    for (recordIndex = 0; recordIndex < recordCount; recordIndex++, encoded++) {
        value = *encoded;
        if (value == delimiter) {
            encoded++;
            repeatedByte = *encoded++;
            repeats = *encoded;
            if (repeats != 0) {
                do {
                    *destination++ = repeatedByte;
                    repeats--;
                } while (repeats != 0);
            }
        } else {
            *destination++ = value;
        }
    }
}
