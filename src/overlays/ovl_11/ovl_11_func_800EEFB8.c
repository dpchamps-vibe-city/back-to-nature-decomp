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
#include "ovl11_record_slot_table.h"
#include "ovl11_object_state_dispatch.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_object_record_clear.h"
#include "surface_collision.h"
#include "ovl11_direction_vector.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_primary_object_item_sprite_source.h"
#include "ovl11_record_partition_flags.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_condition_record_fields.h"
#include "ovl11_season_mask.h"
#include "ovl11_selected_record_pair_commands.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_record_pair_slot_output.h"
#include "ovl11_interaction_state_access.h"
#include "ovl11_record_family_lookup.h"

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

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} M2C_800EEFB8_Arg1;

typedef struct {
    char pad_000[0x44B8];
    u16 field_44B8;
    s16 field_44BA;
    s16 field_44BC;
} M2C_800EEFB8_C838View;

int CdControl (u_char com, u_char *param, u_char *result);
void CdFlush (void);
CdlLOC *CdIntToPos (int i, CdlLOC *p);
int CdRead (int sectors, u_long *buf, int mode);
void CdReadBreak (void);
int CdReadSync (int mode, u_char *result);
int CdSync (int mode, u_char *result);
void *memmove (unsigned char *, const unsigned char *, int);
s32 ovl_11_func_800EF8BC (s16 arg0, s16 arg1, s16 arg2);
void DecodeAndUploadSpriteImages (u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void Ovl11SelectPrimaryAnimationForFlag10000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag08000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryState2Animation (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag100 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
s32 ovl_11_func_80109188 (M2C_708e65b63435_Ovl11D04D4Obj *arg0, s32 arg1);
s32 Ovl11IsItemIdEnabled (s32 arg0);
s32 Ovl11ResetStaleRecordSlot(s32 arg0);
s32 Ovl11ServiceRecordSlot (s32 arg0);
s32 ovl_11_func_800EFA1C (s16 arg0);
void ovl_11_func_80103830 (void);
s32 ovl_11_func_801037DC (void);
s32 ovl_11_func_801165C8 (void);
s32 ovl_11_func_80103770 (void);
void ClearActiveOrderingTables (void);
void ovl_11_func_800EF870 (s16 arg0);
s32 Ovl11ConditionDayWindowMatches (s32 arg0, s32 arg1, s32 arg2);
s32 Ovl11ConditionHourWindowMatches (u16 arg0, u16 arg1, s16 *arg2);
s32 Ovl11ConditionValueRangeMatches (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Ovl11ConditionCounterMatches (u8 arg0, u16 arg1, s32 arg2);
s32 Ovl11ConditionFlagIdsMatch (u32 arg0, s32 arg1);
s32 Ovl11ConditionFlagPairMatches (s32 arg0);
s32 Ovl11ConditionCodeIsAvailable (s32 arg0);
s32 Ovl11QueueConditionResultFirstSlot (s32 arg0, s32 arg1);
s32 ovl_11_func_800EEBF4 (s32 arg0);
s32 Ovl11ConditionObjectStateMatches (s32 arg0);
s32 Ovl11HasInactiveConditionRowCode (u16 arg0);
void Ovl11QueueImmediateEvent (s16 kind, s16 value);
void ovl_11_func_801037EC (void);

void ovl_11_func_800EEFB8(s16 arg0, M2C_800EEFB8_Arg1 *arg1) {
    s16 var_s0;
    s16 var_s1;
    s32 var_v1;
    s32 temp_v1;
    s32 var_s2;
    s32 var_s5;

    var_s5 = 0;
    var_s1 = ((M2C_800EEFB8_C838View *)D_8006C838)->field_44BA;
    var_s0 = ((M2C_800EEFB8_C838View *)D_8006C838)->field_44BC;
    if (arg0 == 4) {
        var_v1 = ((M2C_800EEFB8_C838View *)D_8006C838)->field_44B8;
        arg1->unk2 = 0;
        arg1->unk4 = 0x1D;
        var_v1 += 1;
        arg1->unk0 = var_v1;
    } else {
        var_s0 += 6;
        var_s2 = 0x10000;
        do {
            var_s0 += 1;
            if (var_s0 >= 0x1E) {
                var_s0 -= 0x1E;
                var_s1 += 1;
                if (var_s1 >= 4) {
                    var_s1 -= 4;
                    temp_v1 = var_s2;
                    var_s2 += 0x10000;
                    var_s5 = temp_v1;
                    var_s5 = var_s5 >> 0x10;
                }
            }
            ovl_11_func_800EF8BC(arg0, var_s1, var_s0);
        } while (Ovl11FindCalendarDateExtended(var_s1, var_s0) != 0);
        var_v1 = var_s5 + ((M2C_800EEFB8_C838View *)D_8006C838)->field_44B8;
        arg1->unk2 = var_s1;
        arg1->unk4 = var_s0;
        arg1->unk0 = var_v1;
    }
}
