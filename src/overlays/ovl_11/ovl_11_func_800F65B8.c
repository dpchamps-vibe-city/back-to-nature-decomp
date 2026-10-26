#include "common.h"
#include "game_types.h"
#include "ovl11_calendar_panel.h"
#include "ovl11_item_transfer_menu.h"
#include "ovl11_clock_hud.h"
#include "selection_panel.h"
#include "ovl11_counter_choice.h"
#include "ovl11_interaction_sequence.h"
#include "ovl11_selection_mode_state.h"
#include "ovl11_options_menu.h"

void func_8002470C(s16 arg0);

void ovl_11_func_800FC054(void);

void ovl_11_func_801038E4(void);

void ovl_11_func_800F65B8(void) {
    func_8002470C(0);
    Ovl11ServiceInteractionSequence();
    Ovl11UpdateCalendarPanel();
    Ovl11UpdateItemTransferMenu();
    Ovl11UpdateClockHud();
    Ovl11DrawOptionsMenu();
    Ovl11ServiceSelectionMode();
    Ovl11ServiceMenuChoice();
    ovl_11_func_800FC054();
    ovl_11_func_801038E4();
    Ovl11UpdateSelectionPanel();
    Ovl11UpdateCounterChoice();
}
