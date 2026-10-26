#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800BE6E4(s32 **arg0, s32 arg1);
s32 ovl_11_func_800BE94C(s32 **arg0, Ovl11ItemCollisionObject *arg1);
s32 ovl_11_func_800BEF64(Ovl11CollisionQuery *arg0);
s32 ovl_11_func_800BE4A8(s32 **arg0);
s32 ovl_11_func_800BE2C4(s32 **arg0, u16 *arg1);
s32 ovl_11_func_800BE5A0(Ovl11CollisionQuery *arg0);
s32 ovl_11_func_800BF720(Ovl11CollisionQuery *arg0);

s32 ovl_11_func_800BEB28(s32 *arg0, s32 arg1, void **arg2, s32 *arg3, s32 arg4, s32 arg5) {
    Ovl11CollisionQuery query;
    Ovl11ItemCollisionObject *item;
    u16 *entry;
    s32 *smallEntry;
    u32 i;
    u32 j;
    s32 result;
    s32 returnValue;

    query.object = (Ovl11CollisionTag *) &arg1;
    query.position = arg0;
    query.tile = arg2;
    query.flags = arg3;
    query.field_C = arg4;
    query.field_14 = arg5;
    if (arg5 & 0x20) {
        query.field_18 = 0x20;
        result = ovl_11_func_800BE6E4((s32 **) &query, 0);
        if (result == 1) {
            goto collision_hit;
        }
        if (result == 2) {
            if (arg5 & 0x40) {
                returnValue = 0;
            } else {
                returnValue = 1;
            }
            goto return_value;
        }
    }
    if (arg5 & 0x8000) {
        item = ((struct Ovl11CollisionDispatchState *) D_8006C838)->pool;
        query.field_18 = 0;
        for (i = 0; i < 0xC5U; i++, item += 4) {
            if (*(s16 *) item == query.object->field_2 && (item->unk4 & 1) &&
                ovl_11_func_800BE94C((s32 **) &query, item)) {
                goto item_hit;
            }
        }
    }
    if (arg5 & 0x80) {
        query.field_18 = 0x80;
        if (ovl_11_func_800BEF64(&query)) {
            goto collision_hit;
        }
    }
    goto after_actor;
collision_hit:
    returnValue = 1;
return_value:
    return returnValue;
after_actor:
    if (arg5 & 1) {
        query.field_18 = 1;
        if (ovl_11_func_800BE4A8((s32 **) &query)) {
            return 1;
        }
    }
    if (arg5 & 0x800) {
        query.field_18 = 0x2800;
        entry = (u16 *) D_80075BC4_reset;
        for (i = 0; i < 6U; i++, entry += 0x58) {
            if (*entry && ovl_11_func_800BE2C4((s32 **) &query, entry)) {
                return 1;
            }
        }
    }
    if (*((struct Ovl11SpriteWork *) &D_8007AFF0)->header == 0) {
        if (arg5 & 2) {
            query.field_18 = 0x14002;
            entry = (u16 *) &gTaggedRecordBase;
            for (i = 0; i < 0x14U; i++, entry += 0x5C) {
                if ((u32) (*entry - 0x160) < 4U &&
                    ovl_11_func_800BE2C4((s32 **) &query, entry)) {
                    return 1;
                }
            }
        }
        if (*((struct Ovl11SpriteWork *) &D_8007AFF0)->header == 0) {
            if (arg5 & 4) {
                query.field_18 = 0x14004;
                entry = (u16 *) &gTaggedRecordBase;
                for (i = 0; i < 0x14U; i++, entry += 0x5C) {
                    if ((u32) (*entry - 0x164) < 3U &&
                        ovl_11_func_800BE2C4((s32 **) &query, entry)) {
                        return 1;
                    }
                }
            }
        }
    }
    if (arg5 & 8) {
        query.field_18 = 0x2008;
        smallEntry = &D_800742EC;
        for (j = 0; j < 0xAU; j++, smallEntry += 0x2D) {
            if (ovl_11_func_800BE2C4((s32 **) &query, (u16 *) smallEntry)) {
                goto other_hit;
            }
        }
    }
    if (arg5 & 0x200) {
        query.field_18 = 0x200;
        if (ovl_11_func_800BE2C4((s32 **) &query, &D_80075AD4)) {
            *arg3 = 0x14200;
            goto collision_hit;
        }
    }
    if (arg5 & 0x100) {
        if (!(((struct Ovl11CollisionDispatchState *) D_8006C838)->flags & 0x20000) &&
            ((struct Ovl11CollisionDispatchState *) D_8006C838)->region == query.object->field_2) {
            query.field_18 = 0x2100;
            if (ovl_11_func_800BE2C4((s32 **) &query, &D_800759E4.f00)) {
                goto collision_hit;
            }
        }
    }
    if (arg5 & 0x400) {
        query.field_18 = 0x400;
        entry = (u16 *) D_8007A638;
        for (i = 0; i < 7U; i++, entry += 0x7C) {
            if (!(entry[0x5C] & 0x20) &&
                ovl_11_func_800BE2C4((s32 **) &query, entry)) {
                return 1;
            }
        }
    }
    if (arg5 & 0x10) {
        query.field_18 = 0x10;
        if (ovl_11_func_800BE5A0(&query)) {
            return 1;
        }
    }
    goto final_probe;
item_hit:
    *arg3 |= 0x8000;
    return 1;
other_hit:
    *arg3 |= 0x2000;
    return 1;
final_probe:
    if (arg5 & 0x1000) {
        result = ovl_11_func_800BF720(&query);
        returnValue = 1;
        if (result) {
            return returnValue;
        }
    }
    return 0;
}
