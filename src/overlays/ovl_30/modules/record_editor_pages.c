/* Whole record/flag diagnostic page unit: animal/resident/Korobokkuru
 * record menus, a game-flag editor, event-slot listing and system-flag menu.
 * Shared animal labels, the ordered literal pool and the private selector
 * run bind all eight pages, including the two non-descriptor listings.
 * Automatic descriptors capture the record before fade/selector updates;
 * keep their field addresses/widths and duplicate rows. Labels translate
 * text, not disputed field interpretations. Public contracts:
 * ovl30_record_editor_pages.h; membership/boundaries: notes/file-groupings.md. */

#include "ovl30_record_editor_pages.h"
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "ovl30_record_menus.h"
#include "game_flags.h"

s32 Ovl30EditCowSheepData(s32 cursor) {
    Ovl30ValueMenuEntry entries[25] = {
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].number, 0, 0, 2, -2, gOvl30AnimalNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].affection, 0, 0xFF, 2, -1, gOvl30AnimalAffectionLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].birth[1], 0, 3, 2, -1, gOvl30AnimalBirthMonthLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].birth[2], 0, 0x1D, 2, -1, gOvl30AnimalBirthDayLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].grazing, 0, 0x1F, 2, -1, gOvl30AnimalGrazingTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].sick, 0, 0x1F, 2, -1, gOvl30AnimalSicknessDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].growth, 0, 0x1F, 2, -1, gOvl30AnimalGrowthDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].growth_time, 0, 0x1F, 2, -1, gOvl30AnimalPregnancyDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].pregnancy, 0, 0x1F, 2, -1, gOvl30AnimalDaysSinceShearingLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].gestation, 0, 0x63, 2, -2, gOvl30AnimalActionLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].action, 0, 0, 2, -2, gOvl30AnimalProcessNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].process_no, 0, 0, 2, -2, gOvl30AnimalProcessIdLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].process_id, 0, 0, 2, -2, gOvl30AnimalProcessTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].process_time, 0, 0, 2, -2, gOvl30AnimalInterruptTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].elapsed[1], 0, 0x40, 2, -1, gOvl30AnimalCurrentLocationLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].flags, 0, 0, 4, -2, gOvl30AnimalFlagsLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].position[0], 0, 0x98967F, 4, -1, gOvl30AnimalXLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].position[1], 0, 0x98967F, 4, -2, gOvl30AnimalYLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].position[2], 0, 0x98967F, 4, -1, gOvl30AnimalZLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].speed[0], 0, 0, 4, -2, gOvl30AnimalSpeedXLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].speed[1], 0, 0, 4, -2, gOvl30AnimalSpeedYLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].speed[2], 0, 0, 4, -2, gOvl30AnimalSpeedZLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].target[0], 0, 0, 4, -2, gOvl30AnimalDestinationXLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].target[1], 0, 0, 4, -2, gOvl30AnimalDestinationYLabel},
        {(u8 *)&Ovl30_AnimalRecords749F4[gOvl30CowSheepIndex].target[2], 0, 0, 4, -2, gOvl30AnimalDestinationZLabel}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30CowSheepHeading, gOvl30CowSheepIndex);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(entries, 25);
        cursor = gOvl30RecordMenuCursor;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            Ovl30StepFieldValue((u8 *)&gOvl30CowSheepIndex, 4, 0x13, 0, -1);
            return cursor;
        }
        if (buttons & 8) {
            Ovl30StepFieldValue((u8 *)&gOvl30CowSheepIndex, 4, 0x13, 0, 1);
        }
    }
    return cursor;
}

s32 Ovl30EditChickenData(s32 cursor) {
    Ovl30ValueMenuEntry entries[23] = {
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].number, 0, 0, 2, -2, gOvl30AnimalNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].affection, 0, 0xFF, 2, -1, gOvl30AnimalAffectionLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].birth[1], 0, 3, 2, -1, gOvl30AnimalBirthMonthLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].birth[2], 0, 0x1D, 2, -1, gOvl30AnimalBirthDayLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].grazing, 0, 0x1F, 2, -1, gOvl30AnimalGrazingTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].sick, 0, 0x1F, 2, -1, gOvl30AnimalSicknessDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].growth, 0, 0x1F, 2, -1, gOvl30AnimalPregnancyDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].gestation, 0, 0x63, 2, -2, gOvl30AnimalActionLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].action, 0, 0, 2, -2, gOvl30AnimalProcessNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].process_no, 0, 0, 2, -2, gOvl30AnimalProcessIdLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].process_id, 0, 0, 2, -2, gOvl30AnimalProcessTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].process_time, 0, 0, 2, -2, gOvl30AnimalInterruptTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].elapsed[1], 0, 0, 2, -2, gOvl30AnimalCurrentLocationLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].flags, 0, 0, 4, -2, gOvl30AnimalFlagsLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].position[0], 0, 0, 4, -2, gOvl30AnimalXLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].position[1], 0, 0, 4, -2, gOvl30AnimalYLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].position[2], 0, 0, 4, -2, gOvl30AnimalZLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].speed[0], 0, 0, 4, -2, gOvl30AnimalSpeedXLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].speed[1], 0, 0, 4, -2, gOvl30AnimalSpeedYLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].speed[2], 0, 0, 4, -2, gOvl30AnimalSpeedZLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].target[0], 0, 0, 4, -2, gOvl30AnimalDestinationXLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].target[1], 0, 0, 4, -2, gOvl30AnimalDestinationYLabel},
        {(u8 *)&Ovl30_AnimalRecords[gOvl30ChickenIndex].target[2], 0, 0, 4, -2, gOvl30AnimalDestinationZLabel}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30ChickenHeading, gOvl30ChickenIndex);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(entries, 23);
        cursor = gOvl30RecordMenuCursor;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            Ovl30StepFieldValue((u8 *)&gOvl30ChickenIndex, 4, 9, 0, -1);
            return cursor;
        }
        if (buttons & 8) {
            Ovl30StepFieldValue((u8 *)&gOvl30ChickenIndex, 4, 9, 0, 1);
        }
    }
    return cursor;
}

s32 Ovl30EditCoopChickenData(s32 cursor) {
    Ovl30ValueMenuEntry entries[23] = {
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].number, 0, 0, 2, -2, gOvl30AnimalNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].affection, 0, 0xFF, 2, -1, gOvl30AnimalAffectionLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].birth[1], 0, 3, 2, -1, gOvl30AnimalBirthMonthLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].birth[2], 0, 0x1D, 2, -1, gOvl30AnimalBirthDayLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].grazing, 0, 0x1F, 2, -1, gOvl30AnimalGrazingTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].sick, 0, 0x1F, 2, -1, gOvl30AnimalSicknessDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].growth, 0, 0x1F, 2, -1, gOvl30AnimalPregnancyDaysLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].gestation, 0, 0x63, 2, -2, gOvl30AnimalActionLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].action, 0, 0, 2, -2, gOvl30AnimalProcessNumberLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].process_no, 0, 0, 2, -2, gOvl30AnimalProcessIdLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].process_id, 0, 0, 2, -2, gOvl30AnimalProcessTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].process_time, 0, 0, 2, -2, gOvl30AnimalInterruptTimeLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].elapsed[1], 0, 0, 2, -2, gOvl30AnimalCurrentLocationLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].flags, 0, 0, 4, -2, gOvl30AnimalFlagsLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].position[0], 0, 0, 4, -2, gOvl30AnimalXLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].position[1], 0, 0, 4, -2, gOvl30AnimalYLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].position[2], 0, 0, 4, -2, gOvl30AnimalZLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].speed[0], 0, 0, 4, -2, gOvl30AnimalSpeedXLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].speed[1], 0, 0, 4, -2, gOvl30AnimalSpeedYLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].speed[2], 0, 0, 4, -2, gOvl30AnimalSpeedZLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].target[0], 0, 0, 4, -2, gOvl30AnimalDestinationXLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].target[1], 0, 0, 4, -2, gOvl30AnimalDestinationYLabel},
        {(u8 *)&Ovl30_AnimalRecords75FE4[gOvl30CoopChickenIndex].target[2], 0, 0, 4, -2, gOvl30AnimalDestinationZLabel}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30CoopChickenHeading, gOvl30CoopChickenIndex);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(entries, 23);
        cursor = gOvl30RecordMenuCursor;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            Ovl30StepFieldValue((u8 *)&gOvl30CoopChickenIndex, 4, 2, 0, -1);
            return cursor;
        }
        if (buttons & 8) {
            Ovl30StepFieldValue((u8 *)&gOvl30CoopChickenIndex, 4, 2, 0, 1);
        }
    }
    return cursor;
}

s32 Ovl30EditResidentData(s32 cursor) {
    s32 *position = (s32 *)((u8 *)D_80076250 + Ovl30_ResidentSelection->resident * 0x1D4);
    u16 *flags = (u16 *)((u8 *)D_8007624C + Ovl30_ResidentSelection->resident * 0x1D4);
    s32 *destination = (s32 *)((u8 *)gObjectPositionRecordCoordinates + Ovl30_ResidentSelection->resident * 0x1D4);
    s32 *destinationSpeed = (s32 *)((u8 *)D_80076290 + Ovl30_ResidentSelection->resident * 0x1D4);
    s16 *birth = (s16 *)((u8 *)D_80076244 + Ovl30_ResidentSelection->resident * 0x1D4);
    Ovl30ValueMenuEntry entries[32] = {
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x27, 2, -2, gOvl30ResidentNameNumberLabel},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 1, 2, -2, gOvl30ResidentSexLabel},
        {(u8 *)D_80076222 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0xFF, 2, -1, gOvl30ResidentFriendshipLabel},
        {(u8 *)D_80076224 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0xFFFF, 2, -1, gOvl30ResidentAffectionLabel},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x1F, 2, -2, gOvl30ResidentTalkIntervalLabel},
        {(u8 *)D_80076226 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 3, 2, -2, gOvl30ResidentDirectionLabel},
        {(u8 *)D_80076228 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentControlDisableDcLabel},
        {(u8 *)D_8007622A + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentSDelayLabel},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x1F, 2, -2, gOvl30ResidentTalkIntervalLabel},
        {(u8 *)D_8007622C + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentLsNumberLabel},
        {(u8 *)D_8007623A + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentActionLabel},
        {(u8 *)D_8007623C + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentPreviousActionLabel},
        {(u8 *)D_80076242 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentLsTimeLabel},
        {(u8 *)D_8007623E + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, gOvl30ResidentXFlagsLabel},
        {(u8 *)&flags[0], 0, 0, 2, -2, gOvl30ResidentGlobalLocationLabel},
        {(u8 *)&flags[1], 0, 0, 2, -2, gOvl30ResidentLocalLocationLabel},
        {(u8 *)&position[0], 0, 0, 4, -2, gOvl30ResidentXLabel},
        {(u8 *)&position[1], 0, 0, 4, -2, gOvl30ResidentYLabel},
        {(u8 *)&position[2], 0, 0, 4, -2, gOvl30ResidentZLabel},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, gOvl30ResidentSpeedXLabel},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, gOvl30ResidentSpeedYLabel},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, gOvl30ResidentSpeedZLabel},
        {(u8 *)&destination[0], 0, 0, 4, -2, gOvl30ResidentEXLabel},
        {(u8 *)&destination[1], 0, 0, 4, -2, gOvl30ResidentEYLabel},
        {(u8 *)&destination[2], 0, 0, 4, -2, gOvl30ResidentEZLabel},
        {(u8 *)&destinationSpeed[0], 0, 0, 4, -2, gOvl30ResidentESpeedXLabel},
        {(u8 *)&destinationSpeed[1], 0, 0, 4, -2, gOvl30ResidentESpeedYLabel},
        {(u8 *)&destinationSpeed[2], 0, 0, 4, -2, gOvl30ResidentESpeedZLabel},
        {(u8 *)&birth[0], 0, 0, 2, -2, gOvl30ResidentBirthMonthLabel},
        {(u8 *)&birth[1], 0, 0, 2, -2, gOvl30ResidentBirthDayLabel},
        {(u8 *)D_80076234 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 100, 2, -1, gOvl30ResidentDestinationLabel},
        {(u8 *)D_80076236 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 100, 2, -1, gOvl30ResidentPreviousDestinationLabel}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30ResidentHeading, Ovl30_ResidentSelection->resident);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(entries, 32);
        cursor = gOvl30RecordMenuCursor;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            Ovl30StepFieldValue((u8 *)&Ovl30_ResidentSelection->resident, 2, 0x24, 0, -1);
            return cursor;
        }
        if (buttons & 8) {
            Ovl30StepFieldValue((u8 *)&Ovl30_ResidentSelection->resident, 2, 0x24, 0, 1);
        }
    }
    return cursor;
}

/* Keep the endpoint asymmetry: subtracting five/fifty to exactly zero
 * wraps to the last index. The displayed ten-row numbers do not wrap. */
s32 Ovl30EditGameFlagIndex(s32 handlerState) {
    u32 var_s0;

    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x20) {
        gOvl30GameFlagIndex += 5;
        if (gOvl30GameFlagIndex >= 0x4A0) {
            gOvl30GameFlagIndex = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x40) {
        gOvl30GameFlagIndex -= 5;
        if (gOvl30GameFlagIndex <= 0) {
            gOvl30GameFlagIndex = 0x49F;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x10) {
        gOvl30GameFlagIndex += 0x32;
        if (gOvl30GameFlagIndex >= 0x4A0) {
            gOvl30GameFlagIndex = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x80) {
        gOvl30GameFlagIndex -= 0x32;
        if (gOvl30GameFlagIndex <= 0) {
            gOvl30GameFlagIndex = 0x49F;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x4000) {
        gOvl30GameFlagIndex++;
        if (gOvl30GameFlagIndex >= 0x4A0) {
            gOvl30GameFlagIndex = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x1000) {
        if (gOvl30GameFlagIndex <= 0) {
            gOvl30GameFlagIndex = 0x49F;
        } else {
            gOvl30GameFlagIndex--;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x8000) {
        SetGameFlag((u16) (*(s32 *) ((u8 *) &gOvl30GameFlagIndex + 0)), 1U);
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x2000) {
        SetGameFlag((u16) (*(s32 *) ((u8 *) &gOvl30GameFlagIndex + 0)), 0U);
    }
    if (GetGameFlag((u32) (u16) (*(s32 *) ((u8 *) &gOvl30GameFlagIndex + 0))) == 1) {
        FntPrint(&gOvl30GameFlagOnFormat, gOvl30GameFlagIndex);
    } else {
        FntPrint(&gOvl30GameFlagOffFormat, gOvl30GameFlagIndex);
    }
    var_s0 = 0;
    do {
        if (GetGameFlag((u16)((u16)gOvl30GameFlagIndex + var_s0 - ((gOvl30GameFlagIndex + var_s0) / 0x4A0) * 0x4A0)) == 1) {
            FntPrint(gOvl30GameEventOnFormat, gOvl30GameFlagIndex + var_s0, gOvl30GameFlagIndex + var_s0 - 0xFB);
        } else {
            FntPrint(gOvl30GameEventOffFormat, gOvl30GameFlagIndex + var_s0, gOvl30GameFlagIndex + var_s0 - 0xFB);
        }
        var_s0++;
    } while (var_s0 < 0xAU);
    return handlerState;
}

s32 Ovl30DrawObjectEventSlots(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    u32 var_s0;
    struct_80076220 *rec;

    if (*(s32 *)gPad0Input & 4) {
        temp_v0 = gOvl30ObjectEventSlotIndex - 5;
        gOvl30ObjectEventSlotIndex = temp_v0;
        if (temp_v0 <= 0) {
            gOvl30ObjectEventSlotIndex = 0x24;
        }
    }
    if (*(s32 *)gPad0Input & 8) {
        temp_v0_2 = gOvl30ObjectEventSlotIndex + 5;
        gOvl30ObjectEventSlotIndex = temp_v0_2;
        if (temp_v0_2 >= 0x25) {
            gOvl30ObjectEventSlotIndex = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        temp_v0_3 = gOvl30ObjectEventSlotIndex - 1;
        gOvl30ObjectEventSlotIndex = temp_v0_3;
        if (temp_v0_3 <= 0) {
            gOvl30ObjectEventSlotIndex = 0x24;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        temp_v0_4 = gOvl30ObjectEventSlotIndex + 1;
        gOvl30ObjectEventSlotIndex = temp_v0_4;
        if (temp_v0_4 >= 0x25) {
            gOvl30ObjectEventSlotIndex = 0;
            var_s0 = 0;
        }
    }
    var_s0 = 0;
    do {
        rec = &gObjectEventSlotStorage[(gOvl30ObjectEventSlotIndex + var_s0) % 0x25];
        FntPrint(&gOvl30ObjectEventSlotFormat, (gOvl30ObjectEventSlotIndex + var_s0) % 0x25, *(s32 *)rec->unk28);
        var_s0 += 1;
    } while (var_s0 < 0xAU);
    return 0;
}

s32 Ovl30EditKorobokkuruData(s32 cursor) {
    Ovl30ValueMenuEntry entries[29] = {
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_AE, 0, 0xFF, 2, -1, gOvl30HarvestTrainingLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_B0, 0, 0xFF, 2, -1, gOvl30WateringTrainingLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_B2, 0, 0xFF, 2, -1, gOvl30AnimalCareTrainingLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_B4, 0, 0x1F, 2, -1, gOvl30WorkDaysLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_B6, 0, 0, 2, -2, gOvl30HeldItemLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_B8, 0, 0, 2, -2, gOvl30AnimalFlagsLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].field_BA, 0, 0, 2, -2, gOvl30TargetLivestockLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].affection, 0, 0xFF, 2, -1, gOvl30AnimalAffectionLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].gestation, 0, 0x63, 2, -2, gOvl30AnimalActionLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].action, 0, 0, 2, -2, gOvl30AnimalProcessNumberLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].process_no, 0, 0, 2, -2, gOvl30AnimalProcessIdLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].process_id, 0, 0, 2, -2, gOvl30AnimalProcessTimeLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].process_time, 0, 0, 2, -2, gOvl30AnimalInterruptTimeLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].elapsed[1], 0, 0, 2, -2, gOvl30AnimalCurrentLocationLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].flags, 0, 0, 4, -2, gOvl30AnimalFlagsLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].position[0], 0, 0, 4, -2, gOvl30AnimalXLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].position[1], 0, 0, 4, -2, gOvl30AnimalYLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].position[2], 0, 0, 4, -2, gOvl30AnimalZLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].speed[0], 0, 0, 4, -2, gOvl30AnimalSpeedXLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].speed[1], 0, 0, 4, -2, gOvl30AnimalSpeedYLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].speed[2], 0, 0, 4, -2, gOvl30AnimalSpeedZLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].target[0], 0, 0, 4, -2, gOvl30AnimalDestinationXLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].target[1], 0, 0, 4, -2, gOvl30AnimalDestinationYLabel},
        {(u8 *)&Ovl30_RecordsA638[gOvl30KorobokkuruIndex].target[2], 0, 0, 4, -2, gOvl30AnimalDestinationZLabel},
        {(D_8007A638 - 0x4426), 0, 999, 2, -1, gOvl30MoveSp0Label},
        {(D_8007A638 - 0x4424), 0, 999, 2, -1, gOvl30MoveSp1Label},
        {(D_8007A638 - 0x4422), 0, 999, 2, -1, gOvl30MoveSp2Label},
        {(D_8007A638 - 0x4420), 0, 999, 2, -1, gOvl30MoveSp3Label},
        {(D_8007A638 - 0x441E), 0, 999, 2, -1, gOvl30MoveSp4Label}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30KorobokkuruAndFlagTextPool, gOvl30KorobokkuruIndex);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(entries, 29);
        cursor = gOvl30RecordMenuCursor;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            Ovl30StepFieldValue((u8 *)&gOvl30KorobokkuruIndex, 4, 6, 0, -1);
            return cursor;
        }
        if (buttons & 8) {
            Ovl30StepFieldValue((u8 *)&gOvl30KorobokkuruIndex, 4, 6, 0, 1);
        }
    }
    return cursor;
}

s32 Ovl30EditSystemFlagBits(s32 cursor) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30SystemFlagHeading);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditFlagMenu(&gOvl30SystemFlagMenu, 0xC);
        result = gOvl30RecordMenuCursor;
    } else {
        result = cursor;
    }
    return result;
}
