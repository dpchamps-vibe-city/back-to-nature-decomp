#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "pad_actuator_sequences.h"
#include "card_deal.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "text_entry.h"
#include "sprite_source.h"
#include "random.h"
#include "sprite_upload.h"
#include "ovl11_state_field_access.h"
#include "integer_power.h"
#include "game_flags.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_record_flag_query.h"
#include "ovl11_record_pair_slots.h"
#include "psyq/abs.h"
#include "ovl11_object_events.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_condition_masks.h"
#include "ovl11_progress_status.h"
#include "ovl11_record_flag_counters.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_object_state_dispatch.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_object_record_clear.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} M2C_a34e94965883_M2C_800EEFB8_Arg1;

typedef struct {
               char pad0[0x38];
               u16 unk38;
               char pad3A[0x100 - 0x3A];
                s32 unk100;
                s32 unk104;
                s32 unk108;
} M2C_ad72053e5aa9_StructOvl11CE034A;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_ad72053e5aa9_StructOvl11CE034B;

typedef struct {
    char pad_00[0x24];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x06];
    s32 unk34;
} M2C_708e65b63435_Ovl11D04D4Obj;

typedef struct {
               s16 unk0;
               s16 unk2;
} M2C_d453384c461b_UnkStruct800D0DBC;

int CdControl (u_char com, u_char *param, u_char *result);
void CdFlush (void);
CdlLOC *CdIntToPos (int i, CdlLOC *p);
int CdRead (int sectors, u_long *buf, int mode);
void CdReadBreak (void);
int CdReadSync (int mode, u_char *result);
int CdSync (int mode, u_char *result);
void *memmove (unsigned char *, const unsigned char *, int);
void ovl_11_func_800EEFB8 (s16 arg0, M2C_a34e94965883_M2C_800EEFB8_Arg1 *arg1);
void ovl_11_func_800EF870 (s16 arg0);
void DecodeAndUploadSpriteImages (u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void SetVal8005E2E8 (s32 arg0);
s32 func_8001E38C (void);
void ovl_11_func_800CCFF8(Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1);
void Ovl11SelectPrimaryAnimationForFlag10000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag08000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryState2Animation (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag100 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
s32 ovl_11_func_80109188 (M2C_708e65b63435_Ovl11D04D4Obj *arg0, s32 arg1);
s32 Ovl11IsItemIdEnabled (s32 arg0);
s32 ovl_11_func_800D2E20 (void);
void ovl_11_func_800DF0F8 (s32 arg0);
void ovl_11_func_800E2904 (s32 arg0);
s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s32 ovl_11_func_80118C6C (void);
s32 Ovl11ResetStaleRecordSlot(s32 arg0);
s32 Ovl11ServiceRecordSlot (s32 arg0);
void ovl_11_func_800F4AC0 (s32 arg0);
s32 ovl_11_func_800F4B14 (void);
s32 ovl_11_func_800EFA1C (s16 arg0);
void ovl_11_func_80103830 (void);
s32 ovl_11_func_801037DC (void);
s32 ovl_11_func_801165C8 (void);
s32 ovl_11_func_80103770 (void);
void ovl_11_func_800F0358 (void);
s32 ovl_11_func_800F0474 (s16 arg0);
s32 ovl_11_func_80118C7C (s16 arg0, s16 arg1, s16 arg2);
void ClearActiveOrderingTables (void);
s32 ovl_11_func_800E9104 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 Ovl11TestStateFieldCondition (s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800EF8BC (s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800F14D8 (s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800F144C (u16 arg0, u16 arg1, s16 *arg2);
s32 ovl_11_func_800F15B4 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 ovl_11_func_800F1744 (u8 arg0, u16 arg1, s32 arg2);
s32 ovl_11_func_800F1878 (u32 arg0, s32 arg1);
s32 ovl_11_func_800F1954 (s32 arg0);
s32 ovl_11_func_800F1AE0 (s32 arg0);
s32 ovl_11_func_800F12D0 (s32 arg0, s32 arg1);
s32 ovl_11_func_800EEBF4 (s32 arg0);
s32 ovl_11_func_800F1EE4 (s32 arg0);
void ovl_11_func_800F1E44 (u32 arg0, s16 *arg1, s32 arg2);
s32 ovl_11_func_800F19C8 (s32 arg0);
s32 ovl_11_func_800F13D8 (u16 arg0);
void Ovl11QueueImmediateEvent (s16 kind, s16 value);
void ovl_11_func_801037EC (void);
s32 ovl_11_func_800EC354 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F0FB0 (s32 arg0);

extern M2C_a34e94965883_M2C_800EEFB8_Arg1 D_80071CC0;
extern M2C_a34e94965883_M2C_800EEFB8_Arg1 D_8007AD10[];

/* Only a0/a1 are inputs: the original defines a2 before every use and never
 * reads a3. Both callers pass two arguments; no trailing parameters exist. */
void ovl_11_func_800EF6E4(s16 arg0, s16 arg1) {
    M2C_a34e94965883_M2C_800EEFB8_Arg1 *temp_s0;
    u8 *temp_v1;
    char *base;
    s16 var_a2;
    s16 var_s0;
    s16 var_s1;
    s16 var_s2;
    u8 *temp_v0;

    var_s0 = arg0;
    var_s2 = arg1;
    if (var_s2 == 5) {
        var_s2 = 4;
        var_s0 = 0;
        SetGameFlag(0x25U, 1U);
        base = (char *) D_8006C838;
        base += 0x8000;
        (*(u16 *) (base + 0x47BA)) = (u16) ((*(u16 *) (base + 0x47BA)) | 8);
        (*(u16 *) (base + 0x3E96)) = (u16) ((*(u16 *) (base + 0x3E96)) | 8);
    }
    if (var_s0 == 2) {
        var_a2 = 0x29;
        var_s1 = var_s2;
    } else {
        switch (var_s2) {
        case 0:
            var_s1 = 3;
            var_a2 = 0x13;
            break;
        case 1:
            var_s1 = 7;
            var_a2 = 0xB;
            break;
        case 2:
            var_s1 = 9;
            var_a2 = 8;
            break;
        case 3:
            var_s1 = 0xE;
            var_a2 = 5;
            break;
        case 4:
        default:
            var_s1 = 0x14;
            var_a2 = 0x19;
            break;
        }
    }
    if (var_s0 != 0) {
        temp_s0 = &D_80071CC0;
        temp_s0->unk8 = 0x29;
        temp_s0->unkA = var_s1;
        ovl_11_func_800EEFB8(-1, temp_s0);
        ovl_11_func_800EF870(var_s2);
        temp_v0 = (u8 *) temp_s0 - 0x5488;
        temp_v1 = temp_v0 + var_s1 * 0x1D4;
        (*(u16 *) (temp_v1 + 0x9A06)) = (u16) ((*(u16 *) (temp_v1 + 0x9A06)) | 8);
        return;
    }
    temp_s0 = &D_8007AD10[var_s2];
    temp_s0->unk8 = var_a2;
    temp_s0->unkA = var_s1;
    ovl_11_func_800EEFB8(var_s2, temp_s0);
}
