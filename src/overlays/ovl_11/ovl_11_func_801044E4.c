#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "card_deal.h"
#include "ovl11_selection_message.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vertex_blend.h"
#include "message_window.h"
#include "ovl11_choice_messages.h"
#include "ovl11_daily_conditions.h"
#include "ovl11_sprite_effect_pool.h"
#include "ovl11_object_event_pool.h"

void func_80021D64 (void);
void ovl_11_func_80104A80 (void);
void ovl_11_func_801047FC (void);
void ovl_11_func_800D6628 (void);
void ovl_11_func_800C9C60 (void);
void ovl_11_func_800BFEA4 (void);
void ovl_11_func_800BFF00 (void);
void ovl_11_func_800DBFA0 (void);
void ovl_11_func_800DAFD4 (void);
void ovl_11_func_800DB0E4 (void);
void ovl_11_func_800DBFC4 (void);
void ovl_11_func_800D7B24 (void);
void ovl_11_func_800BD358 (void);
void ovl_11_func_800BD5A0 (void);
void ovl_11_func_800BD538 (void);
s32 ovl_11_func_800CDE3C (u8 *arg0);
void ovl_11_func_800BD488 (s32 arg0);
void ovl_11_func_800DC990 (void);
void ovl_11_func_800F6630 (void);
void ovl_11_func_800F42A8 (u16 arg0, s32 arg1);
void ovl_11_func_800CE96C (void);
void ovl_11_func_800CBEF8 (void);
void ovl_11_func_8010C6A0 (void);
s32 *ovl_11_func_80111E38 (void);
void ovl_11_func_800F2508 (void);
void ovl_11_func_80104898 (void);
void ovl_11_func_800FFA28 (void);
void ovl_11_func_801128B4 (void);
void ovl_11_func_800BD668 (void);
void ovl_11_func_800F1F48 (s32 arg0);
void ovl_11_func_80104A58 (void);
void ovl_11_func_80104A88 (void);
u16 *ovl_11_func_800CE744 (s32 arg0, s32 arg1);
void ovl_11_func_800DB9A4(void);
void ovl_11_func_8010476C(void);
void ovl_11_func_80112D10(void);

void ovl_11_func_801044E4(void) {
    Ovl11StartupView *base;
    Ovl11FarInitView *far;

    func_80021D64();
    base = (Ovl11StartupView *)D_8006C838;
    if (base->flags & 0x8000) {
        *base->reset = 0;
        gRandomState = 1;
    }
    ovl_11_func_80104A80();
    ovl_11_func_801047FC();
    ovl_11_func_800D6628();
    ovl_11_func_800DB9A4();
    Ovl11InitClockAndCalendar();
    ovl_11_func_800C9C60();
    Ovl11ResetAllEventSlots();
    Ovl11AllocateAllEventSlots();
    ovl_11_func_800BFEA4();
    ovl_11_func_800BFF00();
    ovl_11_func_800DBFA0();
    ovl_11_func_800DAFD4();
    ovl_11_func_800DB0E4();
    ovl_11_func_800DBFC4();
    ovl_11_func_800D7B24();
    ovl_11_func_800BD358();
    ovl_11_func_800BD5A0();
    ovl_11_func_800BD538();
    ovl_11_func_800CDE3C(base->field_51C8);
    ovl_11_func_800BD488((s32) base->field_44BA);
    ovl_11_func_800DC990();
    Ovl11ResetVertexBlendPool();
    ovl_11_func_800F6630();
    ovl_11_func_800F42A8((u16) base->field_44BA, 0);
    ovl_11_func_800CE96C();
    ovl_11_func_800CBEF8();
    Ovl11ResetChoiceMessages();
    ovl_11_func_800F6630();
    Ovl11ResetSelectionMessage();
    ovl_11_func_8010C6A0();
    ResetMessageWindow();
    ovl_11_func_80111E38();
    ovl_11_func_800F2508();
    Ovl11ClearSpriteEffects();
    ovl_11_func_80104898();
    ovl_11_func_800FFA28();
    ovl_11_func_8010476C();
    ovl_11_func_801128B4();
    ovl_11_func_800BD668();
    ovl_11_func_800F1F48(gGraphicsHeapBase + 0x4290);
    far = (Ovl11FarInitView *)&D_8007AFF0;
    far->field_25498 = -1;
    base->field_4438 = -1;
    ovl_11_func_80112D10();
    ovl_11_func_80104A58();
    DealCardGameTable();
    Ovl11ApplyCurrentDayCondition();
    Ovl11ChooseNextDayCondition();
    ovl_11_func_80104A88();
    if (base->flags & 0x8000) {
        ovl_11_func_800CE744(0x162, -1);
        ovl_11_func_800CE744(0x160, -1);
        ovl_11_func_800CE744(0x161, -1);
        ovl_11_func_800CE744(0x162, -1);
        ovl_11_func_800CE744(0x164, -1);
        ovl_11_func_800CE744(0x165, -1);
        ovl_11_func_800CE744(0x10A, -1);
        ovl_11_func_800CE744(0x109, -1);
        ovl_11_func_800CE744(0x109, -1);
        ovl_11_func_800CE744(0x10A, -1);
        ovl_11_func_800CE744(0x109, -1);
        base->field_4502 = 0x32;
        base->field_4508 = 0x32;
    }
}
