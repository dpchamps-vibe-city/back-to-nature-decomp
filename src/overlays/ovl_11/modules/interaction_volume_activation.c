/*
 * Directional interaction-volume activation for the primary object.
 *
 * The probe publishes a borrowed selection to the separate area-mode
 * controller. Return 2 permits normal motion, 0 consumes the frame without
 * activation, and 1 reports activation. This is not a pure hit test: prompts,
 * input overrides, requests and the shared area mode can all change.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_interaction_volume_activation.h"
#include "pad_actuator_sequences.h"
#include "message_window.h"
#include "ovl11_interaction_volume_query.h"
#include "game_flags.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_input_override.h"

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);

s32 Ovl11ActivateInteractionVolume(Ovl11C7010Object *object) {
    s32 messages[2];
    s16 lookupCode;
    s16 recordDirection;
    s16 code;
    s32 flags;
    s32 result;
    s32 mode;
    u16 direction;
    u16 recordFlags;
    Ovl11InteractionVolumeSelection *record;

    if (((struct Ovl11C7F0Flags *)D_8006C838)->flagsC & 0x08000000) {
        return 2;
    }
    if (!(object->unk6C & 0x40000008)) {
        if (object->unk6C & 0x20000000) {
            return 0;
        }
    }
    if (object->unk6C & 0x50) {
        return 0;
    }
    if (D_80128CB0 == 2) {
        return 0;
    }
    if (((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x20000000) {
        return 2;
    }
    record = Ovl11ProbeInteractionVolume(gLoadedAreaHeaderCopy,
                 (Ovl11Func800BDCE8Arg1 *)&object->unk100, 1);
    gOvl11SelectedInteractionVolume = record;
    if (record != NULL) {
        flags = object->unk6C;
        mode = 1;
        if (flags & 0x102) {
            mode = 0;
        }
        if (flags & 0x800000) {
            mode = 1;
        }
        if (flags & 0x100) {
            if (record->actionCode == 3) {
                mode = 2;
            }
            if (record->selectionId == 0) {
                mode = 2;
            }
        }
        if (mode != 0) {
            recordDirection = gOvl11SelectedInteractionVolume->direction;
            if ((recordDirection == 0x3E7) ||
                (recordFlags = gOvl11SelectedInteractionVolume->flags,
                 ((recordFlags & 0x1200) != 0x1000))) {
                if (gOvl11SelectedInteractionVolume->flags & 0x200) {
                    lookupCode = gOvl11SelectedInteractionVolume->actionCode;
                    if ((lookupCode != 0x63) &&
                        (ovl_11_func_80107C80(lookupCode,
                            (u16)gOvl11SelectedInteractionVolume->direction,
                            &messages[0], 0) != 1)) {
                        if (messages[0] != -1) {
                            object->unk38 = (u16)gOvl11SelectedInteractionVolume->direction;
                            RequestMessageWindow(2, messages[0]);
                            Ovl11BeginPrimaryObjectInputOverride(0x14);
                            Ovl11ResetPrimaryObjectRequest();
                        }
                        return 0;
                    }
                }
                if (GetGameFlag(0x15U) != 1) {
                    if (GetGameFlag(0x16U) != 1) {
                        ((struct Ovl11SelectionResetMode *)&D_8007AFF0)->mode = 3;
                        object->unk6C |= 0x400;
                        Ovl11ResetPrimaryObjectRequest();
                        return 1;
                    }
                    return 2;
                }
                return 2;
            }
            if (gOvl11PrimarySurfaceFlags & 0x2000) {
                if (recordDirection == object->unk38) {
                    if (!(gOvl11PrimaryPressedButtons & 0x40)) {
                        return 0;
                    }
                    goto check_direction;
                }
                return 2;
            }
            direction = object->unk38;
            if (gOvl11SelectedInteractionVolume->actionCode != 0x2F) {
                if (mode != 2) {
                    if (!(recordFlags & 0x200) && !(gOvl11PrimaryPressedButtons & 0x40)) {
                        return (recordDirection != direction) * 2;
                    }
                    goto check_direction;
                }
                if (recordDirection != direction) {
                    return 2;
                }
                goto activate;
            }
        check_direction:
            if (gOvl11SelectedInteractionVolume->direction == object->unk38) {
            activate:
                messages[1] = 0;
                code = gOvl11SelectedInteractionVolume->actionCode;
                if (code == 0x63) {
                    return 1;
                }
                if (code == 0x2E) {
                    if (object->unk6C & 0x18008000) {
                        return 0;
                    }
                }
                result = ovl_11_func_80107C80(gOvl11SelectedInteractionVolume->actionCode,
                            (u16)gOvl11SelectedInteractionVolume->direction,
                            &messages[1], 0);
                if (result == 1) {
                    RequestPadActuatorSequence(0, 2);
                    ((struct Ovl11SelectionResetMode *)&D_8007AFF0)->mode = result;
                    Ovl11BeginPrimaryObjectInputOverride(0xA);
                    Ovl11ResetPrimaryObjectRequest();
                    if (gOvl11SelectedInteractionVolume->selectionId != 0xFFFE) {
                        SetGameFlag(3U, 1U);
                        return 1;
                    }
                    return 1;
                }
                if (messages[1] != -1) {
                    RequestMessageWindow(2, messages[1]);
                }
                Ovl11BeginPrimaryObjectInputOverride(5);
                Ovl11ResetPrimaryObjectRequest();
                return 0;
            }
            return 2;
        }
        if (gOvl11SelectedInteractionVolume->direction == object->unk38) {
            Ovl11BeginPrimaryObjectInputOverride(0xA);
            return 2;
        }
        return 2;
    }
    return 2;
}
