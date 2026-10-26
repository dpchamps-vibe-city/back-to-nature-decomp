/*
 * ID-keyed views used together to populate an overlay scene record: persistent
 * value update/read, class-dependent constant and a selected sprite object.
 * Units/gameplay meanings of the range values are not established here.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl21_entity_record_views.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void Ovl21RaiseEntityRangeValue(s16 entityId) {
    if (entityId != 0) {
        if (entityId == 7 || entityId == 9 || entityId == 20) {
            char *work = (char *)&D_8006C838;
            char *record = work + entityId * 0x1D4;
            s32 value = *(u16 *)(record + 0x8000 + 0x19EC) + 0x1F4;
            if (value > 0xFE0A) {
                value = 0xFFFF;
            }
            *(u16 *)(record + 0x8000 + 0x19EC) = value;
            return;
        } else {
            char *work = (char *)&D_8006C838;
            char *record = work + entityId * 0x1D4;
            s32 value = *(s16 *)(record + 0x8000 + 0x19EA) + 0x14;
            if (value >= 0xEB) {
                value = 0xFF;
            }
            *(s16 *)(record + 0x8000 + 0x19EA) = value;
        }
    }
}

s16 Ovl21ReadEntityRangeValue(s16 entityId) {
    s32 root;

    if (entityId == 7 || entityId == 9 || entityId == 20) {
        char *work;
        char *record;
        work = (char *)&D_8006C838;
        record = work + entityId * 0x1D4;
        root = SquareRoot0(*(u16 *)(record + 0x8000 + 0x19EC));
        return (s16)root;
    }
    {
        char *work;
        char *record;
        work = (char *)&D_8006C838;
        record = work + entityId * 0x1D4;
        return *(s16 *)(record + 0x8000 + 0x19EA);
    }
}

s32 Ovl21GetEntityRangeConstant(s16 entityId) {
    if (entityId == 0x10 || entityId == 0x22) {
        return gOvl21EntityRange360;
    }
    if (entityId == 7 || entityId == 9 || entityId == 0x14 || entityId == 0x16) {
        return gOvl21EntityRange420;
    }
    return gOvl21EntityRange480;
}

u8 *Ovl21GetEntityObjectRecord(s16 entityId) {
    if (entityId == 5) {
        return gOvl21EntitySprite5;
    }
    if (entityId == 6) {
        return gOvl21EntitySprite6;
    }
    if (entityId == 7) {
        return gOvl21EntitySprite7;
    }
    if (entityId == 9) {
        return gOvl21EntitySprite9;
    }
    if (entityId == 11) {
        return gOvl21EntitySprite11;
    }
    if (entityId == 16) {
        return gOvl21EntitySprite16;
    }
    if (entityId == 19) {
        return gOvl21EntitySprite19;
    }
    if (entityId == 20) {
        return gOvl21EntitySprite20;
    }
    if (entityId == 21) {
        return gOvl21EntitySprite21;
    }
    if (entityId == 22) {
        return gOvl21EntitySprite22;
    }
    if (entityId == 25) {
        return gOvl21EntitySprite25;
    }
    if (entityId == 34) {
        return gOvl21EntitySprite34;
    }
    return gOvl21DefaultEntitySprite;
}
