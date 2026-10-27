#ifndef OVL11_CALENDAR_HOOKS_H
#define OVL11_CALENDAR_HOOKS_H

#include "common.h"

/* Calendar-panel lifecycle, retained even though retail hooks have no effects.
 * OpenHook runs after the panel samples CloseHookEnabled; CloseHook runs on
 * either close-input path only when that sampled value is exactly 1.
 * No arguments, storage, drawing, or clock changes; the query always returns 1.
 * These are callable functions, not macros: their original calls are retained.
 */
void Ovl11CalendarOpenHook(void);
void Ovl11CalendarCloseHook(void);
s32 Ovl11CalendarCloseHookEnabled(void);

#endif
