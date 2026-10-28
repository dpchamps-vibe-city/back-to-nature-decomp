/*
 * stage_mode1_record — reconstructed translation unit.
 *
 * Mode-1 stage record lifecycle and both halves of its state machine.
 * The paired 19-slot tables select transition checks and per-frame scripts
 * for the same state. Probes publish found-record results consumed by the
 * scripts. Accepted transitions reset script/timer fields; the frame driver
 * clears the shared motion variant after executing the selected script.
 * The fixed-slot snapshot helper belongs to this same lifecycle.
 *
 * ovl11_stage_mode1_record.h publishes the external record interface. Callback
 * bodies and helpers retain external linkage for their original table entries,
 * but are not independent public animation services. The singleton, probe
 * results and script timers make this unit non-reentrant. State/facing indices
 * are unchecked; counters measure eligible updates, not elapsed time.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "random.h"
#include "game_types.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_stage_mode1_record.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_object_state_callbacks.h"
#include "ovl11_record_movement.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_direction_vector.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_selected_record_flag_guard.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_record_pair_order.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_record_position_set.h"

/* Implementation helpers used before their definitions; linkage is unchanged. */
s32 Ovl11LookupStageMode1Animation(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
s32 Ovl11RefreshActionMotion(Recon_ovl_11_func_800D2594_A0View *object);
void Ovl11PublishObjectToFixedSpawnRecord(Ovl11SpawnHandoffObject *object);

void Ovl11DrawStageMode1Record(u8 *arg0) {
    u16 animation;
    u16 subAnimation;

    if (!(D_8006C844 & 0x800000)) {
        Ovl11LookupStageMode1Animation(*(u16 *)(arg0 + 0x24), *(u16 *)(arg0 + 0x22),
                            &animation, &subAnimation);
        if (animation != arg0[0x7C]) {
            SelectSpriteAnimation((ObjectState *)(arg0 + 0x78), (u8)animation);
        }
        if (subAnimation != 0 && subAnimation != arg0[0x7D]) {
            SelectSpriteAnimationFrame((ObjectState *)(arg0 + 0x78), (u8)subAnimation);
        }
        SetSpriteAnimationUpdateLatch(0);
        Ovl11DrawObjectWithSourceOffsets(arg0, (s32)(arg0 + 0xE0 + gActiveRenderContextIndex * 8), 0x2800);
    }
}

void Ovl11AccumulateStageMode1Direction(s32 recordAddress) {
    Ovl11AccumulateDirectionVector((Recon800D0408A1View *)(recordAddress + 0x38),
                                  (Recon800D0408A1View *)(recordAddress + 0x48), 0);
}

s32 Ovl11InitStageMode1Record(s32 arg0, s32 arg1);

struct_800759E4 *Ovl11AcquireStageMode1Record(s32 id, s32 mode) {
    struct_800759E4 *record;

    record = NULL;
    if ((u32) (id - 0x106) >= 2U) {
        return NULL;
    }
    if (mode == 1) {
        record = &gStageMode1Record;
        if (gStageMode1Record.f00 != 0) {
            return NULL;
        }
        Ovl11InitStageMode1Record((s32) record, id);
    }
    return record;
}

typedef struct {
    /* 0x00 */ u16 field_0;
    /* 0x02 */ char pad_02[0x2C - 0x02];
    /* 0x2C */ u16 field_2C;
    /* 0x2E */ char pad_2E[0x34 - 0x2E];
    /* 0x34 */ s32 field_34;
} Ov11FlagSet80108D38;

s32 Ovl11ExpireStageMode1SequenceOutsideWindow(Ov11FlagSet80108D38 *arg0) {
    if (arg0->field_0 == 0) {
        return -1;
    }
    if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        arg0->field_2C = 0x12C;
        arg0->field_34 &= ~0x800;
    }
    return 0;
}

s32 Ovl11RefreshStageMode1Sprite(s32 *arg0);
s32 Ovl11SetStageMode1State(Ovl11D04D4Obj *arg0, s32 arg1);

s32 Ovl11UpdateStageMode1Counters(s32 *arg0) {
    s16 temp_v0_5;
    s16 temp_v1;
    s32 var_a0;
    s32 var_v0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_6;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;

    if (*(u16 *)arg0 == 0) {
        return -1;
    }
    if ((*(s32 *)((u8 *)arg0 + 0x34) & 0x400) &&
        Ovl11FindCalendarDate(((struct Ovl11B264StateView *)D_8006C838)->field_44BA,
                            ((struct Ovl11B264StateView *)D_8006C838)->field_44BC) == 8) {
        return 0;
    }
    {
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 2) {
            temp_v0 = (*(u16 *) ((u8 *) arg0 + 0x16)) + 2;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0;
            if ((s16) temp_v0 >= 0x100) {
                var_v1 = 0xFF;
            } else {
                var_v1 = temp_v0;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1;
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 4) {
            temp_v0_2 = (*(u16 *) ((u8 *) arg0 + 0x16)) + 2;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_2;
            if ((s16) temp_v0_2 >= 0x100) {
                var_v1_2 = 0xFF;
            } else {
                var_v1_2 = temp_v0_2;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_2;
        }
        if (!((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x26)) {
            temp_v0_3 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 1;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_3;
            if ((s16)temp_v0_3 < 0) {
                var_v1_3 = 0;
            } else {
                var_v1_3 = temp_v0_3;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_3;
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x20) {
            temp_v1 = (s16) (*(u16 *) ((u8 *) arg0 + 0x16));
            var_a0 = 1;
            if (temp_v1 >= 0xA) {
                var_a0 = 3;
                if (temp_v1 < 0x14) {
                    var_a0 = 2;
                }
            }
            temp_v0_4 = (*(u16 *) ((u8 *) arg0 + 0xAC)) + var_a0;
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = temp_v0_4;
            if ((s16) temp_v0_4 >= 0x100) {
                var_v1_4 = 0xFF;
            } else {
                var_v1_4 = temp_v0_4;
            }
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = var_v1_4;
        }
        if (((struct Ovl11B264StateView *)D_8006C838)->field_E4C8 != 0) {
            temp_v0_5 = (*(u16 *) ((u8 *) arg0 + 0x30)) - 1;
            switch (temp_v0_5) {
            case 0x0:
            case 0x7:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0x13:
            case 0x19:
            case 0x1C:
            case 0x21:
            case 0x26:
            case 0x29:
            case 0x2B:
            case 0x2F:
            case 0x33:
                temp_v0_6 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 5;
                (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_6;
                if ((s16)temp_v0_6 < 0) {
                    var_v1_5 = 0;
                } else {
                    var_v1_5 = temp_v0_6;
                }
                (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1_5;
                break;
            }
        }
        if ((((struct Ovl11B264StateView *)D_8006C838)->field_44BA == 2) && (((struct Ovl11B264StateView *)D_8006C838)->field_44BC >= 0) && (((struct Ovl11B264StateView *)D_8006C838)->field_44B8 == 0) && ((*(u16 *) ((u8 *) arg0 + 0)) == 0x107)) {
            (*(u16 *) ((u8 *) arg0 + 0)) = 0x106U;
            (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) | 0x8000);
            Ovl11RefreshStageMode1Sprite(arg0);
        }
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x26);
        Ovl11DeactivateUnselectedRecord(arg0);
        Ovl11ResetTimedSpriteEffect((s16 *) ((arg0 + 0x2A)));
        var_v0 = 0;
        if ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x100000) {
            Ovl11PublishObjectToFixedSpawnRecord((Ovl11SpawnHandoffObject *)arg0);
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
            (*(s16 *) ((u8 *) arg0 + 0x26)) = 0;
            (*(s32 *) ((u8 *) arg0 + 0x34)) &= ~0x100000;
            (*(s32 *) ((u8 *) arg0 + 0x34)) &= ~0x2000;
            Ovl11SetStageMode1State((Ovl11D04D4Obj *) arg0, 0);
            var_v0 = 0;
        }
    }
    return var_v0;
}

typedef struct {
    /* 0x00 */ u16 field_0;
    /* 0x02 */ char pad_02[0x34 - 0x02];
    /* 0x34 */ s32 field_34;
} Ov11FlagSet;

s32 Ovl11PropagateStageMode1Flag8000(Ov11FlagSet *arg0) {
    s32 temp_v1;

    if (arg0->field_0 == 0) {
        return -1;
    }
    temp_v1 = arg0->field_34;
    if (temp_v1 & 0x8000) {
        arg0->field_34 = temp_v1 | 0x800000;
    }
    return 0;
}

s32 Ovl11SelectStageMode1State(Ovl11D04D4Obj *arg0);

s32 Ovl11StepStageMode1Record(Ovl11HandlerObjectView *arg0) {
    void (*handler)(Ovl11HandlerObjectView *);

    if (arg0->unk0 == 0) {
        return -1;
    }
    handler = (void (*)(Ovl11HandlerObjectView *)) gOvl11StageMode1StepCallbacks[arg0->unk26];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(arg0->unk34 & 0x800) && arg0->unk2C >= 0x12C) {
        Ovl11SelectStageMode1State((Ovl11D04D4Obj *)arg0);
        arg0->unk2C = 0;
    }
    arg0->unk2C = (u16) arg0->unk2C + 1;
    if ((u32) (arg0->unk24 - 0x12) < 4) {
        arg0->unk34 |= 0x100000;
    } else {
        arg0->unk34 &= 0xFFEFFFFF;
    }
    gOvl11ActionMotionVariant = 0;
    return 0;
}

typedef struct {
    char pad_000[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
} D8006C838ButtonView;

s32 Ovl11SetStageMode1State(Ovl11D04D4Obj *arg0, s32 arg1) {
    s32 var_s1;
    s32 var_s3;
    u16 temp_a2;

    var_s1 = arg1;
    var_s3 = 0;
    if (arg0->unk34 & 0x400) {
        if (Ovl11FindCalendarDate(
                ((D8006C838ButtonView *) D_8006C838)->field_44BA,
                ((D8006C838ButtonView *) D_8006C838)->field_44BC) == 8) {
            return -1;
        }
    }
    temp_a2 = *(u16 *) &arg0->unk26;
    if (arg0->unk26 == 7 && var_s1 == 4) {
        return 0;
    }
    if (arg0->unk34 & 0x01000000) {
        var_s1 = 0x11;
    }
    if (gOvl11StageMode1TransitionCallbacks[var_s1] != 0) {
        if ((s16) temp_a2 == 7) {
            var_s3 = -1;
        } else if ((s16) temp_a2 == 5 && var_s1 == (s16) temp_a2) {
            var_s3 = -1;
        } else {
            var_s3 = gOvl11StageMode1TransitionCallbacks[var_s1](arg0);
        }
    }
    if (gOvl11StageMode1StepCallbacks[var_s1] != 0) {
        if (var_s3 != -1) {
            arg0->unk26 = var_s1;
            arg0->unk28 = 0;
            arg0->unk2A = 0;
            arg0->unk2C = 0;
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
    }
    return var_s3;
}

void Ovl11ClearStageMode1Record(s32 arg0) {
    memset(arg0, 0, 0xF0);
    *(s16 *)arg0 = 0;
}

s32 Ovl11RefreshStageMode1Sprite(s32 *arg0) {
    s32 *temp_s1;
    s32 var_a1;
    s32 var_s3;

    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    if ((*(u16 *) ((u8 *) arg0 + 0)) == 0x107) {
        (*(s32 *) ((u8 *) arg0 + 0x34)) &= 0xFFFF7FFF;
        (*(s32 *) ((u8 *) arg0 + 0x34)) &= 0xFF7FFFFF;
        var_a1 = 1;
        var_s3 = 0x1420;
    } else {
        (*(s32 *) ((u8 *) arg0 + 0x34)) |= 0x808000;
        var_a1 = 0;
        var_s3 = 0x23C8;
    }
    Ovl11LoadCopiedSpriteData(var_a1);
    temp_s1 = arg0 + 0x1E;
    InitSpriteSourceData((SpriteSourceData *) temp_s1, (SpriteDataHeader *) &D_8007F7F8);
    SetSpriteSourceRecordAddress((SomeStruct *) temp_s1, (s32) ((void *) ((u8 *) &D_8007F7F8 + var_s3)));
    (*(s16 *) ((u8 *) arg0 + 0xE0)) = 0;
    (*(s16 *) ((u8 *) arg0 + 0xE2)) = 0x1ED;
    (*(s16 *) ((u8 *) arg0 + 0xE4)) = 0x3C0;
    (*(s16 *) ((u8 *) arg0 + 0xE6)) = 0x180;
    (*(s16 *) ((u8 *) arg0 + 0xE8)) = 0x10;
    (*(s16 *) ((u8 *) arg0 + 0xEA)) = 0x1ED;
    (*(s16 *) ((u8 *) arg0 + 0xEC)) = 0x3E0;
    (*(s16 *) ((u8 *) arg0 + 0xEE)) = 0x180;
    return 0;
}

void Ovl11SetStageMode1PositionPreset(s32 arg0) {
    struct_800759E4 *t = &gStageMode1Record;

    switch (arg0) {
    case 0:
        t->f30 = 1;
        t->f38 = -0x708;
        t->f3C = 0;
        t->f40 = 0xC1C;
        break;
    case 1:
        t->f30 = 9;
        t->f38 = 0xE6;
        t->f3C = 0;
        t->f40 = -0x514;
        break;
    }
}

/* Fields preserved across the memset that clears the record. */
typedef struct {
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    u16 field_14;
} Ovl11Saved8010946C;

/* Two unaligned s32 words copied as one 8-byte packed aggregate, as in
 * ovl_11_func_8010D04C (D_8006C838+0x44B8 -> object+0x1A). */
typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk1;
} Ovl11UnalignedPair8010946C;

s32 Ovl11InitStageMode1Record(s32 arg0, s32 arg1) {
    Ovl11Saved8010946C saved;
    char *base;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
    u16 t4;
    s32 r0;
    s32 r1;
    s32 r2;
    s32 r3;
    u16 r4;

    if (*(u16 *) arg0 != 0) {
        return -1;
    }
    t0 = *(s32 *) (arg0 + 4);
    t1 = *(s32 *) (arg0 + 8);
    t2 = *(s32 *) (arg0 + 0xC);
    t3 = *(s32 *) (arg0 + 0x10);
    t4 = *(u16 *) (arg0 + 0x14);
    saved.field_4 = t0;
    saved.field_8 = t1;
    saved.field_C = t2;
    saved.field_10 = t3;
    saved.field_14 = t4;
    memset((void *) arg0, 0, 0xF0);
    *(u16 *) arg0 = arg1;
    *(s16 *) (arg0 + 0x16) = 0xA;
    r0 = saved.field_4;
    r1 = saved.field_8;
    r2 = saved.field_C;
    r3 = saved.field_10;
    r4 = saved.field_14;
    base = (char *) &D_8006C838;
    *(Ovl11UnalignedPair8010946C *) (arg0 + 0x1A) = *(Ovl11UnalignedPair8010946C *) (base + 0x44B8);
    *(s32 *) (arg0 + 4) = r0;
    *(s32 *) (arg0 + 8) = r1;
    *(s32 *) (arg0 + 0xC) = r2;
    *(s32 *) (arg0 + 0x10) = r3;
    *(u16 *) (arg0 + 0x14) = r4;
    Ovl11RefreshStageMode1Sprite((s32 *) arg0);
    Ovl11ResetTimedSpriteEffect((s16 *) (arg0 + 0xA8));
    return 0;
}

s32 Ovl11LookupStageMode1Animation(u16 state, u16 facing, u16 *animation, u16 *frame) {
    *animation = gOvl11StageMode1AnimationPairs[state][facing].animation;
    if (frame != 0) {
        *frame = gOvl11StageMode1AnimationPairs[state][facing].frame;
    }
    return 0;
}

s32 Ovl11ProbeStageMode1Target(Ovl11Func801098B0Arg *arg0);

s32 Ovl11SelectStageMode1State(Ovl11D04D4Obj *arg0) {
    s32 var_a1;
    s32 var_a0;
    s32 temp_v0;
    s32 var_s1;

    if (Ovl11ProbeStageMode1Target((Ovl11Func801098B0Arg *)arg0) == 1) {
        var_a1 = 6;
    } else if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        var_a1 = 2;
    } else {
        if (arg0->unk26 == 0xF) {
            var_a1 = 1;
        } else {
            if (arg0->unk34 & 0x2000) {
                if (arg0->unk34 & 0x8000) {
                    var_a0 = 0xA;
                } else {
                    var_a0 = 0xD;
                }
                var_s1 = 0x11;
            } else {
                if (arg0->unk34 & 0x8000) {
                    var_a0 = 6;
                } else {
                    var_a0 = 8;
                }
                var_s1 = 0x12;
            }
            temp_v0 = Rand(var_a0);
            switch (temp_v0) {
            case 0:
                var_a1 = var_s1;
                break;
            case 1:
                var_a1 = 8;
                break;
            case 2:
                var_a1 = 0xA;
                break;
            case 3:
                var_a1 = 0xC;
                break;
            case 4:
                var_a1 = 0xD;
                break;
            case 5:
                if (!(arg0->unk34 & 0x8000)) {
                    var_a1 = 0xE;
                } else {
                    var_a1 = 0xB;
                }
                break;
            case 6:
                if (!(arg0->unk34 & 0x8000)) {
                    var_a1 = 0xF;
                    break;
                } else {
                    var_a1 = var_s1;
                }
            case 7:
                if (!(arg0->unk34 & 0x8000)) {
                    var_a1 = 0x10;
                    break;
                }
            default:
                var_a1 = var_s1;
                break;
            }
        }
    }
    if (var_a1 != arg0->unk26) {
        Ovl11SetStageMode1State(arg0, var_a1);
    }
    return 0;
}

s32 Ovl11ProbeNearbyTaggedRecord(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 *flag, s32 **out) {
    Ovl11RecD749F4 *base;
    s32 result;
    s32 i;
    s16 sel;
    s32 r;
    s32 lo0;
    s32 hi0;
    s32 lo1;
    s32 hi1;
    s32 one;
    s32 notmatch;
    s32 mask;

    result = 0;
    i = 0;
    notmatch = 0x12;
    mask = 0x10000;
    base = (Ovl11RecD749F4 *)&gTaggedRecordBase;
    sel = key;
    r = radius;
    lo0 = position.vx - r;
    hi0 = position.vx + r;
    lo1 = position.vz - r;
    hi1 = position.vz + r;
    one = 1;
    do {
        if (base[i].unk0 != 0 && sel == base[i].unk30 && base[i].unk26 != notmatch && lo0 < base[i].unk38 && base[i].unk38 < hi0 && lo1 < base[i].unk40 && base[i].unk40 < hi1) {
            if (base[i].unk34 & mask) {
                *flag = 0;
                *out = (s32 *)&base[i];
                result = 1;
            } else {
                *flag = one;
                *out = (s32 *)&base[i];
                result = 1;
            }
            break;
        }
        i++;
    } while (i < 20);
    return result;
}

/* Open X/Z square, not a radial-distance test. A miss leaves *output alone.
 * Keep the kind constant after the two zero initializers: its birth site
 * preserves the independent result-address and record-kind register webs. */
s32 Ovl11ProbeNearbyCode15BRecord(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 **output) {
    s32 found;
    s32 index;
    UnkStruct80075BC4 *record;
    s16 selector;
    s32 halfWidth;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;
    s32 targetId;

    found = 0;
    index = 0;
    targetId = 0x15B;
    record = gStateObjectPool;
    selector = key;
    halfWidth = radius;
    minX = position.vx - halfWidth;
    maxX = position.vx + halfWidth;
    minZ = position.vz - halfWidth;
    maxZ = position.vz + halfWidth;
    do {
        if (record->unk0 == targetId && selector == record->unk30 && minX < record->unk38 && record->unk38 < maxX && minZ < record->unk40 && record->unk40 < maxZ) {
            *output = (s32 *)record;
            found = 1;
            break;
        }
        index++;
        record++;
    } while (index < 6);
    return found;
}

s32 Ovl11ProbeNearbyCode15BRecord(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 **out);

s32 Ovl11ProbeStageMode1Target(Ovl11Func801098B0Arg *arg0) {
    s16 temp_v1;
    s32 var_a0;
    s32 var_v0;

    var_v0 = 0;
    if (arg0->unk0 != 0) {
        if (arg0->unk34 & 0x8000) {
            temp_v1 = arg0->unkAC;
            var_a0 = 0x640;
            if (temp_v1 >= 0x32) {
                var_a0 = 0x7D0;
                if (temp_v1 >= 0x64) {
                    var_a0 = 0xAF0;
                    if (temp_v1 >= 0x96) {
                        var_a0 = 0x1770;
                        if (temp_v1 < 0xC8) {
                            var_a0 = 0xFA0;
                        }
                    }
                }
            }
            if (Ovl11ProbeNearbyCode15BRecord(*(Ovl11PaddedVec3 *)((u8 *)arg0 + 0x38), arg0->unk30, var_a0, &gOvl11ActionProbedRecord) == 1) {
                var_v0 = 1;
            } else {
                var_v0 = 0;
                gOvl11ActionProbedRecord = 0;
            }
            return var_v0;
        }
    }
    return var_v0;
}

s32 Ovl11MoveToNearbySpawn65(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 position, s16 arg5, s32 arg6) {
    Ovl11PaddedVec3 found;
    s16 kind;
    s32 result;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;

    result = 0;
    minX = position.vx - arg6;
    maxX = position.vx + arg6;
    minZ = position.vz - arg6;
    maxZ = position.vz + arg6;
    if (Ovl11ReadSpawnRecord(0x65, &found.vx, &kind, 0) == 0) {
        return 0;
    }
    if (arg5 != kind) {
        return 0;
    }
    if ((minX < found.vx) && (found.vx < maxX) &&
        (minZ < found.vz) && (found.vz < maxZ)) {
        Ovl11SetRecordPosition(arg0, found);
        result = 1;
    }
    return result;
}

typedef struct {
    /* 0x0000 */ char pad_000[0x52C8];
    /* 0x52C8 */ s32 field_52C8;
    /* 0x52CC */ s32 field_52CC;
    /* 0x52D0 */ s32 field_52D0;
    /* 0x52D4 */ s32 field_52D4;
} D8006C838_52C8View;

/* Per-frame state handler for the current stage mode. Rejects the object when
 * its id is unset, when the mode halfword at D_8007AFF0 + 0x25476 differs from
 * the object's mode, when a 0x400-flagged object is in the button state
 * returned by ovl_11_func_800C1224, or when the frame counter plus its 0x32
 * window has already passed. In the accepted path it optionally fires the
 * 0x19 button sound, copies the four D_8006C838 + 0x52C8 words into the
 * object's vector fields, marks the object with bit 1 of unk34, and dispatches
 * state 0x11. Returns 0 on success and -1 otherwise. */
s32 Ovl11TryStageMode1PositionSnapshot(Ovl11D04D4Obj *arg0) {
    char *far_base;
    char *far_base2;
    D8006C838_52C8View *base52;

    if (arg0->unk0 == 0) {
        return -1;
    }
    far_base = (char *) &D_8007AFF0;
    if (*(s16 *) (far_base + 0x25476) != arg0->unk30) {
        return -1;
    }
    if (arg0->unk34 & 0x400) {
        if (Ovl11FindCalendarDate(((D8006C838ButtonView *) D_8006C838)->field_44BA,
                                 ((D8006C838ButtonView *) D_8006C838)->field_44BC) == 8) {
            return -1;
        }
    }
    if ((s32)Rand(0xC8) < arg0->unk16 + 0x32) {
        far_base2 = (char *) &D_8007AFF0;
        if (*(s16 *) (far_base2 + 0x25476) == arg0->unk30) {
            PlaySoundEffect(0x19);
        }
        base52 = (D8006C838_52C8View *) D_8006C838;
        Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, *(Ovl11PaddedVec3 *) &base52->field_52C8);
        arg0->unk34 |= 0x2;
        Ovl11SetStageMode1State(arg0, 0x11);
        return 0;
    }
    return -1;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x16 - 0x02];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
} Struct_80109B88;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

s32 Ovl11ReduceStageMode1CounterAndStartEffect(Struct_80109B88 *arg0) {
    s32 temp;
    s32 out;
    char *far_base;

    if (arg0->unk0 == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        PlaySoundEffect(0x1A);
    }
    temp = arg0->unk16 - 10;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x1F, 0x1E);
    return 0;
}

s32 Ovl11MoveToNearbySpawn65(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 position, s16 arg5, s32 arg6);

s32 Ovl11TryStageMode1Spawn65Move(Ovl11SetFieldsView *arg0) {
    s32 result;
    char *far_base;

    if (*(u16 *) arg0 == 0) {
        return -1;
    }
    if (!(arg0->field_34 & 0x8000) ||
        (u32) (D_80070CF8_halfword - 6) >= 0xF) {
        return -1;
    }
    result = -1;
    if (Ovl11MoveToNearbySpawn65(arg0,
            *(Ovl11PaddedVec3 *) ((u8 *) arg0 + 0x38),
            *(s16 *) ((u8 *) arg0 + 0x30), 0x7D0) == 1) {
        far_base = (char *) &D_8007AFF0;
        if (*(s16 *) (far_base + 0x25476) ==
            *(s16 *) ((u8 *) arg0 + 0x30)) {
            PlaySoundEffect(0x19);
        }
        arg0->field_34 |= 0x20;
        result = 0;
    }
    return result;
}

s32 Ovl11ProbeNearbyTaggedRecord(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 *flag, s32 **record);

s32 Ovl11TryStageMode1TaggedRecordMove(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_t0;
    s32 found;

    if (*(u16 *) arg0 == 0) {
        return -1;
    }
    if (!(arg0->field_34 & 0x8000)) {
        return -1;
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0xAC);
    var_t0 = 0x4B0;
    if (temp_v1 >= 0x32) {
        var_t0 = 0x640;
        if (temp_v1 >= 0x64) {
            var_t0 = 0x7D0;
            if (temp_v1 >= 0x96) {
                var_t0 = 0xFA0;
                if (temp_v1 < 0xC8) {
                    var_t0 = 0xAF0;
                }
            }
        }
    }
    gOvl11ActionFoundRecord = NULL;
    found = Ovl11ProbeNearbyTaggedRecord(
        *(Ovl11PaddedVec3 *) ((u8 *) arg0 + 0x38), arg0->field_30,
        var_t0, &gOvl11ActionFoundRecordCategory, &gOvl11ActionFoundRecord);
    if (found != 1) {
        found = -1;
    } else {
        Ovl11SetRecordPosition(arg0,
            *(Ovl11PaddedVec3 *) ((u8 *) gOvl11ActionFoundRecord + 0x38));
        found = 0;
    }
    return found;
}

s32 Ovl11MoveStageMode1ToProbedRecord(u16 *arg0) {
    if (*arg0 == 0) {
        return -1;
    }
    if (gOvl11ActionProbedRecord != 0) {
        Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0,
            *(Ovl11PaddedVec3 *)&gOvl11ActionProbedRecord[14]);
        return 0;
    }
    return -1;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_2[0x32];
    /* 0x34 */ s32 unk34;
} UnkStruct80109E64;

s32 Ovl11StartStageMode1Flag4Effect(UnkStruct80109E64 *arg0) {
    if (arg0->unk0 == 0) {
        return -1;
    }
    if ((arg0->unk34 & 4) == 0) {
        Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x25, 0x2D);
    }
    arg0->unk34 |= 4;
    return 0;
}

/* Per-frame scripts: +28 is phase/cursor, +2A is the update count, and +34 bit
 * 800 locks a sequence. Phase 0 starts; reaching a script's end sets -1 but
 * completion is reported on the following call. Most completions also reset
 * the 300-update timer (slot 2 does not). Keep distinct callback bodies and
 * original order: the table shares slots 8/9 and 4/17 deliberately.
 */
s32 Ovl11TryActionSpawnHandoff(Ovl11SetFieldsView *object);

s32 Ovl11StepActionKeyframes0(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry0Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes2(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry2Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes1(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry1Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes8(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry8Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes10(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry10Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes11(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry11Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes12(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry12Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes13(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry13Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepFoundRecordAction(Ovl11A47CObject *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11FoundRecordActionTimer = 0;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, gOvl11FoundRecordActionKeyframes, &object->unk28, -1, &gOvl11FoundRecordActionTimer) != 0) {
            object->unk28 = -1;
        }
        if (object->unk34 & 0x2000) {
            object->unk58 = gOvl11ActionFoundRecord[0x38 / 4];
            object->unk5C = gOvl11ActionFoundRecord[0x3C / 4];
            object->unk60 = gOvl11ActionFoundRecord[0x40 / 4];
            object->unk64 = gOvl11ActionFoundRecord[0x44 / 4];
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            if (object->unk28 != 1) {
                object->unk28 = -1;
            }
        } else {
            if (gOvl11ActionFoundRecordCategory != 0) {
                Ovl11DispatchCode160Handler((Ovl11ActionDispatchView *)gOvl11ActionFoundRecord, 0xC);
            } else {
                Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)gOvl11ActionFoundRecord, 0xC);
            }
            if (object->unk28 == 1) {
                gOvl11FoundRecordActionTimer = 0;
                object->unk28++;
                if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                    PlaySoundEffect(0x19);
                }
            }
            Ovl11SelectRecordPairCode((Recon_ovl_11_func_800D2594_A0View *)object);
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepProbedRecordAction(Ovl11A654Object *object) {
    s16 phase;
    s32 completed;

    completed = 0;
    phase = object->unk28;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11ProbedRecordActionTimer = 0;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        Ovl11SetStageMode1State((Ovl11D04D4Obj *)object, 0);
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11A47CObject *)object, gOvl11ProbedRecordActionKeyframes, &object->unk28, -1, &gOvl11ProbedRecordActionTimer) != 0) {
            object->unk28 = -1;
        }
        if (object->unk34 & 0x2000) {
            object->unk58 = gOvl11ActionProbedRecord[0x38 / 4];
            object->unk5C = gOvl11ActionProbedRecord[0x3C / 4];
            object->unk60 = gOvl11ActionProbedRecord[0x40 / 4];
            object->unk64 = gOvl11ActionProbedRecord[0x44 / 4];
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            if (object->unk28 != 1) {
                object->unk28 = -1;
            }
        } else {
            if (object->unk28 == 1) {
                if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                    PlaySoundEffect(0x19);
                }
                Ovl11SetStateDebitAndDirection(((s16)object->unkAC / 10) & 0xFFFF, object->unk22);
                Ovl11EnterObjectState((u16 *)gOvl11ActionProbedRecord, 6);
                gOvl11ProbedRecordActionTimer = 0;
                object->unk28 = (u16)object->unk28 + 1;
            }
            Ovl11SelectRecordPairCode((Recon_ovl_11_func_800D2594_A0View *)object);
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepActionRecordHandoff(Ovl11A47CObject *object) {
    s32 completed;
    s32 motionFlag;
    u32 handoffStatus;

    motionFlag = object->unk34 & 0x01000000;
    completed = 0;
    switch (object->unk28) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11RecordHandoffTimer = 0;
        gOvl11RecordHandoffPhase = 0;
        break;
    case -3:
        if (object->unk34 & 0x01000000) {
            gOvl11ActionMotionVariant = 1;
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            break;
        }
        /* fallthrough */
    case -2:
        Ovl11PublishObjectToFixedSpawnRecord((Ovl11SpawnHandoffObject *)object);
        /* fallthrough */
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, gOvl11RecordHandoffKeyframes, &object->unk28, -1, &gOvl11RecordHandoffTimer)) {
            object->unk28 = -2;
        }
        switch (object->unk28) {
        case 1:
            handoffStatus = Ovl11TryActionSpawnHandoff((Ovl11SetFieldsView *)object);
            switch (handoffStatus) {
            case 1:
                object->unk28++;
                gOvl11RecordHandoffPhase = 1;
                break;
            case 0:
                break;
            case 2:
                object->unk28 = -1;
                break;
            }
            break;
        case 2:
            if ((u32)((u16)gOvl11RecordHandoffPhase - 1) >= 2U) {
                object->unk28 = -1;
                break;
            }
            gOvl11ActionMotionVariant = 1;
            Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *)(((struct Ovl11A850State *)D_8006C838)->record + 0x18), NULL);
            Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, ((struct Ovl11A850State *)D_8006C838)->position);
            if (object->unk7A & 0x300) {
                if (gOvl11RecordHandoffPhase == 2) {
                    object->unk28++;
                }
                gOvl11RecordHandoffPhase = 2;
            }
            break;
        case 3:
            if (gOvl11RecordHandoffPhase == 2) {
                if (object->unk34 & 0x2000) {
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, ((struct Ovl11A850State *)D_8006C838)->position);
                    gOvl11ActionMotionVariant = 1;
                    Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
                } else {
                    if (!motionFlag) {
                        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
                    }
                    object->unk28++;
                    gOvl11RecordHandoffTimer = 0;
                    gOvl11RecordHandoffPhase = 3;
                    if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                        PlaySoundEffect(0x19);
                    }
                }
            } else {
                object->unk28 = -1;
            }
            break;
        case 4:
            if ((gOvl11RecordHandoffPhase != 3) || motionFlag) {
                object->unk28 = -3;
            }
            break;
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes14(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry14Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes15(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry15Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes16(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry16Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionMovement(Recon_ovl_11_func_800D2594_A0View *object) {
    object->unk34 &= 0xFF7FFFFF;
    Ovl11StepRecordMovement(object, 0xC, 0x31);
    if ((*(s32 *)((u8 *)object + 0x48) == 0) && (object->unk4C == 0) && (*(s32 *)((u8 *)object + 0x50) == 0)) {
        *(s16 *)((u8 *)object + 0x24) = 0;
    } else {
        *(s16 *)((u8 *)object + 0x24) = 1;
    }
    object->unk34 &= ~0x800;
    return 1;
}

s32 Ovl11RefreshActionMotion(Recon_ovl_11_func_800D2594_A0View *object) {
    u16 *animations;
    s32 stepSize = 0x18;

    animations = gOvl11ActionMotionAnimations;
    if (gOvl11ActionMotionVariant != 0) {
        animations += 3;
    }
    if (object->unk34 & 0x8000) {
        object->unk34 |= 0x800000;
        stepSize = 0x24;
    }
    Ovl11StepRecordMovement(object, stepSize, 0x31);
    object->unk34 |= 0x800;
    if ((*(s32 *)((u8 *)object + 0x48) == 0) && (object->unk4C == 0) && (*(s32 *)((u8 *)object + 0x50) == 0)) {
        if (*(u16 *)((u8 *)object + 0x7A) & 0x300) {
            *(u16 *)((u8 *)object + 0x24) = animations[0];
        } else {
            *(u16 *)((u8 *)object + 0x24) = animations[1];
        }
    } else if (object->unk34 & 0x01000000) {
        *(u16 *)((u8 *)object + 0x24) = animations[2];
    } else {
        *(u16 *)((u8 *)object + 0x24) = animations[1];
    }
    if (*(u16 *)((u8 *)object + 0x7A) & 0x300) {
        if (!(object->unk34 & 0x01000000)) {
            object->unk34 &= ~0x800;
        }
    }
    if (!(object->unk34 & 0x2000)) {
        *(s16 *)((u8 *)object + 0x2C) = 0x12C;
        if ((*(u16 *)((u8 *)object + 0x7A) & 0x300) && !(object->unk34 & 0x01000000)) {
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
    }
    return 1;
}

/* Only the record-handoff callback calls this helper. Flag 2000 keeps the
 * target position synchronized to spawn 65; absence means clear direction.
 * Results are the caller's protocol: 0 = tracking, 1 = released, 2 = no spawn.
 */
s32 Ovl11TryActionSpawnHandoff(Ovl11SetFieldsView *object) {
    Ovl11PaddedVec3 position;
    u16 spawnKind;

    if (object->field_34 & 0x2000) {
        if (Ovl11ReadSpawnRecord(0x65, &position.vx, &spawnKind, 0) == 0) {
            return 2;
        }
        Ovl11SetRecordPosition(object, position);
        Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
        return 0;
    }
    Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
    return 1;
}

/* Both dispatch phases publish through this snapshot adapter. It leaves the
 * source object unchanged and updates fixed spawn slot 1 (kind 0x65), setting
 * its active bit while preserving unrelated fields. Pools and selector must
 * already be valid; coordinates are unscaled world units.
 */
void Ovl11PublishObjectToFixedSpawnRecord(Ovl11SpawnHandoffObject *object) {
    Ovl11SpawnHandoffObject snapshot;

    snapshot = *object;
    /* Keep the complete object snapshot and all four position ABI words.
     * The fixed-slot writer consumes XYZ but receives an SDK VECTOR by value. */
    Ovl11SetSpawnRecord(0x65, *(VECTOR *)&snapshot.position, snapshot.selector, 0);
}
