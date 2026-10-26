#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"

/* Game callees (signatures from include/functions.h; not included to avoid
 * SDK type conflicts — project convention, see func_80011370.c). */
void func_80017A48(u32 arg0);
u32 func_80017A64(void);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2) {
    u32 temp_s3;

    temp_s3 = func_80017A64();
    func_80017A48(3U);
    DrawMenuPanel(arg0, 0, 0x1A, 0xA4, 0x10B, 0x3E);
    func_80017B3C(arg1, arg2, 0x1E, 0xA8);
    func_80017A48(temp_s3);
}
