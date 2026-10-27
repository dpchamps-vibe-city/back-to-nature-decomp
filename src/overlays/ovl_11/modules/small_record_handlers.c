/* Small tagged-record lifecycle, original ovl_11:800E1EC0-800E351C.
 * Codes 0x109/0x10A share 0xB4-byte records, a two-pool factory, sprite
 * descriptors and complementary twenty-slot enter/step banks. Handler names
 * use table slot ids, not guessed gameplay roles. Some slots share callbacks.
 *
 * +26 selects the handler; +28/+2A hold sequence state/frame; +2C is elapsed
 * updates. Flag 0x800 locks selection until a sequence releases it. Public
 * contracts: ovl11_small_record_handlers.h. This module borrows the common
 * record pools/work area; neither external storage nor linkage is changed. */

#include "common.h"
#include "game_types.h"
#include "ovl11_small_record_handlers.h"
#include "random.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_direction_vector.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_spawn_record_access.h"
#include "psyq/memory.h"
#include "ovl11_record_effect_threshold.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_selected_record_flag_guard.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"
#include "ovl11_record_position_set.h"

s32 Ovl11GetSmallRecordSpriteDescriptor(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void Ovl11SelectAndDrawSmallRecordSprite(u8 *record) {
    u16 animation;
    u16 frame;
    u16 descriptorRow;

    Ovl11GetSmallRecordSpriteDescriptor(*(u16 *) ((u8 *) record + 0x24), *(u16 *) ((u8 *) record + 0x22), &animation, &frame);
    if (animation != (*(u8 *) ((u8 *) record + 0x7C))) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (u8) animation);
    }
    if ((frame != 0) && (frame != (*(u8 *) ((u8 *) record + 0x7D)))) {
        SelectSpriteAnimationFrame((ObjectState *) (record + 0x78), (u8) frame);
    }
    Ovl11DrawObjectWithMarker(record);
    descriptorRow = *(u16 *) ((u8 *) record + 0x24);
    if ((descriptorRow == 0) || (descriptorRow == 5)) {
        (*(u16 *) ((u8 *) record + 0x7A)) = (u16) ((*(u16 *) ((u8 *) record + 0x7A)) | 0x100);
    }
}

void Ovl11ApplySmallRecordDisplacement(s32 recordAddress) {
    Ovl11AccumulateDirectionVector(recordAddress + 0x38, recordAddress + 0x48, 0);
}

s32 Ovl11GetSmallRecordIndex(void *arg0);
s32 Ovl11InitSmallRecord(s16 *arg0, s32 arg1);

s16 *Ovl11AllocateSmallRecord(s32 code, s32 index, s32 mainPool) {
    u16 *tagCursor;
    u16 *auxiliaryRecord;
    u16 *result;
    u16 *nextRecord;
    s32 *record;
    s32 slot;

    slot = 0;
    if ((u32) (code - 0x109) >= 2U) {
        goto ret;
    }
    if (mainPool == 1) {
        if (index != -1) {
            record = (s32 *) ((u8 *) &gSmallTaggedRecordBase + (index * 0xB4));
        } else {
            record = Ovl11FindFreeB4Record();
        }
        if (record == 0) {
            return 0;
        }
        slot = Ovl11GetSmallRecordIndex(record);
        Ovl11InitSmallRecord((s16 *) record, code);
        Ovl11ApplyRecordRowPlacement((s32) record, (s16) slot, 0);
        return (s16 *) record;
    }
    auxiliaryRecord = &gSmallRecordTripletBase;
    if (gSmallRecordTripletBase != 0) {
        tagCursor = &gSmallRecordTripletBase;
        nextRecord = &gSmallRecordTripletBase;
next_slot:
        tagCursor += 0x5A;
        slot += 1;
        nextRecord += 0x5A;
        if (slot < 3) {
            auxiliaryRecord = nextRecord;
            if (*tagCursor != 0) {
                goto next_slot;
            }
        }
    }
    if (slot == 3) {
ret:
        return 0;
    }
    Ovl11InitSmallRecord((s16 *) auxiliaryRecord, code);
    result = auxiliaryRecord;
    return (s16 *) result;
}

s32 Ovl11CountSmallRecords(void) {
    s32 count = 0;
    s8 *p = (s8 *)&gSmallTaggedRecordBase;
    s32 n = 9;

    do {
        if (*(u16 *)p != 0u) {
            count++;
        }
        p += 0xB4;
        n--;
    } while (n >= 0);
    return count;
}

s32 Ovl11UpdateSmallRecordKind1(u16 *arg0) {
    s16 temp_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    u16 temp_v1;
    s32 var_v1;

    temp_v1 = *arg0;
    if (temp_v1 == 0) {
        return -1;
    }
    if (temp_v1 != 0x109) {
        return 0;
    }
    temp_a1 = *(s16 *) ((u8 *) arg0 + 0x30);
    if ((temp_a1 == 1) && (((Ovl11E20F0StateView *)D_8006C838)->field_44BA != 3)) {
        if (((Ovl11E20F0StateView *)D_8006C838)->field_E4C8 != temp_a1) {
            temp_v0 = (*(u16 *) ((u8 *) arg0 + 0xAC)) + 1;
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = temp_v0;
            if ((u32) (temp_v0 & 0xFFFF) >= 5U) {
                (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) | 1);
            }
        } else {
            s32 checkedEnergy;
            temp_v0_2 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 5;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_2;
            checkedEnergy = (s16)temp_v0_2;
            var_v1 = temp_v0_2;
            if (checkedEnergy < 0) {
                var_v1 = 0;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1;
        }
    }
    if ((u32) (((Ovl11E20F0StateView *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
        (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0x12C;
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x800);
    }
    return 0;
}

s32 Ovl11RefreshSmallRecordSprite(s16 *arg0);
void Ovl11ApplySmallRecordRowPlacements(s32 arg0);
s32 Ovl11RandomizeSmallRecordPosition(E21E4Position *src, E21E4Position *dst);

s32 Ovl11UpdateSmallRecordState(E21E4Object *arg0) {
    s32 index;
    s32 i;
    s32 energy;
    s32 temp;
    s32 out;
    s32 selection;
    E21E4Position position;

    if (arg0->type == 0 && !(arg0->flags & 0x2000000)) {
        return -1;
    }
    if (arg0->type != 0x109) {
        if (arg0->type == 0x10A && (arg0->flags & 0x2000000)) {
            return 0;
        }
        arg0->timer++;
        Ovl11RefreshSmallRecordSprite((s16 *)arg0);
        Ovl11ApplySmallRecordRowPlacements((s32)arg0);
        return 0;
    }
    index = Ovl11GetSmallRecordIndex(arg0);
    if (arg0->kind == 4 && !(arg0->flags & 1)) {
        for (i = 0; i < 10; i++) {
            if (((E21E4State *)D_8006C838)->pending[i] != 0) {
                arg0->flags |= 1;
                ((E21E4State *)D_8006C838)->pending[i] = 0;
                break;
            }
        }
    }
    Ovl11ApplySmallRecordRowPlacements((s32)arg0);
    if (arg0->flags & 0x10) {
        if ((s16)arg0->energy < 100) {
            selection = 0x122;
        } else if ((s16)arg0->energy < 200) {
            selection = 0x123;
        } else if (arg0->flags & 0x40) {
            selection = 0xE8;
        } else {
            selection = 0x124;
        }
        if (arg0->kind == 4) {
            Ovl11RandomizeSmallRecordPosition(&arg0->position, &position);
        } else {
            position = arg0->position;
        }
        /* The object's four-word position has the SDK VECTOR ABI. */
        Ovl11SetSpawnRecord(selection, *(VECTOR *)&position, arg0->kind, index);
    }
    if (arg0->flags & 1) {
        s32 checkedEnergy;
        energy = arg0->energy + 10;
        arg0->energy = energy;
        checkedEnergy = (s16)energy;
        temp = energy;
        if (checkedEnergy >= 256) {
            temp = 255;
        }
        arg0->energy = temp;
        arg0->flags |= 0x10;
    } else {
        s32 checkedEnergy;
        energy = arg0->energy - 20;
        arg0->energy = energy;
        checkedEnergy = (s16)energy;
        temp = energy;
        if (checkedEnergy < 0) {
            temp = 0;
        }
        arg0->energy = temp;
        arg0->flags &= ~0x10;
    }
    if (!(arg0->flags & 0x100)) {
        temp = arg0->counter;
        if (temp != 0) {
            temp++;
            arg0->counter = temp;
            if ((u16)temp >= 32) {
                temp = 31;
            }
            arg0->counter = temp;
            if (arg0->counter >= 3 && !(arg0->flags & 0x40000) &&
                (s32)Rand(100) <= arg0->counter * 10) {
                arg0->flags |= 0x100;
            }
        } else if ((s16)arg0->energy <= 0 && Rand(100) < 11) {
            arg0->counter++;
        }
    }
    if (!(arg0->flags & 0x100)) {
        if (!(arg0->flags & 0x40000)) {
            if (arg0->age < 3) {
                selection = 0;
            } else if (arg0->age == 3) {
                selection = 25;
            } else if (arg0->age == 4) {
                selection = 75;
            } else {
                selection = 100;
            }
            if (selection != 0 && Rand(100) <= (u32)selection) {
                arg0->flags |= 0x80100;
            }
        }
    }
    temp = arg0->timer;
    temp++;
    arg0->timer = temp;
    out = temp;
    if ((u16)out >= 32) {
        out = 31;
    }
    arg0->timer = out;
    Ovl11RefreshSmallRecordSprite((s16 *)arg0);
    if (arg0->day == ((E21E4State *)D_8006C838)->day &&
        arg0->mode == ((E21E4State *)D_8006C838)->mode) {
        arg0->age++;
    }
    if (!(arg0->flags & 1)) {
        ((E21E4State *)D_8006C838)->flags |= 0x8000000;
    }
    Ovl11DeactivateUnselectedRecord((s32 *)arg0);
    arg0->fieldAC = 0;
    arg0->flags &= ~1;
    arg0->flags &= ~0x2000;
    Ovl11ResetTimedSpriteEffect(arg0->reset);
    return 0;
}

/* The caller's four-word position view is canonical. Only x/y/z are read
 * or written here; the fourth word is left untouched. */
s32 Ovl11RandomizeSmallRecordPosition(E21E4Position *src, E21E4Position *dst) {
    s32 result;
    s32 lo;
    s32 hi;
    s32 range;

    result = 0;
    dst->y = src->y;

    if (src->x < -1000 || src->x > 1200) {
        dst->x = Rand(0x898) - 1000;
        result = 1;
    } else {
        dst->x = src->x;
    }

    if (src->z >= 0) {
        hi = 1700;
        lo = 400;
        range = 1300;
    } else {
        hi = -500;
        lo = -2000;
        range = 1500;
    }
    if (hi < src->z || src->z < lo) {
        dst->z = lo + Rand(range);
        result = 1;
    } else {
        dst->z = src->z;
    }
    return result;
}

s32 Ovl11GetSmallRecordCode(void *arg0) {
    u16 v;

    if (*(u16 *)arg0 == 0 && (*(u32 *)((char *)arg0 + 0x34) & 0x02000000) == 0) {
        return -1;
    }
    v = *(u16 *)((char *)arg0 + 0xB0);
    if (v < 3) {
        return 0x86;
    }
    if (v < 0xA) {
        return 0x10A;
    }
    return 0x109;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x24];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ char pad_28[0xC];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x76];
    /* 0xAE */ u16 unkAE;
} Struct_800E2AF0;

s32 Ovl11SelectSmallRecordHandler(Struct_800E2AF0 *arg0);

s32 Ovl11StepSmallRecordHandler(s16 *arg0) {
    s32 temp_v0;

    if (*(u16 *)arg0 == 0) {
        return -1;
    }
    if (*(s32 *)((u8 *)arg0 + 0x8C) == 0) {
        Ovl11RefreshSmallRecordSprite(arg0);
    }
    temp_v0 = gOvl11SmallRecordStepHandlers[*(u16 *)((u8 *)arg0 + 0x26)];
    if (temp_v0 != 0) {
        ((s32 (*)(s16 *))temp_v0)(arg0);
    }
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x800) &&
        (*(s16 *)((u8 *)arg0 + 0x2C) >= 0x12C)) {
        Ovl11SelectSmallRecordHandler((Struct_800E2AF0 *)arg0);
        *(s16 *)((u8 *)arg0 + 0x2C) = 0;
    }
    *(s16 *)((u8 *)arg0 + 0x2C) = *(u16 *)((u8 *)arg0 + 0x2C) + 1;
    return 0;
}

s32 Ovl11DispatchSmallRecordHandler(u16 *record, s32 handlerId) {
    s32 result;

    result = 0;
    if (gOvl11SmallRecordEnterHandlers[handlerId] != 0) {
        if ((*((s16 *) ((u8 *) record + 0x26)) == handlerId) && (*((s32 *) ((u8 *) record + 0x34)) & 0x800)) {
            result = -1;
        } else {
            result = gOvl11SmallRecordEnterHandlers[handlerId](record);
        }
    }
    if ((gOvl11SmallRecordStepHandlers[handlerId] != 0) && (result != -1)) {
        *((s16 *) ((u8 *) record + 0x26)) = handlerId;
        *((s16 *) ((u8 *) record + 0x28)) = 0;
        *((s16 *) ((u8 *) record + 0x2A)) = 0;
        *((s16 *) ((u8 *) record + 0x2C)) = 0;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
    }
    return result;
}

void Ovl11ClearSmallRecord(s32 recordAddress) {
    memset(recordAddress, 0, 0xB4);
    *(s16 *)recordAddress = 0;
}

s32 Ovl11GetSmallRecordIndex(void *record) {
    s32 i;
    s8 *p;

    i = 0;
    p = (s8 *)&gSmallTaggedRecordBase;
    for (; i < 10; i++, p += 0xB4) {
        if (record == (void *)p) {
            return i;
        }
    }
    return -1;
}

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk1;
} Ovl11E2968UnalignedWord;

s32 Ovl11InitSmallRecord(s16 *record, s32 code) {
    char *base;

    if (*(u16 *) record != 0) {
        return -1;
    }
    memset(record, 0, 0xB4);
    if (code != 0x109) {
        if (code == 0x10A) {
            *(s16 *) ((u8 *) record + 0xB0) = 3;
        }
    } else {
        *(s16 *) ((u8 *) record + 0xB0) = 0xA;
    }
    *(s16 *) record = code;
    *(s16 *) ((u8 *) record + 0x16) = 0xA;
    base = (char *) &D_8006C838;
    /* Two packed words copy the unaligned date/state span before age resets. */
    *(Ovl11E2968UnalignedWord *) ((u8 *) record + 0x1A) =
        *(Ovl11E2968UnalignedWord *) (base + 0x44B8);
    *(s16 *) ((u8 *) record + 0x1A) = 0;
    *(u16 *) ((u8 *) record + 0x4) = 0xFFFF;
    Ovl11RefreshSmallRecordSprite(record);
    Ovl11ResetTimedSpriteEffect(record + 0x54);
    return 0;
}

s32 Ovl11GetSmallRecordCode(void *arg0);

s32 Ovl11RefreshSmallRecordSprite(s16 *record) {
    s32 code;
    s32 index;
    s32 result;

    code = Ovl11GetSmallRecordCode(record);
    index = 1;
    if (code != 0x109) {
        if (code != 0x10A) {
            return -1;
        }
        *record = code;
    } else {
        *record = code;
        index = 2;
    }
    InitSpriteSourceData((SpriteSourceData *)((char *)record + 0x78),
                  (SpriteDataHeader *)(gOvl11FixedIdSpriteOffsets[index] + (s32)&gFixedIdSpriteBankBase));
    result = 0;
    return result;
}

s32 Ovl11GetSmallRecordSpriteDescriptor(u16 row, u16 direction, u16 *animationOut, u16 *frameOut) {
    *animationOut = gOvl11SmallRecordSpriteDescriptors[row][direction].animation;
    if (frameOut != 0) {
        *frameOut = gOvl11SmallRecordSpriteDescriptors[row][direction].frame;
    }
    return 0;
}

s32 Ovl11DispatchSmallRecordHandler(u16 *arg0, s32 arg1);

s32 Ovl11SelectSmallRecordHandler(Struct_800E2AF0 *arg0) {
    s32 var_a1;
    s32 var_a0;
    s32 temp_v0;

    if ((arg0->unkAE != 0) && (arg0->unk0 == 0x109)) {
        var_a1 = 0x13;
    } else if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        var_a1 = 0xF;
    } else {
        if (arg0->unk26 == 0xF) {
            var_a1 = 0x10;
        } else {
            var_a0 = 4;
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
        Ovl11DispatchSmallRecordHandler((u16 *)arg0, var_a1);
    }
    return 0;
}

void Ovl11ApplySmallRecordRowPlacements(s32 arg0) {
    s32 i;
    s16 v;

    for (i = 0; i < 10; i++) {
        v = *(s16 *)(arg0 + 0x30);
        if (v != 4 && v != 1) {
            Ovl11ApplyRecordRowPlacement(arg0, (s16)i, 0);
        }
    }
}

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x16 - 0x02];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[0x30 - 0x18];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ char pad_38[0x58 - 0x38];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ char pad_68[0xA8 - 0x68];
    /* 0xA8 */ char unkA8[0];
} Struct_800E2C64;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

s32 Ovl11StartSmallRecordHandler3(Struct_800E2C64 *arg0) {
    s32 temp;
    s32 out;
    char *far_base;
    char *base;

    if (arg0->unk0 == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x109) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        PlaySoundEffect(0x18);
    }
    temp = arg0->unk16 - 10;
    base = (char *)&D_8006C838;
    arg0->unk58 = *(s32 *)(base + 0x52C8);
    arg0->unk5C = *(s32 *)(base + 0x52CC);
    arg0->unk60 = *(s32 *)(base + 0x52D0);
    arg0->unk64 = *(s32 *)(base + 0x52D4);
    arg0->unk34 = (arg0->unk34 | 0x2000) & ~0x4000;
    arg0->unk16 = temp;
    if ((s16)temp < 0) {
        out = 0;
    } else {
        out = temp;
    }
    arg0->unk16 = out;
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((s32)arg0 + 0xA8), 0x24, 0x2D);
    return 0;
}

s32 Ovl11StartSmallRecordHandler4(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 sp18;
    u16 temp_v1;
    s32 temp_v0;
    s32 var_v1;
    char *far_base;
    u8 *base;

    temp_v1 = *(u16 *) ((u8 *) arg0 + 0);
    if (temp_v1 == 0) {
        return -1;
    }
    if (temp_v1 != 0x109) {
        return -1;
    }
    far_base = (char *) &D_8007AFF0;
    if ((*(s16 *) (far_base + 0x25476)) == (*(s16 *) ((u8 *) arg0 + 0x30))) {
        PlaySoundEffect(0x18);
    }
    temp_v0 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 0xA;
    (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0;
    if ((s16) temp_v0 < 0) {
        var_v1 = 0;
    } else {
        var_v1 = temp_v0;
    }
    (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1;
    base = (u8 *) D_8006C838;
    Ovl11SetDirectionVector(*(u16 *) (base + 0x5200), (Recon800D0408A1View *) &sp18, 0x7D0);
    sp18.vx = sp18.vx + (*(s32 *) ((u8 *) arg0 + 0x38));
    sp18.vy = sp18.vy + (*(s32 *) ((u8 *) arg0 + 0x3C));
    sp18.vz = sp18.vz + (*(s32 *) ((u8 *) arg0 + 0x40));
    Ovl11SetRecordPosition(arg0, sp18);
    (*(u16 *) ((u8 *) arg0 + 0x22)) = (*(u16 *) (base + 0x5200));
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *) ((u8 *) arg0 + 0xA8), 0x24, 0x2D);
    return 0;
}

s32 Ovl11StartSmallRecordHandler9(Ovl11DF72CResetView *record) {
    if (*(u16 *)record == 0) {
        return -1;
    }
    if (*(u16 *)record != 0x109) {
        return -1;
    }
    if (*(u16 *)((u8 *)record + 0xAE) == 0) {
        return -1;
    }
    *(u16 *)((u8 *)record + 0xAE) = 0;
    Ovl11StartRecordEffectAboveThreshold(record);
    return 0;
}

s32 Ovl11StartSmallRecordHandler14(void) {
    return 0;
}

s32 Ovl11StepSmallRecordHandler0(Ovl11D04D4Obj *arg0) {
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
        if (Ovl11StepPackedKeyframes(arg0, gOvl11SmallRecordHandler0Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepSmallRecordHandler1(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;
    u8 *var_a1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk0 == 0x10A) {
            var_a1 = gOvl11SmallRecordHandler1Code10AKeyframes;
        } else {
            var_a1 = gOvl11SmallRecordHandler1Code109Keyframes;
        }
        if (Ovl11StepPackedKeyframes(arg0, var_a1, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

typedef struct {
    /* 0x00 */ u16 unk0;
    char pad_02[0x24 - 0x02];
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
} Ovl11E3020Obj;

s32 Ovl11StepSmallRecordHandler2(Ovl11E3020Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;
    u8 *var_a1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case 4:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    default:
    case 1:
        if (arg0->unk0 == 0x10A) {
            var_a1 = gOvl11SmallRecordHandler2Code10AKeyframes;
        } else {
            var_a1 = gOvl11SmallRecordHandler2Code109Keyframes;
        }
        if (Ovl11StepPackedKeyframes(arg0, var_a1, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = 4;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepSmallRecordHandler3(Ovl11D04D4Obj *arg0) {
    s16 temp_s0;
    s32 result;

    result = 0;
    switch (arg0->unk28) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk2C = 0;
        arg0->unk34 |= 0x800;
        arg0->unk34 &= ~0x2000;
        Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x31);
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x4000;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, gOvl11SmallRecordHandler3Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2C) != 0) {
            arg0->unk28 = -1;
        } else {
            temp_s0 = arg0->unk2A;
            Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x12, 0x31);
            arg0->unk2A = temp_s0;
        }
        break;
    }
    return result;
}

s32 Ovl11StepSmallRecordHandler4(Ovl11D04D4Obj *arg0) {
    s16 temp_s0;
    s16 temp_v1;
    s32 var_s2;

    temp_v1 = arg0->unk28;
    var_s2 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s2 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        arg0->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, gOvl11SmallRecordHandler4Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else {
            temp_s0 = arg0->unk2A;
            Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *) arg0, 0x12, 0x31);
            arg0->unk2A = temp_s0;
        }
        break;
    }
    return var_s2;
}

s32 Ovl11StepSmallRecordHandler14(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;
    u16 temp_a0;
    char *far_base;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        far_base = (char *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
            temp_a0 = arg0->unk0;
            if (temp_a0 == 0x109) {
                PlaySoundEffect(0x17);
            } else if (temp_a0 == 0x10A) {
                PlaySoundEffect(0x1B);
            }
        }
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, gOvl11SmallRecordHandler14Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepSmallRecordHandler18(Recon_ovl_11_func_800D2594_A0View *arg0) {
    Ovl11StepRecordMovement(arg0, 6, 0x31);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}

s32 Ovl11StepSmallRecordHandler19(Ovl11D04D4Obj *arg0) {
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
        if (Ovl11StepPackedKeyframes(arg0, gOvl11SmallRecordHandler19Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}
