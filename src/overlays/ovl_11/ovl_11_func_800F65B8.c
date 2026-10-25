#include "common.h"
#include "game_types.h"
#include "ovl11_calendar_panel.h"
#include "ovl11_item_transfer_menu.h"
#include "ovl11_clock_hud.h"

void func_8002470C(s16 arg0);

void ovl_11_func_800F67E0(void);

void ovl_11_func_800FECB8(void);

void ovl_11_func_800FFF7C(void);

void ovl_11_func_800FFA64(void);

void ovl_11_func_800FC054(void);

void ovl_11_func_801038E4(void);

void ovl_11_func_8011E5B0(void);

void ovl_11_func_8011F5BC(void);

void ovl_11_func_800F65B8(void) {
    func_8002470C(0);
    ovl_11_func_800F67E0();
    Ovl11UpdateCalendarPanel();
    Ovl11UpdateItemTransferMenu();
    Ovl11UpdateClockHud();
    ovl_11_func_800FECB8();
    ovl_11_func_800FFF7C();
    ovl_11_func_800FFA64();
    ovl_11_func_800FC054();
    ovl_11_func_801038E4();
    ovl_11_func_8011E5B0();
    ovl_11_func_8011F5BC();
}
