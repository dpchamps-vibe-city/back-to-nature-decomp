/*
 * Lifecycle, per-call counters, mode admission/service and sprite selection
 * for the seven F8-byte object records. The local helper graph and private
 * initialization/animation tables join this whole original-order run.
 * object_mode_entry_guards supplies admission callbacks; mode handlers and
 * area_record_claims are separate service interfaces.
 *
 * Keep the existing per-site partial views: timer/area/value fields are read
 * with different signedness at different operations. The public contract is
 * ovl11_object_record_state.h; grouping evidence is in file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_record_state.h"
#include "psyq/memory.h"
#include "random.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_record_position_tables.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_area_record_claims.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_item_sprite_source.h"
#include "ovl11_direction_vector.h"

/* Byte-aligned bank copies retain the original unaligned load/store ABI. */
typedef struct __attribute__((packed)) {
    s32 word0;
    s32 word4;
    s32 word8;
    s32 wordC;
    s8 byte10;
    s8 byte11;
} ObjectInitialBytes;

typedef struct __attribute__((packed)) {
    s32 word;
} ObjectInitialLocation;

typedef struct {
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ ObjectInitialBytes initial;
    /* 0x16 */ char pad1[0xAC - 0x16];
    /* 0xAC */ u16 recordIndex;
    /* 0xAE */ char pad2[0xC4 - 0xAE];
    /* 0xC4 */ ObjectInitialLocation location;
    /* 0xC8 */ char pad3[0xF8 - 0xC8];
} ObjectInitView;

s32 Ovl11ResetObjectRecord(s32 arg0);

void Ovl11InitObjectRecordPool(void) {
    ObjectInitView *record;
    s16 *workIndex;
    u8 *pool;
    u8 *work;
    char *initial;
    s32 recordIndex;
    s32 indexValue;
    s32 bankOffset;
    s32 bankAddress;

    pool = (u8 *)gObjectRecordPool;
    memset(pool, 0, 0x6C8);
    record = (ObjectInitView *)pool;
    for (recordIndex = 0; recordIndex < 7; recordIndex++, record++) {
        Ovl11ResetObjectRecord((s32)record);
        bankOffset = gTextBankOffsets[1];
        bankAddress = (s32)D_8005175C;
        initial = (char *)(bankOffset + gOvl11ObjectInitialRecordOffsets[recordIndex]) + bankAddress;
        record->initial = *(ObjectInitialBytes *)initial;
        record->recordIndex = recordIndex;
        Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)record);
        record->location = *(ObjectInitialLocation *)&gOvl11ObjectInitialAreaRoomPairs[recordIndex];
    }
    recordIndex = 4;
    work = (u8 *)D_8006C838;
    workIndex = (s16 *)(work + 0x99E2);
    indexValue = 0x24;
    for (; recordIndex >= 0; recordIndex -= 1) {
        *workIndex = indexValue;
        workIndex -= 1;
        indexValue -= 6;
    }
}

void Ovl11ResolveObjectRecordMessage(Ovl11CF58Object *arg0);
s32 Ovl11RunObjectRecordMode(Recon_ovl_11_func_8010D250_A0View *arg0);
void Ovl11AccumulateObjectRecordDirection(s32 arg0);
s16 Ovl11RefreshObjectRecordSprite(M2C_90bbdd1382f9_Struct_8010D0EC *arg0);
void Ovl11DrawObjectRecordSprite(Ovl11ItemSpriteObject *arg0);

void Ovl11UpdateObjectRecordPool(void) {
    s32 recordIndex;
    u8 *record;
    struct Ovl11C7F0NearState *nearState;
    struct Ovl11UpdateWork *farState;
    s32 farOffset;
    struct Ovl11C7F0Flags *flags;

    if (!(((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x100)) {
        record = gObjectRecordPool;
        for (recordIndex = 0; recordIndex < 7; recordIndex++) {
            farState = (struct Ovl11UpdateWork *)&D_8007AFF0;
            farOffset = 0x20000;
            flags = (struct Ovl11C7F0Flags *)D_8006C838;
            nearState = (struct Ovl11C7F0NearState *)D_8009AFF0;
            if (!((*(u16 *) ((u8 *) record + 0xB8)) & 0x20)) {
                if (((struct Ovl11C7F0NearState *)((u8 *)farState + farOffset))->area != 0x28) {
                    (*(s16 *) ((u8 *) record + 0xBC)) = 1;
                }
                Ovl11ResolveObjectRecordMessage((Ovl11CF58Object *) record);
                if (!(flags->flagsC & 0x08000000)) {
                    if (!(flags->flagsC & 0x40000000)) {
                        if (((struct Ovl11C7F0NearState *)((u8 *)farState + farOffset))->area == (*(s16 *) ((u8 *) record + 0x30))) {
                            Ovl11RunObjectRecordMode((Recon_ovl_11_func_8010D250_A0View *) record);
                            Ovl11AccumulateObjectRecordDirection((s32) record);
                            goto service_sprite;
                        }
                    } else {
                        Ovl11RunObjectRecordMode((Recon_ovl_11_func_8010D250_A0View *) record);
                        Ovl11AccumulateObjectRecordDirection((s32) record);
                        goto service_sprite;
                    }
                } else {
service_sprite:
                    if (nearState->area == (*(s16 *) ((u8 *) record + 0x30))) {
                        if (!(flags->flagsC & 0x08000000)) {
                            Ovl11TickTimedSpriteEffect((Ov11Timer *) &record[0xA8]);
                        }
                        Ovl11RefreshObjectRecordSprite((M2C_90bbdd1382f9_Struct_8010D0EC *) record);
                        Ovl11DrawObjectRecordSprite((Ovl11ItemSpriteObject *) record);
                    }
                }
            }
            record += 0xF8;
        }
    }
}

s32 Ovl11TryDispatchObjectRecordAreaAction (u8 *arg0);

void Ovl11DispatchObjectPoolAreaActions(void) {
    s32 lastIndex;
    u8 *record;

    record = gObjectRecordPool;
    lastIndex = 6;
    do {
        if (!((*(u16 *) ((u8 *) record + 0xB8)) & 0x20)) {
            Ovl11TryDispatchObjectRecordAreaAction(record);
        }
        lastIndex -= 1;
        record += 0xF8;
    } while (lastIndex >= 0);
}

s32 Ovl11AdvanceObjectRecordCounters(Ovl11ObjectCB6C *arg0);

void Ovl11AdvanceObjectPoolCounters(void) {
    s32 recordIndex;
    u8 *record;

    record = gObjectRecordPool;
    recordIndex = 0;
    do {
        if (!(((Ovl11ObjectCB6C *)record)->unkB8 & 0x20)) {
            Ovl11AdvanceObjectRecordCounters((Ovl11ObjectCB6C *) record);
        }
        recordIndex += 1;
        record += 0xF8;
    } while (recordIndex < 7);
}

s32 Ovl11LookupObjectRecordAnimation(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void Ovl11DrawObjectRecordSprite(Ovl11ItemSpriteObject *arg0) {
    u16 sp10;
    u16 sp12;
    u16 temp_v1;

    Ovl11LookupObjectRecordAnimation(*(u16 *) ((u8 *) arg0 + 0x24), *(u16 *) ((u8 *) arg0 + 0x22), &sp10, &sp12);
    if (sp10 != (*(u8 *) ((u8 *) arg0 + 0x7C))) {
        SelectSpriteAnimation((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp10);
    }
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x24);
    if (((temp_v1 == 0) || (temp_v1 == 0xF)) && (sp12 != (*(u8 *) ((u8 *) arg0 + 0x7D)))) {
        SelectSpriteAnimationFrame((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp12);
    }
    Ovl11InstallItemSpriteSource(arg0);
    Ovl11DrawObjectWithExtraSprite((u8 *) arg0);
}

void Ovl11AccumulateObjectRecordDirection(s32 arg0) {
    Ovl11AccumulateDirectionVector(arg0 + 0x38, arg0 + 0x48, 0);
}

s32 Ovl11TryDispatchObjectRecordAreaAction(u8 *arg0) {
    if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_E4C8 - 3) >= 2U) {
        if (Ovl11IsClockAdvanceActive() != 1) {
            return 0;
        }
        if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
            return 0;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB8)) & 0x40) {
            return 0;
        }
        Ovl11DispatchAreaRecordAction(arg0);
    }
    return 0;
}

s32 Ovl11SetObjectRecordMode(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);

s32 Ovl11AdvanceObjectRecordCounters(Ovl11ObjectCB6C *arg0) {
    s32 increment;
    u16 narrow;
    s32 index;
    s32 x;
    x = arg0->unk34;
    x &= 2;
    if (x) {
        arg0->unk16 += 1;
    }
    if (arg0->unkB8 & 7) {
        arg0->unk16 -= 2;
    }
    if (arg0->unkB8 & 1) {
        increment = arg0->unkAE;
        increment += 1;
        narrow = increment;
        arg0->unkAE = increment;
        index = increment;
        if (narrow > 255) {
            index = 255;
        }
        arg0->unkAE = index;
    }
    if (arg0->unkB8 & 2) {
        arg0->unkB0 += 1;
        arg0->unkB0 = arg0->unkB0 > 255 ? 255 : arg0->unkB0;
    }
    if (arg0->unkB8 & 4) {
        arg0->unkB2 += 1;
        arg0->unkB2 = arg0->unkB2 > 255 ? 255 : arg0->unkB2;
    }
    arg0->unk16 = arg0->unk16 < 0 ? 0 : arg0->unk16;
    arg0->unk16 = arg0->unk16 > 255 ? 255 : arg0->unk16;
    Ovl11ClearRecordItemSprite((u8 *)arg0);
    if (arg0->unkB4 != 0) {
        arg0->unkB4 -= 1;
    }
    if (arg0->unkB4 != 0) {
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)arg0, 11);
        x = -3300;
        index = arg0->unkAC;
        increment = 1;
        arg0->unk30 = increment;
        arg0->unk22 = 0;
        arg0->unk38 = x;
        arg0->unk3C = 0;
        increment = 5800;
        increment -= index * 200;
        arg0->unk40 = increment;
    } else {
        arg0->unkB8 &= 0xFFF8;
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)arg0, 0);
        arg0->unkBE = 0;
        Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)arg0);
    }
    arg0->unk2C = Rand(300);
    arg0->unkB8 &= 0xFCA7;
    arg0->unk34 &= ~2;
    arg0->unk34 &= ~0x2000;
    return 0;
}

s32 Ovl11SelectObjectRecordMode(Recon_ovl_11_func_8010D250_A0View *arg0);

s32 Ovl11RunObjectRecordMode(Recon_ovl_11_func_8010D250_A0View *arg0) {
    s32 (*temp_v0)(void *);

    temp_v0 = gOvl11ObjectModeHandlers[(u16) arg0->unk26];
    if (temp_v0 != 0) {
        temp_v0(arg0);
    }
    if (!((*(s32 *) ((u8 *) arg0 + 0x34)) & 0x800) && (arg0->unk2C >= 0x12C)) {
        Ovl11SelectObjectRecordMode(arg0);
        arg0->unk2C = 0;
    }
    if ((arg0->unk30 != 0x28) && ((u32) (D_80070CF8_halfword - 6) >= 0xFU) && (arg0->unk26 != 0xC) && ((*(u16 *) ((u8 *) arg0 + 0xB6)) == 0)) {
        arg0->unkBE = 0;
        (*(u16 *) ((u8 *) arg0 + 0xB8)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xB8)) | 0x40);
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x800);
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 0xC);
    }
    arg0->unk2C = (u16) arg0->unk2C + 1;
    return 0;
}

s32 Ovl11SetObjectRecordMode(Recon_ovl_11_func_8010CE80_A0View *object, s32 mode) {
    s32 admission = 0;
    if (gOvl11ObjectModeEntryGuards[mode] != 0) {
        if (object->unk26 == mode && (object->unk34 & 0x800)) {
            admission = -1;
        } else {
            admission = gOvl11ObjectModeEntryGuards[mode](object);
        }
    }
    if (gOvl11ObjectModeHandlers[mode] != 0 && admission != -1) {
        object->unk26 = mode;
        object->unk28 = 0;
        object->unk2A = 0;
        object->unk2C = 0;
        object->unkB8 &= 0xF7FF;
    }
    return admission;
}

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

void Ovl11ResolveObjectRecordMessage(Ovl11CF58Object *arg0) {
    if (arg0->unkB8 & 0x800) {
        if (GetMessageWindowStatus() == 2) {
            arg0->unkB8 &= ~0x800;
            ovl_11_func_800E6EE0(Ovl11Item_80071A8A.id, 1, 0, 1);
            if (GetGameFlag(2) == 0 && arg0->unk30 == 0x28 && arg0->unk26 != 6) {
                Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)arg0);
            }
        }
    } else if (arg0->unk30 == 0x28 && GetMessageWindowStatus() == 2 && GetGameFlag(2) == 0 && arg0->unk26 != 6) {
        Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)arg0);
    }
}

typedef struct __attribute__((packed)) {
    s32 first;
    s32 second;
} PackedWorkPair;

s32 Ovl11ResetObjectRecord(s32 objectAddress) {
    char *work;
    s32 timer;

    memset((void *)objectAddress, 0, 0xF8);
    work = (char *)&D_8006C838;
    *(PackedWorkPair *)(objectAddress + 0x1A) = *(PackedWorkPair *)(work + 0x44B8);
    *(s16 *)(objectAddress + 0x30) = 1;
    *(u16 *)(objectAddress + 0x4) = 0xFFFF;
    *(u16 *)(objectAddress + 0xBA) = 0xFFFF;
    *(s16 *)(objectAddress + 0xBE) = 0;
    *(s32 *)(objectAddress + 0x8C) = 0;
    *(s16 *)(objectAddress + 0x16) = 0x32;
    *(s32 *)(objectAddress + 0x34) |= 0x3C000000;
    Ovl11ResetTimedSpriteEffect((s16 *)(objectAddress + 0xA8));
    timer = Rand(0x12C);
    *(s16 *)(objectAddress + 0x2C) = timer;
    return 0;
}

/* Area 28 hex uses the work header; areas 1/3/4/5/6 use bank-offset entry
 * 10; other areas retain the current header. A changed header is rebound
 * only for an object in the current area, using its palette-Y offset.
 * Always stamp kind 176 hex. The variable bank index and separate work-base
 * reads preserve the legacy shared-constant and address webs. */
s16 Ovl11RefreshObjectRecordSprite(M2C_90bbdd1382f9_Struct_8010D0EC *arg0) {
    SpriteDataHeader *header;
    s32 index;
    s32 offset;
    char *far_base;
    char *far_base2;
    char *base;

    index = 10;
    offset = D_80124FCC_spriteOffsets[index];
    far_base = (char *) &D_8007AFF0;
    switch (*(s16 *) (far_base + 0x25476)) {
    case 0x28:
        base = (char *) &D_8006C838;
        base += 0x8000;
        header = *(SpriteDataHeader **) (base + 0x4BD8);
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
        header = (SpriteDataHeader *) (offset + (s32) D_8008F7F8_words);
        break;
    default:
        header = (SpriteDataHeader *) arg0->sprite.field_14;
        break;
    }
    if (header != (SpriteDataHeader *) arg0->sprite.field_14) {
        far_base2 = (char *) &D_8007AFF0;
        if (*(s16 *) (far_base2 + 0x25476) == arg0->unk30) {
            InitSpriteSourceData(&arg0->sprite, header);
            SetSpriteSourceTextureOffsets((Struct_800154CC *) &arg0->sprite, 0, 0, 0, gOvl11ObjectPaletteYOffsets[arg0->unkAC]);
        }
    }
    arg0->field_0 = 0x176;
    return 0;
}

/* Both indices are unchecked. Only the frame output is optional. */
s32 Ovl11LookupObjectRecordAnimation(u16 state, u16 direction, u16 *animation, u16 *frame) {
    *animation = gOvl11ObjectAnimationPairs[state][direction].animation;
    if (frame != 0) {
        *frame = gOvl11ObjectAnimationPairs[state][direction].frame;
    }
    return 0;
}

s32 Ovl11SelectObjectRecordMode(Recon_ovl_11_func_8010D250_A0View *object) {
    s32 nextMode = 0;
    s32 choice = Rand(4);

    if (object->unk30 != 0x28) {
        if (object->unk26 == 0xF) {
            nextMode = 1;
        } else {
            nextMode = gOvl11ObjectRandomModes[choice];
        }
    }

    /* A staged mode/phase triple bypasses admission instead of restarting it. */
    if (object->unkBE != 0 && object->unk26 != 0xF) {
        object->unk26 = object->unkBE;
        object->unk28 = object->unkC0;
        object->unk2A = object->unkC2;
    } else if (nextMode != object->unk26) {
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)object, nextMode);
    }

    return 0;
}
