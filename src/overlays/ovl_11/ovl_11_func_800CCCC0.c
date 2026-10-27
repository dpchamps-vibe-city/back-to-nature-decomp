#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_primary_object_requests.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_sixty_step_deltas.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_status_gauge_deltas.h"
#include "ovl11_active_record_slot_drain.h"
#include "ovl11_primary_object_level_bands.h"
#include "ovl11_object_area_sprite_reset.h"


void ovl_11_func_800C9D38 (void);

void ovl_11_func_800CCCC0(void) {
    struct Ovl11CCCC0State *temp_s0;
    Ovl11C7010Object *player = (Ovl11C7010Object *)&gPrimaryObjectBase;
    s32 temp_s1;
    s8 temp_s0_2;

    if (player->unk6C & 0x1000) {
        Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *)player);
    }
    player->unk22 = -1;
    temp_s0 = (struct Ovl11CCCC0State *)((u8 *)player - 0x51C8);
    player->unk1A = 0;
    player->unk18 = 0;
    player->unk24 = 0;
    player->unk38 = 0;
    player->unk6C &= 0xBD9B22A4;
    temp_s0->status = (s32) (temp_s0->status & 0xFFFE3FFF);
    ovl_11_func_800C9D38();
    Ovl11ResetPrimaryObjectRequest();
    SetGameFlag(0x3FU, 0U);
    SetGameFlag(0x40U, 0U);
    if (!(temp_s0->flags & 4)) {
        if (GetGameFlag(0x47U) != 1) {
            Ovl11ApplyStatusDelta(0, 0xA, 0, 0);
        } else {
            temp_s1 = temp_s0->angle + 0x5A0;
            temp_s1 -= player->unk2C;
            temp_s0_2 = Ovl11ComputeSixtyStepIncrease(temp_s1);
            Ovl11ApplyStatusDelta(temp_s0_2, Ovl11ComputeSixtyStepDecrease(temp_s1), 0, 0);
        }
    } else {
        if ((u32) ((u16) player->unk2C - 0x168) < 0x2D0U) {
            Ovl11ApplyStatusDelta(0x64, -0x19, 0, 0);
            Ovl11ApplyStatusDelta(0x64, 0, 0, 0);
        } else {
            Ovl11ApplyStatusDelta(0x32, -0x16, 0, 0);
        }
        ((struct Ovl11CCCC0State *)D_8006C838)->flags = (s32) (((struct Ovl11CCCC0State *)D_8006C838)->flags & ~4);
    }
    player->unk36 = 0;
    Ovl11UpdatePrimaryObjectLevelBands(player, 1);
    player->unk2A = (u16) ((struct Ovl11CCCC0State *)D_8006C838)->angle;
    if (((struct Ovl11CCCC0State *)D_8006C838)->time >= 8) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter = (u16) ((u16) ((struct Ovl11CCCC0State *)D_8006C838)->time + (((struct Ovl11CCCC0State *)D_8006C838)->counter - 8));
    }
    if ((s16) ((struct Ovl11CCCC0State *)D_8006C838)->counter >= 0x10) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter = 0xFU;
    }
    ((struct Ovl11CCCC0State *)D_8006C838)->pending = 0;
    SetGameFlag(0xAU, 0U);
    SetGameFlag(0x76U, 0U);
    Ovl11InitObjectSpriteSlots((u8 *)player);
    Ovl11InitSharedSpriteSlots((u8 *)player);
    SetGameFlag(0xA0U, 0U);
}
