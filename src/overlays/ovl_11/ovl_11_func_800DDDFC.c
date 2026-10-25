#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "sprite_rotation.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vertex_blend.h"
#include "ovl11_daily_conditions.h"
#include "game_flags.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"

void ovl_11_func_800D7B8C (void);
void ovl_11_func_800CECB0 (void);
void ovl_11_func_800CCCC0 (void);
void ovl_11_func_80111F88 (void);
void ovl_11_func_800DBFEC (u16 arg0, u16 arg1, s32 arg2);
void ovl_11_func_800BD5A8 (s32 arg0);
void ovl_11_func_800BD6A8 (s16 arg0, s16 arg1);
void ovl_11_func_800DB0E4 (void);
void ovl_11_func_800DC990 (void);
void ovl_11_func_800DCA60 (void);
void ovl_11_func_800DB2AC (Ovl11SetupHeader *arg0);
void ovl_11_func_800CD0C0 (s32 *arg0);
void ovl_11_func_800CBEF8 (void);
void ovl_11_func_800CCC5C (void);
void ovl_11_func_800EEECC (void);
void ovl_11_func_800BD668 (void);
void ovl_11_func_800F1F48 (s32 arg0);
void ovl_11_func_800C19B0 (void);
void ovl_11_func_800BFEA4 (void);
void ovl_11_func_800BFF00 (void);
void ovl_11_func_800C044C (s16 arg0);
void ovl_11_func_800BC868 (void);
void ovl_11_func_800BC9AC (void);
void ovl_11_func_800DBC4C (void);
void ovl_11_func_8010C9B4 (void);
void ovl_11_func_800F40A4 (void);
void ovl_11_func_800F3D40 (void);
void ovl_11_func_800F2620 (void);
s32 ovl_11_func_800DE76C (void);
void ovl_11_func_800F4D28 (void);
void ovl_11_func_800BF844 (void);
void ovl_11_func_800D8FC8 (s32 arg0);
void ovl_11_func_800D9064 (void);
void ovl_11_func_800C05F8 (void);
void ovl_11_func_800DE4C0 (void);
void ovl_11_func_800BD3C4(s32 arg0);                /* static */
void ovl_11_func_801129EC();                        /* static */
void ovl_11_func_800F4BB4(void);

s32 ovl_11_func_800DDDFC(void) {
    s32 result;
    s32 id;
    s32 offset;
    s32 same;
    Ovl11DDDFCState *phase0;

    SetGameFlag(3, 1);
    result = 0;
    ((Ovl11DDDFCState *)D_8006C838)->flags |= 0x08000000;
    ((s32 *)gPad0Input)[2] = 0;
    ((s32 *)gPad0Input)[1] = 0;
    ((s32 *)gPad0Input)[0] = 0;
    switch (((Ovl11DDDFCState *)D_8006C838)->phase) {
    case 0:
        phase0 = (Ovl11DDDFCState *)D_8006C838;
        if (phase0->control & 4) {
            phase0->phase += 2;
            break;
        }
        if (GetGameFlag(0x47) == 0) {
            if (IsScreenFadeComplete() != 1) {
                break;
            }
            if (GetScreenFadeRampMode() != 2) {
                BeginScreenFadeIncrease(0x2D, 0, 2);
            }
        }
        phase0->phase++;
        break;
    case 1:
        if (IsScreenFadeComplete() == 1) {
            ((Ovl11DDDFCState *)D_8006C838)->phase++;
            ((Ovl11DDDFCState *)D_8006C838)->flags |= 0x800000;
        }
        break;
    case 2:
        SetGameFlag(0x73, ((Ovl11DDDFCState *)D_8006C838)->mode == 3);
        ovl_11_func_800D7B8C();
        ovl_11_func_800CECB0();
        Ovl11ApplyCurrentDayCondition();
        Ovl11ChooseNextDayCondition();
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        break;
    case 3:
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        break;
    case 4:
        if (IsScreenFadeComplete() == 1) {
            ovl_11_func_800CCCC0();
            Ovl11ResetSpriteRotations();
            ovl_11_func_80111F88();
            ((Ovl11SetupWorkView *)&D_8007AFF0)->alternateId = -1;
            offset = ((Ovl11DDDFCState *)D_8006C838)->variant * 4 + 0x50;
            id = ((Ovl11DDDFCState *)D_8006C838)->index + offset;
            same = ((Ovl11SetupWorkView *)&D_8007AFF0)->id == id;
            ((Ovl11SetupWorkView *)&D_8007AFF0)->id = id;
            ovl_11_func_800DBFEC((u16)((Ovl11SetupWorkView *)&D_8007AFF0)->id, 0x8000, same);
            Ovl11UpdateEventSlotCounters();
            ovl_11_func_801129EC();
            ovl_11_func_800BD5A8(*(s16 *)(((Ovl11SetupWorkView *)&D_8007AFF0)->header + 0xA));
            ovl_11_func_800BD6A8(*(s16 *)(((Ovl11SetupWorkView *)&D_8007AFF0)->header + 0xC), 0);
            ovl_11_func_800BD3C4(0);
            ovl_11_func_800DB0E4();
            Ovl11ResetVertexBlendPool();
            ovl_11_func_800DC990();
            ovl_11_func_800DCA60();
            ovl_11_func_800BD5A8(((Ovl11SetupWorkView *)&D_8007AFF0)->music);
            ovl_11_func_800DB2AC((Ovl11SetupHeader *)D_8009F78C);
            ((Ovl11SetupWorkView *)&D_8007AFF0)->current = ((Ovl11SetupWorkView *)&D_8007AFF0)->previous;
            ((Ovl11SetupWorkView *)&D_8007AFF0)->header = D_8009F78C;
            Ovl11RegisterPoolSpriteRotations((s32)D_8009F78C);
            ovl_11_func_800CD0C0(((Ovl11DDDFCState *)D_8006C838)->position);
            ovl_11_func_800CBEF8();
            ((Ovl11DDDFCState *)D_8006C838)->status = (((Ovl11DDDFCState *)D_8006C838)->status | 0x4000) & 0xFFF07FFF;
            ovl_11_func_800CCC5C();
            ovl_11_func_800EEECC();
            ovl_11_func_800BD668();
            ovl_11_func_800F1F48(gGraphicsHeapBase + 0x4290);
            ovl_11_func_800C19B0();
            Ovl11ClearEventSlotTimeOrigins();
            ovl_11_func_800BFEA4();
            ovl_11_func_800BFF00();
            ovl_11_func_800C044C(((Ovl11DDDFCState *)D_8006C838)->mode);
            ((Ovl11SetupWorkView *)&D_8007AFF0)->state = 0;
            ((Ovl11DDDFCState *)D_8006C838)->phase++;
            if (GetGameFlag(0x48) == 0) {
                ovl_11_func_800BC868();
                while (IsSoundTransferComplete() == 0) {
                    UpdateMasterVolumeAndSoundTransfer();
                }
                OpenSoundSequencesAndStart();
                ovl_11_func_800BC9AC();
            }
        }
        break;
    case 5:
        ovl_11_func_800F4BB4();
        ovl_11_func_800DBC4C();
        ovl_11_func_8010C9B4();
        ovl_11_func_800F40A4();
        ovl_11_func_800F3D40();
        ovl_11_func_800F2620();
        ovl_11_func_800DE76C();
        ovl_11_func_800F4D28();
        ((Ovl11DDDFCState *)D_8006C838)->status &= 0xFBFFFFFF;
        switch (((Ovl11DDDFCState *)D_8006C838)->mode) {
        case 1:
            ovl_11_func_800BF844();
            ovl_11_func_800D8FC8(0);
            break;
        case 2:
            ovl_11_func_800D8FC8(1);
            break;
        case 3:
        case 4:
            ovl_11_func_800D9064();
            ovl_11_func_800C05F8();
            break;
        }
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        if (Ovl11IsClockAdvanceActive() == 0) {
            Ovl11SetClockAdvanceRefresh(0);
            ovl_11_func_800DE4C0();
        } else {
            Ovl11SetClockAdvanceRefresh(1);
        }
        break;
    case 6:
        SetGameFlag(3, 0);
        ((Ovl11DDDFCState *)D_8006C838)->flags &= 0xF7FFFFFF;
        result = 1;
        break;
    }
    return result;
}
