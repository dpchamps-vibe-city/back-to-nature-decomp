/*
 * Default area-selection ID resolution (ovl_11:800DD274-800DD36C).
 * The resident state's base is combined with private signed-offset tables.
 * Area installation and scripted selection share this policy; neither caller
 * owns the tables. See ovl11_default_area_selection.h for selector constraints.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_default_area_selection.h"

s32 Ovl11ResolveDefaultAreaSelection(void) {
    s16 variant70Base;
    s16 variant60Base;
    s32 offset;

    offset = 0;
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 0) {
        variant70Base = gOvl11DefaultAreaPrimaryOffsets[((struct Ovl11MessageState *)D_8006C838)->variant50];
        if (((struct Ovl11MessageState *)D_8006C838)->variant70 == 0) {
            offset = variant70Base + gOvl11DefaultAreaVariant70ZeroOffset;
        } else {
            offset = variant70Base + gOvl11DefaultAreaVariant70NonzeroOffset;
        }
    }
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 1) {
        variant60Base = gOvl11DefaultAreaPrimaryOffsets[((struct Ovl11MessageState *)D_8006C838)->variant50];
        if (((struct Ovl11MessageState *)D_8006C838)->variant60 == 0) {
            offset = variant60Base + gOvl11DefaultAreaVariant60ZeroOffset;
        } else {
            offset = variant60Base + gOvl11DefaultAreaVariant60NonzeroOffset;
        }
    }
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 2) {
        offset = gOvl11DefaultAreaPrimaryOffsets[((struct Ovl11MessageState *)D_8006C838)->variant50];
        offset += gOvl11DefaultAreaVariant80Offsets[((struct Ovl11MessageState *)D_8006C838)->variant80];
    }
    return offset + ((struct Ovl11MessageState *)D_8006C838)->base;
}
