/* Resident menu frame coordinator, ovl_11:800F65B8–800F6630.
 * Called by the area-frame updater. Order is significant: advance the shared
 * animation clock before any controller can submit its menu packets.
 * Individual controllers own their state and decide whether they are active.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */

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
#include "ovl11_status_pages.h"
#include "ovl11_menu_frame_update.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ui_sprites.h"

void ovl_11_func_801038E4(void);

void Ovl11UpdateMenuFrame(void) {
    StepUiSpriteFourFramePhase(0);
    Ovl11ServiceInteractionSequence();
    Ovl11UpdateCalendarPanel();
    Ovl11UpdateItemTransferMenu();
    Ovl11UpdateClockHud();
    Ovl11DrawOptionsMenu();
    Ovl11ServiceSelectionMode();
    Ovl11ServiceMenuChoice();
    Ovl11UpdateStatusPages();
    ovl_11_func_801038E4();
    Ovl11UpdateSelectionPanel();
    Ovl11UpdateCounterChoice();
}
