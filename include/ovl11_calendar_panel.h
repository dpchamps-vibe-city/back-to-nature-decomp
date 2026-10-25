#ifndef OVL11_CALENDAR_PANEL_H
#define OVL11_CALENDAR_PANEL_H

#include "common.h"

/* ovl_11's interactive 30-day calendar. Requires initialized pad input,
 * render contexts, sprite/text assets and the message-window service.
 * No elapsed-time argument: call Update once per rendered frame while the
 * overlay is resident. It draws into the active context and consumes pad 0.
 * This is a viewer: the game clock and saved date are not modified.
 * Drawing shares the engine sprite source and scratch text; do not reenter. */

/* Set inactive, select day 0, enable the retail close hook. Does NOT reset
 * gOvl11CalendarMessageState, close a message window, or restore text state.
 * Original identity: ovl_11:0x800F9D3C. */
void Ovl11ResetCalendarPanel(void);

/* Open with the current zero-based day selected, playing sound 3 and
 * calling the retail entry hook. No return/status value or index checking.
 * Original identity: ovl_11:0x800F9D5C. */
void Ovl11OpenCalendarPanel(void);

/* Inactive: no effects. Interactive: dim the screen, draw year/season and
 * all 30 days, navigate, and request event text from message archive 3.
 * Closing: keep dimming until message status == 2, then reset the message
 * service, become inactive and invoke the surrounding UI's exit helper.
 * Restores the incoming text style; portrait/text controls are stateful.
 * Dates and event days are zero-based internally, rendered as day+1/year+1.
 * Original identity: ovl_11:0x800F9DB0. */
void Ovl11UpdateCalendarPanel(void);

#endif
