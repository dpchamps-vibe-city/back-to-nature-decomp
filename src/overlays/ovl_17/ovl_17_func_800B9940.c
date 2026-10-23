#include "common.h"

INCLUDE_ASM("build/ovl_17/asm/nonmatchings/ovl_17_func_800B9940", ovl_17_func_800B9940);


/* Best attempt, 2026-10-23: natural typed C over the Ovl17ModeView card table
 * and the Ovl17RowsView display rows. Population and schedule are exact; five
 * allocation words remain (blocks 8 and 9). Loop pass 2 hoists the
 * &D_800556A8 high/lo pair (threshold 30 * savings 2 * lifetime 2 = 120 >= 110
 * insns), reload rematerialises it, and the round-robin spill register then
 * swaps block 9. The source is byte-exact under -fno-rerun-loop-opt, but
 * matched neighbours 800B986C, 800B8294, 800B8898 and 800BA704 need the second
 * loop pass, so the flag is not the answer. The original's pass-2 loop body
 * must have had >= 121 insns, or moved another movable first (threshold 27).
 * See the closed-directions record for this function.
 * Earlier parks: notes/human-needed-approvals/ovl_17_func_800B9940.md
 */

#if 0
#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

s16 ovl_17_func_800B9940(s32 arg0) {
    s32 surface;
    u16 total;
    u16 row;
    s16 y;
    s32 i;
    u8 *text;

    surface = D_8005E3C0->field_D8 + 8;
    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    total = 0;
    y = 0x40;
    for (i = 0; i < 6; i++) {
        if (((Ovl17RowsView *)D_800BD848)->selected == i) {
            D_800BD738[0] = 0x90;
            D_800BD738[1] = 0xFFFF;
            func_80017B3C(surface, (s32)D_800BD738, 0x2E, y);
        }
        D_800BD738[1] = 0xFFFF;
        D_800BD738[0] = D_800BB6EC[i];
        func_80017B3C(surface, (s32)D_800BD738, 0x38, y);
        if (((Ovl17ModeView *)D_8006C838)->mode == 3 && i == 2) {
            text = (u8 *)&D_80075AD8;
        } else {
            text = &D_800556A8[((Ovl17RowsView *)D_800BD848)->rows[i].nameOffset];
        }
        func_80017B3C(surface, (s32)text, 0x46, y);
        func_80017B3C(surface, (s32)D_800BB49A, 0x8E, y);
        *(u16 *)func_8001A970(((Ovl17ModeView *)D_8006C838)->cards[row][i].weight, (s16 *)D_800BD738, 2) = 0xFFFF;
        func_80017B3C(surface, (s32)D_800BD738, 0xB2, y);
        *(u16 *)func_8001A970(((Ovl17ModeView *)D_8006C838)->cards[row][i].unkC, (s16 *)D_800BD738, 2) = 0xFFFF;
        func_80017B3C(surface, (s32)D_800BD738, 0xCA, y);
        func_80017B3C(surface, (s32)&D_800BB49A[5], 0xDE, y);
        if (((Ovl17RowsView *)D_800BD848)->selected == i) {
            total += (u16)((Ovl17ModeView *)D_8006C838)->cards[row][i].weight * (u16)((Ovl17ModeView *)D_8006C838)->cards[row][i].unkC;
        }
        y += 13;
    }
    func_80017B3C(surface, (s32)&D_800BB49A[13], 0x4E, 0x90);
    *(u16 *)func_8001A970(arg0 != 0 ? total : 0, (s16 *)D_800BD738, 4) = 0xFFFF;
    func_80017B3C(surface, (s32)D_800BD738, 0xB6, 0x90);
    return total;
}
#endif
