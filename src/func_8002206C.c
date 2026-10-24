#include "common.h"
#include "psyq/memory.h"

u32 Rand(s32 arg0);
s32 func_80022014(s32 arg0, s16 arg1);

void func_8002206C(void) {
    CardGameState *state = (CardGameState *)&D_8007AD4C;
    s16 *walk;
    s16 *pW;
    u32 rows;
    u32 n;
    s16 v;
    s32 saved;
    s32 max;
    s32 modeY;

    saved = state->mode;
    memset(state, 0, 0x108);
    for (rows = 0; rows < 3U; rows++) {
        s16 *clear;
        n = 0;
        clear = &D_8007AD4E;
        clear = (s16 *)((u8 *)clear + rows * 0x54);
        do {
            *clear = -1;
            clear += 7;
            n++;
        } while (n < 6U);
    }
    for (rows = 0; rows < 3U; rows++) {
        for (n = 0; n < 6U; n++) {
            do {
                v = Rand(40);
            } while (func_80022014((s32)state, v));
            *(s16 *)((u8 *)&D_8007AD4E + rows * 84 + n * 14) = v;
            *(u16 *)((u8 *)&D_8007AD54 + rows * 84 + n * 14) = D_800558A4[v * 2];
            *(u16 *)((u8 *)&D_8007AD56 + rows * 84 + n * 14) = D_800558A4[(v * 2) | 1];
            *(s16 *)((u8 *)&D_8007AD50 + rows * 84 + n * 14) = D_80055944[n * 4] +
                Rand((u16)(D_80055944[n * 4 + 1] - D_80055944[n * 4]));
            *(s16 *)((u8 *)&D_8007AD52 + rows * 84 + n * 14) = D_80055944[n * 4 + 2] +
                Rand((u16)(D_80055944[n * 4 + 3] - D_80055944[n * 4 + 2]));
        }
    }
    if (saved == 3) {
        Ovl17ModeView *q = (Ovl17ModeView *)D_8006C838;
        u32 modeX = q->unk9348;
        CardGameState *p = state;
        modeY = q->unk92B2;
        p->cards[2][2].value = -1;
        p->cards[2][2].x = modeX;
        p->cards[2][2].y = modeY;
        p->cards[2][2].attr[0] = 0;
        p->cards[2][2].attr[1] = 0;
    }
    rows = 0;
    do {
        max = 0;
        for (n = 0; n < 6U; n++) {
            if ((u32)max < (u32)state->cards[rows][n].x) {
                max = state->cards[rows][n].x;
            }
        }
        n = 0;
        pW = (s16 *)((char *)&D_8007AD58 + rows * 0x54);
        walk = (s16 *)((u32)&D_8007AD50 + rows * 84);
        do {
            *pW = (s16)(Rand(4) +
                ((u32)((max - *walk) * (max - *walk)) / 1000 + 1));
            walk += 7;
            n++;
            pW += 7;
        } while (n < 6U);
        rows++;
    } while (rows < 3U);
}

/* Historical notes and the old attempt are retained below. The active
 * implementation above matches using halfword-indexed colour-table reads;
 * the historical byte-address scratch hypothesis is superseded.
 */

/* PARKED by /auto_decompilation_loop on 2026-08-22T07:34:12.228Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/func_8002206C.md
 */

/* Breadcrumbs for the next attempt (2026-10-23): the methods that apply here,
 * each with its target evidence. Addresses are target addresses.
 *
 * Start point. The attempt preserved below is the 2026-08-22 park. A session
 * on 2026-10-22 corrected two operation boundaries. Its notes are in
 * build/func_8002206C/SESSION.md (local, untracked). Its most faithful source
 * is build/func_8002206C/weight-base-plus-random-jitter.c, [0,10,22,37]:
 *  - the first colour address is computed in place: colorAddress = v;
 *    colorAddress <<= 2; colorAddress += (s32)D_800558A4; then load it;
 *  - the weight calls Rand first: (s16)(Rand(4) + ((u32)((max - *pPos1) *
 *    (max - *pPos1)) / 1000 + 1)).
 * That key ranks below [0,9,24,44], and that source is still the better
 * base. Do not rank by the key alone.
 *
 * Data. D_8007AD4C is &((Ovl17ModeView *)D_8006C838)->mode, and the table
 * this clears and deals is that view's cards[3][6] (CardGameSlot, 14 bytes;
 * rows 0x54). ovl_17_func_800BA704 reads the same table through the cast.
 * The mode-3 block here (0x8002223C) has the far-field split documented on
 * the view: base + 0x8000, then offsets 0x1348 and 0x12B2.
 *
 * Flags: none. The weight pass's counter has the GCSE PRE shape
 * addiu s5,fp,1 (0x80022284) ... addu fp,s5,zero (0x8002234C). The corrected
 * source reproduces it under the default flags and loses it under
 * -fno-gcse. flagProbe's "in-place bottom increment" fingerprint is a known
 * false positive (ovl_21_func_800BA300). -mno-split-addresses scores worse on
 * the full byte oracle and loses the prologue's separate lui v0 / lui a0.
 *
 * 1. Three passes, three loop.c outcomes. Write each pass so that it gets
 *    its own:
 *    - clear: the row pointer is stepped in a register (addiu a0,a0,0x54 at
 *      0x800220F4);
 *    - deal: the row offset is stepped in a stack slot (0x14(sp): set at
 *      0x800220FC, stepped at 0x80022210-0x8002221C). Five field pointers
 *      s2/s4/s5/s6/s7 are built from separate symbol+offset constants,
 *      D_8007AD4E/50/52/54/56 (0x80022104-0x8002214C): one giv per field,
 *      not one merged pointer;
 *    - weights: not reduced. row * 84 is rebuilt by shifts twice per row
 *      (0x80022288, 0x800222CC), because the PRE copy above means fp is not
 *      a biv. notes/research/ovl_21_func_800BA300-shared-high.md has the same
 *      outer-increment shape.
 *
 * 2. Spill-slot provenance. The target spills two values:
 *    - 0x10: the mode halfword (lh at 0x80022080, stored in memset's delay
 *      slot, read back at 0x8002222C);
 *    - 0x14: the deal pass's row offset.
 *    reload assigns slots in pseudo order: source variables in declaration
 *    order, then GCSE temporaries, then loop.c registers. A row offset
 *    stepped by 0x54 in the slot after the mode fits either a source variable
 *    declared after the mode variable or a loop.c giv of rows * 0x54. Compare
 *    the candidate's slot owners with these before reordering declarations.
 *    The value is spilled rather than caller-saved, so its refs are at most
 *    4 x (calls crossed) (regs.h:201).
 *
 * 3. Allocation reads register usage recomputed after sched1. Before treating
 *    an s-register rotation as an allocation problem, diff each pseudo's
 *    "crosses N calls" between the .flow and .lreg dumps of
 *    tools/agent/compilerTrace.ts. A pseudo that gains a call had an insn
 *    floated across one. An empty do { } while (0); directly after the call
 *    holds such a write in plain C.
 *
 * 4. Priority inequalities. Every callee-saved register is in use. From the
 *    target's assignment, derive the global-alloc order it implies
 *    (floor_log2(refs) * refs / live). Compare it with the candidate's table
 *    from the diagnostic cc1 before sweeping spellings; see the
 *    ovl_23_func_800B954C case study.
 *
 * 5. HIGH reuse against rematerialisation is most of the 2026-10-22 residual
 *    map:
 *    - the prologue has two separate lui of D_8007AD4C;
 *    - the weight pass rebuilds D_8007AD50 (0x800222E8) although s6 already
 *      holds it (0x80022274).
 *    See notes/research/harness-improvement-case-studies/ovl_11_func_80110944-dead-bindings-and-compiler-history.md
 *
 * 6. A residual key of [0,0,0,0] that is not EXACT can hide a swap of two
 *    instructions of the same shape. Read the byte diff.
 *
 * Method write-up:
 * notes/research/harness-improvement-case-studies/ovl_11_func_800F7230-spill-slot-provenance-and-post-schedule-allocation.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

u32 Rand(s32 arg0);
s32 func_80022014(s32 arg0, s16 arg1);

void func_8002206C(void) {
    s16 *walk;
    s16 *pVal;
    s16 *pPos1;
    s16 *pPos2;
    s16 *pColor1;
    s16 *pColor2;
    s16 *pW;
    u16 *src;
    u16 *sbase;
    u32 rows;
    u32 rowByte;
    u32 n;
    s16 v;
    s32 saved;
    s32 max;

    saved = D_8007AD4C;
    memset((void *)&D_8007AD4C, 0, 0x108);

    /* Mark every slot empty. */
    for (rows = 0; rows < 3U; rows++) {
        for (n = 0; n < 6U; n++) {
            *(s16 *)((char *)&D_8007AD4E + rows * 0x54 + n * 14) = -1;
        }
    }

    /* Deal cards: unique value, colors, and position range. */
    rows = 0;
    n = 0;
    do {
        u32 rb = rows * 0x54;
        sbase = D_80055944;
        src = sbase + 2;
        pPos2 = (s16 *)((char *)&D_8007AD52 + rb);
        pPos1 = (s16 *)((char *)&D_8007AD50 + rb);
        pColor2 = (s16 *)((char *)&D_8007AD56 + rb);
        pColor1 = (s16 *)((char *)&D_8007AD54 + rb);
        pVal = (s16 *)((char *)&D_8007AD4E + rb);
    loop_card:
        v = (s16)Rand(0x28);
        if (func_80022014((s32)&D_8007AD4C, v) != 0) {
            goto loop_card;
        }
        *pVal = v;
        pVal += 7;
        n++;
        *pColor1 = *(u16 *)((char *)&D_800558A4 + (v * 4));
        *pColor2 = *(u16 *)((char *)&D_800558A4 + ((v * 4) | 2));
        pColor2 += 7;
        pColor1 += 7;
        *pPos1 = (s16)(src[-2] + Rand((u16)(src[-1] - src[-2])));
        *pPos2 = (s16)(src[0] + Rand((u16)(src[1] - src[0])));
        src += 4;
        pPos1 += 7;
        pPos2 += 7;
        if (n < 6U) {
            goto loop_card;
        }
        n = 0;
        rows++;
    } while (rows < 3U);

    /* Set a specific card for the special game mode. */
    if (saved == 3) {
        char *p = (char *)&D_8007AD4C;
        char *q = (char *)&D_8006C838;
        u16 r0 = *(u16 *)(q + 0x9348);
        u16 r1 = *(u16 *)(q + 0x92B2);
        *(s16 *)(p + 0xC6) = -1;
        *(s16 *)(p + 0xCC) = 0;
        *(s16 *)(p + 0xCE) = 0;
        *(s16 *)(p + 0xC8) = (s16)r0;
        *(s16 *)(p + 0xCA) = (s16)r1;
    }

    /* Per-row weights from the row's largest pos1. */
    rows = 0;
    max = 0;
    do {
        n = 0;
        walk = (s16 *)((char *)&D_8007AD50 + rows * 0x54);
        do {
            if ((u32)max < (u32)*walk) {
                max = *walk;
            }
            n++;
            walk += 7;
        } while (n < 6U);
        n = 0;
        pW = (s16 *)((char *)&D_8007AD58 + rows * 0x54);
        pPos1 = (s16 *)((char *)&D_8007AD50 + rows * 0x54);
        do {
            s32 dd = max - *pPos1;
            u32 sq = (u32)(dd * dd);
            *pW = (s16)(Rand(4) + sq / 1000 + 1);
            pPos1 += 7;
            n++;
            pW += 7;
        } while (n < 6U);
        rows++;
        max = 0;
    } while (rows < 3U);
}
#endif
