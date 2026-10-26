/*
 * cell_status_panel — reconstructed translation unit.
 *
 * Nine-cell status confirmation panel. Start snapshots the first status-2
 * cell (or -1), resets the choice cursor and returns 1. The frame callback
 * returns 0 while displaying prompts, 1 after closing. Both are installed
 * by the overlay's interaction callback tables; all helpers keep external
 * linkage without being published as a caller interface.
 *
 * Weekday 6 permits selection. The first confirmation debits 10 balance
 * units; the second checks the table item's cost, debits it, changes the
 * selected cell to 3 and sets game flag cellIndex+0x67. Cancelling the second
 * prompt does not refund the first debit. Shared message/cursor state makes
 * this protocol non-reentrant.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_balance_accounting.h"

s32 Ovl11FindPendingCellStatus(void);
void Ovl11ResetCellStatusPrompt(void);
s32 Ovl11PollCellStatusChoice(void);
s32 Ovl11ConfirmCellStatus(u32 cellIndex);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D6014(u16 arg0);
void SetVal8005E2BC(s32 arg0);

s32 Ovl11BeginCellStatusPanel(void) {
    s32 selectedCell;
    gOvl11CellStatusPhase = 1;
    selectedCell = Ovl11FindPendingCellStatus();
    gOvl11CellStatusSelection = selectedCell;
    gOvl11InteractionDrawMode = 0;
    ovl_11_func_80118B84(0, -1, 0);
    return 1;
}

s32 Ovl11UpdateCellStatusPanel(void) {
    s32 message;
    s32 choice;

    switch (gOvl11CellStatusPhase) {
    case 0:
    default:
        gOvl11InteractionDrawMode = 0;
        CloseMessageWindow();
        return 1;
    case 1:
        if (gCalendarWeekday == 6) {
            if (gOvl11CellStatusSelection != -1) {
                message = 0x11B;
                goto beginSelection;
            }
            message = 0x11A;
            gOvl11CellStatusPhase = 8;
            goto display;
        } else {
            message = 0x11A;
            gOvl11CellStatusPhase = 8;
            goto display;
        }
    beginSelection:
        gOvl11CellStatusPhase = 2;
        Ovl11ResetCellStatusPrompt();
        gOvl11InteractionDrawMode = 3;
    display:
        RequestMessageWindow(2, message);
        return 0;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            gOvl11CellStatusPhase = 3;
        }
        break;
    case 3:
        switch (Ovl11PollCellStatusChoice()) {
        case 1:
            if (gSpendableBalance >= 10) {
                Ovl11AdjustSpendableBalance(-10, 1);
                gOvl11CellStatusPhase = 4;
                message = 0x11C;
            } else {
                gOvl11CellStatusPhase = 8;
                message = 0x4F;
            }
            CloseMessageWindow();
            goto display;
        case 2:
            gOvl11CellStatusPhase = 0;
            break;
        }
        break;
    case 4:
        if (GetMessageWindowStatus() == 2) {
            gOvl11CellStatusPhase = 5;
        }
        break;
    case 5:
        message = gOvl11CellStatusSelection + 0x11D;
        gOvl11CellStatusPhase = 6;
        Ovl11ResetCellStatusPrompt();
        goto display;
    case 6:
        if (GetMessageWindowStatus() == 5) {
            gOvl11CellStatusPhase = 7;
        }
        break;
    case 7:
        choice = Ovl11PollCellStatusChoice();
        switch (choice) {
        case 1:
            message = 0x4F;
            if (Ovl11ConfirmCellStatus(gOvl11CellStatusSelection) == 1) {
                message = 0x126;
            }
            gOvl11CellStatusPhase = 8;
            CloseMessageWindow();
            goto display;
        case 2:
            gOvl11CellStatusPhase = 8;
            CloseMessageWindow();
            RequestMessageWindow(2, 0x127);
            break;
        }
        break;
    case 8:
        if (GetMessageWindowStatus() == 2) {
            gOvl11CellStatusPhase = 0;
        }
        break;
    }
    return 0;
}

s32 Ovl11FindPendingCellStatus(void) {
    s32 cellIndex;
    u8 *cell;

    cellIndex = 0;
    cell = gChoiceCellStates;
    while (cellIndex < 9) {
        if (*cell == 2) {
            return cellIndex;
        }
        cellIndex++;
        cell++;
    }
    return -1;
}

void Ovl11ResetCellStatusPrompt(void) {
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
}

s32 Ovl11PollCellStatusChoice(void) {
    if (GetMessageWindowStatus() == 5) {
        return ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x28, 0x83, 0x48);
    }
    return 0;
}

/* The index guard precedes table lookup. The shared nine-cell view also
 * covers index 8, which the former eight-byte local view did not describe. */
s32 Ovl11ConfirmCellStatus(u32 cellIndex) {
    s32 cost;
    u8 *base;

    if ((cellIndex >= 9U) || (cost = ovl_11_func_800D6014(gOvl11CellStatusItemIds[cellIndex]), base = (u8 *)D_8006C838, (*(s32 *)(base + 0x5224) < cost) != 0)) {
        return 0;
    }
    Ovl11AdjustSpendableBalance(-cost, 1);
    ((Ovl11StatesE7A2View *)base)->states[cellIndex] = 3;
    SetGameFlag((cellIndex + 0x67) & 0xFFFF, 1U);
    return 1;
}
