/* Shared type definitions for the game */
#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "common.h"

/* ovl_25 800BB628/800BB788: 0x16-byte sprite descriptor; destination
 * coordinates at +4/+6, counters at +8/+A/+C, source rectangle at +E. */
typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    u16 unk14;
} Ovl25SpriteDesc;

/* 800DB2AC's matched parameter: nine halfwords; 800DC560 forwards its header. */
typedef struct {
    u16 unk0;
    s16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
} Ovl11SetupHeader;

/* 800CC7CC passes this halfword request to 800CB540: id +0, count +4. */
typedef struct {
    u16 id;
    u16 pad2;
    s16 count;
    s16 pad6;
} Ovl11CC7CCRequest;

/* 800CC7CC: item +2, state +4/+6/+C, direction +8, height +10,
 * fixed-point velocity +14/+1C and four position words at +24. */
typedef struct {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    u16 unk6;
    u16 direction;
    s16 padA;
    s32 unkC;
    s32 height;
    Ovl11CC7CCPosition velocity;
    Ovl11CC7CCPosition position;
} Ovl11CC7CCObject;

/* 8010E6E8: direction +22, state +28, timer +2C, flags +34,
 * unsigned index +AC and halfword choice/attributes +B4/+B8. */
typedef struct {
    u8 pad0[0x22];
    u16 direction;
    u8 pad24[4];
    s16 state;
    s16 pad2A;
    s16 timer;
    u8 pad2E[6];
    s32 flags;
    u8 pad38[0xAC - 0x38];
    u16 index;
    u8 padAE[6];
    u16 choice;
    u16 padB6;
    u16 attributes;
} Ovl11E6E8Object;

/* 8010E644: sh +24, lw/sw +34, lw +48/+4C/+50, lhu +B6. */
typedef struct {
    char pad0[0x24];
    s16 unk24;
    char pad26[0x34 - 0x26];
    s32 unk34;
    char pad38[0x48 - 0x38];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    char pad54[0xB6 - 0x54];
    u16 unkB6;
} Ovl11E644Object;

/* 80110E34's matched object view, also passed by 8010F4CC: direction +22,
 * map +30, position +38/+3C/+40 and unsigned table index +AC. */
typedef struct {
    char pad[0x22];
    u16 unk22;
    char pad2[0x30 - 0x24];
    u16 unk30;
    char pad3[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    char pad4[0xAC - 0x44];
    u16 unkAC;
} M2C_269c1560d5f5_Unk80110E34;

/* 801110CC reads these fields; 8010CA0C forwards the same object. */
typedef struct {
    char pad_00[0x30];
    s16 unk30;
    char pad_32[0xB6 - 0x32];
    u16 unkB6;
    char pad_B8[0xDC - 0xB8];
    s32 unkDC;
} M2C_ececd3ff98bf_Struct_801110CC;

/* 800DC9D4 returns 0x30-byte entries; 800DCA60 writes flags at +0x28. */
typedef struct {
    u8 pad[0x28];
    u16 unk28;
    u8 pad2[0x6];
} Ovl11FuncC9D4Entry;

/* 800DCF10 returns 0x34-byte entries; 800DCA60 writes +0x2E/+0x30. */
typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u8 pad1[0x0E];
    u16 unk2C;
    u8 pad2[0x6];
} Ovl11FuncDCF10Entry;

/* 800FA1CC: lh at +0/+2/+6; lhu at +4 narrowed before signed modulo. */
typedef struct {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    s16 unk6;
} Ovl11FA1CCArg;

/* 800D0DBC clears both signed halfwords; 800F05C0 passes a selected pair. */
typedef struct {
    s16 unk0;
    s16 unk2;
} UnkStruct800D0DBC;

/* 800DB354/800DB4E0 consume these 0x16-byte slots; 800DB5CC walks eight. */
typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    u16 unk14;
} Ovl11AnimationSlot;

/* 800F8480/800F84D4 copy these three halfwords; 800F7E38 passes both records. */
typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
} Ovl11CopyRecord;

/* 800C6770: lhu state at +3C, lw/sw flags at +6C, six-byte item at +8A. */
typedef struct {
    u8 pad0[0x3C];
    u16 unk3C;
    u8 pad3E[0x6C - 0x3E];
    s32 unk6C;
    u8 pad70[0x8A - 0x70];
    Ovl11ItemState item;
} Ovl11C6770Object;

/* 800E21E4 copies all four words at +38 and passes them by value to F43CC. */
typedef struct {
    s32 x, y, z, w;
} E21E4Position;

/* 8010B05C copies 0xAC bytes, then passes the u16 at +30 and vector at +38. */
typedef struct {
    s32 pad0[0xC];
    u16 selector;
    u16 pad32;
    s32 flags;
    E21E4Position position;
    u8 pad48[0x64];
} Ovl11B05CObject;

/* 800D09E0 writes state +22, flags +34, and the position at +38..+40. */
typedef struct {
    u8 pad_0[0x22];
    s16 state;
    u8 pad_24[0x10];
    s32 flags;
    E21E4Position position;
} Ovl11D09E0Object;

/* 800E21E4: 0xB4-byte entry, lhu tag/counters, lh selectors and lw flags. */
typedef struct {
    u16 type;
    u8 pad2[0x16 - 2];
    u16 energy;
    u8 pad18[2];
    s16 age, mode, day;
    u8 pad20[0x30 - 0x20];
    s16 kind;
    u8 pad32[2];
    s32 flags;
    E21E4Position position;
    u8 pad48[0xA8 - 0x48];
    s16 reset[2];
    u16 fieldAC, counter, timer, fieldB2;
} E21E4Object;

/* CE034 reads +38 and +100/+104/+108; C55FC passes this object onward. */
typedef struct {
    char pad0[0x38];
    u16 unk38;
    char pad3A[0x100 - 0x3A];
    s32 unk100;
    s32 unk104;
    s32 unk108;
} Ovl11CE034Object;

/* CD5BC tests flags at +36 and updates a u16 slot at +40 using +84.
 * C523C also reads the signed +18/+38/+86 fields and +100..+108 position. */
typedef struct {
    char pad0[0x18];
    s16 power;
    char pad1A[0x36 - 0x1A];
    u16 flags;
    s16 direction;
    char pad3A[0x40 - 0x3A];
    u16 vals[(0x84 - 0x40) / 2];
    s16 value;
    s16 amount;
    char pad88[0x100 - 0x88];
    s32 x;
    s32 y;
    s32 z;
} Ovl11CD5BCObject;

/* C4A2C reads signed halfwords at +38/+84 and coordinates at +100..+108. */
typedef struct {
    u8 pad0[0x38];
    s16 unk38;
    u8 pad3A[0x84 - 0x3A];
    s16 unk84;
    u8 pad86[0x100 - 0x86];
    s32 unk100;
    s32 unk104;
    s32 unk108;
} Ovl11C4A2CObject;

/* CE034 writes the three coordinates passed to C55FC's grid callback. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl11CE034Position;

/* 80102F14 scans stride-four IDs and explicitly sign-extends the byte at +2;
 * 801032BC consumes the same table through the pointer at object +1C. */
typedef struct {
    u16 id;
    u8 adjustment;
    u8 pad3;
} Ovl11IdAdjustment;

/* 80102BCC scans 0x20-byte objects: u16 ID at +4, u8 value at +6.
 * 80102E1C scans eight u16 IDs at +0xA; 80102F14 uses +0x1C. */
typedef struct Ovl11IdAdjustmentObject {
    u8 pad0[4];
    u16 id;
    u8 value;
    u8 pad7[3];
    u16 ids[8];
    u8 pad1A[2];
    Ovl11IdAdjustment *adjustments;
} Ovl11IdAdjustmentObject;

/* 800BDF98 forwards this four-byte, halfword-aligned object by value;
 * 800BE6E4 reads its signed type at +2. 800BDB88 forwards the same pair
 * stored by 800B7F3C at work-area +25474. */
typedef Ovl25SetupStatePair Ovl11CollisionTag;

/* 800CEFCC clears flag 0x20000 at +34, copies the tag at +2E,
 * and copies four position words at +38 before calling 800BDB88. */
typedef struct {
    char pad0[0x2E];
    Ovl11CollisionTag collision;
    char pad32[2];
    s32 flags;
    s32 position[4];
} Ovl11CEFCCObject;

/* 800D0FB4 resets the tag state at +30 and position at +38 in
 * ten 0xB4 records and twenty 0xB8 records. */
typedef struct {
    char pad00[0x2E];
    Ovl11CollisionTag collision;
    char pad32[6];
    s32 position[3];
    char pad44[0xB4 - 0x44];
} Ovl11ResetRecordB4;

typedef struct {
    char pad00[0x2E];
    Ovl11CollisionTag collision;
    char pad32[6];
    s32 position[3];
    char pad44[0xB8 - 0x44];
} Ovl11ResetRecordB8;

/* 800BDF98 builds this 0x20-byte query; 800BE6E4 follows +0/+4/+8/+10
 * and reads the word at +18. */
typedef struct {
    s32 *position;
    Ovl11CollisionTag *object;
    void **tile;
    s32 field_C;
    s32 *flags;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
} Ovl11CollisionQuery;

/* ovl_11_func_800BFADC advances signed halfwords at +0/+2 of this by-reference pair. */
typedef struct {
    s16 unk0;
    s16 unk2;
} Ovl11CounterPair;

/* 8010B264 updates the u16 counters at +16/+AC, compares lh at +1A/+1C/+1E
 * and +30, and passes the leading tag/flags view to 8010B830. */
typedef struct {
    u16 field_0;
    u8 pad_2[0x16 - 2];
    u16 field_16;
    u8 pad_18[2];
    s16 field_1A;
    s16 field_1C;
    s16 field_1E;
    u8 pad_20[0x30 - 0x20];
    s16 field_30;
    u8 pad_32[2];
    s32 field_34;
    u8 pad_38[0xA8 - 0x38];
    s16 field_A8[2];
    u16 field_AC;
} Ovl11B264Object;

/* 800DEB00: lhu/sh counters at +16/+2C/+AC..+B2, lh state at
 * +1A/+1C/+1E/+30, lw/sw flags at +34, and the cleared word at +8C. */
typedef struct {
    u8 pad0[0x16];
    u16 field_16;
    u8 pad18[2];
    s16 field_1A;
    s16 field_1C;
    s16 field_1E;
    u8 pad20[0x2C - 0x20];
    u16 field_2C;
    u8 pad2E[2];
    s16 field_30;
    u8 pad32[2];
    s32 field_34;
    u8 pad38[0x8C - 0x38];
    s32 field_8C;
    u8 pad90[0xA8 - 0x90];
    s16 field_A8[2];
    u16 field_AC;
    u16 field_AE;
    u16 field_B0;
    u16 field_B2;
    /* 800E06E0 increments and saturates this lhu/sh counter at 7. */
    u16 field_B4;
} Ovl11Deb00Object;

/* Shared struct for functions that access void* with offset 0x14 and 0x18 */
typedef struct {
    /* 0x00 */ s32 field_0x0;
    /* 0x04 */ s32 field_0x4;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ s32 field_0xC;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ s32 field_0x18;
    /* 0x1C */ s32 field_0x1C;
} SomeStruct;

/* Legacy partial signed-word view of pad input, not a graphics object.
 * The complete layout is PadInputState below; retained here for existing
 * consumers of the held/pressed button words. */
typedef struct {
    /* 0x00 */ char pad_00[0x4];
    /* 0x04 */ s32 field_4;
    /* 0x08 */ s32 field_8;
    /* 0x0C */ char pad_0C[0x18 - 0x0C];
    /* 0x18 */ s32 field_18;
    /* 0x1C */ s32 field_1C;
    /* 0x20 */ char pad_20[0x0C];
    /* 0x2C */ s32 field_2C;
    /* 0x30 */ s32 field_30;
} GfxObj;

/* ovl_11_func_800BE94C and 800F4240 read the id at +2 and mask at +4. */
typedef struct {
    u8 pad_0[2];
    s16 unk2;
    u16 unk4;
} Ovl11ItemCollisionObject;

/* ovl_11_func_80120F30 reads eight UV bytes and a u16 at +8. */
typedef struct Ovl11TextureCoordinates {
    u8 u0, v0;
    u8 u1, v1;
    u8 u2, v2;
    u8 u3, v3;
    u16 clut;
} Ovl11TextureCoordinates;

/* Simple 3-element vector (used for position, rotation, etc.) */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3;

/* Halfword position view read by ovl_11_func_800CE0F0. */
typedef struct {
    /* 0x00 */ u16 field_0;
    /* 0x02 */ u16 pad_2;
    /* 0x04 */ u16 field_4;
    /* 0x06 */ u16 pad_6;
    /* 0x08 */ u16 field_8;
} Ovl11SpritePositionView;

/* Simple 2-word structure for basic pair initialization */
typedef struct {
    s32 field_0;
    s32 field_4;
} PairS32;

/* Struct initialized by func_800154CC (0x18 bytes)
 * Offset 0x00 is accessed as s32; offsets 0x03-0x07 as s8; rest as s16.
 * First 3 bytes are separate so field_3 lands at offset 3.
 */
typedef struct {
    /* 0x00 */ s8  field_0;
    /* 0x01 */ s8  field_1;
    /* 0x02 */ s8  field_2;
    /* 0x03 */ s8  field_3;
    /* 0x04 */ s8  field_4;
    /* 0x05 */ s8  field_5;
    /* 0x06 */ s8  field_6;
    /* 0x07 */ s8  field_7;
    /* 0x08 */ s16 field_8;
    /* 0x0A */ s16 field_A;
    /* 0x0C */ s16 field_C;
    /* 0x0E */ s16 field_E;
    /* 0x10 */ s16 field_10;
    /* 0x12 */ s16 field_12;
    /* 0x14 */ s16 field_14;
    /* 0x16 */ s16 field_16;
} Struct_800154CC;

/* Object state/flags structure accessed by animation/state functions */
typedef struct {
    /* 0x00 */ char pad_00[0x02];
    /* 0x02 */ u16 field_2;     /* flags/status word */
    /* 0x04 */ s8 field_4;      /* state identifier */
    /* 0x05 */ s8 field_5;      /* sub-state or animation index */
    /* 0x06 */ s16 field_6;     /* timer or counter */
} ObjectState;

/* 8011FFA8: byte +38, u16 +3C, word +6C, s16 +84 and animation +260. */
typedef struct {
    u8 pad0[0x38];
    u8 direction;
    u8 pad39[3];
    u16 phase;
    u8 pad3E[0x6C - 0x3E];
    s32 flags;
    u8 pad70[0x84 - 0x70];
    s16 id;
    u8 pad86[0x260 - 0x86];
    ObjectState animation;
} Ovl11SelectionObject;

/* 80120358 reads +8 and writes +0; 8011FFA8 also maintains +4. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl11SelectArg;

/* Per-pad input/auto-repeat state (0x38 bytes). Boot binds gPad0Input and
 * gPad1Input to gPadInputStateStorage and +0x38. UpdatePadInputRecord processes
 * independent repeat outputs at +0x00 and +0x24 using the two timing pairs;
 * ClearPadInputTransientState preserves timings and actuator requests.
 * Timing fields are signed words; their units/constraints are documented
 * in pad_input_setup.h. Button masks are active-high; repeat outputs include
 * new presses as well as subsequent repeats. Counters and active flags are
 * independent for the two streams. stickMagnitude is -1 when unavailable,
 * otherwise the decoder's dead-zone/clamped 0..90 value (not an angle).
 * The final bytes feed the actuator formatter without being cleared by
 * transient input reset. All original widths and signedness are retained. */
typedef struct PadInputState {
    /* 0x00 */ s32 repeatButtons;
    /* 0x04 */ s32 heldButtons;
    /* 0x08 */ s32 pressedButtons;
    /* 0x0C */ s32 previousHeldButtons;
    /* 0x10 */ s32 repeatCounter;
    /* 0x14 */ s32 repeatActive;
    /* 0x18 */ s32 repeatDelay;
    /* 0x1C */ s32 repeatInterval;
    /* 0x20 */ s32 secondaryRepeatActive;
    /* 0x24 */ s32 secondaryRepeatButtons;
    /* 0x28 */ s32 secondaryRepeatCounter;
    /* 0x2C */ s32 secondaryRepeatDelay;
    /* 0x30 */ s32 secondaryRepeatInterval;
    /* 0x34 */ s16 stickMagnitude;
    /* 0x36 */ u8 actuatorRequest0;
    /* 0x37 */ u8 actuatorRequest1;
} PadInputState;

/* D_8006C838 view for UpdatePadInputRecord: the flag at +0xC (bit 16) enables
 * pad-0 input substitution. The pointer at +0x1C supplies a u16 button mask
 * at +4 and receives a Start report (0x800) at +0; keep the wider object's
 * meaning neutral. func_80021DA8 also writes this pointer. */
typedef struct {
    /* 0x00 */ s32 field_0;
    /* 0x04 */ u16 field_4;
} D8006C838Inner;

typedef struct {
    /* 0x00 */ char pad_00[0x0C];
    /* 0x0C */ s32 field_0C;
    /* 0x10 */ char pad_10[0x08];
    /* 0x18 */ s32 field_18;
    /* 0x1C */ D8006C838Inner *field_1C;
} D8006C838View;

/* Five 12-byte records within D_8006C838, also addressed as D_800742B0.
 * This is a partial view, not the full containing object's layout. */
typedef struct {
    /* 0x0000 */ char pad[0x7A78];
    /* 0x7A78 */ s16 records[5][6];
} D8006C838RecordTableView;

/* One record of the 0x18-byte pools referenced by the pointer array at
 * D_8006C838+0xDD8C (built by func_80021E60 over D_8004ED04; 0xC5 records
 * per pool). func_800BF2F4 scans one pool for a record whose s16 at 0
 * matches a reference halfword and whose s32 at 8 / 0x10 lies inside a
 * range box; the u16 at 2 is the record's id/return value. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ char pad_04[0x04];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ char pad_0C[0x04];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ char pad_14[0x04];
} PoolRecord18;

/* Sprite data header: tag + offsets into the sprite's sub-tables.
 * Tag 0xE is the expected magic value (func_80015704 validates this).
 * Offsets at 0x10–0x20 are added to the header base to produce the
 * table pointers stored in SpriteSourceData. */
typedef struct {
    /* 0x00 */ s32 tag;          /* magic value 0xE */
    /* 0x04 */ s32 field_4;
    /* 0x08 */ s32 field_8;
    /* 0x0C */ s32 field_C;
    /* 0x10 */ s32 offset_tex;   /* -> SpriteTex[] (cel/texture rectangles) */
    /* 0x14 */ s32 offset_ref;   /* -> SpriteRef[] (sprite entry indices) */
    /* 0x18 */ s32 offset_18;    /* -> entry data (SpriteEntry[]) */
    /* 0x1C */ s32 offset_anim;  /* -> SpriteRef[] (animation frame index table) */
    /* 0x20 */ s32 offset_frame; /* -> frame data (SpriteFrame[] / control bytes) */
} SpriteDataHeader;

/* Sprite tables: 800F6A24 reads a signed count/byte-offset pair, a
 * 12-byte piece's u16 texture index, and the texture's lh width/lhu height. */
typedef struct {
    s16 count;
    u16 offset;
} SpriteTableRef;

typedef struct {
    u16 index;
    u16 rest[5];
} SpriteFramePiece;

typedef struct {
    s16 x;
    s16 y;
    s16 width;
    u16 height;
} SpriteTextureRect;

/* Sprite source-data / animation object (0x30 bytes).
 * Initialized by func_80015704 from a SpriteDataHeader, updated by
 * func_800158E4 (animation advance), and consumed by the renderer
 * wrappers (func_80016C08, func_800165D8, func_80016280).
 *
 * func_80015704 validates the header (alignment + tag == 0xE), zeroes
 * the state, sets field_8 to 0x1000, computes five table pointers from
 * header offsets, then calls func_80015880 to store the header pointer
 * at field_14 and clear field_18.
 *
 * func_800158E4 advances animation: field_4 indexes the animation table
 * (field_28), field_5 indexes frames within the selected animation,
 * field_6 is a frame-counter/timer, and field_2 holds loop/pause flags
 * (0x100 = loop, 0x200 = pause/hold).
 *
 * func_80016C08 reads field_24 (entry data), field_20 (sprite refs),
 * field_1C (texture cels), field_28 (animation index table), and
 * field_2C (frame data) to render one animation frame as POLY_FT4
 * primitives. */
typedef struct SpriteSourceData {
    /* 0x00 */ u16 field_0;     /* bit-flags (bit 2 = pause guard in func_800158E4) */
    /* 0x02 */ u16 field_2;     /* loop/pause flags (0x100 = loop, 0x200 = pause) */
    /* 0x04 */ u8  field_4;     /* current animation index */
    /* 0x05 */ u8  field_5;     /* current frame index within animation */
    /* 0x06 */ u16 field_6;     /* frame counter / timer */
    /* 0x08 */ s32 field_8;     /* initialized to 0x1000 (capacity / size hint) */
    /* 0x0C */ u16 field_C;     /* zeroed on init, purpose unknown */
    /* 0x0E */ u16 field_E;     /* zeroed on init, purpose unknown */
    /* 0x10 */ u16 field_10;    /* zeroed on init, purpose unknown */
    /* 0x12 */ u16 field_12;    /* zeroed on init, purpose unknown */
    /* 0x14 */ s32 field_14;    /* header pointer (set by func_80015880) */
    /* 0x18 */ s32 field_18;    /* reserved, cleared on init (set by func_80015880) */
    /* 0x1C */ s32 field_1C;    /* header + offset_tex  (SpriteTex * in func_80016C08) */
    /* 0x20 */ s32 field_20;    /* header + offset_ref  (SpriteRef * in func_80016C08) */
    /* 0x24 */ s32 field_24;    /* header + offset_18   (entry data base) */
    /* 0x28 */ s32 field_28;    /* header + offset_anim (animation index table) */
    /* 0x2C */ s32 field_2C;    /* header + offset_frame (frame data / control bytes) */
} SpriteSourceData;

/* Scrolling-grid visitor: ordering-table address, source, sprite index,
 * x/y in pixels. Coordinates are narrowed to s16 by the renderer callbacks. */
typedef void (*SpriteGridCallback)(s32, SpriteSourceData *, s16, s32, s32);

/* 8010D0EC's matched sprite-object view, also passed by 8010C7F0. */
typedef struct {
    s16 field_0;
    char pad_02[0x30 - 0x02];
    s16 unk30;
    char pad_32[0x78 - 0x32];
    SpriteSourceData sprite;
    char pad_A8[0xAC - 0xA8];
    u16 unkAC;
} M2C_90bbdd1382f9_Struct_8010D0EC;

/* 80121500 initializes this 0x18-byte effect: sh at 0/2, sw at 4/8/C,
 * sprite pointer at 14. The neighbouring update/clear routines use +14. */
typedef struct {
    s16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    SpriteSourceData *unk14;
} Ovl11SpriteEffect;

/* 80121500 homes each four-byte coordinate in its incoming argument slot.
 * This byte-aligned by-value view preserves that memory residence; the
 * original field interpretation is unknown, and the payload is copied whole. */
typedef struct {
    u8 bytes[4];
} Ovl11EffectCoordinate;

/* 800F46A4 reads s16 at 2, clears bit 0 at 4, and forwards words at 8/C/10/14. */
typedef struct {
    u8 pad0[2];
    s16 unk2;
    u16 unk4;
    u16 pad6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Ovl11SpawnEffectRecord;

/* ovl_11_func_800C32C8: signed identifiers, halfword state stores and
 * SpriteSourceData passed to the animation helpers at +0x80. */
typedef struct {
    s16 id;
    char pad_2[4];
    s16 field_6;
    s16 field_8;
    char pad_A[4];
    s16 field_E;
    char pad_10[0xA];
    u16 field_1A;
    u16 field_1C;
    u16 flags;
    char pad_20[0x60];
    SpriteSourceData sprite;
} Ovl11AnimationObject;

/* 800C2B04 advances the u16 index at +0x22 through 30 eight-byte
 * entries at +0xE4; 800C2A98 also reads the state fields at +0x14..+0x22. */
typedef struct {
    s16 time;
    u16 data[3];
} Ovl11EventEntry;

typedef struct {
    u8 pad0[0x12];
    s16 field12;
    s16 field14;
    u16 field16;
    u8 pad18[6];
    u16 flags;
    u16 flags20;
    u16 entryIndex;
    u8 pad24[0xE4 - 0x24];
    Ovl11EventEntry entries[30];
} Ovl11EventObject;

/* 80107DE0 stores two signed halfwords; 800C35EC passes its object at +0xE0. */
typedef struct {
    s16 unk0;
    s16 unk2;
} Ovl11C35ECStatePair;

/* Backward-compatibility alias for func_800158E4. */
typedef SpriteSourceData Struct_S;

/* One 0x78-byte entry of the per-object runs iterated by ovl_25_func_800B941C
 * (bases D_800BFE54 and D_800C0124, stride 0x78). ovl_25_func_800B9B28 reads
 * the s16 at 0x00 and 0x04, ovl_25_func_800B9C90 the s16 at 0x02 and the u16
 * at 0x04, and both pass the raw u16 buffer at 0x10 as the second argument to
 * ovl_25_func_800BA5BC/ovl_25_func_800BA758 with the SpriteSourceData record
 * at 0x18 (and, for 800B9B28, 0x48). The +0x04 byte of each record is read
 * unsigned and drives func_80015840. */
typedef struct Ovl25ObjectRecord {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;             /* state selector */
    /* 0x04 */ u16 unk4;             /* timer / counter */
    /* 0x06 */ char pad_06[0x0A];
    /* 0x10 */ u16 source[4];        /* raw buffer fed to the transform pair */
    /* 0x18 */ SpriteSourceData record0;
    /* 0x48 */ SpriteSourceData record1;
} Ovl25ObjectRecord;

/* ovl_25_func_800B9758 reads only the signed state selector at +2. */
typedef struct {
    char pad_0[2];
    s16 unk2;
} Ovl25StateSelectorView;

/* Polygon-list query consumed by func_8001EAE4. */
typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
} EAE4Query;

/* 8001E4C0/8001E340: 0x1C-byte cells, query prefix at +0 and flag at +0x18. */
typedef struct {
    u8 filler[0x18];
    s32 flag;
} CellGrid_1E4C0;

/* 8001E4C0: count at +8, followed by cells at +0xC. */
typedef struct {
    s32 field_0;
    s32 field_4;
    s32 count;
    CellGrid_1E4C0 cells[1];
} Grid_1E4C0;

/* 8001E4C0: signed XYZ halfwords in 8-byte entries, starting at +4. */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} Pt_1E4C0;

typedef struct {
    s32 field_0;
    Pt_1E4C0 pt[1];
} Verts_1E4C0;

/* 8001E4C0 loads the grid pointer at +0 and vertex-buffer pointer at +4. */
typedef struct {
    Grid_1E4C0 *grid;
    Verts_1E4C0 *verts;
} DriverArg_1E4C0;

/* 0x60-byte record copied out of the zero-init global D_8012D548 and passed
 * by value to ovl_11_func_8011D98C. That callee returns data[index]: the
 * s16 table sits at 0x28 and the s16 selector at 0x5C. The first 16 bytes
 * ride in $a0-$a3 and the rest are placed in the outgoing stack area.
 * The word-copy loops in ovl_11_func_8011BAFC establish 4-byte alignment. */
typedef struct {
    /* 0x00 */ s32 pad_00[0x28 / 4];
    /* 0x28 */ s16 data[26];
    /* 0x5C */ s16 index;
    /* 0x5E */ char pad_5E[0x60 - 0x5E];
} StructD548;

/* 0x14-byte argument passed by value starting in $a3 to 8011BAFC.
 * Its record pointer is spilled at incoming +0xC; the two halfwords are
 * read at incoming +0x18/+0x1A. Callers forward five contiguous words. */
typedef struct {
    /* 0x00 */ StructD548 *record;
    /* 0x04 */ s32 field_4;
    /* 0x08 */ s32 field_8;
    /* 0x0C */ s16 field_C;
    /* 0x0E */ s16 field_E;
    /* 0x10 */ s32 field_10;
} Ovl11BAFCParams;

/* State written by 8011DD48 and passed to it by 8011BAFC; the selector
 * at +8 is also used through the unsigned-halfword interface of 8011CF40. */
typedef struct {
    /* 0x00 */ s32 field_0;
    /* 0x04 */ s32 field_4;
    /* 0x08 */ s16 field_8;
    /* 0x0A */ s16 field_A;
} ovl_11_DD48_arg0;

/* 0x60-byte record view passed by value from ovl_11_func_8011D734, which
 * supplies the selector word at 0x60 as a separate argument. Same layout
 * and word alignment as StructD548. */
typedef struct {
    /* 0x00 */ s32 field_00[23];
    /* 0x5C */ s16 index;
    /* 0x5E */ s16 field_5E;
} Ovl11D548Arg;

/* Argument-record views recovered by automatic matching reconstruction
 * (tools/agent/reconstructFunction.ts). Field offsets and widths are
 * witnessed by the functions' own accesses; names are placeholders pending
 * semantics. */
typedef struct {
    /* 0x00 */ s16 unk0;
} Ovl15Func80134444Arg;

typedef struct {
    /* 0x00 */ char pad_0[0x2];
    /* 0x02 */ u16 unk2;
} Ovl19Func800BA73CArg;

/* ovl_19_func_800BAB4C: signed halfwords at +0/+2/+6, position passed
 * at +8, SpriteSourceData at +0x10 with its unsigned state byte at +0x14. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 position[4];
    SpriteSourceData sprite;
} Ovl19Func800BAB4CArg;

/* Shared by ovl_19_func_800BAC40 and ovl_19_func_800BAC50. */
typedef struct {
    /* 0x00 */ char pad_0[0x2];
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
} Ovl19Func800BAC40Arg;

/* Shared by ovl_23_func_800BB0C8 and ovl_23_func_800BB0D8. */
typedef struct {
    /* 0x00 */ char pad_0[0x4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
} Ovl23Func800BB0C8Arg;

/* Minimal views recovered from exact-match functions. Only the named fields
 * are witnessed; padding does not establish the full object extent. Names
 * include the container/function identity to avoid overlay-address collisions. */
typedef struct {
    u8 unk0;
} Recon_ovl_11_func_800BF3D0_CallRet1View;

typedef struct {
    char pad_0[0x38];
    u16 unk38;
    char pad_3A[0x2];
    u16 unk3C;
} Recon_ovl_11_func_800C6F0C_A0View;

/* 800CBF70's matched definition copies this three-word position into an effect. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl11CBF70Position;

/* Fields returned by 800CBF70, as witnessed by its matched definition. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    char pad0A[2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    char pad20[4];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
} Ovl11CBF70Effect;

/* 800C7010: halfwords at 38/3C, flags at 6C, animation data at 260.
 * 800C69B8 reads the signed item-table index at 8A.
 * 800CCCC0 resets halfwords at 18/1A/22/24/2A/36 and reads u16 at 2C. */
typedef struct {
    char pad_0[0x18];
    s16 unk18;
    s16 unk1A;
    char pad_1C[6];
    s16 unk22;
    s16 unk24;
    char pad_26[4];
    s16 unk2A;
    s16 unk2C;
    char pad_2E[8];
    s16 unk36;
    u16 unk38;
    char pad_3A[2];
    u16 unk3C;
    char pad_3E[0x2E];
    s32 unk6C;
    char pad_70[0x14];
    s16 unk84; /* 800CDD7C tests this element against 0x39. */
    char pad_86[4];
    s16 itemIndex;
    char pad_8C[0x100 - 0x8C];
    /* 800CD9A0 passes the position words at +100 to 800BDCE8. */
    s32 unk100;
    s32 unk104;
    s32 unk108;
    char pad_10C[0x260 - 0x10C];
    SpriteSourceData sprite;
} Ovl11C7010Object;

/* 800CB114 reads u16 +38/+3C, updates flags +6C, and forwards the
 * halfword-aligned collision tag at +FC by value. */
typedef struct {
    char pad0[0x38];
    u16 direction;
    char pad3A[2];
    u16 mode;
    char pad3E[0x6C - 0x3E];
    s32 flags;
    char pad70[0xFC - 0x70];
    Ovl11CollisionTag collision;
} Ovl11CB114Object;

/* 800C498C indexes with u16 +38 and copies the four words at +100. */
typedef struct {
    char pad0[0x38];
    u16 direction;
    char pad3A[0x100 - 0x3A];
    s32 position[4];
} Ovl11C498CObject;

/* 800C9938's matched parameter view, also used by its caller 800C99C8. */
typedef struct {
    char pad_0[0x38];
    u16 unk38;
    char pad_3A[0x2];
    u16 unk3C;
    char pad_3E[0x2E];
    s32 unk6C;
    char pad_70[0xA0];
    s32 unk110;
    char pad_114[0x4];
    s32 unk118;
} Ovl119938View;

/* 800CEEF8 and 800F4240 pass records with s16 +2 and u16 +4. */
typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} Ovl11InteractionRecord;

/* 800C8FA8: sh at 0x22, lhu at 0x3C and lw/sw flags at 0x6C;
 * 800C907C also reads lh at 0x34 and writes sh at 0x38. */
typedef struct {
    char pad0[0x22];
    u16 unk22;
    char pad24[0x34 - 0x24];
    s16 unk34;
    char pad36[2];
    u16 unk38;
    char pad3A[2];
    u16 unk3C;
    char pad3E[0x6C - 0x3E];
    s32 unk6C;
} Ovl11C8FA8Object;

/* 800C8808: halfword fields +22/+38, flags +6C and a query point at +FC. */
typedef struct {
    char pad0[0x22];
    u16 unk22;
    char pad24[0x38 - 0x24];
    u16 unk38;
    char pad3A[0x6C - 0x3A];
    s32 unk6C;
    char pad70[0xFC - 0x70];
    s16 point[3];
} Ovl11C8808Object;

typedef struct {
    union {
        u8 unk0_b;
        s32 unk0_w;
    } unk0;
} Recon_ovl_11_func_800C6F0C_A1View;

typedef struct {
    char pad_0[0x1C];
    /* 0x1C */ u16 unk1C;
    char pad_1E[0x1A];
    /* 0x38 */ u8 unk38;
    char pad_39[0x3];
    /* 0x3C */ u16 unk3C;
    /* 0x3E */ u16 unk3E;
    char pad_40[0x2C];
    /* 0x6C */ s32 unk6C;
    char pad_70[0x14];
    /* 0x84 */ u16 unk84;
    char pad_86[0x6A];
    /* 0xF0 */ u16 unkF0;
    char pad_F2[0x173];
    /* 0x265 */ u8 unk265;
} Recon_ovl_11_func_800C7270_A0View;

typedef struct {
    char pad_0[0x38];
    u16 unk38;
    char pad_3A[0x50];
    s16 unk8A;
} Recon_ovl_11_func_800C8334_A0View;

typedef struct {
    char pad_0[0x1A];
    s16 unk1A;
    char pad_1C[0x50];
    s32 unk6C;
} Recon_ovl_11_func_800C97D0_A0View;

typedef struct {
    char pad_0[0x3C];
    /* 0x3C */ u16 unk3C;
} Recon_ovl_11_func_800CCFF8_A0View;

typedef struct {
    char pad_0[0x6C];
    s32 unk6C;
} Recon_ovl_11_func_800CD08C_A0View;

typedef struct {
    char pad_0[0x34];
    s32 unk34;
    char pad_38[0x4];
    s32 unk3C;
    char pad_40[0xC];
    s32 unk4C;
} Recon_ovl_11_func_800D2594_A0View;

typedef struct {
    s16 unk0;
} Recon_ovl_11_func_800D3404_A0View;

/* 0xB0-byte object initialised by ovl_11_func_800D3390: a u16 tag at 0x00
 * that selects the already-initialised path, an s16 selector at 0x00, a
 * u16 0xFFFF marker at 0x04, and two s16 at 0xA8/0xAA cleared by
 * ovl_11_func_80107DD0. Only the witnessed fields are named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x2];
    /* 0x04 */ u16 unk4;
    /* 0x06 */ char pad_06[0xA2];
    /* 0xA8 */ s16 unkA8;
    /* 0xAA */ s16 unkAA;
    /* 0xAC */ char pad_AC[0x4];
} Recon_ovl_11_func_800D3390_A0View;

typedef struct {
    s32 unk0;
} Recon_ovl_11_func_800D6380_A3View;

/* Argument view of ovl_11_func_800D85F8: the item id at 0x00 is read both
 * unsigned (lhu) for the returned default and signed (lh) by the flag guards
 * in ovl_11_func_800D5868/800D589C. 0x02 is a next-id halfword copied back to
 * 0x00 on reset, 0x04 a state byte compared against the ovl_11_func_800D5C3C
 * lookup, and 0x06 a flags halfword whose bit 15 is set when the
 * ovl_11_func_800D5C90 lookup succeeds. */
typedef struct {
    union {
        /* 0x00 */ u16 unk0;
        /* 0x00 */ s16 unk0_s;
    } u0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ s8 unk5;
    /* 0x06 */ u16 unk6;
} Recon_ovl_11_func_800D85F8_A0View;

/* 800C7878 stores the selected item pointer at +0x50 and flags at +0x54;
 * 800C8CC8 passes this same object to it. */
typedef struct {
    char pad0[0x50];
    Recon_ovl_11_func_800D85F8_A0View *unk50;
    s32 unk54;
} M2C_15512f31231d_Func800C7878Arg0;

/* 800C7878 writes the six-byte result's id, state and count at +0/+2/+4. */
typedef struct {
    s16 unk0;
    char pad2[2];
    u16 unk4;
} M2C_15512f31231d_F413CArg1;

typedef struct { u16 unk0; } UnkStruct800DF4F0;

/* 800D0DB0 stores the two signed indices passed by 800DF72C. */
typedef struct {
    s16 unk0;
    s16 unk2;
} Ovl11DF72CPair;

/* 800D049C passes its +0xA8 pair to 80107DE0. */
typedef struct {
    s16 unk0;
    s16 unk2;
} Ovl11DF72CResetPair;

typedef struct {
    char pad_00[0x16];
    s16 unk16;
    char pad_18[0xA8 - 0x18];
    Ovl11DF72CResetPair unkA8;
} Ovl11DF72CResetView;

typedef struct {
    char pad_0[0xB0];
    u16 unkB0;
} Recon_ovl_11_func_800DEEE0_A0View;

typedef struct {
    char pad_0[0x2C];
    s16 unk2C;
    s16 unk2E;
    char pad_30[0x4];
    s32 unk34;
} Recon_ovl_11_func_800E0220_A0View;

typedef struct { u16 unk0; } UnkStruct800E109C;

/* 800E0D3C copies eight bytes from state +0x44B8 to object +0x1A with lwl/lwr and swl/swr. */
typedef struct __attribute__((packed)) {
    s32 word0;
    s32 word4;
} Ovl11InitStamp;

typedef struct {
    char pad_0[0xB0];
    u16 unkB0;
} Recon_ovl_11_func_800E0AFC_A0View;

typedef struct {
    char pad_0[0x2C];
    s16 unk2C;
    s16 unk2E;
    char pad_30[0x4];
    s32 unk34;
} Recon_ovl_11_func_800E1D48_A0View;

typedef struct {
    s16 unk0;
} Recon_ovl_11_func_800FB394_A0View;

typedef struct {
    char pad_0[0x2C];
    s16 unk2C;
    s16 unk2E;
    char pad_30[0x4];
    s32 unk34;
} Recon_ovl_11_func_8010BF8C_A0View;

typedef struct {
    char pad_0[0x26];
    s16 unk26;                /* 0x26 - selected handler id */
    s16 unk28;                /* 0x28 */
    s16 unk2A;                /* 0x2A */
    s16 unk2C;                /* 0x2C */
    char pad_2E[0x34 - 0x2E];
    s32 unk34;                /* 0x34 - flags (bit 11 = 0x800) */
    char pad_38[0xB8 - 0x38];
    u16 unkB8;                /* 0xB8 - flags, bit 11 cleared on select */
} Recon_ovl_11_func_8010CE80_A0View;

/* ovl_11_func_8010CB6C: witnessed signed meter, counters, flags and
 * movement words; offsets follow the target's lh/lhu/lw/sh/sw accesses. */
typedef struct {
    char pad[0x16];
    s16 unk16;
    char pad18[0x22 - 0x18];
    u16 unk22;
    char pad24[0x2C - 0x24];
    u16 unk2C;
    char pad2E[2];
    u16 unk30;
    char pad32[2];
    u32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    char pad44[0xAC - 0x44];
    u16 unkAC;
    u16 unkAE;
    u16 unkB0;
    u16 unkB2;
    u16 unkB4;
    char padB6[2];
    u16 unkB8;
    char padBA[4];
    u16 unkBE;
} Ovl11ObjectCB6C;

/* 8010CF58 reads signed state/handler halfwords at +30/+26 and
 * reads/clears flag 0x800 in the unsigned halfword at +B8. */
typedef struct {
    char pad0[0x26];
    s16 unk26;
    char pad28[0x30 - 0x28];
    s16 unk30;
    char pad32[0xB8 - 0x32];
    u16 unkB8;
} Ovl11CF58Object;

/* The matched ovl_11_func_80110E34 definition supplies this movement view. */
typedef struct {
    char pad[0x22];
    u16 unk22;
    char pad2[0x30 - 0x24];
    u16 unk30;
    char pad3[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    char pad4[0xAC - 0x44];
    u16 unkAC;
} Ovl11MotionState;

/* ovl_11_func_8010D250 argument view: reads the handler id at 0x26, a mode
 * at 0x30, and the staged s16 triple at 0xBE/0xC0/0xC2. */
typedef struct {
    char pad_0[0x26];
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    s16 unk30;
    char pad_32[0xBE - 0x32];
    s16 unkBE;
    s16 unkC0;
    s16 unkC2;
} Recon_ovl_11_func_8010D250_A0View;

typedef struct {
    char pad_0[0x24];
    u8 unk24;
} Recon_ovl_17_func_800BAFAC_A0View;

typedef struct {
    char pad_0[0x24];
    u8 unk24;
} Recon_ovl_17_func_800BB094_A0View;

typedef struct {
    char pad_0[0x24];
    u8 unk24;
} Recon_ovl_17_func_800BB020_A0View;

/* ovl_17_func_800BAD9C argument view: a D_800BD870 record (0x50 stride).
 * +0x14 points at a small animation/reference triple read as three s16s
 * (+0/+2/+4); +0x20 is passed to func_80015868 as a Struct_800154CC; the
 * bytes +0x24/+0x25 are read unsigned (lbu) and passed to func_80015EE8. */
typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
} Recon_ovl_17_func_800BAD9C_AnimRef;

typedef struct {
    u8 field_0;                                  /* 0x00 */
    char pad_1[0x10 - 0x01];
    s32 field_10;                                /* 0x10 */
    Recon_ovl_17_func_800BAD9C_AnimRef *field_14; /* 0x14 */
    char pad_18[0x24 - 0x18];
    u8 field_24;                                 /* 0x24 */
    u8 field_25;                                 /* 0x25 */
} Recon_ovl_17_func_800BAD9C_A0View;

typedef struct {
    char pad_0[0x4];
    s16 unk4;
    s16 unk6;
} Recon_ovl_19_func_800B9DD0_A0View;

typedef struct {
    char pad_0[0x4];
    s16 unk4;
    s16 unk6;
    char pad_8[0xA];
    s16 unk12;
} Recon_ovl_19_func_800BA054_A0View;

/* ovl_19_func_800BA0A0 arg0: state record whose +0x2 halfword selects the
 * rate table entry, +0x4 selects the direction, and +0x10/+0x14 are the
 * quantities the function advances. */
typedef struct {
    char pad_0[0x2];
    u16 unk2;
    s16 unk4;
    char pad_6[0xA];
    s16 unk10;
    char pad_12[0x2];
    s16 unk14;
} Recon_ovl_19_func_800BA0A0_A0View;

/* ovl_11_func_800CADBC arg0: array of three-s16 records; the function moves
 * each later non-empty record into the first empty slot and zeroes its source. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Ovl11Func800CADBCArg0;

/* ovl_11_func_800D0408 arg1: three s32 vector components selected by a
 * direction code; the switch stores into exactly one of the fields. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Recon800D0408A1View;

/* Pointer argument to ovl_21_func_800B98CC: a signed selector at +2,
 * a stored s32 predicate at +0x18, and an unsigned halfword at +0x1C. */
typedef struct {
    /* 0x00 */ u8 pad_00[2];
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad_04[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u16 unk1C;
} Ovl21Func800B98CCArg;

/* Four-byte entries scanned by ovl_11_func_800F27B0. The signed byte
 * at +2 is the -1 empty marker; +0 is a halfword and +3 a byte. */
typedef struct {
    /* 0x00 */ s16 value;
    /* 0x02 */ s8 kind;
    /* 0x03 */ u8 day;
} Ovl11QueuedEntry;

/* D_8006C838 row view shared by the +0xE514 selector consumers.
 * Each row contains six seven-halfword entries (0x54 bytes). */
typedef struct {
    char pad_000[0xE514];
    s16 selector;
    s16 pad_516[6];
    s16 rows[1][6][7];
} Ovl11RowE522View;

/* Five 0x0C-byte records at D_8006C838 +0xE4D8, independently
 * indexed by the record initializer and resetter. */
typedef struct {
    char pad[0xE4D8];
    s16 recs[5][6];
} Ovl11RecordE4D8View;

/* ovl_17's 90-entry region at D_800BD848 +0x2D8 and the
 * following +0x5A8 cursor, shared by the initializer and append routine. */
typedef struct {
    s32 word;
    s16 field;
    s16 pad;
} Ovl17QueueEntry;

typedef struct {
    u8 pad[0x2D8];
    Ovl17QueueEntry entries[90];
    s16 cursor;
} Ovl17QueueView;

/* Fixed 41-halfword text scan buffer used by the two multi-row
 * layout wrappers. Whole-object assignment preserves the 82-byte copy. */
typedef struct {
    u16 data[41];
} TextCopyBlock;

/* ovl_11_func_800CC4E0 object view: sh at +0x6 and sw at +0xC.
 * The object identifier at +0x2 is also read as signed/unsigned halfwords. */
typedef struct {
    /* 0x00 */ char pad_0[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ char pad_8[4];
    /* 0x0C */ s32 unkC;
} Ovl11CC4E0Object;

/* 800DB00C copies the +0/+2/+4 coordinates and the +8/+A/+C triplet. */
typedef struct Ovl11SceneCoordinates {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} Ovl11SceneCoordinates;

/* func_8001E340's matched grid definition: count +8, 0x1C-byte cells. */
typedef struct {
    u8 filler[0x18];
    s32 flag;
} Ovl11SceneCell;

typedef struct {
    s32 field_0;
    s32 field_4;
    s32 count;
    Ovl11SceneCell cells[1];
} Ovl11SceneGrid;

/* Matched 800BDCE8 reads the position words at +0/+8; 800D2DB8
 * forwards the same three-word object to 800BDB88. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl11Func800BDCE8Arg1;

/* Object reached through D_8007AFF0+0x25388: lh at +0x2. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} Ovl11CC4E0Head;

/* ovl_15_func_80136288 copies the first 0x3C bytes of a card block. */
typedef struct {
    u32 words[15];
} Ovl15SaveSummary;

/* 800C2C74 forwards this emitter and 0xC-byte event to 800C3B78.
 * 800C2044 also accesses signed indices at C/E and fixed positions at 40/44/48. */
typedef struct {
    u8 pad0[6];
    u16 unk6;
    u8 pad8[4];
    s16 unkC;
    s16 unkE;
    u8 pad10[0x20];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 pad3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 pad4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
} Ovl11C3B78Arg0;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Ovl11C3B78Arg1;

/* 800C3838 forwards this object to 800C3F6C, whose matched body reads +0x2E. */
typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
} Ovl11C3F6CObjectView;

/* ovl_15_func_8013345C scans these pools for a free record; 801331C4 passes
 * D_800742EC (D_8006C838 + 0x7AB4): ten 0xB4 records, then twenty 0xB8. */
typedef struct {
    struct Ovl11ResetB4 small[10];
    struct Ovl11ResetB8 large[20];
} Ovl15RecordPools;

/* Matched ovl_15_func_80132A8C parameter view, also passed by 801331C4. */
struct ovl_15_80132A8C_data {
    s16 pad0[17];
    s16 field_22;
    s16 pad1[6];
    s16 field_30;
    s16 pad2[3];
    s32 field_38;
    s32 field_3C;
    s32 field_40;
};

/* Matched ovl_15_func_80132B44 parameter view, also passed by 801331C4. */
typedef struct {
    char pad[0x22];
    u16 unk22;
    char pad2[0x30 - 0x24];
    u16 unk30;
    char pad3[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
} Unk80132B44;

/* 8010A654: unsigned call operands +22/+AC, state +28, timer +2C,
 * flags +34, key +30 and copied motion words +58..+64. */
typedef struct {
    char pad_00[0x22];
    u16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[2];
    s16 unk30;
    char pad_32[2];
    s32 unk34;
    char pad_38[0x58 - 0x38];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    char pad_68[0xAC - 0x68];
    u16 unkAC;
} Ovl11A654Object;

/* 8010DE68 / 80110E98: the matched 8010DA04 caller passes this same object view. */
typedef struct {
    u8 pad_00[0x16];
    u16 unk16;
    u8 pad_18[0x24 - 0x18];
    s16 unk24;
    s16 unk26;
    u8 pad_28[0x2C - 0x28];
    s16 unk2C;
    u8 pad_2E[0x30 - 0x2E];
    s16 unk30;
    u8 pad_32[0x34 - 0x32];
    s32 unk34;
    u8 pad_38[0xAC - 0x38];
    u16 unkAC;
    s16 unkAE;
    s16 unkB0;
    s16 unkB2;
    u8 pad_B4[0xB8 - 0xB4];
    u16 unkB8;
    u8 pad_BA[0xC4 - 0xBA];
    s16 unkC4;
    s16 unkC6;
} Ovl11DE68Object;

/* Gradient-draw command shared by func_8001FA0C and func_8001F8A4 (0xC..0x14
 * is a RECT handed to SetDrawLoad; field_0/field_8 feed func_8001F774). */
typedef struct {
    /* 0x00 */ s32 field_0;     /* source u16 array */
    /* 0x04 */ s16 field_4;
    /* 0x06 */ u16 field_6;
    /* 0x08 */ s16 field_8;     /* gradient step count */
    /* 0x0A */ s16 field_A;
    /* 0x0C */ u16 field_C;     /* RECT x */
    /* 0x0E */ u16 field_E;     /* RECT y */
    /* 0x10 */ u16 field_10;    /* RECT w */
    /* 0x12 */ u16 field_12;    /* RECT h */
} GradientCmd;

/* Records of the table at D_80075BC4, scanned by ovl_11_func_801097F4:
 * six 0xB0-byte entries; entry u16 tag 0x15B at 0x00, an s16 selector at
 * 0x30, and two s32 coordinates at 0x38/0x40 range-tested against an
 * arg0/arg2 window of half-width arg5. Only the witnessed fields are named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x2E];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[6];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ char pad_3C[4];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ char pad_44[0x6C];
} UnkStruct80075BC4;

/* Element of the D_800C0448 state table, stride 0x108. The two leading
 * halfwords are the state pair written by ovl_21_func_800B9538 /
 * ovl_21_func_800B95EC; ovl_21_func_800B9798 writes a halfword at 0x18 of
 * three consecutive entries starting at index arg0*3. Only the witnessed
 * fields are named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ char pad_04[0xC];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ char pad_20[2];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ char pad_26[2];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ void *unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ char pad_3E[2];
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42;
    /* 0x44 */ u16 unk44;
    /* 0x46 */ char pad_46[2];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ char pad_4E[0x108 - 0x4E];
} UnkStruct800C0448;

/* 800BA698 reads unsigned coordinates at +0/+4; 800B9CA4 passes record +0x1A. */
typedef struct {
    u16 unk0;
    u16 pad_02;
    u16 unk4;
} Ovl21DistanceOrigin;

/* 800B9F3C forms signed 16-bit displacements from the positions at +0x1A/+0x1E. */
typedef struct {
    char pad_00[0x18];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    char pad_20[4];
    s16 unk24;
    char pad_26[6];
    s16 unk2C;
    char pad_2E[0x108 - 0x2E];
} Ovl21SignedRecordView;

/* 800B9F3C writes the projectile fields at record +0x34..+0x4C,
 * stepping between projectiles by 0x4C bytes. */
typedef struct {
    char pad_00[0x34];
    s32 active;
    u16 kind;
    s16 life;
    s16 duration;
    s16 steps;
} Ovl21ProjectileView;

/* 800B836C's three 0x4C-byte states start at record +0x24. */
typedef struct {
    char pad_00[0x10];
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    char pad_1A[2];
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    char pad_22[2];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    char pad_2A[0x4C - 0x2A];
} Ovl21InitSubstate;

/* 800B836C initializes six 0x108 records, then selects the two trailing tables. */
struct Ovl21InitState {
    UnkStruct800C0448 records[6];
    char pad_630[0x10];
    s16 tableIndex;
    u16 variantIndex;
};

/* 800B836C reads the signed entity selector at D_8006C838 + 0xE628. */
typedef struct {
    char pad_0[0xE628];
    s16 selector;
} Ovl21InitSelectorView;

/* One of the six 0x108-byte records of the D_800C0448 state block, at
 * 0x14 + i * 0x108. It is the storage UnkStruct800C0448 indexes from the block
 * base: this record's unk0/unk4/unk6 are that element's unk14/unk18/unk1A. The
 * halfwords at unk6 are the position ovl_21_func_800BA698 reads (+0 and +4).
 * Only the fields ovl_21_func_800B990C touches are named. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u16 unk6[3];
    /* 0x0C */ char pad_0C[0x1A - 0x0C];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ char pad_1C[0x108 - 0x1C];
} Ovl21C0448Record;

/* The whole D_800C0448 state block: a 0x14-byte header, the six records as an
 * array member, then scalar fields. The records must be an array member, not
 * pointer arithmetic: GCC adds a 4-aligned s32 field's offset to the base
 * before the scaled index only for an array member of a BLKmode object
 * (expr.c expand_expr COMPONENT_REF). Only witnessed fields are named. */
typedef struct {
    /* 800B8B3C updates the pause word and signed frame counter. */
    /* 0x000 */ u16 unk0;
    /* 0x002 */ u16 unk2;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s16 unk8;
    /* 0x00A */ char pad_00A[2];
    /* 0x00C */ void *unkC;
    /* 0x010 */ char pad_010[4];
    /* 0x014 */ Ovl21C0448Record recs[6];
    /* 0x644 */ char pad_644[2];
    /* 0x646 */ s16 unk646;
    /* 800B8B3C reads three signed update periods at +648/+64E/+650. */
    /* 0x648 */ s16 unk648;
    /* 0x64A */ char pad_64A[4];
    /* 0x64E */ s16 unk64E;
    /* 0x650 */ s16 unk650;
    /* 0x652 */ char pad_652[0x65C - 0x652];
    /* 0x65C */ s16 unk65C;
    /* 0x65E */ char pad_65E[0x984 - 0x65E];
    /* 0x984 */ s16 unk984;
    /* 0x986 */ s16 unk986;
    /* 0x988 */ s16 unk988;
} Ovl21C0448View;

/* ovl_11_func_801097F4 argument record, passed by value: the first 16 bytes
 * ride in $a0-$a3 and are homed to 0x0($sp) on entry; the witnessed tail
 * fields sit at 0x10 (s16 selector), 0x14 (s32 radius) and 0x18 (out
 * pointer). Only the fields the function reads are named. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad_04[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ char pad_0C[4];
    /* 0x10 */ s16 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ UnkStruct80075BC4 **unk18;
} Ovl11Func801097F4Arg;

/* ovl_11_func_800CF95C argument record, passed by value like
 * Ovl11Func801097F4Arg: all 16 bytes ride in $a0-$a3 and are homed to
 * 0x0($sp) on entry. Only the fields the function reads are named. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} Ovl11Func800CF95CArg;

/* ovl_11_func_801096FC argument record, passed by value like
 * Ovl11Func801097F4Arg: the first 16 bytes ride in $a0-$a3 and are homed to
 * 0x0($sp) on entry; the tail fields sit at 0x10 (s16 selector), 0x14 (s32
 * half-width), 0x18 (s32 flag out) and 0x1C (record out). Only the fields the
 * function reads are named. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ char pad_12[2];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 *unk18;
    /* 0x1C */ s32 **unk1C;
} Ovl11Func801096FCArg;

/* Records of the D_800749F4 table scanned by ovl_11_func_801096FC: twenty
 * 0xB8-byte entries sharing the Ovl11Func801097F4 record layout with an extra
 * halfword at 0x26 and a flag word at 0x34. Only the witnessed fields are
 * named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x24];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ char pad_28[8];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[2];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ char pad_3C[4];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ char pad_44[0xB8 - 0x44];
} Ovl11RecD749F4;

/* ovl_11_func_801098B0 argument record: a D_80075BC4-shaped record with an
 * extra s16 at 0xAC. The u16 tag at 0x00 gates the body, bit 0x8000 of the s32
 * at 0x34 gates the scan, the s16 at 0xAC selects the radius, 0x30 is the
 * scan selector, and 0x38/0x3C/0x40/0x44 are the four words forwarded to
 * ovl_11_func_801097F4. Only the witnessed fields are named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x30 - 0x02];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ char pad_48[0xAC - 0x48];
    /* 0xAC */ s16 unkAC;
} Ovl11Func801098B0Arg;

/* Object viewed by the D_800B8E6C state-handler ovl_11_func_800D4030: the u16
 * state id at +0x00 and the s16 selector at +0x30. Only the two fields the
 * handler reads are named. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x2E];
    /* 0x30 */ s16 unk30;
} Ovl11D4030Arg;

/* Projected-primitive source, exe:8001BFA8/8001C0D4. The initializer stores
 * the batch and, for at least two entries, the following culling-point block.
 * That block has a four-byte prefix followed by 8-byte SVECTOR records. */
typedef struct {
    /* 0x00 */ void *batch;
    /* 0x04 */ s8 *cullPointData;
} ProjectedPrimitiveSource;

/* One 8-byte entry of the D_800BF87C record array written by
 * ovl_23_func_800B9454: s32 at +0, s16 at +4, s16 at +6. */
typedef struct {
    s32 unk0;   /* 0x00 */
    s16 unk4;   /* 0x04 */
    s16 unk6;   /* 0x06 */
} Ovl23D87CEntry;

/* D_800BF87C view for ovl_23_func_800B9454: a 54-entry array at 0x418
 * followed by the s16 write cursor at 0x5C8 (the 54 bound equals the array
 * length, matching the sibling ovl_23_func_800B94D0 whose 12-entry array at
 * 0x5CC is followed by its cursor at 0x62C). */
typedef struct {
    u8 pad_000[0x418];
    Ovl23D87CEntry unk418[54];  /* 0x418 */
    s16 unk5C8;                 /* 0x5C8 */
    u8 pad_5CA[2];
    Ovl23D87CEntry unk5CC[12];  /* 0x5CC */
    s16 unk62C;                 /* 0x62C */
} Ovl23D87CView;

/* One 0x44-byte entry of the D_800BF87C record array read by
 * ovl_23_func_800BA1E0; the value the function reads is the leading s32,
 * selected by the record's s16 index. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ char pad_04[0x40];
} Ovl23D87CEntry0;

/* D_800BF87C view for ovl_23_func_800BA1E0: a 0x44-stride record array at
 * offset 0x210. */
typedef struct {
    u8 pad_000[0x210];
    Ovl23D87CEntry0 unk210[32]; /* 0x210 */
} Ovl23D87CView210;

/* One 0x50-byte entry of the D_800BF87C record array: 800B8354 writes
 * +0x24..+0x3C; 800BA494 reads u16 +0x28, s16 +0x2E and s32 +0x38.
 * The entries are read as an array member, not with byte-pointer arithmetic:
 * cc1 adds the base before the scaled index (expr.c EXPAND_SUM), which is
 * what puts the target's base register first in the `addu`. */
typedef struct {
    u8 pad_00[0x24];
    u16 unk24;         /* 0x24 */
    u16 unk26;         /* 0x26 */
    u16 unk28;         /* 0x28 */
    u16 unk2A;         /* 0x2A */
    u16 unk2C;         /* 0x2C */
    s16 unk2E;         /* 0x2E */
    u16 unk30;         /* 0x30 */
    u16 unk32;         /* 0x32 */
    u32 unk34;         /* 0x34 */
    s32 unk38;         /* 0x38 */
    u32 unk3C;         /* 0x3C */
    u8 pad_40[0x10];
} Ovl23D87CEntry50;

/* D_800BF87C view for ovl_23_func_800BA494: a 0x50-stride record array. */
typedef struct {
    Ovl23D87CEntry50 unk0[32];
} Ovl23D87CView50;

/* D_800BF87C view for ovl_23_func_800B9A00 (s16 fields at 0x39C and 0x3A0)
 * and ovl_23_func_800BA610 (which also reads the s16 at 0x3A2); the target
 * keeps the aggregate base and uses these offsets. */
typedef struct {
    u8 pad_000[0x39C];
    s16 unk39C;        /* 0x39C */
    u8 pad_39E[0x2];
    s16 unk3A0;        /* 0x3A0 */
    s16 unk3A2;        /* 0x3A2 */
    u8 pad_3A4[0xE];
    s16 unk3B2;        /* 0x3B2 */
} Ovl23D87CView39C;

/* ovl_23_func_800BA73C reads these fields in each 0x50-byte record
 * passed by ovl_23_func_800B9310; the sprite begins at +0x1C. */
typedef struct {
    u8 pad_00[4];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 pad_0A[0xA];
    s32 unk14;
    s32 unk18;
    u8 pad_1C[4];
    u8 unk20;
    u8 unk21;
    u8 pad_22[0x2A];
    s16 unk4C;
    s16 unk4E;
} Ovl23BA73CArg;

/* ovl_23_func_800BAA60 argument record: one 0x44-stride entry of the
 * D_800BFA80 array driven by ovl_23_func_800B9310. The s16 selector switched
 * on is at 0x04, the ObjectState-ish block begins at 0x10 (its +0x04 byte is
 * read unsigned), the position word is at 0x0C and the s16 at 0x40 is the
 * fourth func_80015868 argument. Only the fields the function reads are
 * named; the full object extent is not established. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ char pad_10[4];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ char pad_15[0x2B];
    /* 0x40 */ s16 unk40;
} Ovl23BAA60Arg;

/* D_800BF87C view for ovl_23_func_800BAA60: the s32 state id at 0x04 and the
 * u16 cursor counter at 0x14, both read from the single aggregate base. */
typedef struct {
    /* 0x00 */ char pad_00[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ char pad_08[0xC];
    /* 0x14 */ u16 unk14;
} Ovl23D87CViewBAA60;

/* ovl_23_func_800BA1E0 argument record: s16 selector at 0x00, s32 at 0x14
 * and s32 at 0x18. Only the fields the function reads are named. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ char pad_02[0x12];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} Ovl23Func800BA1E0Arg;

/* D_8006C838 status view used by ovl_11_func_800D0BC8. The final
 * halfword array is indexed with stride two; its full extent is unknown.
 * 800D0FB4 reaches the record arrays and the two single records from this
 * same base: its loop hoist matches only when they share the symbol with
 * +44D4. The single records are 0xF0 bytes; only their 0xB4-byte prefix
 * is typed. */
typedef struct {
    char pad_000[0x44D0];
    u16 field_44D0;
    u16 field_44D2;
    /* 800D0FB4 tests this status before resetting a record. */
    u16 field_44D4;
    char pad_44D6[0x7AB4 - 0x44D6];
    Ovl11ResetRecordB4 records_7AB4[10];
    Ovl11ResetRecordB8 records_81BC[20];
    char pad_901C[0x91AC - 0x901C];
    Ovl11ResetRecordB4 record_91AC;
    char pad_9260[0x929C - 0x9260];
    Ovl11ResetRecordB4 record_929C;
    char pad_9350[0x99C8 - 0x9350];
    s16 field_99C8[1];
} Ovl11Status99C8View;

/* D_8006C838 halfword triple at +0x5488 selected by ovl_11_func_800FA410. */
typedef struct {
    char pad_5488[0x5488];
    s16 field_5488;
    s16 field_548A;
    s16 field_548C;
} Ovl11Status5488View;

/* D_8006C838 slots compared by ovl_11_func_800C12DC: the +0x44B8 triple,
 * the +0x5488 triple, and the +0xE4DC halfword array stepped by six. */
typedef struct {
    char pad_000[0x44B8];
    s16 field_44B8;
    s16 field_44BA;
    s16 field_44BC;
    char pad_44BE[0x5488 - 0x44BE];
    s16 field_5488;
    s16 field_548A;
    s16 field_548C;
    char pad_548E[0xE4DC - 0x548E];
    s16 field_E4DC[1];
} Ovl11Status44B8View;

/* D_80074838 five 12-byte records read by ovl_11_func_800C12DC. */
typedef struct {
    char pad_000[0x64D8];
    s16 recs[5][6];
} Ovl11Rec64D8View;

/* D_80074838 work-area view used by ovl_11_func_800D079C: it reads an s16 at
 * +0x19C8 and steps it by two halfwords. The array extent is unknown. */
typedef struct {
    u8 pad_000[0x19C8];
    s16 field_19C8[1];
} Ovl11Work19C8View;

/* 0x30-byte table entry scanned by ovl_11_func_800DDC64. */
typedef struct {
    u8 pad[0x28];
    u16 unk28;
    u8 pad2[6];
} Ovl11FuncDDC64Entry;

/* Signed halfword lookup at D_8006C838+99DA; full extent is unknown. */
typedef struct {
    u8 pad[0x99DA];
    s16 values[1];
} Ovl11Value99DAView;

/* Nine byte cells at D_8006C838+E7A2, accessed by ovl_11_func_80108B8C. */
typedef struct {
    u8 pad[0xE7A2];
    u8 states[9];
} Ovl11StatesE7A2View;

/* Object fields written by ovl_11_func_800D05D0; 80109D08 also reads the
 * signed selector at +0x30. */
typedef struct {
    char pad_00[0x30];
    s16 field_30;
    char pad_32[2];
    s32 field_34;
    char pad_38[0x58 - 0x38];
    s32 field_58;
    s32 field_5C;
    s32 field_60;
} Ovl11SetFieldsView;

/* Parameter view of the four words ovl_11_func_800D062C reads (0x38, 0x40,
 * 0x58, 0x60) and passes in pairs to ovl_11_func_800D0600. */
typedef struct {
    /* 0x00 */ char pad_00[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ char pad_3C[0x4];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ char pad_44[0x14];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ char pad_5C[0x4];
    /* 0x60 */ s32 unk60;
} Ovl11RankPair;

/* 80107DE0 writes two consecutive halfwords; 800D506C passes object+0xA8. */
typedef struct {
    s16 unk0;
    s16 unk2;
} M2C_729a6ccf4038_UnkStruct80107DE0;

/* Callers pass four words; D0408 initializes and D05D0 consumes only
 * the three components. The final word is unused byte padding. */
typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    u8 pad_C[4];
} Ovl11PaddedVec3;

/* 800D5224 copies these four-word positions from the indexed pool bases. */
typedef struct {
    u8 pad0[0x81F4];
    Ovl11PaddedVec3 position;
} Ovl11D5224LargeView;

typedef struct {
    u8 pad0[0x7AEC];
    Ovl11PaddedVec3 position;
} Ovl11D5224SmallView;

/* Six-byte slot record at D_8006C838+0x524C; only the leading halfword is
 * read by ovl_11_func_800F9BE4. */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} Ovl11Slot524C;

/* D_8006C838 view of the eighteen slot records at +0x524C (the D_80071A84 /
 * D_80071A8A aliases are slots 0 and 1), read by ovl_11_func_800F9BE4. */
typedef struct {
    char pad_000[0x524C];
    Ovl11Slot524C slots[18];
} Ovl11Slots524CView;

/* ovl_11_func_80110188 arg0 view: the s32 at +0x38 and the s32 at +0x40
 * select the alternate half of the keyed-position table. */
typedef struct {
    /* 0x00 */ char pad_00[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ char pad_3C[0x4];
    /* 0x40 */ s32 unk40;
} Ovl11StateArg0;

/* ovl_11_func_80116F4C object view. The builder reads a signed halfword at
 * +0x1A (text source), an unsigned halfword at +0x16 divided by 25, signed
 * halfwords at +0x1C and +0x1E, and tests bit 0x40 of the flag word at +0x34.
 * Only the witnessed fields are named. */
typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ char pad_18[2];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ char pad_20[0x14];
    /* 0x34 */ s32 unk34;
} Ovl11Func80116F4CArg;

/* ovl_11_func_800CE37C parameter view: the coordinate words written at
 * +0x100, +0x104 and +0x108. */
typedef struct {
    /* 0x00 */ char pad_00[0x100];
    /* 0x100 */ s32 unk100;
    /* 0x104 */ s32 unk104;
    /* 0x108 */ s32 unk108;
} Ovl11CE37CArg0;

/* ovl_11_func_800CE37C stack result of func_8001F278: three s32 words. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} Ovl11CE37CSample;

/* ovl_11_func_80109188 argument: signed state fields and flags at +0x34. */
typedef struct {
    char pad_00[0x24];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[6];
    s32 unk34;
} Ovl11PrimaryStageHandlerObj;

/* ovl_11_func_800DF010/800E0C24: state fields, flags, and reset halfword. */
typedef struct {
    char pad_00[0x26];
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x34 - 0x2E];
    s32 unk34;
    char pad_38[0xB6 - 0x38];
    s16 unkB6;
} Ovl11StageHandlerObj;

/* Rotation-source view: F20 reads flags/id, E38 reads direction, and 7044
 * reads the signed sprite index at +0xC. All original widths are retained. */
typedef struct {
    u16 flags;
    char pad2[6];
    u16 direction;
    s16 id;
    s16 sprite;
} Ovl11RotationSource;

/* Three halfwords at +0xC/+0xE/+0x10 in the rotation state. */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Ovl11RotationVector;

/* 0x18-byte state: reset/step share the full record; signed hold timer and
 * step widths match the original accesses. Padding halfwords are untouched. */
typedef struct Ovl11RotationState {
    u16 state;
    u16 direction;
    s16 holdTicks;
    s16 unused6;
    s32 angularStep;
    Ovl11RotationVector rotation;
    s16 unused12;
    Ovl11RotationSource *source;
} Ovl11RotationState;

/* 80107480 reads two u8 selectors, two u16 payloads and a u16 result. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Ovl11GuardRecord;

/* ovl_11_func_800F5BC0 reads these halfwords while walking 0x18-byte records. */
typedef struct {
    s16 unk0;
    u16 unk2;
    u16 unk4;
    u16 pad6;
    u16 unk8;
    u16 padA;
    u16 unkC;
    u16 padE;
    u16 unk10;
    u8 pad12[0x18 - 0x12];
} M2C_0584a699a1f0_Ovl11Func5BC0Entry;

/* 800F5944 copies six words, tests the low halfword at +4 and adds to +0xC. */
typedef struct Ovl11DispatchRecord {
    s16 id;
    u16 kind;
    u16 flags;
    u16 pad6;
    s32 x;
    s32 y;
    s32 z;
    s32 unk14;
} Ovl11DispatchRecord;

/* 80133454 walks 20-byte records: pointer +0, lh lower bound +4,
 * lw upper bound +8, lbu width +C, lb id +D, and text pointer +10.
 * 80130C20's automatic initializer leaves +6/+E as implicit padding. */
typedef struct Ovl30ValueMenuEntry {
    u8 *value;
    s16 lower;
    s32 upper;
    u8 width;
    s8 id;
    char *text;
} Ovl30ValueMenuEntry;

/* 80133A8C walks 20-byte menu records: pointer +0, mask +8, width +C,
 * signed selection id +D and display text +10. */
typedef struct Ovl30FlagMenuEntry {
    void *value;
    u8 pad4[4];
    s32 mask;
    u8 width;
    s8 id;
    u8 padE[2];
    char *text;
} Ovl30FlagMenuEntry;

/* 800D7B8C keeps a pointer to the grid entry's four-byte state at +4. */
typedef struct {
    u8 stage;
    u8 days;
    u16 flags;
} Ovl11GrowthState;

/* 800DA390 resets u16 tags at +0/+2, bytes at +4/+5 and u16 flags at +6. */
typedef struct {
    u16 unk0;
    u16 unk2;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} Ovl11GridResetEntry;

/* 800D2160 passes the record's +0xA8 pair to 80107DEC, whose matched
 * definition reads/decrements the signed halfword at +2. */
typedef struct {
    s16 field_0;
    s16 field_2;
} Ov11Timer;

/* 801090A4's matched object view, passed by 800D2334. */
typedef struct {
    u16 unk0;
    char pad_02[0x22];
    u16 unk24;
    u16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x06];
    s32 unk34;
} Ovl11HandlerObjectView;

/* 800D266C's matched parameter view, also passed by 800D2334. */
typedef struct {
    char pad_00[0x26];
    s16 field_26;
    char pad_28[0x34 - 0x28];
    s32 field_34;
} Ov11D266CFields;

/* 80109188's matched parameter view, also passed by 80108D84. */
typedef struct {
    char pad_00[0x24];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x06];
    s32 unk34;
} Ovl11D04D4Obj;

/* 800CA580 and 8010C1C0 copy these four words from the drawing object. */
typedef struct {
    char pad0[0x100];
    s32 unk100;
    s32 unk104;
    s32 unk108;
    s32 unk10C;
} Ovl11DrawState;

/* 8011031C: u16 direction +22, halfword-aligned tag +2E,
 * coordinates +38/+40 and cleared action state +DC. */
typedef struct {
    char pad0[0x22];
    u16 unk22;
    char pad24[0x2E - 0x24];
    Ovl11CollisionTag collision;
    char pad32[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    char pad44[0xDC - 0x44];
    s32 unkDC;
} Ovl11ActionProbeObject;

/* 800D4314: state/timer +28/+2A, halfword-aligned tag +2E,
 * position +38 and velocity +48; direction +22 excludes value 1. */
typedef struct {
    char pad0[0x22];
    u16 direction;
    s16 unk24;
    s16 unk26;
    s16 state;
    u16 timer;
    s16 unk2C;
    Ovl11CollisionTag collision;
    char pad32[2];
    s32 flags;
    s32 position[3];
    char pad44[4];
    Recon800D0408A1View velocity;
} Ovl11D4314Object;

/* 800C580C: state +3C, flags +6C, item +8A and script pointer +4C. */
typedef struct {
    u8 pad0[0x18];
    u16 power;
    u8 pad1A[4];
    s16 kind;
    u16 slot;
    u8 pad22[0x36 - 0x22];
    u16 attributes;
    u16 direction;
    u8 pad3A[2];
    u16 state;
    u8 pad3E[0x4C - 0x3E];
    void *script;
    u8 pad50[0x6C - 0x50];
    s32 unk6C;
    u8 pad70[0x84 - 0x70];
    u16 value;
    u16 amount;
    s16 timer;
    s16 item;
    u8 pad8C[0xFC - 0x8C];
    u16 collisionType;
    u16 collisionId;
    s32 position[4];
    s32 pad110;
    s32 timer114;
    u8 pad118[0x264 - 0x118];
    u8 phase;
} Ovl11C580CObject;

/* Parameter views used by 800C580C: layouts from the selected callee access witnesses. */
typedef struct {
               u8 unk0[4];
               s16 unk4;
} C580C_7f0a9cc905ed_UnkStruct800D72E8;

typedef struct {
               u16 unk0;
               u16 unk2;
} C580C_b6c4596908ca_Ovl11CF314Arg1;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} C580C_b6c4596908ca_Ovl11CF314Arg2;

typedef struct {
               char pad0[0x38];
               u16 unk38;
               char pad3A[0x100 - 0x3A];
                s32 unk100;
                s32 unk104;
                s32 unk108;
} C580C_ad72053e5aa9_StructOvl11CE034A;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} C580C_ad72053e5aa9_StructOvl11CE034B;

typedef struct {
               s16 unk0;
               s16 unk2;
               s16 unk4;
               s16 unk6;
               s16 unk8;
               char pad0A[0x0C - 0x0A];
               s32 unkC;
               s32 unk10;
               s32 unk14;
               s32 unk18;
               s32 unk1C;
               char pad20[0x24 - 0x20];
               s32 unk24;
               s32 unk28;
               s32 unk2C;
} C580C_f3fe4d9631b5_UnkStruct800CBF40;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} C580C_f3fe4d9631b5_UnkStruct800CBF70Arg0;

typedef struct {
               char pad0[0x36];
               u16 flags;
               char pad38[0x40 - 0x38];
               u16 vals[(0x84 - 0x40) / 2];
               s16 value;
} C580C_583b0b68ff5a_UnkStruct800CD5BC;

/* 80110494's matched parameter layout, also passed by 8010F80C. */
typedef struct {
    char pad0[0x24];
    u16 unk24;
    char pad26[4];
    s16 unk2A;
    char pad2C[4];
    s16 unk30;
    char pad32[0x48];
    u16 unk7A;
} Ovl11StateKeyProbeView;

/* 801103E8's matched parameter layout, also passed by 8010F80C. */
typedef struct {
    char pad0[0x22];
    u16 unk22;
    char pad24[0x38 - 0x24];
    s32 unk38;
    char pad3C[4];
    s32 unk40;
} Ovl11StateDirectionView;

/* 800D55F8: direction +22, state/timers +28/+2A/+2C, map +30 and flags +34. */
typedef struct {
    char pad0[0x22];
    u16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad2E[2];
    s16 unk30;
    char pad32[2];
    s32 unk34;
} Ovl11D55F8Object;

/* 8010EEE4: movement flags/index +B8/+BA and retained point +BE/+C0/+C2. */
typedef struct {
    char pad0[0xB8];
    u16 flags;
    s16 index;
    u16 unkBC;
    u16 saved26;
    u16 saved28;
    u16 saved2A;
} Ovl11EEE4Object;

/* 80110118's matched parameter view, also passed by 8010F6C0. */
typedef struct {
    char pad_00[0x30];
    s16 unk30;
    char pad_32[0xB6 - 0x32];
    u16 unkB6;
    char pad_B8[0xDC - 0xB8];
    s32 unkDC;
} Ovl11ActionCompletionView;

/* 8010A47C: action state +28, timer +2C, flags +34 and four-word
 * motion state +58; 800D04D4 also reads the key +30 and flags +7A. */
typedef struct {
    char pad_00[0x24];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[2];
    s16 unk30;
    char pad_32[2];
    s32 unk34;
    char pad_38[0x58 - 0x38];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    char pad_68[0x7A - 0x68];
    u16 unk7A;
} Ovl11A47CObject;

/* 800DF010/800E0C24 matched dispatch views, called by 8010A47C. */
typedef struct {
    char pad_00[0x26];
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x34 - 0x2E];
    s32 unk34;
    char pad_38[0xB6 - 0x38];
    s16 unkB6;
} Ovl11ActionDispatchView;

/* 800F413C's matched parameter views, also passed by 8010A850. */
typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} Ovl11F413CArg0View;

typedef struct {
    s16 unk0;
    char pad2[2];
    u16 unk4;
} Ovl11F413CArg1View;

/* 800F1078 walks 0x24-byte records; widths follow its lbu/lhu/lw loads. */
typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u8 unkC;
    u8 unkD;
    u16 unkE;
    u32 unk10;
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
} Ovl11ConditionRecord;

#endif /* GAME_TYPES_H */
