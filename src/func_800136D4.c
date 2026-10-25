#include "common.h"
#include "screen_primitives.h"

void func_800136D4(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s16 temp_s0;
    s16 temp_s1;
    s16 temp_s2;
    s16 temp_s3;
    s16 temp_s5;
    s16 temp_s6;
    s32 temp_v0;
    s32 temp_v0_2;

    sp28 = (s16)(arg3 - 1);
    temp_v0 = arg1 - 1;
    sp2C = (s16)(sp28 + temp_v0);
    temp_s2 = arg4 - 1;
    DrawScreenPoint(arg0, sp2C, arg2, 0xF8DBAF, 0);
    temp_s5 = arg2 + 1;
    DrawScreenPoint(arg0, sp2C, temp_s5, 0xDEBB84, 0);
    temp_v0_2 = arg1 + sp28;
    sp30 = (s16)temp_v0_2;
    DrawScreenPoint(arg0, (s16)temp_v0_2, temp_s5, 0xC89F60, 0);
    temp_s3 = arg1 + 1;
    temp_s1 = temp_s2 - 1 + arg2;
    DrawScreenPoint(arg0, temp_s3, temp_s1, 0xE2C08B, 0);
    temp_s6 = arg2 + temp_s2;
    DrawScreenPoint(arg0, temp_s3, temp_s6, 0xC89F60, 0);
    DrawScreenPoint(arg0, arg1, temp_s1, 0xF8DBAF, 0);
    temp_s0 = (s16)(sp28 - 2 + arg1);
    {
        s32 sp34;

        DrawScreenLine(arg0, temp_s3, temp_s5, temp_s0, temp_s5, 0xFDECD2, 0);
        DrawScreenLine(arg0, temp_s3, temp_s5, temp_s3, temp_s1, 0xFDECD2, 0);
        /* Assigned at first use, not at the declaration: the assignment's
         * statement position decides this constant's materialization order
         * against the 0xFDECD2 web in the prologue. */
        sp34 = 0xBA8B47;
        DrawScreenLine(arg0, sp2C, temp_s5, sp2C, temp_s1, sp34, 0);
        DrawScreenLine(arg0, temp_s3, temp_s1, temp_s0, temp_s1, sp34, 0);
        DrawScreenLine(arg0, temp_s3, arg2, (s16)sp2C, arg2, 0xFDECD2, 0);
        DrawScreenLine(arg0, arg1, temp_s5, arg1, temp_s1, 0xFDECD2, 0);
        DrawScreenLine(arg0, (s16)temp_v0_2, temp_s5, (s16)temp_v0_2, temp_s1, sp34, 0);
        DrawScreenLine(arg0, temp_s3, temp_s6, (s16)sp2C, temp_s6, sp34, 0);
    }
    DrawScreenGradientRect((s32 *)arg0, (s16)(arg1 + 2), (s16)(arg2 + 2), sp28 - 3, temp_s2 - 3, 0xF4DFBD, 0xF4DFBD, 0xF9CF8F, 0xF9CF8F, 0);
}
