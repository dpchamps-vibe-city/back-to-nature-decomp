/* Read-only adjustment shared by held-item and cell-action clients.
 * Keep the work-area-relative reads: these are existing aggregate views,
 * not new definitions of the clock/day-condition or primary-object state.
 * Original entry: ovl_11:800CD3C4; boundaries: notes/file-groupings.md. */
#include "ovl11_action_status_delta.h"

s32 Ovl11AdjustActionStatusDelta(s32 baseDelta) {
    char *object = (char *)&gPrimaryObjectBase;
    char *conditionBase = object + 0x2E38;
    char *work = object - 0x51C8;
    u16 hour;

    if (*(s16 *)(conditionBase + 0x64C8) != 0) {
        baseDelta += 1;
    }
    hour = *(u16 *)(work + 0x44C0);
    if ((u32)(hour - 0x16) < 2) {
        baseDelta += 1;
    }
    if (hour < 6) {
        baseDelta += 2;
    }
    if (*(s16 *)(object + 0x12) == 0) {
        baseDelta += 2;
    }
    if (*(s32 *)(object + 0x6C) & 0x200) {
        baseDelta /= 2;
    }
    if (*(u16 *)(object + 0x36) & 8) {
        baseDelta = 0;
    }
    return baseDelta;
}
