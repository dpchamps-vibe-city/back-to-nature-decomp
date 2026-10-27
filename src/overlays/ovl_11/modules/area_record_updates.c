/*
 * area_record_updates — reconstructed translation unit.
 *
 * Four ordered area-frame sweeps over tagged, small, state-object and paired
 * stage-object pools. Dispatch record handlers, advance eligible
 * positions/timers, and draw or reset effects according to stage and work
 * flags.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_area_record_updates.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_tagged_record_presentation.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_object_record_lifecycle.h"

void Ovl11UpdateTaggedRecordPools(void) {
    Recon_ovl_11_func_8010CE80_A0View *entry;
    s32 *entryB4;
    s32 flags;
    s32 update_mask;
    s32 i;
    u8 *base;
    u8 *far_base;
    u8 *base2;
    u8 *far_base2;

    for (i = 0; i < 20; i++) {
        entry = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)&gTaggedRecordBase + i * 0xB8);
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if (*(u16 *)entry != 0 && (flags = entry->unk34, !(flags & 0x400))) {
            if (flags & 0x10000) {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11StepTaggedRecordHandler(entry);
                            Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                        }
                    } else {
                        Ovl11StepTaggedRecordHandler(entry);
                        Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                    Ovl11SelectAndDrawTaggedRecordSprite((u8 *)entry);
                } else {
                    Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                }
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                            Ovl11AdvanceTaggedRecordPosition((s32)entry);
                        }
                    } else {
                        Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                        Ovl11AdvanceTaggedRecordPosition((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (!(entry->unk34 & 0x20000)) {
                    if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                        Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                    } else {
                        Ovl11DrawTaggedRecordSprite((u8 *)entry);
                    }
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        entryB4 = (s32 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4);
        far_base2 = D_8009AFF0;
        /* The existing s32 anchor gives resident work: 742EC - 7AB4 = 6C838. */
        base2 = (u8 *)(&gSmallTaggedRecordBase - 0x1EAD);
        if (*(u16 *)entryB4 != 0) {
            Ovl11StepSmallRecordHandler((s16 *)entryB4);
            update_mask = 0x20000;
            if (*(s16 *)(far_base2 + 0x5476) != *(s16 *)((u8 *)entryB4 + 0x30)) {
                Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
            } else {
                if (!(update_mask & *(s32 *)((u8 *)entryB4 + 0x34))) {
                    if (!(*(s32 *)(base2 + 0xC) & 0x08000000)) {
                        Ovl11ApplySmallRecordDisplacement((s32)entryB4);
                    }
                    if (!(*(s32 *)((u8 *)entryB4 + 0x34) & 0x02000000)) {
                        Ovl11SelectAndDrawSmallRecordSprite((u8 *)entryB4);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
                    }
                }
            }
        }
    }
}

void Ovl11UpdateSmallRecordTriplet(void) {
    s16 *entry;
    s32 i;
    u8 *far_base;
    u8 *base;

    for (i = 0; i < 3; i++) {
        entry = (s16 *)((u8 *)&gSmallRecordTripletBase + i * 0xB4);
        far_base = D_8009AFF0;
        base = (u8 *)D_8006C838;
        if (*(u16 *)entry != 0) {
            Ovl11StepSmallRecordHandler(entry);
            if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                Ovl11ResetTimedSpriteEffect(entry + 0x54);
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    Ovl11ApplySmallRecordDisplacement((s32)entry);
                }
                Ovl11SelectAndDrawSmallRecordSprite((u8 *)entry);
                Ovl11TickTimedSpriteEffect((Ov11Timer *)(entry + 0x54));
            }
        }
    }
}

void Ovl11UpdateStateObjectPool(void) {
    s32 recordIndex;
    u16 *record;
    u8 *stateWork;
    u8 *stageWork;

    for (recordIndex = 0; recordIndex < 6; recordIndex++) {
        record = (u16 *)(gStateObjectPoolBytes + recordIndex * 0xB0);
        stateWork = (u8 *)D_8006C838;
        stageWork = D_8009AFF0;
        if ((*record != 0) && !((*(s32 *) ((u8 *) record + 0x34)) & 0x20000)) {
            Ovl11UpdateObjectState(record);
            if (!((*(s32 *) (stateWork + 0xC)) & 0x08000000)) {
                Ovl11AdvanceStateObjectPosition((s32) record);
                Ovl11TickTimedSpriteEffect((Ov11Timer *) ((record + 0x54)));
            }
            if ((*(s16 *) (stageWork + 0x5476)) != (*(s16 *) ((u8 *) record + 0x30))) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((record + 0x54)));
            } else {
                Ovl11DrawStateObjectRecord((u8 *) record);
            }
        }
    }
}

s32 ovl_11_func_801090A4(Ovl11HandlerObjectView *arg0);
s32 ovl_11_func_8010B4D8(u16 *arg0);
void ovl_11_func_800D266C(Ov11D266CFields *arg0, u16 *arg1, s32 arg2);
void ovl_11_func_80108CA4(s32 arg0);
void ovl_11_func_80108BE8(u8 *arg0);
void ovl_11_func_8010B198(s32 arg0);
void ovl_11_func_8010B0E8(u8 *arg0);

/* The two paths share presentation, but only slot 0 has the date-row-8
 * exception. Retain the shared tail rather than conflating their guards. */
void Ovl11UpdatePairedStageObjects(void) {
    s32 slotIndex;
    struct_800759E4 *record;
    u8 *stateWork;
    u8 *stageWork;
    u8 *stateWork2;
    s32 skipPresentationMask;

    for (slotIndex = 0; slotIndex < 2; slotIndex++) {
        stateWork = (u8 *)D_8006C838;
        stageWork = D_8009AFF0;
        if (slotIndex == 0) {
            record = &gStageMode1Record;
            if ((gStageMode1Record.f00 != 0) && (!(gStageMode1Record.f34 & 0x400) || (Ovl11FindCalendarDate(*(s16 *) (stateWork + 0x44BA), *(s16 *) (stateWork + 0x44BC)) != 8))) {
                ovl_11_func_801090A4((Ovl11HandlerObjectView *) &gStageMode1Record);
                goto present_record;
            }
            goto next_record;
        }
        record = (struct_800759E4 *) &gStageMode2Record;
        if ((gStageMode2Record != 0) && !(((struct_800759E4 *) &gStageMode2Record)->f34 & 0x400)) {
            ovl_11_func_8010B4D8(&((struct_800759E4 *) &gStageMode2Record)->f00);
present_record:
            skipPresentationMask = 0x20000;
            if ((*(s16 *) (stageWork + 0x5476)) != (s16) record->f30) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((u8 *) record + 0xA8));
            } else if (!(record->f34 & skipPresentationMask)) {
                if (slotIndex == 0) {
                    if (!((*(s32 *) (stateWork + 0xC)) & 0x08000000)) {
                        ovl_11_func_800D266C((Ov11D266CFields *) record, (u16 *) &gOvl11Mode1DispatchCounter, 0x11);
                        ovl_11_func_80108CA4((s32) record);
                    }
                    ovl_11_func_80108BE8((u8 *) record);
                    Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) record + 0xA8));
                } else {
                    stateWork2 = (u8 *)D_8006C838;
                    if (!((*(s32 *) (stateWork2 + 0x5234)) & 0x100)) {
                        if (!((*(s32 *) (stateWork2 + 0xC)) & 0x08000000)) {
                            ovl_11_func_800D266C((Ov11D266CFields *) record, (u16 *) &gOvl11Mode2DispatchCounter, 0x11);
                            ovl_11_func_8010B198((s32) record);
                        }
                        ovl_11_func_8010B0E8((u8 *) record);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) record + 0xA8));
                    }
                    goto next_record;
                }
            } else {
                goto next_record;
            }
        } else {
next_record:
            ;
        }
    }
}
