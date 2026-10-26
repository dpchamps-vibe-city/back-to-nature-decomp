#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

/* Draws the six-row card table and returns the selected row's weight * unkC
 * total (shown only when arg0 is set).
 *
 * `text` is reused as a scratch string pointer, and it is bound to the row
 * record before the name is chosen. All four sets are dead. cse forwards the
 * three string addresses into the call arguments, and both arms overwrite the
 * row binding. Because `text` has live uses, the sets survive cse's
 * trivially-dead sweep and count as real loop insns until flow deletes them.
 * The re-run loop pass needs at least 117 such insns to judge the
 * loop-invariant &D_800556A8 high/lo pair "not desirable"
 * (threshold 29 * savings 2 * lifetime 2 = 116 < insn_count). The pair then
 * stays local and takes $v0 ahead of the nameOffset load, as in the target.
 * Removing any one of the four sets leaves 116 insns, and the pair is hoisted
 * and rematerialised by reload instead (186/194). See
 * notes/research/harness-improvement-case-studies/ovl_11_func_80110944-dead-bindings-and-compiler-history.md
 * for the dead-binding mechanism. */
s16 ovl_17_func_800B9940(s32 arg0) {
    s32 surface;
    u16 total;
    u16 row;
    s16 y;
    s32 i;
    u8 *text;

    surface = gActiveRenderContext->field_D8 + 8;
    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    total = 0;
    y = 0x40;
    for (i = 0; i < 6; i++) {
        if (((Ovl17RowsView *)gOvl17RecordWorkBase)->selected == i) {
            D_800BD738[0] = 0x90;
            D_800BD738[1] = 0xFFFF;
            text = (u8 *)D_800BD738;
            func_80017B3C(surface, (s32)text, 0x2E, y);
        }
        D_800BD738[1] = 0xFFFF;
        D_800BD738[0] = D_800BB6EC[i];
        text = (u8 *)D_800BD738;
        func_80017B3C(surface, (s32)text, 0x38, y);
        text = (u8 *)&((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[i];
        if (((Ovl17ModeView *)D_8006C838)->mode == 3 && i == 2) {
            text = (u8 *)&D_80075AD8;
        } else {
            text = &D_800556A8[((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[i].nameOffset];
        }
        func_80017B3C(surface, (s32)text, 0x46, y);
        func_80017B3C(surface, (s32)D_800BB49A, 0x8E, y);
        *(u16 *)func_8001A970(((Ovl17ModeView *)D_8006C838)->cards[row][i].weight, (s16 *)D_800BD738, 2) = 0xFFFF;
        func_80017B3C(surface, (s32)D_800BD738, 0xB2, y);
        *(u16 *)func_8001A970(((Ovl17ModeView *)D_8006C838)->cards[row][i].unkC, (s16 *)D_800BD738, 2) = 0xFFFF;
        func_80017B3C(surface, (s32)D_800BD738, 0xCA, y);
        text = &D_800BB49A[5];
        func_80017B3C(surface, (s32)text, 0xDE, y);
        if (((Ovl17RowsView *)gOvl17RecordWorkBase)->selected == i) {
            total += (u16)((Ovl17ModeView *)D_8006C838)->cards[row][i].weight * (u16)((Ovl17ModeView *)D_8006C838)->cards[row][i].unkC;
        }
        y += 13;
    }
    func_80017B3C(surface, (s32)&D_800BB49A[13], 0x4E, 0x90);
    *(u16 *)func_8001A970(arg0 != 0 ? total : 0, (s16 *)D_800BD738, 4) = 0xFFFF;
    func_80017B3C(surface, (s32)D_800BD738, 0xB6, 0x90);
    return total;
}
