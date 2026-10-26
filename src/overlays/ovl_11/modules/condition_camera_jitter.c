/*
 * condition_camera_jitter — reconstructed translation unit.
 *
 * Condition-gated random perturbation of the shared camera-target Z halfword.
 * The area-frame update calls this before the camera is applied. No particle
 * state is involved; the condition selector and scene gates are shared inputs.
 * See ovl11_condition_camera_jitter.h for gating, units and wrap behavior.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "random.h"
#include "ovl11_condition_camera_jitter.h"

/* The area-frame caller supplies 1 in a0; the value is deliberately unread. */
void Ovl11JitterCameraTargetZ(s32 unused) {
    char *state;
    char *conditionState;
    char *cameraWork;
    u16 condition;
    s32 randomSample;

    state = (char *)&D_8006C838;
    if (*(u32 *)(state + 0xC) & 0x08000000) {
        return;
    }
    conditionState = state + 0x8000;
    condition = *(u16 *)(conditionState + 0x64C8);
    if ((u32)(condition - 3) >= 2) {
        return;
    }
    if ((*(u32 *)(state + 0x8) & 7) != 3) {
        return;
    }
    randomSample = Rand(100);
    cameraWork = (char *)&D_8007AFF0;
    *(u16 *)(cameraWork + 0x253B8) = *(u16 *)(cameraWork + 0x253B8) - (randomSample - 50);
}
