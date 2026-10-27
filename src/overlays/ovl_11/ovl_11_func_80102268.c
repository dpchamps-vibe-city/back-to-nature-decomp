#include "common.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

extern s8 gOvl11SelectionModePhase;

void *ovl_11_func_80102844(s8 arg0, s8 arg1);

void ovl_11_func_80102268(s8 arg0, s8 arg1) {
    s16 *p;
    s32 v;

    p = ovl_11_func_80102844(arg1, arg0);
    v = p[0];
    if (v == 0xA4) {
        RequestMessageWindow(3, Ovl11GetRecordItemId(p) + 0x3E8);
    } else {
        RequestMessageWindow(3, v);
    }
    if (GetMessageWindowStatus() == 2) {
        gOvl11SelectionModePhase = 3;
    }
}
