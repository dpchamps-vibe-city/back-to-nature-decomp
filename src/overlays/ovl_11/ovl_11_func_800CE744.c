#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libsnd.h"
#include "psyq/memory.h"
#include "psyq/libapi.h"
#include "packet_work.h"
#include "aged_item_pool.h"
#include "cd_audio.h"
#include "ovl11_clock_calendar.h"
#include "sprite_render.h"
#include "sprite_source.h"
#include "random.h"
#include "debug_break.h"
#include "game_flags.h"
#include "psyq/abs.h"
#include "ovl11_object_event_script.h"
#include "point_projection.h"
#include "ovl11_tagged_record_creation.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_code160_record_creation.h"
#include "ovl11_tagged_record_presentation.h"
#include "ovl11_random_state_choice.h"
#include "ovl11_projected_record_sprites.h"

typedef struct {
               s16 unk0;
               u16 unk2;
} M2C_d4baf9e5b2ba_SpriteRef;

typedef struct {
               s16 unk0;
               s16 unk2;
               s16 unk4;
               s16 unk6;
} M2C_d4baf9e5b2ba_SpriteTex;

typedef struct {
               u16 unk0;
               u16 unk2;
               u8 unk4;
               u8 unk5;
               u16 unk6;
               u8 pad8[0x14];
               M2C_d4baf9e5b2ba_SpriteTex *unk1C;
               M2C_d4baf9e5b2ba_SpriteRef *unk20;
               u8 *unk24;
               M2C_d4baf9e5b2ba_SpriteRef *unk28;
               u8 *unk2C;
} M2C_d4baf9e5b2ba_SpriteSourceData;

typedef struct {
    s16 field_00;
    u16 field_02;
} M2C_ab2039654fca_Group;

typedef struct {
    s16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
} M2C_ab2039654fca_Vertex;

typedef struct {
    u16 field_00;
    u16 field_02;
    u16 field_04;
    u16 field_06;
    u16 field_08;
    u16 field_0A;
    s16 field_0C;
    s16 field_0E;
    s16 field_10;
    s16 field_12;
    u32 pad_14;
    u32 pad_18;
    M2C_ab2039654fca_Vertex *field_1C;
    M2C_ab2039654fca_Group *field_20;
    u8 *field_24;
    M2C_ab2039654fca_Group *field_28;
    u8 *field_2C;
    s32 field_30;
} M2C_ab2039654fca_SourceData;

typedef struct {
               s16 field_0;
               s16 field_2;
} M2C_42071334137b_Ov11Timer;

typedef struct {
    char pad_00[0x26];
    s16 field_26;
    char pad_28[0x34 - 0x28];
    s32 field_34;
} M2C_d59586abe320_Ov11D266CFields;

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
} M2C_8d3156bb36aa_Struct_800DF010;

typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
} M2C_057dca34f681_Struct_800D03B4;

typedef struct {
               u16 unk0;
} M2C_a5df3a0a31de_UnkStruct800DF4F0;

typedef struct {
               u16 unk0;
} M2C_1023b6857e0a_UnkStruct800E109C;

typedef struct {
               u16 unk0;
               char pad_02[0x24];
               s16 unk26;
               char pad_28[0xC];
               s32 unk34;
               char pad_38[0x76];
               u16 unkAE;
} M2C_bd4797768b62_Struct_800E2AF0;

typedef struct {
    u16 field_0;
    u16 field_2;
} M2C_e88a4985b4f4_Cell4;

int CdControlB (u_char com, u_char *param, u_char *result);
void CdFlush (void);
int CdPosToInt (CdlLOC *p);
void SsSetSerialVol (char, short, short);
void SystemError (char, long);
s16 *ovl_11_func_800E1F9C (s32 arg0, s32 arg1, s32 arg2);
Ovl11ObjectInitRecord *ovl_11_func_800D2F48 (s32 arg0);
struct_800759E4 *ovl_11_func_80108CD0 (s32 arg0, s32 arg1);
u16 *ovl_11_func_8010B1C4 (s32 arg0);
void ovl_11_func_800D2D54 (void *arg0);
void SetSpriteSourceHeaderAndRecord (SpriteSourceData *src, s32 header, s32 field_18);
void ovl_11_func_800CFD9C (void);
s32 ovl_11_func_800D2E20 (void);
s32 ovl_11_func_800D2950 (s32 arg0);
void func_80016054 ();
s32 Ovl11SubmitObjectMarker (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80015DD4 ();
void ovl_11_func_800C9E90 (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800E2904 (s32 arg0);
void ovl_11_func_800E0450 (s32 arg0);
s32 ovl_11_func_800E276C (s16 *arg0);
void ovl_11_func_800E1F70 (s32 arg0);
void ovl_11_func_800D2F1C (s32 arg0);
void ovl_11_func_800D266C (M2C_d59586abe320_Ov11D266CFields *arg0, u16 *arg1, s32 arg2);
void ovl_11_func_80108CA4 (s32 arg0);
void ovl_11_func_8010B198 (s32 arg0);
void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_800CBDFC (void);
s32 ovl_11_func_800DF010 (M2C_8d3156bb36aa_Struct_800DF010 *arg0, s32 arg1);
s32 ovl_11_func_8010B57C (u16 *arg0, s32 arg1);
s32 ovl_11_func_800D3424 (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
void ovl_11_func_800D03B4 (M2C_057dca34f681_Struct_800D03B4 *arg0, M2C_057dca34f681_Struct_800D03B4 *arg1, s32 arg2);
s32 ovl_11_func_800D3468 (u16 *arg0);
s32 ovl_11_func_800D3104 (u16 *arg0, s32 arg1);
s32 ovl_11_func_800DF3BC (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
s32 ovl_11_func_800DF4F0 (M2C_a5df3a0a31de_UnkStruct800DF4F0 *arg0);
s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s16 ovl_11_func_800DEEE0(Recon_ovl_11_func_800DEEE0_A0View *arg0);
s32 ovl_11_func_800E0F8C (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
s32 ovl_11_func_800E109C (M2C_1023b6857e0a_UnkStruct800E109C *arg0);
s32 ovl_11_func_800E0FD0 (Recon_ovl_11_func_8010CE80_A0View *arg0);
s32 ovl_11_func_800E0AFC ();
s32 ovl_11_func_800E2AAC (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
s32 ovl_11_func_800E2A30 (s16 *arg0);
s32 ovl_11_func_800E2AF0 (M2C_bd4797768b62_Struct_800E2AF0 *arg0);
void ovl_11_func_8010B64C (s32 arg0);
s32 ovl_11_func_80109550 (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
s32 ovl_11_func_801098B0 (Ovl11Func801098B0Arg *arg0);
s32 ovl_11_func_801097F4 (Ovl11Func801097F4Arg x);
s32 ovl_11_func_8010B898 (u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
void Ovl11InitItemAge (M2C_e88a4985b4f4_Cell4 *arg0, s32 arg1);

u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1) {
    s32 var_a0;
    s32 var_s0;
    s32 var_v0;

    var_s0 = 0;
    switch (arg0) {
    case 0x160:
    case 0x161:
    case 0x162:
        var_s0 = Ovl11AllocateCode160Record(arg0, arg1, 1);
        break;
    case 0x163:
        var_a0 = 0x162;
        goto block_8;
    case 0x164:
    case 0x165:
        var_s0 = Ovl11AllocateTaggedRecord(arg0, arg1, 1);
        break;
    case 0x166:
        var_a0 = 0x165;
        goto block_8;
    case 0x109:
    case 0x10A:
        var_s0 = ovl_11_func_800E1F9C(arg0, arg1, 1);
        break;
    case 0x108:
    case 0x10B:
    case 0x10C:
    case 0x10D:
    case 0x10E:
    case 0x10F:
    case 0x110:
    case 0x112:
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x156:
    case 0x157:
    case 0x159:
    case 0x15B:
    case 0x17A:
    case 0x17B:
        var_a0 = arg0;
block_8:
        var_s0 = ovl_11_func_800D2F48(var_a0);
        break;
    default:
        break;
    case 0x106:
    case 0x107:
        var_s0 = ovl_11_func_80108CD0(arg0, 1);
        break;
    case 0x15E:
    case 0x15F:
        var_s0 = ovl_11_func_8010B1C4(arg0);
        break;
    case 0xA1:
    case 0xA2:
    case 0xA3:
        Ovl11AddAgedItem(arg0);
        break;
    }
    var_v0 = 0;
    if (var_s0 != 0) {
        ovl_11_func_800D2D54((void *) var_s0);
        var_v0 = var_s0;
    }
    return (u16 *) var_v0;
}
