#include "common.h"

void func_80024A4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);

/* The wrapper itself narrows all three words (800248EC..80024910).
 * Word-sized parameters also describe its unprototyped overlay callers;
 * do not move those conversions to the caller. */
void func_800248E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 x = arg1;
    s16 y = arg2;
    s16 frame = arg3;

    func_80024A4C(arg0, 0x18, frame, x, y);
}
