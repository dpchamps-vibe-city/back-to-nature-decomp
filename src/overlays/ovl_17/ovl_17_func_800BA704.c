#include "common.h"

/* Resets the six D_800BD848 display rows and loads each with the x/y and
 * attribute halfwords of one card-table row: the row named by the card-game
 * mode, or row 2 in mode 3, which also flags display row 2 and shows the
 * special slot from +0x9348/+0x92B2. */
void ovl_17_func_800BA704(void) {
    u16 row;
    s32 i;

    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    for (i = 0; i < 6; i++) {
        ((Ovl17RowsView *)D_800BD848)->rows[i].flags = 0;
        ((Ovl17RowsView *)D_800BD848)->rows[i].unk2A = 0xFF;
        ((Ovl17RowsView *)D_800BD848)->rows[i].unk2C = 0;
        ((Ovl17RowsView *)D_800BD848)->rows[i].unk2E = 0;
        ((Ovl17RowsView *)D_800BD848)->rows[i].unk30 = 0;
        ((Ovl17RowsView *)D_800BD848)->rows[i].x =
            ((Ovl17ModeView *)D_8006C838)->cards[row][i].x;
        ((Ovl17RowsView *)D_800BD848)->rows[i].y =
            ((Ovl17ModeView *)D_8006C838)->cards[row][i].y;
        ((Ovl17RowsView *)D_800BD848)->rows[i].attr[0] =
            ((Ovl17ModeView *)D_8006C838)->cards[row][i].attr[0];
        ((Ovl17RowsView *)D_800BD848)->rows[i].attr[1] =
            ((Ovl17ModeView *)D_8006C838)->cards[row][i].attr[1];
        ((Ovl17RowsView *)D_800BD848)->rows[i].unk38 = -0x20000;
        ((Ovl17RowsView *)D_800BD848)->rows[i].data = &D_800BD758[i * 6];
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        ((Ovl17RowsView *)D_800BD848)->rows[2].flags = 1;
        ((Ovl17RowsView *)D_800BD848)->rows[2].x =
            ((Ovl17ModeView *)D_8006C838)->unk9348;
        ((Ovl17RowsView *)D_800BD848)->rows[2].y =
            ((Ovl17ModeView *)D_8006C838)->unk92B2;
    }
}
