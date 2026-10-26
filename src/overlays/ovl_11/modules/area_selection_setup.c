/*
 * Area selection resolution and installation (ovl_11:800DC388–800DC990).
 * The resolver chooses a resource ID; installation commits that choice and
 * the loaded record, position, sprite resources and scene callbacks. Event
 * flags 0x42/0x43 select seasonal IDs before installation consumes them for
 * position overrides. See ovl11_area_selection_setup.h for the public contract.
 * Shared work storage is borrowed, not defined or made private here.
 */

#include "common.h"
#include "ovl11_clock_calendar.h"
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
#include "ovl11_area_selection_setup.h"
#include "ovl11_scene_setup_install.h"
#include "psyq/libetc.h"
#include "ovl11_area_scene_load.h"

#define WORK ((Ovl11SetupWorkView *)&D_8007AFF0)
#define RESIDENT ((Ovl11SetupResidentView *)D_8006C838)
#define VARIANTS ((struct Ovl11MessageState *)D_8006C838)
#define SELECTION_HEADER ((struct Ovl11SpriteWork *)&D_8007AFF0)

s32 ovl_11_func_800E3D88(void);
s32 ovl_11_func_800DD274(void);

s32 Ovl11ResolveAreaSelection(s32 selection) {
    s32 id;

    id = selection;
    if (ovl_11_func_800E3D88() == 0x12D) {
        return gCalendarSeason + 0xBC;
    }
    if (id == 0) {
        id = ovl_11_func_800DD274();
    }
    if (id == 0x50) {
        s32 variant = VARIANTS->variant50 * 4 + 0x50;
        id = VARIANTS->base + variant;
    }
    if (id == 0x70) {
        id = VARIANTS->variant70 * 4 + VARIANTS->base + 0x70;
    }
    if (id == 0x60) {
        id = VARIANTS->variant60 * 4 + VARIANTS->base + 0x60;
    }
    if (SELECTION_HEADER->header[0] == 0x43) {
        if ((GetGameFlag(0x44U) == 0) && (SELECTION_HEADER->header[1] == 0x44)) {
            id = gCalendarSeason + 0x48;
        }
        if (GetGameFlag(0x42U) == 1) {
            id = gCalendarSeason + 0x48;
        }
    }
    if (SELECTION_HEADER->header[0] == 0x37) {
        if ((GetGameFlag(0x44U) == 0) && (SELECTION_HEADER->header[1] == 0x38)) {
            id = gCalendarSeason + 0x44;
        }
        if (GetGameFlag(0x43U) == 1) {
            id = gCalendarSeason + 0x44;
        }
    }
    return Ovl11RemapCalendarValue((s16)id);
}

void ovl_11_func_8010B67C(void);
void ovl_11_func_800CE5FC(void);

void Ovl11InstallAreaSelection(s32 restorePosition, s32 loadOptions) {
    s16 headerMusic;
    s16 spriteSet;
    s16 conditionalSpriteSet;
    s16 previousMusic;
    s16 previousKind;
    u16 recordFlags;

    previousMusic = WORK->music;
    previousKind = WORK->previous.field_0;
    Ovl11ClearAreaSpriteWork();
    WORK->id = Ovl11ResolveAreaSelection(WORK->id);
    if (WORK->record != NULL) {
        recordFlags = *(u16 *)WORK->record;
        if (restorePosition != 0) {
            RESIDENT->x = *(s16 *)(WORK->record + 0x26);
            RESIDENT->y = *(s16 *)(WORK->record + 0x28);
            RESIDENT->z = *(s16 *)(WORK->record + 0x2A);
        }
        Ovl11InstallSceneSetup((u16)WORK->id, recordFlags, loadOptions & 0xFFFF);
        if (restorePosition != 0) {
            Ovl11RestoreAreaGridPosition();
            if (GetGameFlag(0x42U) == 1) {
                RESIDENT->x = -0x2CBD;
                RESIDENT->y = -0x764;
                RESIDENT->z = 0x25EA;
                SetGameFlag(0x42U, 0U);
            }
            if (GetGameFlag(0x43U) == 1) {
                RESIDENT->x = -0x2A9F;
                RESIDENT->y = -0x695;
                RESIDENT->z = 0x241A;
                SetGameFlag(0x43U, 0U);
            }
        }
        if (recordFlags & 0x8000) {
            WORK->alternateId = (s32)WORK->id;
        } else {
            WORK->normalId = (s32)WORK->id;
        }
    } else {
        Ovl11InstallSceneSetup((u16)WORK->id, 0U, loadOptions & 0xFFFF);
        WORK->normalId = (s32)WORK->id;
        if (RESIDENT->flags & 0x2000) {
            Ovl11LoadIndexedSpriteRecords(0);
        }
    }
    WORK->current = WORK->previous;
    WORK->header = gLoadedAreaHeaderCopy;
    WORK->buffer = gAreaSetupBuffer;
    if (WORK->current.field_2 == 3) {
        ((struct Ovl11SceneHeader *)WORK->header)->music = RESIDENT->music;
    }
    headerMusic = *(s16 *)((u8 *)WORK->header + 0xA);
    if (previousMusic != headerMusic) {
        Ovl11LoadIndexedSpriteRecords((s32)headerMusic);
    }
    /* Preserve the shared conditional load paths: set 0xE can precede the
     * header-selected set. Flattening them into one choice loses that effect. */
    if (!(RESIDENT->control & 0x10)) {
        if (previousKind == 0) {
            if (*(s16 *)((u8 *)WORK->header + 0) != 0) {
                if (*(s16 *)((u8 *)WORK->header + 0xC) != 0) {
                    Ovl11LoadCalendarSpriteSet(0xE, 0);
                }
                goto check_state;
            }
            goto check_mode;
        }
check_mode:
        if (WORK->state == 0x19) {
            if (RESIDENT->control & 0x2000) {
                Ovl11LoadCalendarSpriteSet(0xE, 0);
            }
            goto check_state;
        }
        goto load_header_set;
    }
check_state:
    if (WORK->state != 0x19) {
load_header_set:
        if (GetGameFlag(2U) != 1) {
            spriteSet = *(s16 *)((u8 *)WORK->header + 0xC);
            if (spriteSet != 0x3E7) {
                Ovl11LoadCalendarSpriteSet(spriteSet, 0);
            }
        } else if (WORK->state == 0x19) {
            goto load_conditional_set;
        }
    } else {
load_conditional_set:
        if (RESIDENT->control & 0x2000) {
            conditionalSpriteSet = *(s16 *)((u8 *)WORK->header + 0xC);
            if (conditionalSpriteSet != 0x3E7) {
                Ovl11LoadCalendarSpriteSet(conditionalSpriteSet, 0);
            }
        }
    }
    RESIDENT->control = (s32)(RESIDENT->control & ~0x2000);
    Ovl11RequestVramAnimations((Ovl11SetupHeader *)WORK->header);
    if (WORK->current.field_0 == 0) {
        Ovl11LoadIndexedSpriteSet(0);
    }
    if (WORK->current.field_2 == 5) {
        Ovl11LoadIndexedSpriteSet(1);
    }
    ovl_11_func_8010B67C();
    Ovl11RebuildObjectEventsForTime();
    Ovl11RegisterPoolSpriteRotations((s32)WORK->header);
    if ((*(s16 *)((u8 *)WORK->header + 2) == 1) && (RESIDENT->time >= 0x438)) {
        Ovl11LoadRenderContextSpriteBank();
        SetGameFlag(0x40U, 1U);
    }
    ovl_11_func_800CE5FC();
}
