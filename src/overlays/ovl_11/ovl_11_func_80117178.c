#include "common.h"

s32 ovl_11_func_80117178(s16 arg0, s16 arg1, s16 arg2) {
    s32 count = 0;
    s16 result = 0;
    s32 i;
    u8 *base;

    switch (arg0) {
    case 0:
        for (i = 0; i < 10; i++) {
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[i].type == 0x109) {
                if (!(((struct Ovl11Scan109Globals *)D_8006C838)->records[i].flags & 0x40000)) {
                    count = (s16)(count + 1);
                }
                if (count == arg2) {
                    result = i;
                    break;
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < 20; i++) {
            base = (u8 *)D_8006C838 + i * 0xB8;
            if (*(u16 *)(base + 0x81BC) == 0x162) {
                if (arg1 == 0) {
                    if (*(u16 *)(base + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                } else if (arg1 == 1) {
                    if (*(u16 *)(base + 0x826A) == 0 && *(u16 *)(base + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                }
                if (count == arg2) {
                    result = i;
                    break;
                }
            }
        }
        break;
    case 2:
        for (i = 0; i < 20; i++) {
            base = (u8 *)D_8006C838 + i * 0xB8;
            if (*(u16 *)(base + 0x81BC) == 0x165) {
                if (arg1 == 0) {
                    count = (s16)(count + 1);
                } else if (arg1 == 1) {
                    if (*(u16 *)(base + 0x8270) >= 7 && *(u16 *)(base + 0x826A) == 0 &&
                        *(u16 *)(base + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                }
                if (count == arg2) {
                    result = i;
                    break;
                }
            }
        }
        break;
    }
    if (arg2 == -1) {
        return count;
    }
    return result;
}
