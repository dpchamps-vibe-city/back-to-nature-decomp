#include "common.h"
#include "message_window.h"

extern s16 D_800BFE44;

void ovl_25_func_800BA908(void) {
    char *base;
    char *p;
    char *ref;
    s16 value;

    base = (char *)&D_8007AFF0;
    p = base + 0x20000;
    ref = (char *)&D_800BFE44;
    value = *(s16 *)(p + 0x53B4);
    if (value <= *(s16 *)(ref + 0x20)) {
        *(u16 *)(p + 0x53B4) = *(u16 *)(ref + 0x20);
        RequestMessageWindow(4, 0x33);
        if (GetMessageWindowStatus() == 2) {
            if (GetMessageWindowChoice() == 1) {
                *(s16 *)(ref + 2) = 4;
            } else {
                *(s16 *)(ref + 2) = 0;
            }
        }
    } else {
        *(u16 *)(p + 0x53B4) = *(u16 *)(p + 0x53B4) - 0x21;
    }
}
