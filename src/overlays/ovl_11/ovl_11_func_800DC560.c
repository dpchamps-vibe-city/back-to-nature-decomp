#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "sprite_rotation.h"
#include "game_flags.h"
#include "psyq/memory.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_vram_animation_slots.h"
#include "area_grid_triggers.h"
#include "psyq/abs.h"
#include "ovl11_object_events.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"

void ovl_11_func_80104A58 (void);
s32 ovl_11_func_800DC388 (s32 arg0);
void ovl_11_func_800DBFEC (u16 arg0, u16 arg1, s32 arg2);
void ovl_11_func_8010B67C (void);
void ovl_11_func_800CE5FC (void);

void ovl_11_func_800DC560(s32 arg0, s32 arg1) {
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_a0_3;
    s16 temp_s6;
    s16 temp_s7;
    u16 temp_s2;

    temp_s6 = ((Ovl11SetupWorkView *) &D_8007AFF0)->music;
    temp_s7 = ((Ovl11SetupWorkView *) &D_8007AFF0)->previous.field_0;
    ovl_11_func_80104A58();
    (((Ovl11SetupWorkView *) &D_8007AFF0)->id) = ovl_11_func_800DC388(((Ovl11SetupWorkView *) &D_8007AFF0)->id);
    if ((((Ovl11SetupWorkView *) &D_8007AFF0)->record) != NULL) {
        temp_s2 = *(u16 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->record);
        if (arg0 != 0) {
            (((Ovl11SetupResidentView *) D_8006C838)->x) = *(s16 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->record + 0x26);
            (((Ovl11SetupResidentView *) D_8006C838)->y) = *(s16 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->record + 0x28);
            (((Ovl11SetupResidentView *) D_8006C838)->z) = *(s16 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->record + 0x2A);
        }
        ovl_11_func_800DBFEC((u16) (((Ovl11SetupWorkView *) &D_8007AFF0)->id), temp_s2, arg1 & 0xFFFF);
        if (arg0 != 0) {
            Ovl11RestoreAreaGridPosition();
            if (GetGameFlag(0x42U) == 1) {
                (((Ovl11SetupResidentView *) D_8006C838)->x) = -0x2CBD;
                (((Ovl11SetupResidentView *) D_8006C838)->y) = -0x764;
                (((Ovl11SetupResidentView *) D_8006C838)->z) = 0x25EA;
                SetGameFlag(0x42U, 0U);
            }
            if (GetGameFlag(0x43U) == 1) {
                (((Ovl11SetupResidentView *) D_8006C838)->x) = -0x2A9F;
                (((Ovl11SetupResidentView *) D_8006C838)->y) = -0x695;
                (((Ovl11SetupResidentView *) D_8006C838)->z) = 0x241A;
                SetGameFlag(0x43U, 0U);
            }
        }
        if (temp_s2 & 0x8000) {
            (((Ovl11SetupWorkView *) &D_8007AFF0)->alternateId) = (s32) (((Ovl11SetupWorkView *) &D_8007AFF0)->id);
        } else {
            (((Ovl11SetupWorkView *) &D_8007AFF0)->normalId) = (s32) (((Ovl11SetupWorkView *) &D_8007AFF0)->id);
        }
    } else {
        ovl_11_func_800DBFEC((u16) (((Ovl11SetupWorkView *) &D_8007AFF0)->id), 0U, arg1 & 0xFFFF);
        (((Ovl11SetupWorkView *) &D_8007AFF0)->normalId) = (s32) (((Ovl11SetupWorkView *) &D_8007AFF0)->id);
        if ((((Ovl11SetupResidentView *) D_8006C838)->flags) & 0x2000) {
            Ovl11LoadIndexedSpriteRecords(0);
        }
    }
    ((Ovl11SetupWorkView *) &D_8007AFF0)->current = ((Ovl11SetupWorkView *) &D_8007AFF0)->previous;
    (((Ovl11SetupWorkView *) &D_8007AFF0)->header) = D_8009F78C;
    (((Ovl11SetupWorkView *) &D_8007AFF0)->buffer) = D_800A0368;
    if ((((Ovl11SetupWorkView *) &D_8007AFF0)->current.field_2) == 3) {
        ((struct Ovl11SceneHeader *) ((Ovl11SetupWorkView *) &D_8007AFF0)->header)->music = ((Ovl11SetupResidentView *) D_8006C838)->music;
    }
    temp_a0 = *(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 0xA);
    if (temp_s6 != temp_a0) {
        Ovl11LoadIndexedSpriteRecords((s32) temp_a0);
    }
    if (!((((Ovl11SetupResidentView *) D_8006C838)->control) & 0x10)) {
        if (temp_s7 == 0) {
            if ((*(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 0)) != 0) {
                if ((*(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 0xC)) != 0) {
                    Ovl11LoadCalendarSpriteSet(0xE, 0);
                }
                goto block_27;
            }
            goto block_23;
        }
block_23:
        if ((((Ovl11SetupWorkView *) &D_8007AFF0)->state) == 0x19) {
            if ((((Ovl11SetupResidentView *) D_8006C838)->control) & 0x2000) {
                Ovl11LoadCalendarSpriteSet(0xE, 0);
            }
            goto block_27;
        }
        goto block_28;
    }
block_27:
    if ((((Ovl11SetupWorkView *) &D_8007AFF0)->state) != 0x19) {
block_28:
        if (GetGameFlag(2U) != 1) {
            temp_a0_2 = *(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 0xC);
            if (temp_a0_2 != 0x3E7) {
                Ovl11LoadCalendarSpriteSet(temp_a0_2, 0);
            }
        } else if ((((Ovl11SetupWorkView *) &D_8007AFF0)->state) == 0x19) {
            goto block_33;
        }
    } else {
block_33:
        if ((((Ovl11SetupResidentView *) D_8006C838)->control) & 0x2000) {
            temp_a0_3 = *(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 0xC);
            if (temp_a0_3 != 0x3E7) {
                Ovl11LoadCalendarSpriteSet(temp_a0_3, 0);
            }
        }
    }
    (((Ovl11SetupResidentView *) D_8006C838)->control) = (s32) ((((Ovl11SetupResidentView *) D_8006C838)->control) & ~0x2000);
    Ovl11RequestVramAnimations((Ovl11SetupHeader *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header));
    if ((((Ovl11SetupWorkView *) &D_8007AFF0)->current.field_0) == 0) {
        Ovl11LoadIndexedSpriteSet(0);
    }
    if ((((Ovl11SetupWorkView *) &D_8007AFF0)->current.field_2) == 5) {
        Ovl11LoadIndexedSpriteSet(1);
    }
    ovl_11_func_8010B67C();
    Ovl11RebuildObjectEventsForTime();
    Ovl11RegisterPoolSpriteRotations((s32) (((Ovl11SetupWorkView *) &D_8007AFF0)->header));
    if (((*(s16 *) ((u8 *) (((Ovl11SetupWorkView *) &D_8007AFF0)->header) + 2)) == 1) && ((((Ovl11SetupResidentView *) D_8006C838)->time) >= 0x438)) {
        Ovl11LoadRenderContextSpriteBank();
        SetGameFlag(0x40U, 1U);
    }
    ovl_11_func_800CE5FC();
}
