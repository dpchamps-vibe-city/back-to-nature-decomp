/*
 * moving_items — reconstructed translation unit.
 *
 * Three-slot moving-item lifecycle, motion, collision response and projected
 * sprite drawing. See ovl11_moving_items.h for capacity, coordinate units,
 * preserved fields and side effects.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_moving_items.h"
#include "game_types.h"
#include "ovl11_collision_query_budget.h"
#include "aged_item_pool.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "ovl11_paired_item_ids.h"
#include "psyq/abs.h"
#include "psyq/memory.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "point_projection.h"
#include "ovl11_object_events.h"
#include "ovl11_primary_object_requests.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_interaction_id_predicates.h"
#include "ovl11_geometry_hit_flags.h"
#include "ovl11_grid_position.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_shared_animation_icon.h"

s32 ovl_11_func_800F3BCC(u16 arg0);
void Ovl11ReleaseMovingItemRecord(Ovl11CC4E0Object *arg0);

s32 Ovl11ProbeMovingItemHit(Ovl11CC4E0Object *arg0) {
    void *object;
    s32 result;

    if (*((struct Ovl11SpriteWork *)&D_8007AFF0)->header != 0) {
        goto failed;
    }
    if (((Ovl11CBE2CState *)D_8006C838)->selectedRecord != 0x15F) {
        return 0;
    }
    if (Ovl11QueryCollisionBudgeted((s32 *)((u8 *)arg0 + 0x24),
            ((Ovl11CBE2CState *)D_8006C838)->collision,
            0, 0, &object, &result, 0, 0x200) != 0) {
        goto success;
    }
failed:
    return 0;
success:
    ovl_11_func_800F3BCC(*(u16 *)((u8 *)arg0 + 2));
    PlaySoundEffect(0x13);
    Ovl11ReleaseMovingItemRecord(arg0);
    return 1;
}

void Ovl11ClearMovingItemRecord(Ovl11MovingItemRecord *record);

void Ovl11ResetMovingItemRecords(void) {
    u8 *var_s0;
    u32 var_s1;

    var_s1 = 0;
    var_s0 = gOvl11MovingItemRecords;
    do {
        Ovl11ClearMovingItemRecord((Ovl11MovingItemRecord *)var_s0);
        var_s1 += 1;
        var_s0 += 0x34;
    } while (var_s1 < 3U);
}

/* Direction, ground height and the two reserved words intentionally survive. */
void Ovl11ClearMovingItemRecord(Ovl11MovingItemRecord *record) {
    record->unk0 = 0;
    record->state = 0;
    record->timer = 0;
    record->itemId = 0;
    record->velocity.vx = 0;
    record->velocity.vy = 0;
    record->velocity.vz = 0;
    record->fallStep = 0;
    record->position.vx = 0;
    record->position.vy = 0;
    record->position.vz = 0;
}

Ovl11MovingItemRecord *Ovl11SpawnMovingItemRecord(Vec3 *position, u16 *itemId, s16 direction) {
    Ovl11MovingItemRecord *record;
    s32 *velocity;
    u16 id;
    u32 checked;

    record = (Ovl11MovingItemRecord *)gOvl11MovingItemRecords;
    checked = 0;
    do {
        checked += 1;
        if (record->itemId == 0) {
            velocity = (s32 *)(gOvl11MovingItemDirectionVelocity + direction * 8);
            record->direction = direction;
            Ovl11ClearMovingItemRecord(record);
            id = *itemId;
            record->itemId = (s16)id;
            record->position.vx = position->x;
            record->position.vy = position->y - 0x12C;
            record->position.vz = position->z;
            record->velocity.vx = velocity[0];
            record->velocity.vy = -0x1E;
            record->velocity.vz = velocity[1];
            if ((s16)id == 0x65) {
                record->velocity.vy = -0xC;
                record->fallStep = -0x1C;
                record->velocity.vx = velocity[0] * 2;
                record->velocity.vz = velocity[1] * 2;
            }
            if (record->itemId == 0x186) {
                record->fallStep = -0x4E;
                record->velocity.vy = -0x1E;
                record->velocity.vx = (velocity[0] * 2) / 3;
                record->velocity.vz = (velocity[1] * 2) / 3;
            }
            record->height = ((struct struct_8006C838_800CBF70 *)D_8006C838)->field_52CC;
            return record;
        }
        record = (Ovl11MovingItemRecord *)((u8 *)record + 0x34);
    } while (checked < 3U);
    return 0;
}

s32 Ovl11StepMovingItemMotion(Ovl11MovingItemRecord *arg0);
s32 Ovl11ProbeMovingItemHit(Ovl11CC4E0Object *arg0);
s32 ovl_11_func_800CE550(Vec3 *arg0);

void Ovl11UpdateMovingItemRecord(Ovl11MovingItemRecord *arg0) {
    VECTOR next;
    Ovl11CC7CCRequest request;
    s16 temp_a0;
    s16 temp_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 depth;
    s32 temp_v1_2;
    s32 var_a1;
    s32 var_s3;
    s32 var_v0;
    s32 var_v1;
    u16 temp_v0_3;
    u16 temp_v1;

    var_s3 = 0;
    gOvl11MovingItemProjectionInput[0] = (*(u16 *) ((u8 *) arg0 + 0x24)) + 0x96;
    gOvl11MovingItemProjectionInput[1] = ((*(u16 *) ((u8 *) arg0 + 0x28)) + (*(u16 *) ((u8 *) arg0 + 0xC))) - 0x64;
    gOvl11MovingItemProjectionInput[2] = (*(u16 *) ((u8 *) arg0 + 0x2C)) - 0x96;
    if (!(D_8006C844 & 0x08000000) && IsScreenFadeComplete() == 1) {
            temp_v1 = arg0->state;
            switch (temp_v1) {
            default:
                break;
            case 0:
                arg0->position.vx += arg0->velocity.vx / 4096;
                arg0->position.vz += arg0->velocity.vz / 4096;
                arg0->position.vy += arg0->fallStep;
                arg0->fallStep += 9;
                if (arg0->fallStep >= 0x2E) {
                    arg0->fallStep = 0x2D;
                }
                var_s3 = Ovl11StepMovingItemMotion(arg0);
                if (var_s3 == 1 || Ovl11ProbeMovingItemHit((Ovl11CC4E0Object *)arg0) != 0) return;
                break;
            case 1:
                if (--*(s16 *)&arg0->timer == 0) {
                    arg0->state = 2;
                    arg0->timer = 4;
                    arg0->unk0 = 1;
                }
                break;
            case 2:
                arg0->timer--;
                if ((s16)arg0->timer >= 4) {
                    if (arg0->timer & 4) return;
                } else if ((s16)arg0->timer >= 3) {
                    if (arg0->timer & 2) return;
                } else if ((s16)arg0->timer >= 2) {
                    if (arg0->timer & 1) return;
                }
                if ((s16)arg0->timer == 0) {
                    Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *)arg0);
                    return;
                }
                break;
            case 3:
            case 4:
                if (gOvl11MovingItemSpriteSource.field_2 & 0x100) {
                    Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *) arg0);
                    if (arg0->state == 4) {
                        request.id = 0x87;
                        request.count = 1;
                        Ovl11InstallPrimaryObjectRequest(&request.id);
                        return;
                    }
                    return;
                }
                break;
            }
    }

        depth = ProjectPointToScreen(gOvl11MovingItemScreenPosition, (SVECTOR *)gOvl11MovingItemProjectionInput);
        if (depth != 0) {
            temp_v1_2 = gActiveRenderContext->field_120 + ((depth >> 2) * 4);
            if ((u32) (arg0->state - 3) < 2U) {
                DrawSpriteAnimationRectsAndUpdate(temp_v1_2, &gOvl11MovingItemSpriteSource, *(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 0), *(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 4));
                return;
            }
            temp_a0 = arg0->itemId;
            if (temp_a0 == 0x186) {
                DrawSpriteFrameRects(temp_v1_2, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gOvl11HeldActionSpriteHeader + 0)), 0, (s16) (s32) (*(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 0)), (s16) (s32) (*(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 4)));
            } else {
                Ovl11DrawSlotSprite(temp_a0, arg0->unk0, (s32 *) temp_v1_2, *(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 0), (s16) (s32) (*(s16 *) ((u8 *) gOvl11MovingItemScreenPosition + 4)), 2);
            }
            next.vx = arg0->position.vx;
            next.vy = arg0->height;
            next.vz = arg0->position.vz;
            ovl_11_func_800CE550((Vec3 *) &next);
            if ((var_s3 == 2) && ((s32)Ovl11CacheGeometryHit() == 0)) {
                arg0->height = next.vy;
            }
            if (arg0->itemId != 0x186) {
                gOvl11MovingItemProjectionInput[0] = (*(u16 *) ((u8 *) arg0 + 0x24)) + 0x96;
                gOvl11MovingItemProjectionInput[1] = (u16) (*(s32 *) ((u8 *) arg0 + 0x10)) - 0x64;
                gOvl11MovingItemProjectionInput[2] = (*(u16 *) ((u8 *) arg0 + 0x2C)) - 0x96;
                depth = ProjectPointToScreen(gOvl11MovingItemScreenPosition, (SVECTOR *)gOvl11MovingItemProjectionInput);
                if (depth != 0) {
                    Ovl11DrawSharedAnimationIcon(gActiveRenderContext->field_120 + ((depth >> 2) * 4), gOvl11MovingItemScreenPosition[0], *(s32 *) ((u8 *) gOvl11MovingItemScreenPosition + 4), 0);
                }
            }
        }
}

s32 ovl_11_func_800C79F8(Vec3 *arg0);
void ovl_11_func_800F0FB0(s32 arg0);

s32 Ovl11HandleMovingItemGeometryHit(Ovl11CC4E0Object *arg0, Vec3 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    u16 temp_a0;
    char *far_base;
    char *main_base;

    temp_v0 = Ovl11ClassifyGeometryHit();
    if (temp_v0 != 0) {
        InitSpriteSourceData(&gOvl11MovingItemSpriteSource, (SpriteDataHeader *) &gSharedSpriteData);
        SelectSpriteAnimation((ObjectState *) &gOvl11MovingItemSpriteSource, 0x17);
        PlaySoundEffect(0x10);
        arg0->state = 3;
        arg0->fallStep = 0;
        if (temp_v0 == 2) {
            temp_a0 = *(u16 *) ((u8 *) arg0 + 2);
            if (((u32) (temp_a0 - 0xA1) < 3U) && (Ovl11AddAgedItem((s32) (s16) temp_a0) == 0)) {
                RequestMessageWindow(3, 0x382);
            }
            if ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0x84) {
                Ovl11QueueAgedItemUpdate();
            }
        } else {
            far_base = (char *)&D_8007AFF0;
            if (((*(Ovl11CC4E0Head **) (far_base + 0x25388))->unk2 == 0x2C) && ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0xA3)) {
                SetGameFlag(0x5EU, 1U);
            }
            if ((temp_v0 != 3) && ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0x186)) {
                gOvl11HeldActionSignal = 1;
            }
        }
        temp_v0_2 = ovl_11_func_800C79F8(arg1);
        if (temp_v0_2 == 0) {
            goto success;
        }
        switch (temp_v0_2) {
        case 38:
            if (Ovl11IsInteractionItemSetA((s16) (*(u16 *) ((u8 *) arg0 + 2))) != 0) {
                arg0->state = 4;
            }
            break;
        case 44:
            main_base = (char *)D_8006C838;
            (*(u16 *) (main_base + 0x51F0)) = (u16) (*(u16 *) ((u8 *) arg0 + 2));
            if ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0xA4) {
                (*(u16 *) (main_base + 0x51F0)) = (u16) (*(u16 *) (main_base + 0x5254));
            }
            ovl_11_func_800F0FB0(4);
            break;
        default:
            return 1;
        }
success:
        return 1;
    } else {
        return 0;
    }
}

/* Modes 1/6 query the main/alternate cell grid. Contact enters the one-tick
 * wait state and clears the fall step; no grid returns 2. Keep the two
 * independent mode bases and block-local result: their lifetimes preserve
 * the original address materialization and return-register allocation. */
s32 Ovl11CheckMovingItemCellContact(Ovl11MovingItemRecord *arg0) {
    u16 col;
    u16 row;
    s16 mode;
    u16 *cell;
    char *far_base;
    char *far_base2;

    cell = NULL;
    far_base = (char *)&D_8007AFF0;
    mode = *(s16 *)(far_base + 0x25476);
    if (mode == 1) {
        if (Ovl11LookupMode1GridPosition((s32 *)((u8 *)arg0 + 0x24), &col, &row) != 0) {
            arg0->state = mode;
            arg0->timer = mode;
            arg0->fallStep = 0;
            return 0;
        }
        cell = (u16 *)&gMainAreaGrid[(s16)row][(s16)col];
    }
    far_base2 = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base2 + 0x25476) == 6) {
        if (Ovl11LookupMode6GridPosition((VECTOR *)((u8 *)arg0 + 0x24), &col, &row) != 0) {
            goto store1;
        }
        cell = (u16 *)&gAlternateAreaGrid[(s16)row][(s16)col];
    }
    if (cell == NULL) {
        return 2;
    }
    {
        s32 r;
store1:
        r = 0;
        arg0->state = 1;
        arg0->timer = 1;
        arg0->fallStep = 0;
        return r;
    }
}

s32 Ovl11HandleMovingItemGeometryHit(Ovl11CC4E0Object *arg0, Vec3 *arg1);
Ovl11CC7CCSpawn *ovl_11_func_800F4994(Vec3 *arg0);
s32 ovl_11_func_800F3E44(s16 arg0);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
s32 ovl_11_func_80109188(Ovl11D04D4Obj *arg0, s32 arg1);
s32 Ovl11CheckMovingItemCellContact(Ovl11MovingItemRecord *arg0);

s32 Ovl11StepMovingItemMotion(Ovl11MovingItemRecord *arg0) {
    VECTOR next;
    Ovl11CC7CCRequest request;
    VECTOR position;
    Ovl11CC7CCSpawn *spawn;
    Ovl11CC7CCSpawn *saved;
    char *base;
    s32 high;
    s32 result;
    s32 kind;
    s32 *offset;

    high = 0;
    next.vx = arg0->position.vx + arg0->velocity.vx / 4096;
    next.vy = arg0->position.vy;
    next.vz = arg0->position.vz + arg0->velocity.vz / 4096;
    ovl_11_func_800CE550((Vec3 *)&next);
    if (arg0->position.vy >= 5001) {
        high = 1;
    }
    if (high == 0 && ((s32)Ovl11CacheGeometryHit() != 0 || next.vy < arg0->position.vy - 100)) {
        arg0->direction = (arg0->direction + 2) & 3;
        offset = (s32 *)(gOvl11MovingItemDirectionVelocity + arg0->direction * 8);
        arg0->velocity.vx = offset[0] * 2;
        arg0->velocity.vz = offset[1] * 2;
        arg0->position.vx += arg0->velocity.vx / 4096;
        arg0->position.vz += arg0->velocity.vz / 4096;
        if (arg0->itemId != 0x65) {
            arg0->velocity.vx = 0;
            arg0->velocity.vz = 0;
        }
        return 0;
    }
    if (arg0->position.vy > arg0->height || high != 0) {
        if (arg0->itemId == 0x65) {
            base = (char *)D_8006C838;
            saved = &(*(Ovl11CC7CCSpawnOwner **)(base + 0x8000 + 0x5DD4))->spawn;
            saved->id = *(u16 *)(base + 0x52C6);
            saved->position = arg0->position;
        }
        kind = Ovl11IsUpperPairedItemId(arg0->itemId);
        if (kind == 1) {
            request.count = kind;
            request.id = Ovl11MapToLowerPairedItemId(arg0->itemId);
            Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *)arg0);
            Ovl11InstallPrimaryObjectRequest(&request.id);
            return 1;
        }
        if (Ovl11HandleMovingItemGeometryHit((Ovl11CC4E0Object *)arg0, (Vec3 *)&next) != 0) {
            return 0;
        }
        if (arg0->itemId == 0x186) {
            goto remove;
        }
        {
            spawn = ovl_11_func_800F4994((Vec3 *)&next);
            if (spawn != NULL) {
                spawn->flags |= 0x100;
                if (D_8006C858[arg0->itemId].field_12 != 0 && ovl_11_func_800F3E44(arg0->itemId) != 0) {
                    RequestMessageWindow(3, 0x383);
                }
remove:
                Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *)arg0);
                return 1;
            }
            if (Ovl11GetGeometryHitHighBit() != 0 && D_8006C858[arg0->itemId].field_12 != 0) {
                ovl_11_func_800F3BCC(arg0->itemId);
                PlaySoundEffect(0x13);
                Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *)arg0);
                SetGameFlag(7, 1);
                return 1;
            }
            if (arg0->itemId == 0x65) {
                position.vx = arg0->position.vx;
                position.vy = arg0->position.vy;
                position.vz = arg0->position.vz;
                Ovl11ReleaseMovingItemRecord((Ovl11CC4E0Object *)arg0);
                ovl_11_func_800F43CC(0x65, position, D_80071AFE, 0);
                ovl_11_func_80109188((Ovl11D04D4Obj *)&D_800759E4, 7);
                return 1;
            }
            result = Ovl11CheckMovingItemCellContact(arg0);
            if (result != 2) {
                return result;
            }
            Ovl11CountDownObjectEventCounters();
            {
                s32 r;
                r = 0;
                arg0->state = 1;
                arg0->timer = 1;
                arg0->fallStep = 0;
                return r;
            }
        }
    }
    return 2;
}

void Ovl11UpdateMovingItemRecord(Ovl11MovingItemRecord *arg0);

void Ovl11UpdateMovingItemRecords(void) {
    Ovl11MovingItemRecord *var_s0;
    u32 var_s1;

    var_s0 = (Ovl11MovingItemRecord *) gOvl11MovingItemRecords;
    var_s1 = 0;
    do {
        if (var_s0->itemId != 0) {
            Ovl11UpdateMovingItemRecord(var_s0);
        }
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 < 3U);
}

void ovl_11_func_800CBD78(Ovl11C7010Object *arg0);

void Ovl11ReleaseMovingItemRecord(Ovl11CC4E0Object *arg0) {
    s16 temp_a0;

    temp_a0 = *(s16 *) ((u8 *) arg0 + 2);
    (*(s16 *) ((u8 *) arg0 + 2)) = 0;
    ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 &= ~0x40;
    if (temp_a0 == 0x186) {
        gOvl11HeldActionSpritePosition.x = *(u16 *) ((u8 *) arg0 + 0x24);
        gOvl11HeldActionSpritePosition.y = (*(u16 *) ((u8 *) arg0 + 0x28)) - 0xA;
        gOvl11HeldActionSpritePosition.z = *(u16 *) ((u8 *) arg0 + 0x2C);
        return;
    }
    ovl_11_func_800CBD78((Ovl11C7010Object *) ((u8 *) D_8006C838 + 0x51C8));
}
