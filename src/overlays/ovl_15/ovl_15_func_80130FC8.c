#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libmcrd.h"
#include "psyq/memory.h"
#include "ovl15_save_menu_lifecycle.h"


long MemCardSync (long mode, long *cmds, long *rslt);
void ovl_15_func_8013468C (void);

void ovl_15_func_80130FC8(void) {
    Ovl15ResetSaveMenuState();
    gOvl15MenuExitRequest = -1;
}
