/*
 * Cell-action variants of one record-consumption protocol: resolve an item,
 * check its attribute/budgets, clear the active bit and append a sprite effect.
 * The shared cell-action callers and record layout join the three variants;
 * the map/attribute services and sprite-effect pool remain separate modules.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "ovl11_spawn_effect_consume.h"
#include "ovl11_sprite_effect_pool.h"
#include "ovl11_save_counters.h"

s16 ovl_11_func_800F4240(Ovl11ItemCollisionObject *record);
s32 ovl_11_func_800D5D38(u16 id, u16 mode);
s32 ovl_11_func_800D5D9C(u16 id, u16 mode);
s32 ovl_11_func_800D5E64(u16 id, u16 mode);
s32 ovl_11_func_800D5ED4(u16 id, u16 mode);
void ovl_11_func_800F0FB0(s32 value);
s32 ovl_11_func_800D5E00(u16 id, u16 mode);

s32 Ovl11ConsumeSpawnEffect(Ovl11SpawnEffectRecord *record) {
    s32 consumed;
    u16 id;
    char *farState;

    consumed = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) record);
    if (!id) {
        return 0;
    }
    farState = (char *) &D_8007AFF0;
    if (*(s16 *) (farState + 0x25474) == 0x13) {
        return 0;
    }
    if (ovl_11_func_800D5D38((u16) record->unk2, 0) == 1) {
        record->unk4 &= 0xFFFE;
        Ovl11SpawnSpriteEffect(record->unk2, *(VECTOR *)&record->unk8);
        consumed = 1;
    }
    return consumed;
}

s32 Ovl11ConsumeSpawnEffectWithBudgets(Ovl11SpawnEffectRecord *record,
                                      s16 attribute2Budget, s16 attribute0Budget) {
    s32 consumed;
    u16 id;

    consumed = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) record);
    if (id == 0) {
        return 0;
    }
    if (ovl_11_func_800D5D9C(id, 0) == 1) {
        if ((u32) attribute0Budget >= ovl_11_func_800D5E64(id, 0)) {
            if ((u32) attribute2Budget >= ovl_11_func_800D5ED4(id, 0)) {
                record->unk4 &= 0xFFFE;
                Ovl11SpawnSpriteEffect(record->unk2, *(VECTOR *)&record->unk8);
                consumed = 1;
            }
        }
    }
    return consumed;
}

s32 Ovl11ConsumeTieredSpawnEffect(Ovl11SpawnEffectRecord *record,
                                 s16 attribute2Budget, s16 attribute0Budget) {
    s32 flags;
    s32 consumed;
    u16 id;

    consumed = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) record);
    if (id == 0) {
        return 0;
    }
    flags = ((struct Ovl11PoolResetView *) D_8006C838)->flags & ~0x20;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags = flags;
    if (id == 0x174) {
        ((struct Ovl11PoolResetView *) D_8006C838)->flags = flags | 0x20;
        ovl_11_func_800F0FB0(5);
        return 0;
    }
    if (ovl_11_func_800D5E00(id, 0) == 1) {
        if ((u32) attribute0Budget >= ovl_11_func_800D5E64(id, 0)) {
            if ((u32) attribute2Budget >= ovl_11_func_800D5ED4(id, 0)) {
                record->unk4 &= 0xFFFE;
                switch (id) {
                case 0x3D:
                case 0x172:
                case 0x173:
                case 0x174:
                    Ovl11AddSaveCounter19(2, 0);
                    break;
                case 0x3E:
                    Ovl11AddSaveCounter19(1, 0);
                    break;
                default:
                    break;
                }
                Ovl11SpawnSpriteEffect(record->unk2, *(VECTOR *)&record->unk8);
                consumed = 1;
            }
        }
    }
    return consumed;
}
