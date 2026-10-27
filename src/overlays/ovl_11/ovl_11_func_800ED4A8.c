#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "pad_actuator_gate.h"
#include "pad_actuator_sequences.h"
#include "sprite_rotation.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "message_window.h"
#include "ovl11_entity_pool_access.h"
#include "ovl11_item_transfer_menu.h"
#include "sprite_source.h"
#include "random.h"
#include "sprite_render.h"
#include "ovl11_state_field_access.h"
#include "ovl11_interaction_volume_query.h"
#include "integer_power.h"
#include "game_flags.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_grouped_value_mapping.h"
#include "point_projection.h"
#include "ovl11_paired_item_ids.h"
#include "ovl11_timed_sprite_effect.h"
#include "selection_panel.h"
#include "area_grid_triggers.h"
#include "psyq/abs.h"
#include "ovl11_object_events.h"
#include "ovl11_counter_choice.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_moving_items.h"
#include "ovl11_work_position_commands.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_item_record_lists.h"
#include "ovl11_default_position.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_pending_stage_dispatch.h"
#include "psyq/libetc.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_object_record_clear.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_scene_header_key.h"
#include "ovl11_seasonal_record_ids.h"
#include "surface_collision.h"
#include "ovl11_primary_object_input_override.h"
#include "ovl11_status_gauge_deltas.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_timed_effect_record_fill.h"
#include "ovl11_interaction_record_prepare.h"
#include "ovl11_primary_object_item_sprite_source.h"
#include "ovl11_object_state_callbacks.h"

typedef struct {
               s16 field_0;
               s16 field_2;
} M2C_42071334137b_Ov11Timer;

typedef struct {
               u8 unk0[4];
               s16 unk4;
} M2C_7f0a9cc905ed_UnkStruct800D72E8;

typedef struct {
               u16 unk0;
               u16 unk2;
} M2C_b6c4596908ca_Ovl11CF314Arg1;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_b6c4596908ca_Ovl11CF314Arg2;

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
} M2C_f3fe4d9631b5_UnkStruct800CBF40;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_f3fe4d9631b5_UnkStruct800CBF70Arg0;

typedef struct {
               char pad0[0x36];
               u16 flags;
               char pad38[0x40 - 0x38];
               u16 vals[(0x84 - 0x40) / 2];
               s16 value;
} M2C_583b0b68ff5a_UnkStruct800CD5BC;

typedef struct {
              s32 unk0;
              s32 unk4;
              s32 unk8;
} M2C_cec44150eede_Ovl11Func800BDCE8Arg1;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} M2C_38c89582ddcb_M2C_4c1a205b3b09_ReconA0View;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} M2C_4c1a205b3b09_ReconA0View;

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
    s32 unk0;
    s32 unk4;
    s32 unk8;
} M2C_93d07a715929_Ovl11SelectArg;

int MoveImage2 (RECT *rect, int x, int y);
int csin (int a);
void SetPadActuatorSequenceAddress (s32 arg0, s32 arg1);
void ResetPadActuatorSequence (s32 arg0);
void Ovl11RemapPrimaryDirectionButtons (void);
void ovl_11_func_800CD08C(Recon_ovl_11_func_800CD08C_A0View *arg0);
void ovl_11_func_800CCF14 (M2C_7f0a9cc905ed_UnkStruct800D72E8 *arg0);
s32 ovl_11_func_800CD3C4 (s32 arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
void ovl_11_func_800CD5BC (M2C_583b0b68ff5a_UnkStruct800CD5BC *arg0, s32 arg1);
void Ovl11SelectPrimaryAnimationForFlag10000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag08000000 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryState2Animation (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void Ovl11SelectPrimaryAnimationForFlag100 (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
s32 ovl_11_func_800FBE4C (void);
s32 ovl_11_func_8010B57C (u16 *arg0, s32 arg1);
s32 Ovl11TryBeginIdleState54 (Recon_ovl_11_func_800C97D0_A0View *arg0);
void ovl_11_func_800CBE14 (s32 arg0);
s32 Ovl11MapDirectionButtons (u16 arg0, u16 arg1);
s32 ovl_11_func_800DF010 (M2C_8d3156bb36aa_Struct_800DF010 *arg0, s32 arg1);
s32 ovl_11_func_80109188 (M2C_708e65b63435_Ovl11D04D4Obj *arg0, s32 arg1);
s32 Ovl11IsItemIdEnabled (s32 arg0);
s32 ovl_11_func_800E5B84 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5704 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5C60(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E63C8(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6D2C (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6BDC(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6CAC(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_801000D4 (s16 arg0);
s32 ovl_11_func_800E5ADC (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6B18 (s16 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800E58DC (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5870(s16 arg0, s16 arg1, s32 arg2, s16 arg3);
s32 ovl_11_func_800E9AFC (s16 arg0, s16 arg1, s32 arg2);
s32 ovl_11_func_800E99EC (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6AB0 (s32 arg0, s32 arg1, s32 arg2);
s32 Ovl11ClampStateField (s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800ED4A8(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_s0;
    s32 var_s4;
    s32 temp_v0;
    u8 *var_s2;
    u8 *base;
    s32 var_s6;
    s32 tpred;

    var_s0 = 1;
    tpred = arg0 - 0x1F;
    var_s6 = (u32) tpred < 2U;
    var_s4 = 0x20000;
    base = (u8 *) D_8006C838;
    var_s2 = base + 0x1D4;
    for (; var_s0 < 0x25; var_s0 = temp_v0 >> 0x10) {
        if ((*(u16 *) (var_s2 + 0x9A06)) & 0x800) {
            if (var_s6) {
                if (Ovl11IsSpecialCounterSlot(var_s0) == 1) {
                    Ovl11UpdateStateField(0x1F, var_s0, arg3, (s32) arg1);
                } else {
                    Ovl11UpdateStateField(0x20, var_s0, arg2, (s32) arg1);
                }
            } else {
                Ovl11UpdateStateField((s32) arg0, var_s0, arg2, (s32) arg1);
            }
        }
        temp_v0 = var_s4;
        var_s4 += 0x10000;
        var_s2 = var_s2 + 0x1D4;
    }
    return 1;
}
