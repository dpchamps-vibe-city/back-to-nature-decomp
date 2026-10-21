#include "common.h"

s32 ovl_11_func_800BF630(s16 arg0, s16 arg1, s32 arg2) {
    Ovl11D124Entry *entry;
    Ovl11TilePools *pools;
    u16 property;

    if (arg2 != 0) {
        entry = &D_80074124[arg1][arg0];
    } else {
        entry = &D_80071DFC[arg1][arg0];
    }
    if ((u16) entry->unk0 == 0x167) {
        return 2;
    }
    pools = (Ovl11TilePools *) D_8006C838;
    property = pools->properties[pools->items[(u16) entry->unk0].variants[entry->unk4]][2];
    switch (property) {
    case 0:
        return 2;
    case 1:
        return 1;
    case 2:
        return 0;
    }
    return 0;
}
