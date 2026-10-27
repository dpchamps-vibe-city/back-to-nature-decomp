/*
 * scene_header_capture — reconstructed translation unit.
 *
 * Capture the bound scene header first word into the primary object, with an
 * additional mirror for two object codes. The packed copies permit unaligned
 * header and mirror storage.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_scene_header_capture.h"

/* The second destination is only halfword-aligned. Retain the unaligned
 * word-copy operation rather than imposing s32 pointer alignment. */
typedef struct __attribute__((packed)) {
    s32 key;
} Ovl11UnalignedWord;

void Ovl11CaptureSceneHeaderKey(void) {
    char *object = (char *)&gPrimaryObjectBase;
    char *work = (char *)&D_8007AFF0;
    Ovl11UnalignedWord *header = *(Ovl11UnalignedWord **)(work + 0x25388);

    *(Ovl11UnalignedWord *)(object + 0xFC) = *header;
    if ((u32)(*(s16 *)(object + 0x8A) - 0x106) < 2) {
        *(Ovl11UnalignedWord *)(object + 0x4012) = *(Ovl11UnalignedWord *)(object + 0xFC);
    }
}
