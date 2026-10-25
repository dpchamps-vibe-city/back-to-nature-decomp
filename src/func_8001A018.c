#include "common.h"
#include "psyq/libapi.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "message_buffer_access.h"

/* GP-relative CD table state reached GP-relatively by this TU.
 * Tentative definitions merged via -fcommon with sibling definitions. */
u32 D_8005E440;
u32 D_8005E45C;
s16 D_8005E460;
u32 D_8005E46C;

void func_8001A018(s32 arg0, s32 arg1) {
    s32 slot;
    s32 offset;
    u8 *buf;

    slot = (s16) arg0;
    if (slot == D_8005E460) {
        return;
    }
    if (slot < 0) {
        return;
    }
    offset = slot * 3 * 2048;
    if (arg1 != 0 && D_8005E45C != 0) {
        goto retry;
    }
    if (GetAvailableMessageReadBuffer() == NULL) {
        SystemError(0x54, 0);
    }
    CdReadArchiveBytes(1, offset, 0x1800, (u8 *)&gMessageWindowReadBuffer, 1, 1);
    D_8005E45C = 1;
    D_8005E440 = slot;
retry:
    while (1) {
        buf = CdReadArchiveBytes(1, offset, 0x1800, (u8 *)&gMessageWindowReadBuffer, 1, 0);
        D_8005E46C = (u32) buf;
        if (buf != NULL) {
            D_8005E460 = *(u16 *)&D_8005E440;
            return;
        }
        if (arg1 != 0) {
            return;
        }
    }
}
