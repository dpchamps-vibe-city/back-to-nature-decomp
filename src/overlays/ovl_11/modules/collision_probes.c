/*
 * collision_probes — reconstructed translation unit.
 *
 * Collision dispatcher and its record, primary-object, region, tile, item and
 * actor probes. See ovl11_collision_probes.h for query masks and
 * output/side-effect constraints.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_collision_probes.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_interaction_id_predicates.h"
#include "ovl11_grid_cell_queries.h"
#include "ovl11_grid_position.h"

s32 Ovl11ProbePlacedRecord(s32 **arg0, u16 *arg1) {
    ItemData *temp_a2;
    ItemData *item_base;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v1;
    s16 lower;
    s16 upper;
    s16 z_lower;
    s32 *temp_a0_5;
    Vec3 *temp_a1;
    s32 *temp_a1_2;
    s32 *temp_v0_2;
    s32 temp_a0_2;
    u16 temp_v1_2;

    temp_s2 = (s16) *arg1;
    if ((*(s16 *) ((u8 *) arg1 + 0x30)) == (*(Ovl11ItemCollisionObject **) ((u8 *) arg0 + 4))->unk2) {
        if ((*(u16 **) ((u8 *) arg0 + 0xC)) != arg1) {
            if (!((*(s32 *) ((u8 *) arg1 + 0x34)) & 0x02020400) || ((*arg1 == 0x106) && (Ovl11FindCalendarDate(((struct struct_8006C838_button *) D_8006C838)->field_44BA, (s16) ((struct Ovl11MenuDateView *) D_8006C838)->field_44BC) != 8))) {
                temp_v1 = *(s16 *) ((u8 *) arg1 + 0x38);
                item_base = D_8006C858;
                temp_a2 = (ItemData *) (temp_s2 * sizeof(ItemData) + (u32) item_base);
                temp_a0 = *(s16 *) ((u8 *) temp_a2 + 0x24);
                temp_a1 = (Vec3 *) *arg0;
                temp_a0_2 = temp_a1->x;
                lower = temp_v1 - temp_a0;
                upper = temp_v1 + temp_a0;
                if ((lower < temp_a0_2) && (temp_a0_2 < upper)) {
                    temp_v0 = *(s16 *) ((u8 *) arg1 + 0x40);
                    temp_a0 = *(s16 *) ((u8 *) temp_a2 + 0x26);
                    z_lower = temp_v0 - temp_a0;
                    upper = temp_v0 + temp_a0;
                    if (z_lower < temp_a1->z) {
                        if (temp_a1->z < upper) {
                            *(*(s32 **) ((u8 *) arg0 + 0x10)) = (*(s32 *) ((u8 *) arg0 + 0x18)) | 0x2000;
                            if ((u32) (*arg1 - 0x160) < 7U) {
                                temp_a0_5 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_a0_5 &= ~0x2000;
                                temp_a1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_a1_2 |= 0x14000;
                            }
                            temp_v1_2 = *arg1;
                            if ((temp_v1_2 == 0x108) || (temp_v1_2 == 0x17A) || (temp_v1_2 == 0x17B) || (temp_v1_2 == 0x15B)) {
                                temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0_2 &= ~0x2000;
                            }
                            *(*(u16 ***) ((u8 *) arg0 + 8)) = arg1;
                            return 1;
                        }
                        return 0;
                    }
                }
                goto block_17;
            }
            return 0;
        }
block_17:
        return 0;
    }
    return 0;
}

s32 Ovl11ProbePrimaryObject(Ovl11CollisionQuery *arg0) {
    s32 *position;
    s32 x;
    s32 z;
    s32 y;
    s16 lower;
    s16 upper;
    s16 center;
    Ovl11PlayerCollisionView *player;

    player = (Ovl11PlayerCollisionView *) &gPrimaryObjectBase;
    if (player->region != arg0->object->field_2) {
        return 0;
    }
    position = arg0->position;
    x = position[0];
    center = player->x;
    lower = center - 0xDA;
    upper = center + 0xDA;
    if ((lower < x) && (x < upper)) {
        z = position[2];
        center = player->z;
        lower = center - 0xDA;
        upper = center + 0xDA;
        if ((lower < z) && (z < upper)) {
            y = position[1];
            center = player->y;
            lower = center - 0x14;
            upper = center + 0x14;
            if ((lower < y) && (y < upper)) {
                *arg0->flags = arg0->field_18;
                *arg0->tile = &gPrimaryObjectBase;
                return 1;
            }
        }
    }
    return 0;
}

/* A hit means outside every selected open X/Z rectangle, not inside one. */
s32 Ovl11IsOutsideCollisionRegions(Ovl11CollisionQuery *query) {
    Ovl11CollisionRegionBounds *bounds;
    s16 region;
    s32 *position;
    s32 x;
    u32 i;
    u32 count;

    region = query->object->field_2;
    if (region == 5) {
        count = 9;
        if (gTaggedRecordPoolExpanded == 0) {
            count = 7;
            bounds = gOvl11Region5Bounds;
        } else {
            bounds = gOvl11Region5ExpandedBounds;
        }
    } else {
        count = 0x19;
        if (region == 4) {
            count = 0xA;
            if (gB4RecordPoolExpanded == 0) {
                count = 5;
                bounds = gOvl11Region4Bounds;
            } else {
                bounds = gOvl11Region4ExpandedBounds;
            }
        } else {
            bounds = gOvl11CollisionRegionBounds;
        }
    }
    if (query->field_14 & 0x20000) {
        count = 4;
        bounds = gOvl11OverrideRegionBounds;
    }
    for (i = 0; i < count; i++) {
        if (bounds->region == query->object->field_2) {
            position = query->position;
            x = position[0];
            if (bounds->minX < x && x < bounds->maxX) {
                if (bounds->minZ < position[2] && position[2] < bounds->maxZ) {
                    return 0;
                }
            }
        }
        bounds = (Ovl11CollisionRegionBounds *)((u8 *)bounds + 0x14);
    }
    return 1;
}

s32 Ovl11ProbeTileMap(s32 **arg0, s32 arg1) {
    VECTOR pos;
    u16 coords[2];
    s16 row;
    s16 col;
    s32 rowOffset;
    u8 *columnBase;
    u8 *mapBase;
    s16 temp_a0_2;
    s16 temp_t3;
    s32 *temp_a0;
    s32 *temp_v0_2;
    s32 *temp_v1_2;
    s32 temp_a3;
    s32 var_s0;
    s32 var_v0;
    u16 temp_a0_3;
    u16 temp_a1;
    u16 temp_v0;
    u8 temp_t2;
    Ovl11TilePools *temp_a2;
    Ovl11TileItem *temp_v1;
    s8 group;
    void *var_t0;

    temp_a0 = *arg0;
    pos.vx = temp_a0[0];
    pos.vy = *(s32 *) ((u8 *) temp_a0 + 4);
    pos.vz = *(s32 *) ((u8 *) temp_a0 + 8);
    temp_a0_2 = ((s16 *) arg0[1])[1];
    var_s0 = (temp_a0_2 != 1) * 2;
    if (temp_a0_2 == 6) {
        var_s0 = 1;
    }
    var_v0 = 0;
    if (var_s0 != 2) {
        if (var_s0 != 0) {
            if (Ovl11LookupMode6GridPosition((VECTOR *) &pos.vx, &coords[0], &coords[1]) != 0) {
                return 0;
            }
            goto block_7;
        }
        var_v0 = 0;
        if (ovl_11_func_800D7EF8(&pos.vx, &coords[0], &coords[1]) == 0) {
block_7:
            row = coords[1];
            col = coords[0];
            rowOffset = row * 0x168;
            arg1 = col * sizeof(Ovl11D124Entry);
            mapBase = (u8 *) gMainAreaGrid;
            columnBase = mapBase + arg1;
            var_t0 = (void *) (rowOffset + (s32) columnBase);
            if (var_s0 != 0) {
                s32 alternateRowOffset;
                u8 *alternateBase;
                u8 *alternateColumnBase;

                alternateRowOffset = row * 0x38;
                alternateBase = mapBase + 0x2328;
                alternateColumnBase = alternateBase + arg1;
                var_t0 = (void *) (alternateRowOffset + (s32) alternateColumnBase);
            }
            if ((*(u16 *) var_t0) == 0x167) {
                return 0;
            }
            {
                temp_a2 = (Ovl11TilePools *) (mapBase - 0x55C4);
                temp_a1 = (*(s16 *) var_t0);
                temp_t2 = *((u8 *) var_t0 + 4);
                temp_a3 = (u16) temp_a1 * 0x28;
                temp_v1 = (Ovl11TileItem *) (temp_a3 + (s32) temp_a2->items);
                temp_t3 = temp_v1->variants[temp_t2];
                temp_v0 = temp_a2->properties[temp_t3][2];
                group = temp_v1->group;
                switch (temp_v0) {
                case 0:
                    *(*(s32 **) ((u8 *) arg0 + 0x10)) = 0;
                    *(*(void ***) ((u8 *) arg0 + 8)) = var_t0;
                    return 0;
                case 1:
                case 2:
                    *(*(s32 **) ((u8 *) arg0 + 0x10)) = (*(s32 *) ((u8 *) arg0 + 0x18)) | 0x4000;
                    if ((u16) temp_a1 == 0x64) {
                        temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                        *temp_v0_2 &= ~0x4000;
                    }
                    temp_a0_3 = ((Ovl11TileItem *) (temp_a3 + (s32) temp_a2->items))->flags;
                    switch (temp_a0_3) {
                    case 0x4040:
                        if (temp_a2->groups[group][1] != temp_t2) {
                            break;
                        }
                    case 0x100:
                    case 0x8000:
                    case 0x8010:
                        temp_v1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                        *temp_v1_2 |= 0x2000;
                        break;
                    }
                    var_v0 = 2;
                    *(*(void ***) ((u8 *) arg0 + 8)) = var_t0;
                    if (D_8006C860[temp_t3][2] == 1) {
                        return var_v0;
                    }
                    return 1;
                default:
                    break;
                }
                goto block_27;
            }
block_27:
            var_v0 = 0;
            return var_v0;
        }
        return var_v0;
    }
    return var_v0;
}

s16 ovl_11_func_800F4240(Ovl11ItemCollisionObject *arg0);

s32 Ovl11ProbeItemBounds(s32 **arg0, Ovl11ItemCollisionObject *arg1) {
    s16 temp_s0;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 lower;
    s16 upper;
    void *temp_a1;
    s32 *temp_a1_2;
    s32 *temp_a2;
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 *temp_v1_3;
    s32 temp_a0;
    s32 temp_a0_2;
    void *item_base;

    if (arg1->unk2 != 0) {
        if (arg1->unk4 & gOvl11ItemCollisionSeasonMasks[((struct Ovl11ItemCollisionGlobals *) D_8006C838)->selector]) {
            temp_a2 = *arg0;
            item_base = ((struct Ovl11ItemCollisionGlobals *) D_8006C838)->items;
            temp_a1 = (void *) ((u16) arg1->unk2 * 0x28 + (s32) item_base);
            temp_s0 = *(s16 *) ((u8 *) temp_a1 + 0x24);
            temp_v1 = *(s16 *) ((u8 *) arg1 + 8);
            temp_a0 = *temp_a2;
            lower = temp_v1 - temp_s0;
            upper = temp_v1 + temp_s0;
            if (lower < temp_a0) {
                if (temp_a0 < upper) {
                    temp_s0 = *(s16 *) ((u8 *) temp_a1 + 0x26);
                    temp_v1_2 = *(s16 *) ((u8 *) arg1 + 0x10);
                    temp_a0_2 = *(s32 *) ((u8 *) temp_a2 + 8);
                    lower = temp_v1_2 - temp_s0;
                    upper = temp_v1_2 + temp_s0;
                    if (lower < temp_a0_2) {
                        if ((temp_a0_2 < upper) && (ovl_11_func_800F4240(arg1) != 0)) {
                            temp_a1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                            item_base = ((struct Ovl11ItemCollisionGlobals *) D_8006C838)->items;
                            temp_s0 = *(s16 *) ((u16) arg1->unk2 * 0x28 + (u8 *) item_base);
                            *temp_a1_2 |= 0x4000;
                            *(*(Ovl11ItemCollisionObject ***) ((u8 *) arg0 + 8)) = arg1;
                            if ((u32) ((u16) arg1->unk2 - 0x64) < 2U) {
                                temp_v0 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0 &= ~0x4000;
                            }
                            if (Ovl11IsInteractionItemSetA(arg1->unk2) != 0) {
                                temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v0_2 &= ~0x4000;
                            }
                            if (temp_s0 & 0x8000) {
                                temp_v1_3 = *(s32 **) ((u8 *) arg0 + 0x10);
                                *temp_v1_3 |= 0x2000;
                            }
                            return 1;
                        }
                        return 0;
                    }
                    goto block_14;
                }
                return 0;
            }
            goto block_14;
        }
        return 0;
    }
block_14:
    return 0;
}

/* Helpers retain their original external linkage. The word-view probes
 * address the same Ovl11CollisionQuery layout as the typed probes. */
s32 Ovl11ProbeActorBounds(Ovl11CollisionQuery *query);

s32 Ovl11DispatchCollisionProbes(s32 *position, Ovl11CollisionTag tag,
                               void **hitObject, s32 *hitFlags,
                               s32 excludedRecord, s32 queryMask) {
    Ovl11CollisionQuery query;
    Ovl11ItemCollisionObject *item;
    u16 *entry;
    s32 *smallEntry;
    u32 i;
    u32 j;
    s32 result;
    s32 returnValue;

    query.object = &tag;
    query.position = position;
    query.tile = hitObject;
    query.flags = hitFlags;
    query.field_C = excludedRecord;
    query.field_14 = queryMask;
    if (queryMask & 0x20) {
        query.field_18 = 0x20;
        result = Ovl11ProbeTileMap((s32 **) &query, 0);
        if (result == 1) {
            goto collision_hit;
        }
        if (result == 2) {
            if (queryMask & 0x40) {
                returnValue = 0;
            } else {
                returnValue = 1;
            }
            goto return_value;
        }
    }
    if (queryMask & 0x8000) {
        item = ((struct Ovl11CollisionDispatchState *) D_8006C838)->pool;
        query.field_18 = 0;
        for (i = 0; i < 0xC5U; i++, item += 4) {
            if (*(s16 *) item == query.object->field_2 && (item->unk4 & 1) &&
                Ovl11ProbeItemBounds((s32 **) &query, item)) {
                goto item_hit;
            }
        }
    }
    if (queryMask & 0x80) {
        query.field_18 = 0x80;
        if (Ovl11ProbeActorBounds(&query)) {
            goto collision_hit;
        }
    }
    goto after_actor;
collision_hit:
    returnValue = 1;
return_value:
    return returnValue;
after_actor:
    if (queryMask & 1) {
        query.field_18 = 1;
        if (Ovl11ProbePrimaryObject(&query)) {
            return 1;
        }
    }
    if (queryMask & 0x800) {
        query.field_18 = 0x2800;
        entry = (u16 *) D_80075BC4_reset;
        for (i = 0; i < 6U; i++, entry += 0x58) {
            if (*entry && Ovl11ProbePlacedRecord((s32 **) &query, entry)) {
                return 1;
            }
        }
    }
    if (*((struct Ovl11SpriteWork *) &D_8007AFF0)->header == 0) {
        if (queryMask & 2) {
            query.field_18 = 0x14002;
            entry = (u16 *) &gTaggedRecordBase;
            for (i = 0; i < 0x14U; i++, entry += 0x5C) {
                if ((u32) (*entry - 0x160) < 4U &&
                    Ovl11ProbePlacedRecord((s32 **) &query, entry)) {
                    return 1;
                }
            }
        }
        if (*((struct Ovl11SpriteWork *) &D_8007AFF0)->header == 0) {
            if (queryMask & 4) {
                query.field_18 = 0x14004;
                entry = (u16 *) &gTaggedRecordBase;
                for (i = 0; i < 0x14U; i++, entry += 0x5C) {
                    if ((u32) (*entry - 0x164) < 3U &&
                        Ovl11ProbePlacedRecord((s32 **) &query, entry)) {
                        return 1;
                    }
                }
            }
        }
    }
    if (queryMask & 8) {
        query.field_18 = 0x2008;
        smallEntry = &gSmallTaggedRecordBase;
        for (j = 0; j < 0xAU; j++, smallEntry += 0x2D) {
            if (Ovl11ProbePlacedRecord((s32 **) &query, (u16 *) smallEntry)) {
                goto other_hit;
            }
        }
    }
    if (queryMask & 0x200) {
        query.field_18 = 0x200;
        if (Ovl11ProbePlacedRecord((s32 **) &query, &D_80075AD4)) {
            *hitFlags = 0x14200;
            goto collision_hit;
        }
    }
    if (queryMask & 0x100) {
        if (!(((struct Ovl11CollisionDispatchState *) D_8006C838)->flags & 0x20000) &&
            ((struct Ovl11CollisionDispatchState *) D_8006C838)->region == query.object->field_2) {
            query.field_18 = 0x2100;
            if (Ovl11ProbePlacedRecord((s32 **) &query, &D_800759E4.f00)) {
                goto collision_hit;
            }
        }
    }
    if (queryMask & 0x400) {
        query.field_18 = 0x400;
        entry = (u16 *) D_8007A638;
        for (i = 0; i < 7U; i++, entry += 0x7C) {
            if (!(entry[0x5C] & 0x20) &&
                Ovl11ProbePlacedRecord((s32 **) &query, entry)) {
                return 1;
            }
        }
    }
    if (queryMask & 0x10) {
        query.field_18 = 0x10;
        if (Ovl11IsOutsideCollisionRegions(&query)) {
            return 1;
        }
    }
    goto final_probe;
item_hit:
    *hitFlags |= 0x8000;
    return 1;
other_hit:
    *hitFlags |= 0x2000;
    return 1;
final_probe:
    if (queryMask & 0x1000) {
        result = Ovl11IsSpecialCollisionCell(&query);
        returnValue = 1;
        if (result) {
            return returnValue;
        }
    }
    return 0;
}

s32 Ovl11ProbeActorBounds(Ovl11CollisionQuery *arg0) {
    s32 box[6];
    s32 *bp;
    Ovl11ActorCollisionRecord *entry;
    s32 *position;
    s32 i;
    u16 flags;
    u16 originalFlags;

    bp = box;
    position = arg0->position;
    box[0] = position[0] - 0xC8;
    box[1] = position[0] + 0xC8;
    box[2] = position[2] - 0xC8;
    box[3] = position[2] + 0xC8;
    entry = D_800763F4;
    box[4] = position[1] - 0x14;
    box[5] = position[1] + 0x14;
    if (gPrimaryObjectFlags & 8) {
        return 0;
    }
    for (i = 1; i < 0x25; i++, entry++) {
        originalFlags = entry->field_1E;
        if (!(originalFlags & 0x1000)) {
            flags = originalFlags & 0xFFFB;
            entry->field_1E = flags;
            if (entry->field_2E == arg0->object->field_2 &&
                (flags & 0x100) && !(entry->field_1E & 3) &&
                !(entry->field_20 & 0x1000) &&
                bp[0] < entry->position[0] && entry->position[0] < bp[1] &&
                bp[2] < entry->position[2] && entry->position[2] < bp[3] &&
                bp[4] < entry->position[1] && entry->position[1] < bp[5]) {
                *arg0->flags = arg0->field_18;
                if (entry->field_A < 0x1E) {
                    *arg0->flags |= 0x4000;
                }
                *arg0->tile = entry;
                return 1;
            }
        } else if ((originalFlags & 0x800) &&
                   bp[0] < entry->alternatePosition[0] && entry->alternatePosition[0] < bp[1] &&
                   bp[2] < entry->alternatePosition[2] && entry->alternatePosition[2] < bp[3]) {
            *arg0->flags = arg0->field_18;
            *arg0->tile = entry;
            *arg0->flags |= 0x4000;
            return 1;
        }
    }
    return 0;
}
