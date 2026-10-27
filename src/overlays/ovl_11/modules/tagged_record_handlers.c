/*
 * The 0x164/0x165 tagged-record family: construction, per-update counters,
 * sprite selection, and complementary request/step handler tables.
 *
 * Records occupy 0xB8 bytes in the shared pool. A nonzero tag alone does not
 * mean active: flag 0x10000 at +34 must also be set. The request table tests
 * eligibility and starts an operation; the step table advances its packed
 * keyframes or pose until flag 0x800 is cleared. Handler IDs are table indices,
 * not gameplay action names. Timers count calls, not elapsed milliseconds.
 *
 * The creation facade remains in ovl11_tagged_record_creation.h; frame and
 * dispatch clients use ovl11_tagged_record_handlers.h. Partial record views
 * deliberately retain their witnessed signed/unsigned halfword accesses.
 * None establishes a new storage layout or private linkage.
 *
 * Membership and the boundaries to the preceding pose family and following
 * sprite/handler bank are recorded in notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_tagged_record_handlers.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_tagged_record_creation.h"
#include "random.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_timed_sprite_effect.h"
#include "psyq/memory.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_direction_vector.h"
#include "ovl11_record_effect_threshold.h"
#include "ovl11_cd_audio_request.h"
#include "ovl11_record_status_message.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"

s32 Ovl11GetTaggedRecordSpriteDescriptor(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void Ovl11SelectAndDrawTaggedRecordSprite(u8 *record) {
    u16 animation;
    u16 frame;
    u16 descriptorRow;

    Ovl11GetTaggedRecordSpriteDescriptor(*(u16 *)(record + 0x24),
        *(u16 *)(record + 0x22), &animation, &frame);
    if (animation != *(u8 *)(record + 0x7C)) {
        SelectSpriteAnimation((ObjectState *)(record + 0x78), (u8)animation);
    }
    /* Zero means retain the current frame, not select frame zero. */
    if (frame != 0 && frame != *(u8 *)(record + 0x7D)) {
        SelectSpriteAnimationFrame((ObjectState *)(record + 0x78), (u8)frame);
    }
    Ovl11DrawObjectWithMarker(record);
    descriptorRow = *(u16 *)(record + 0x24);
    if (descriptorRow == 0 || descriptorRow == 5) {
        *(u16 *)(record + 0x7A) = (u16)(*(u16 *)(record + 0x7A) | 0x100);
    }
}

void Ovl11ApplyTaggedRecordDisplacement(s32 recordAddress) {
    Ovl11AccumulateDirectionVector(recordAddress + 0x38, recordAddress + 0x48, 0);
}

/*
 * Creation facades for the shared 0xB8-byte tagged-record pool. Both use the
 * 0x164/0x165 initializer; the indexed-copy path deliberately ignores its
 * failure result. No ownership or private linkage is inferred for the pool.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
s32 Ovl11InitTaggedRecord(UnkStruct800E109C *record, s32 code);

s32 Ovl11AllocateTaggedRecord(s32 code, s32 unused1, s32 unused2) {
    s32 recordAddress;
    s32 slot;

    if (((u32)(code + -0x164)) < ((u32)2)) {
        recordAddress = (s32)Ovl11FindFreeTaggedRecord();
        if (recordAddress == 0) {
            return 0;
        } else {
            slot = Ovl11GetTaggedRecordIndex((u8 *)recordAddress);
            Ovl11InitTaggedRecord((UnkStruct800E109C *)recordAddress, code);
            Ovl11ApplyRecordRowPlacement(recordAddress, ((s16)slot), 1);
            return recordAddress;
        }
    } else {
        return 0;
    }
}

UnkStruct800E109C *Ovl11InitOffsetRecordCopy(s32 sourceSlot, s32 destinationSlot) {
    UnkStruct800E109C *destination;
    void *source;

    destination = (UnkStruct800E109C *) ((u8 *) &gTaggedRecordBase + (destinationSlot * 0xB8));
    Ovl11InitTaggedRecord(destination, 0x164);
    source = (void *) ((u8 *) &gTaggedRecordBase + (sourceSlot * 0xB8));
    *(s32 *) ((u8 *) destination + 0x38) = *(s32 *) ((u8 *) source + 0x38);
    *(s32 *) ((u8 *) destination + 0x3C) = *(s32 *) ((u8 *) source + 0x3C);
    *(s32 *) ((u8 *) destination + 0x40) = *(s32 *) ((u8 *) source + 0x40) - 0xC8;
    *(u16 *) ((u8 *) destination + 0x30) = *(u16 *) ((u8 *) source + 0x30);
    *(u16 *) ((u8 *) destination + 0x22) = *(u16 *) ((u8 *) source + 0x22);
    return destination;
}

s32 Ovl11IsTaggedRecordActive(UnkStruct800E109C *arg0);
s32 ovl_11_func_800D9108(s32 *arg0);

s32 Ovl11UpdateTaggedRecordActivity(UnkStruct800E109C *arg0) {
    s16 temp_s1;
    s32 temp_a1;
    s32 var_v0;
    s32 temp_v0_2;
    s32 var_v1;

    if (Ovl11IsTaggedRecordActive(arg0) == 0) {
        return -1;
    }
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x34);
    if (temp_a1 & 0x400) {
        return -1;
    }
    temp_s1 = *(s16 *)((u8 *)arg0 + 0x30);
    if ((temp_s1 == 1) &&
        (((struct Ovl11HandlerState *)D_8006C838)->field_44BA != 3)) {
        if (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 != temp_s1) {
            if ((++(*(u16 *)((u8 *)arg0 + 0xAC)) >= 5U) &&
                !(temp_a1 & 1) &&
                (ovl_11_func_800D9108((s32 *)((u8 *)arg0 + 0x38)) == temp_s1)) {
                *(s32 *)((u8 *)arg0 + 0x34) |= 1;
            }
        } else {
            temp_v0_2 = *(u16 *)((u8 *)arg0 + 0x16) - 5;
            *(u16 *)((u8 *)arg0 + 0x16) = temp_v0_2;
            if ((s16)temp_v0_2 < 0) {
                var_v1 = 0;
            } else {
                var_v1 = temp_v0_2;
            }
            *(u16 *)((u8 *)arg0 + 0x16) = var_v1;
        }
    }
    var_v0 = 0;
    if ((u32)(((struct Ovl11HandlerState *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
        *(s16 *)((u8 *)arg0 + 0x2C) = 0x12C;
        *(s32 *)((u8 *)arg0 + 0x34) &= ~0x800;
        var_v0 = 0;
    }
    return var_v0;
}

s32 Ovl11UpdateTaggedRecordCounters(Ovl11Deb00Object *arg0) {
    s32 i;
    s32 temp;
    s32 out;
    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    if (arg0->field_34 & 0x400) {
        return -1;
    }
    Ovl11GetTaggedRecordIndex((u8 *)arg0);
    if (arg0->field_30 == 5 && !(arg0->field_34 & 1)) {
        for (i = 0; i < 22; i++) {
            if (((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] != 0) {
                arg0->field_34 |= 1;
                ((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] = 0;
                break;
            }
        }
    }
    if (arg0->field_34 & 2) {
        arg0->field_16 += 3;
    }
    if (arg0->field_34 & 0x10) {
        temp = arg0->field_B4;
        temp++;
        arg0->field_B4 = temp;
        if ((u16)temp >= 8) {
            out = 7;
        } else {
            out = temp;
        }
        arg0->field_B4 = out;
    }
    if (arg0->field_34 & 1) {
        arg0->field_34 |= 0x10;
        arg0->field_16++;
    } else {
        arg0->field_34 &= ~0x10;
        arg0->field_16 -= 10;
    }
    if (arg0->field_AC >= 5) {
        arg0->field_16++;
    }
    temp = arg0->field_16;
    if ((s16)temp < 0) {
        temp = 0;
    }
    arg0->field_16 = temp;
    out = temp;
    if ((s16)out >= 256) {
        out = 255;
    }
    arg0->field_16 = out;
    if (!(arg0->field_34 & 0x100)) {
        temp = arg0->field_AE;
        if (temp != 0) {
            temp++;
            arg0->field_AE = temp;
            if ((u16)temp >= 32) {
                temp = 31;
            }
            arg0->field_AE = temp;
            if (arg0->field_AE >= 3 && (s32)Rand(100) <= arg0->field_AE * 10) {
                arg0->field_34 |= 0x100;
            }
        } else if ((s16)arg0->field_16 <= 0 && Rand(100) < 11) {
            arg0->field_AE++;
        }
    }
    if (arg0->field_B2 != 0) {
        arg0->field_B2++;
        if (arg0->field_B2 >= 21) {
            arg0->field_B2 = 0;
            arg0->field_34 |= 0x200;
        }
    }
    if (!(arg0->field_34 & 0x100)) {
        if (arg0->field_1A == 3) {
            if (Rand(100) < 26) {
                arg0->field_34 |= 0x80100;
            }
        } else if (arg0->field_1A == 4) {
            if (Rand(100) < 76) {
                arg0->field_34 |= 0x80100;
            }
        }
        if (arg0->field_1A >= 5) {
            arg0->field_34 |= 0x80100;
        }
    }
    temp = arg0->field_B0;
    temp++;
    arg0->field_B0 = temp;
    out = temp;
    if ((u16)out >= 32) {
        out = 31;
    }
    arg0->field_B0 = out;
    arg0->field_8C = 0;
    if (arg0->field_1E == ((Ovl11Deb00State *)D_8006C838)->field_44BC &&
        arg0->field_1C == ((Ovl11Deb00State *)D_8006C838)->field_44BA) {
        arg0->field_1A++;
    }
    arg0->field_2C = Rand(300);
    if (!(arg0->field_34 & 1)) {
        ((Ovl11Deb00State *)D_8006C838)->field_44F8 |= 0x8000000;
    }
    arg0->field_AC = 0;
    arg0->field_34 &= ~1;
    arg0->field_34 &= ~2;
    arg0->field_34 &= ~4;
    arg0->field_34 &= ~8;
    arg0->field_34 &= ~0x2000;
    Ovl11ResetTimedSpriteEffect(arg0->field_A8);
    return 0;
}

s32 Ovl11GetTaggedRecordCode(Recon_ovl_11_func_800E0AFC_A0View *arg0) {
    s32 callRet2;
    callRet2 = Ovl11IsTaggedRecordActive(((UnkStruct800E109C *)(((s32)arg0))));
    if (callRet2 == 0) {
        return -1;
    } else {
        if (((u32)arg0->unkB0) < ((u32)14)) {
            return 0x164;
        } else {
            return 0x165;
        }
    }
}

s16 Ovl11InitTaggedRecordSpriteSource(Recon_ovl_11_func_800E0AFC_A0View *arg0);
s32 Ovl11SelectIdleTaggedRecordHandler(Recon_ovl_11_func_8010CE80_A0View *arg0);

s32 Ovl11StepTaggedRecordHandler(Recon_ovl_11_func_8010CE80_A0View *arg0) {
    s32 (*handler)(void *);
    char *far_base;

    if (*(u16 *)arg0 == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (((*(s16 *)(far_base + 0x25476) == 1) ||
         (*(s16 *)(far_base + 0x25476) == 5)) &&
        (*(s32 *)((u8 *)arg0 + 0x8C) == 0)) {
        Ovl11InitTaggedRecordSpriteSource((Recon_ovl_11_func_800E0AFC_A0View *)arg0);
    }
    handler = gOvl11TaggedRecordStepHandlers[(u16)arg0->unk26];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(arg0->unk34 & 0x800) && arg0->unk2C >= 0x12C) {
        Ovl11SelectIdleTaggedRecordHandler(arg0);
        arg0->unk2C = 0;
    }
    arg0->unk2C++;
    return 0;
}

s32 Ovl11DispatchTaggedRecordHandler(Ovl11ActionDispatchView *record, s32 handlerId) {
    s32 result;

    result = 0;
    if (gOvl11TaggedRecordRequestHandlers[handlerId] != 0) {
        if (record->unk26 == handlerId && (record->unk34 & 0x800)) {
            result = -1;
        } else {
            result = gOvl11TaggedRecordRequestHandlers[handlerId](record);
        }
    }
    if (gOvl11TaggedRecordStepHandlers[handlerId] != 0 && result != -1) {
        record->unk26 = handlerId;
        record->unk28 = 0;
        record->unk2A = 0;
        record->unk2C = 0;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)record + 0x48), 0);
    }
    /* Clear the pending-recipient slot even when the request is refused. */
    record->unkB6 = -1;
    return result;
}

void Ovl11ClearTaggedRecord(s32 recordAddress) {
    memset(recordAddress, 0, 0xB8);
    *(s16 *)recordAddress = 0;
}

s32 Ovl11InitTaggedRecord(UnkStruct800E109C *arg0, s32 arg1) {
    char *base;
    char *far_base;
    s32 flags;

    if (Ovl11IsTaggedRecordActive(arg0) != 0) {
        return -1;
    }
    memset(arg0, 0, 0xB8);
    if (arg1 == 0x165) {
        *(s16 *)((u8 *)arg0 + 0xB0) = 0xE;
    }
    *(s16 *)((u8 *)arg0 + 0x16) = 0xA;
    arg0->unk0 = (u16)arg1;
    *(s16 *)((u8 *)arg0 + 0xB4) = 7;
    base = (char *)D_8006C838;
    *(Ovl11InitStamp *)((u8 *)arg0 + 0x1A) = *(Ovl11InitStamp *)(base + 0x44B8);
    flags = *(s32 *)((u8 *)arg0 + 0x34);
    *(u16 *)((u8 *)arg0 + 4) = 0xFFFF;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    flags |= 0x10000;
    *(s32 *)((u8 *)arg0 + 0x34) = flags;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == 1 || *(s16 *)(far_base + 0x25476) == 5) {
        Ovl11InitTaggedRecordSpriteSource((Recon_ovl_11_func_800E0AFC_A0View *)arg0);
    } else {
        *(s32 *)((u8 *)arg0 + 0x8C) = 0;
    }
    Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)arg0 + 0xA8));
    return 0;
}

s32 Ovl11GetTaggedRecordCode(Recon_ovl_11_func_800E0AFC_A0View *arg0);

s16 Ovl11InitTaggedRecordSpriteSource(Recon_ovl_11_func_800E0AFC_A0View *arg0) {
    SpriteSourceData *temp_s0;
    s32 temp_v0;
    s32 var_a2;

    temp_v0 = Ovl11GetTaggedRecordCode(arg0);
    var_a2 = 5;
    switch (temp_v0) {                              /* switch 1; irregular */
    case 0x164:                                     /* switch 1 */
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        break;
    case 0x165:                                     /* switch 1 */
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        var_a2 = 6;
        break;
    }
    /* Keep these one-iteration scopes: flattening them changes this
     * compiler's temporary allocation in the sprite-source initializer. */
    do {
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0xFBFFFFFF);
        if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
            var_a2 = 8;
        }
    } while (0);
    do {
        do {
            do {
                temp_s0 = (SpriteSourceData *)((u8 *)arg0 + 0x78);
            } while (0);
        } while (0);
    } while (0);
    if ((u16) (*(u16 *) ((u8 *) arg0 + 0xB4)) < 7U) {
        var_a2 = 9;
    }
    InitSpriteSourceData((SpriteSourceData *) temp_s0, (SpriteDataHeader *) ((u8 *) D_8008F7F8_words + D_80124FCC_spriteOffsets[var_a2]));
    if ((*(u16 *) ((u8 *) arg0 + 0xAE)) != 0) {
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0x164:                                 /* switch 2 */
            SetSpriteSourceTextureOffsets((Struct_800154CC *) temp_s0, 0, 0, 0, 3);
            break;
        case 0x165:                                 /* switch 2 */
            SetSpriteSourceTextureOffsets((Struct_800154CC *) temp_s0, 0, 0, 0, 2);
            break;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *) temp_s0, 0, 0, 0x20, 0);
        }
    }
    return 0;
}

s32 Ovl11GetTaggedRecordSpriteDescriptor(u16 row, u16 column,
                                       u16 *animationOut, u16 *frameOut) {
    *animationOut = gOvl11TaggedRecordSpriteDescriptors[row][column].animation;
    if (frameOut != 0) {
        *frameOut = gOvl11TaggedRecordSpriteDescriptors[row][column].frame;
    }
    return 0;
}

s32 Ovl11DispatchTaggedRecordHandler(Ovl11ActionDispatchView *arg0, s32 arg1);

s32 Ovl11SelectIdleTaggedRecordHandler(Recon_ovl_11_func_8010CE80_A0View *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 temp_v0;

    if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        var_a1 = 0xF;
    } else {
        if (arg0->unk26 == 0xF) {
            var_a1 = 0x10;
        } else {
            var_a0 = 3;
            if (arg0->unk34 & 0x2000) {
                var_a0 = 9;
            }
            temp_v0 = Rand(var_a0);
            switch (temp_v0) {
            case 0:
                var_a1 = 0x12;
                break;
            case 1:
                var_a1 = 0xE;
                break;
            case 2:
                var_a1 = 0xF;
                break;
            default:
                var_a1 = 0x12;
                break;
            }
        }
    }
    if (var_a1 != arg0->unk26) {
        Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)arg0, var_a1);
    }
    return 0;
}

s32 Ovl11IsTaggedRecordActive(UnkStruct800E109C *record) {
    if (record->unk0 == 0) {
        return 0;
    }
    return (*(s32 *)((char *)record + 0x34) & 0x10000) > 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0xA8 - 0x32];
    /* 0xA8 */ char unkA8[0];
} Struct_800E10C8;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

s32 Ovl11RequestTaggedRecordEffect24(Struct_800E10C8 *arg0) {
    s32 temp;
    s32 out;
    char *far_base;

    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    temp = arg0->unk16 - 10;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x24, 0x2D);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xC);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0xA8 - 0x18];
    /* 0xA8 */ char unkA8[0];
} Struct_800E1158;

s32 Ovl11RequestTaggedRecordEffect1F(Struct_800E1158 *arg0) {
    s32 temp;
    s32 out;

    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    temp = arg0->unk16 - 1;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x1F, 0x2D);
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0xAE - 0x32];
    /* 0xAE */ u16 unkAE;
} Struct_800E11C0;

s32 Ovl11ClearTaggedRecordAuxLatch(Struct_800E11C0 *arg0) {
    char *far_base;

    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    if (arg0->unkAE == 0) {
        return -1;
    }
    arg0->unkAE = 0;
    SetSpriteSourceTextureOffsets((Struct_800154CC *)((s32)arg0 + 0x78), 0, 0, 0, 0);
    Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xB);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_2[0x2E];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x2];
    /* 0x34 */ s32 unk34;
} UnkStruct800E1254;

s32 Ovl11RequestTaggedRecordFlag4(UnkStruct800E1254 *arg0) {
    char *far_base;

    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    if (!(arg0->unk34 & 4)) {
        Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
    }
    arg0->unk34 |= 4;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xB);
    }
    return 0;
}

s32 Ovl11ReserveTaggedRecordPair(u8 *arg0) {
    Ovl11RecordPairSlot *temp_v0;
    s16 temp_s0;
    s32 *temp_v0_2;
    u16 temp_v1;
    char *far_base;

    temp_v1 = *(u16 *)arg0;
    if (temp_v1 == 0) {
        return -1;
    }
    if (temp_v1 != 0x165) {
        return -1;
    }
    if (*(u16 *)(arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(u16 *)(arg0 + 0xAE) != 0) {
        return -1;
    }
    if (*(u16 *)(arg0 + 0xB4) < 7U) {
        return -1;
    }
    temp_v0 = Ovl11FindFreeRecordPairSlot(1);
    if (temp_v0 == NULL) {
        return -1;
    }
    temp_v0_2 = Ovl11FindFreeTaggedRecord();
    if (temp_v0_2 == NULL) {
        return -1;
    }
    temp_s0 = Ovl11GetTaggedRecordIndex((u8 *)temp_v0_2);
    Ovl11SetRecordPairSlot(temp_v0, temp_s0, Ovl11GetTaggedRecordIndex(arg0));
    *(s32 *)((u8 *)temp_v0_2 + 0x34) |= 0x02000000;
    *(u16 *)(arg0 + 0xB2) = 1;
    Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == *(s16 *)(arg0 + 0x30)) {
        Ovl11RequestCdAudioTrack(0xB);
    }
    return 0;
}

s32 Ovl11SubmitTaggedRecordRequest(UnkStruct800E109C *arg0) {
    s16 sp10[3];
    s16 temp_v1;
    s16 var_s1;
    s32 var_v0;
    Ovl11DF84CStateView *temp_v0;
    u8 *far_base;
    u8 *mode_base;
    u8 *state_base;

    if (Ovl11IsTaggedRecordActive(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x165) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB4) < 7U) {
        return -1;
    }
    temp_v1 = *(s16 *)((u8 *)arg0 + 0x16);
    var_s1 = 0;
    if (temp_v1 >= 0xC9) {
        var_s1 = 0x93;
        if (*(s32 *)((u8 *)arg0 + 0x34) & 0x40) {
            var_s1 = 0xE7;
        }
    } else if (temp_v1 >= 0x65) {
        var_s1 = 0x92;
    } else if (temp_v1 >= 0) {
        var_s1 = 0x91;
    }
    if (*(s16 *)((u8 *)arg0 + 0xB6) == -1) {
        if (var_s1 != 0) {
            *(u16 *)((u8 *)arg0 + 0xB4) = 0;
            Ovl11SetHalfwordTriplet(sp10, var_s1, 0, 1);
            Ovl11InstallPrimaryObjectRequest((u16 *)sp10);
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
            goto block_19;
        }
    } else if (var_s1 != 0) {
        state_base = (u8 *)D_8006C838;
        temp_v0 = (Ovl11DF84CStateView *)
            (state_base + (*(s16 *)((u8 *)arg0 + 0xB6) * 0xF8));
        if (temp_v0->pending == 0) {
            *(u16 *)((u8 *)arg0 + 0xB4) = 0;
            temp_v0->pending = (u16)var_s1;
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
            mode_base = (u8 *)&D_8007AFF0;
            if (*(s16 *)(mode_base + 0x25476) == 1 ||
                *(s16 *)(mode_base + 0x25476) == 5) {
block_19:
                Ovl11InitTaggedRecordSpriteSource((Recon_ovl_11_func_800E0AFC_A0View *)arg0);
            } else {
                *(s32 *)((u8 *)arg0 + 0x8C) = 0;
            }
        } else {
            var_s1 = 0;
        }
        if (var_s1 != 0) {
            far_base = (u8 *)&D_8007AFF0;
            if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
                Ovl11RequestCdAudioTrack(0xB);
            }
        }
    }
    var_v0 = 0;
    if (var_s1 == 0) {
        var_v0 = -1;
    }
    return var_v0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x30];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x2];
    /* 0x34 */ s32 unk34;
} Struct_800E15C8;

s32 Ovl11RequestTaggedRecordFlag2(Struct_800E15C8 *arg0) {
    char *far_base;

    if (Ovl11IsTaggedRecordActive((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    if (!(arg0->unk34 & 2)) {
        Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
    }
    arg0->unk34 |= 2;
    Ovl11RequestRecordStatusMessage((UnkStruct800DF4F0 *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xB);
    }
    return 0;
}

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x58 - 0x38];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} Struct_800E1654;

s32 Ovl11TryCaptureTaggedRecordWorkState(Struct_800E1654 *arg0) {
    char *far_base;
    char *base;
    s32 ret;

    ret = 0;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) != arg0->unk30) {
        return -1;
    }
    if ((s32)Rand(0xC8) < arg0->unk16) {
        base = (char *)&D_8006C838;
        arg0->unk58 = *(s32 *)(base + 0x52C8);
        arg0->unk5C = *(s32 *)(base + 0x52CC);
        arg0->unk60 = *(s32 *)(base + 0x52D0);
        arg0->unk64 = *(s32 *)(base + 0x52D4);
        arg0->unk34 |= 0x2000;
        Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)arg0, 0x12);
    } else {
        ret = -1;
    }
    return ret;
}

typedef struct {
    /* 0x00 */ char pad_00[0x26];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ char pad_2E[0x34 - 0x2E];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x58 - 0x38];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} Struct_800E1714;

s32 Ovl11CaptureTaggedRecordWorkState(Struct_800E1714 *arg0) {
    char *base;

    base = (char *)&D_8006C838;
    arg0->unk58 = *(s32 *)(base + 0x52C8);
    arg0->unk5C = *(s32 *)(base + 0x52CC);
    arg0->unk60 = *(s32 *)(base + 0x52D0);
    arg0->unk64 = *(s32 *)(base + 0x52D4);
    arg0->unk34 |= 0x2000;
    Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)arg0, 0x12);
    return 0;
}

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);

s32 Ovl11OffsetTaggedRecordWorkPosition(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 sp18;
    u8 *base;
    u16 selector;

    base = (u8 *) D_8006C838;
    Ovl11SetDirectionVector(*(u16 *) (base + 0x5200), (Recon800D0408A1View *) &sp18, 0x320);
    sp18.field_0 = sp18.field_0 + *(s32 *) ((u8 *) arg0 + 0x38);
    sp18.field_4 = sp18.field_4 + *(s32 *) ((u8 *) arg0 + 0x3C);
    sp18.field_8 = sp18.field_8 + *(s32 *) ((u8 *) arg0 + 0x40);
    ovl_11_func_800D05D0(arg0, sp18);
    selector = *(u16 *) (base + 0x5200);
    *(u16 *) ((u8 *) arg0 + 0x22) = selector;
    return 0;
}

s32 Ovl11TaggedRecordHandlerNoop(void) {
    return 0;
}

typedef struct {
    char pad_00[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    /* 0x30 */ s16 unk30;
    char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    char pad_38[0x7A - 0x38];
    /* 0x7A */ u16 unk7A;
} Ovl11E180CObj;

s32 Ovl11StepTaggedRecordHandler0(Ovl11E180CObj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler0Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

typedef struct {
    char pad_00[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    /* 0x30 */ s16 unk30;
    char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    char pad_38[0x7A - 0x38];
    /* 0x7A */ u16 unk7A;
} Ovl11E18C4Obj;

s32 Ovl11StepTaggedRecordHandler1(Ovl11E18C4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 = arg0->unk34 | 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 = arg0->unk34 & ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler1Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

typedef struct {
    char pad_00[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    /* 0x30 */ s16 unk30;
    char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    char pad_38[0x7A - 0x38];
    /* 0x7A */ u16 unk7A;
} Ovl11E1974Obj;

s32 Ovl11StepTaggedRecordHandler2(Ovl11E1974Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case 4:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler2Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = 4;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepTaggedRecordHandler3(Ovl11D04D4Obj *arg0) {
    s32 result;

    result = 0;
    switch (arg0->unk28) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk2C = 0;
        arg0->unk34 |= 0x800;
        Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x8031);
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x4000;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk2C != 0) {
            arg0->unk2C--;
        }
        if (Ovl11StepPackedKeyframes(arg0, gOvl11TaggedRecordHandler3Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2C) != 0) {
            arg0->unk28 = -1;
        } else {
            Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x8031);
        }
        break;
    }
    return result;
}

s32 Ovl11StepTaggedRecordHandler4(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler4Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepTaggedRecordHandler5(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler5Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

typedef struct {
    char pad_00[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    /* 0x30 */ s16 unk30;
    char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    char pad_38[0x7A - 0x38];
    /* 0x7A */ u16 unk7A;
} Ovl11E1CA8Obj;

s32 Ovl11StepTaggedRecordHandler14(Ovl11E1CA8Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 = arg0->unk34 & ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11TaggedRecordHandler14Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11SubmitTaggedRecordPose(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1);

s32 Ovl11SubmitDeferredTaggedRecordPose(Recon_ovl_11_func_800E1D48_A0View *arg0) {
    arg0->unk34 = arg0->unk34 | 0x800;
    if (((arg0->unk34 | 0x800) & 0x2000) == 0) {
        arg0->unk34 = (arg0->unk34 | 0x800) & -0x801;
        arg0->unk2C = 0x12C;
        return 1;
    } else {
        arg0->unk2E = 0;
        Ovl11SubmitTaggedRecordPose(((s32)arg0), ((s32)arg0));
        return 0;
    }
}

s32 Ovl11SubmitTaggedRecordPose(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1) {
    s32 temp_s1;
    s32 var_a2;
    u16 state;

    temp_s1 = (*(s16 *) ((u8 *) arg0 + 0x26)) == 0xD;
    state = *(u16 *)((u8 *)arg0 + 0xB2);
    var_a2 = 0x8031;
    if (temp_s1 != 0) {
        var_a2 = 0x8030;
        goto block_4;
    }
    if ((((struct Ovl11HandlerState *)D_8006C838)->field_52C6 == 5) && (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 == 0)) {
block_4:
        if (state == 0) {
            arg0->unk34 |= 0x04000000;
        }
    } else {
        arg0->unk34 &= 0xFBFFFFFF;
    }
    Ovl11StepRecordMovement(arg0, 6, var_a2);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        if (temp_s1 != 0) {
            arg0->unk34 &= ~0x2000;
        }
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}
