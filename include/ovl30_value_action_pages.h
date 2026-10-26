#ifndef OVL30_VALUE_ACTION_PAGES_H
#define OVL30_VALUE_ACTION_PAGES_H

#include "common.h"

/* Overlay-30 page callbacks. Input/output is a descriptor-row cursor, not
 * a data value. While the screen fade is incomplete they return it unchanged.
 * When active, they print through the kanji font, read pad 0 and edit data
 * through the descriptor widget. Tables contain 22 main, 23 player, 29 action
 * and 48 pet rows; callers must supply a cursor valid for the selected page. */
s32 Ovl30EditMainData(s32 cursor);
s32 Ovl30EditPlayerData(s32 cursor);
s32 Ovl30EditPetData(s32 cursor);

/* Uses the same cursor contract, but dispatches gOvl30RecordMenuSelectedId.
 * Completed commands reset that shared selection to -1. Actions can modify
 * work/save records, create objects, request pad vibration and advance time;
 * this is a diagnostic interface, not a validated gameplay command API.
 * Overlay-11 services and the shared game work area must be resident. */
s32 Ovl30DispatchDebugActions(s32 cursor);

#endif
