#include "common.h"
#include "ovl15_save_menu_lifecycle.h"

void ovl_15_func_801300E4(void) {
    Ovl15ResetSaveMenuState();
    gOvl15MenuExitRequest = -1;
}
