/* Primary-object reset used by the day-transition callback after its fade.
 * It applies the existing status adjustments as well as clearing action,
 * request and sprite-slot state; it is not an idempotent initialization.
 * Interface: ovl11_primary_object_day_reset.h. Unit evidence: file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "game_flags.h"
#include "ovl11_primary_object_day_reset.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_sixty_step_deltas.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_status_gauge_deltas.h"
#include "ovl11_primary_object_level_bands.h"
#include "ovl11_object_area_sprite_reset.h"
#include "ovl11_primary_work_counter_reset.h"

void Ovl11ResetPrimaryObjectForDayTransition(void) {
    struct Ovl11CCCC0State *work;
    Ovl11C7010Object *primaryObject = (Ovl11C7010Object *)&gPrimaryObjectBase;
    s32 deltaInput;
    s8 increaseDelta;

    if (primaryObject->unk6C & 0x1000) {
        Ovl11ResetObjectAreaSpriteSource((Recon_ovl_11_func_800CD08C_A0View *)primaryObject);
    }
    primaryObject->unk22 = -1;
    work = (struct Ovl11CCCC0State *)((u8 *)primaryObject - 0x51C8);
    primaryObject->unk1A = 0;
    primaryObject->unk18 = 0;
    primaryObject->unk24 = 0;
    primaryObject->unk38 = 0;
    primaryObject->unk6C &= 0xBD9B22A4;
    work->status = (s32) (work->status & 0xFFFE3FFF);
    Ovl11ClearPrimaryWorkCounters();
    Ovl11ResetPrimaryObjectRequest();
    SetGameFlag(0x3FU, 0U);
    SetGameFlag(0x40U, 0U);
    if (!(work->flags & 4)) {
        if (GetGameFlag(0x47U) != 1) {
            Ovl11ApplyStatusDelta(0, 0xA, 0, 0);
        } else {
            deltaInput = work->angle + 0x5A0;
            deltaInput -= primaryObject->unk2C;
            increaseDelta = Ovl11ComputeSixtyStepIncrease(deltaInput);
            Ovl11ApplyStatusDelta(increaseDelta,
                Ovl11ComputeSixtyStepDecrease(deltaInput), 0, 0);
        }
    } else {
        if ((u32) ((u16) primaryObject->unk2C - 0x168) < 0x2D0U) {
            Ovl11ApplyStatusDelta(0x64, -0x19, 0, 0);
            Ovl11ApplyStatusDelta(0x64, 0, 0, 0);
        } else {
            Ovl11ApplyStatusDelta(0x32, -0x16, 0, 0);
        }
        ((struct Ovl11CCCC0State *)D_8006C838)->flags =
            (s32) (((struct Ovl11CCCC0State *)D_8006C838)->flags & ~4);
    }
    primaryObject->unk36 = 0;
    Ovl11UpdatePrimaryObjectLevelBands(primaryObject, 1);
    primaryObject->unk2A = (u16) ((struct Ovl11CCCC0State *)D_8006C838)->angle;
    /* Work +44C0 is the signed clock hour. Keep the original halfword
     * arithmetic and signed upper-clamp test on the accumulated counter. */
    if (((struct Ovl11CCCC0State *)D_8006C838)->time >= 8) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter =
            (u16) ((u16) ((struct Ovl11CCCC0State *)D_8006C838)->time +
                (((struct Ovl11CCCC0State *)D_8006C838)->counter - 8));
    }
    if ((s16) ((struct Ovl11CCCC0State *)D_8006C838)->counter >= 0x10) {
        ((struct Ovl11CCCC0State *)D_8006C838)->counter = 0xFU;
    }
    ((struct Ovl11CCCC0State *)D_8006C838)->pending = 0;
    SetGameFlag(0xAU, 0U);
    SetGameFlag(0x76U, 0U);
    Ovl11InitObjectSpriteSlots((u8 *)primaryObject);
    Ovl11InitSharedSpriteSlots((u8 *)primaryObject);
    SetGameFlag(0xA0U, 0U);
}
