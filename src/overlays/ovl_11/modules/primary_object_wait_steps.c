/*
 * primary_object_wait_steps — reconstructed translation unit.
 *
 * Two-phase interaction steps: submit an ID-dependent primary-object
 * operation once, then poll its completion flags without advancing the
 * completed step.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"

/* The request service's fourth ABI argument is retained even though unused. */
s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 Ovl11StepPrimaryObjectItemRequest(s32 *step, s32 baseStep, s16 itemId) {
    s32 complete;
    u32 phase;

    complete = 0;
    phase = *step - baseStep;
    if (phase >= 2U) {
        return 0;
    }
    switch (phase) {
    case 0:
        ovl_11_func_800E6F34(itemId, 0, 0, 0);
        phase++;
        break;
    case 1:
        if ((gPrimaryObjectFlags & 0x34008000) == 0x14008000) {
            complete = 1;
        }
        break;
    }
    *step = baseStep + phase;
    return complete;
}

s32 Ovl11StepPrimaryObjectA4Request(s32 *step, s32 baseStep, s16 requestValue) {
    s16 request[3];
    s32 complete;
    u32 phase;

    complete = 0;
    phase = *step - baseStep;
    if (phase >= 2U) {
        return 0;
    }
    switch (phase) {
    case 0:
        Ovl11SetHalfwordTriplet(request, 0xA4, requestValue, 1);
        Ovl11InstallPrimaryObjectRequest((u16 *) request);
        phase++;
        break;
    case 1:
        if (gPrimaryObjectFlags & 0x04000000) {
            complete = 1;
        }
        break;
    }
    *step = baseStep + phase;
    return complete;
}
