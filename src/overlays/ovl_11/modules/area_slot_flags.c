/* Two-bank indexed byte flags. The matching-sensitive per-arm base formation
 * keeps each bank's constant as a load/store displacement; do not replace it
 * with a guessed packed-record layout. Membership: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_area_slot_flags.h"

void Ovl11SetAreaSlotFlag(s32 slot, s32 bank) {
    if (bank == 1) {
        char *base = (char *)&D_8006C838;
        char *flags = base + 0x4AC0;

        *(flags + slot) = 1;
        return;
    }
    {
        char *base = (char *)&D_8006C838;
        char *flags = base + 0x4AD6;

        *(flags + slot) = 1;
    }
}

s32 Ovl11IsAreaSlotFlagSet(s32 slot, s32 bank) {
    if (bank == 1) {
        char *base = (char *)&D_8006C838;
        char *flags = base + 0x4AC0;

        if (*(flags + slot) != 0) {
            return 1;
        }
    } else {
        char *base = (char *)&D_8006C838;
        char *flags = base + 0x4AD6;

        if (*(flags + slot) != 0) {
            return 1;
        }
    }

    return 0;
}
