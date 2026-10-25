#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "packet_work.h"

/* Callee prototypes (project convention: local declarations). */
u16 *func_800191B4(u16 *, u16, s32, s32);
s32 func_8001929C(u16 *, u16, s16, u16 *);
s32 func_800193F0(u16 *, s32);
s32 func_8001945C(u16 *, u16, s16);
void func_80019564(s32, s32, s32, s32);
s32 func_80019CBC(s16, s16, u16 *, s16);
s32 func_80019E50(s32);
void func_80019E80(u_long *, s16, s16, s16);
s32 func_80012CB4(s32, s32 *, s32);
void func_80017A70(u32);
s32 func_80017AA0(void);
void func_80017AE8(void);
u16 func_800195F4(void);
void func_80019600(void);
void func_8001A018(s32, s32);
s32 func_8001A284(s32);
void func_8001A11C(void);
void func_8001A19C(void);
void func_80017ACC(void);
void func_80022008(void);

/* TU-owned globals (GP-relative — tentative definitions). */
s32 D_8005E2BC;
u16 D_8005E44E;
s16 D_8005E454;
s32 D_8005E458;
s32 D_8005E45C;
s32 D_8005E464;
s32 D_8005E46C;
s16 D_8005E470;
s32 D_8005E474;
u16 D_8005E482;
u16 D_8005E484;
s32 D_8005E488;
s32 D_8005E48C;
s16 D_8005E4A0;
s16 D_8005E4A2;
u16 D_8005E4A4;
u16 *D_8005E4A8;    /* extern decl in globals_override.h; tentative def here */
u16 D_8005E444;     /* extern decl in globals_override.h; tentative def here */
s32 D_8005E4AC;
u16 *D_8005E4B0;
s32 D_8005E4B4;
s32 D_8005E4B8;

/* Steps and draws the text window over the u16 script table arg1: a new
 * table is scanned once (0x4000 entries go to func_8001A284, D_8005E444
 * counts entries up to the 0xFFFF terminator) and the window state is reset.
 * Each call then reacts to the func_800195F4 state, pages through the
 * table (0xFFFE and 0xFFC markers), sets the two draw areas
 * around the text at (arg2, arg3) and places the cursor at
 * (arg2 - 4, arg3 - 0x54). Returns the (u16) status, or an early result for
 * states 2 and 7-9.
 *
 * Built with -fno-gcse (configs/flag_overrides.mk has the evidence). The
 * constructs below are there for the compiler, not for behaviour:
 *
 * - A ternary argument is written inside its call. Arguments are expanded
 *   left to right, so the earlier ones are loaded before the branch, as in
 *   the target (func_8001929C, func_8001945C and func_80019E80 calls).
 * - The scan loop computes next = i + 1 before its if and tests next < 4,
 *   which is the target's addiu a0,s1,1 / move s1,a0 counter.
 * - entry (the first walk), list (case 3), area1/area2 (the draw areas) and
 *   step are separate variables. Each keeps its pseudo short, which gives
 *   global-alloc the target's order: cursorY > cursorX > result > arg2 >
 *   arg3 (s4, s5, s6, s7, fp).
 * - status + 0xFFFBU is unsigned so the conversion to s16 is not narrowed
 *   into an addiu of -5; the target adds 0xFFFB in 32 bits.
 * - In the arms that make calls, the calls come before the result and
 *   cursor assignments.
 * - *D_8005E4A8 == 0xFFFE re-reads through the pointer just stored, which
 *   keeps the store ahead of the load.
 * - (s16) on field_2 puts arg3 first in the rect.y addition. */
s32 func_800183E0(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4,
                  s16 arg5) {
    RECT rect;
    s32 height;
    s32 textY;
    s32 savedState;
    s32 cursorX;
    s32 result;
    s32 cursorY;
    s32 step;
    s32 input4;
    s32 input8;
    s32 status;
    s32 value;
    u16 *found;
    u16 *marker;
    u16 *entry;
    u16 *list;
    DR_AREA *area1;
    DR_AREA *area2;

    height = (s32)arg5;
    if (D_8005E4B0 != arg1) {
        D_8005E4B0 = arg1;
        D_8005E4A8 = arg1;
        D_8005E444 = 0;
        entry = arg1;
        if (*entry != 0xFFFF) {
            do {
                if (*entry & 0x4000) {
                    func_8001A284(*entry);
                }
                entry++;
                D_8005E444 = (u16)(D_8005E444 + 1);
            } while (*entry != 0xFFFF);
        }
        D_8005E4AC = 0;
        D_8005E4A2 = 0;
        D_8005E4A0 = 3;
        D_8005E4B4 = 1;
        func_80019600();
        D_8005E4B8 = 0;
        D_8005E458 = 0;
        D_8005E46C = 0;
        D_8005E454 = -1;
        D_8005E464 = 0;
        D_8005E45C = 0;
        D_8005E470 = 0;
        D_8005E474 = 0;
        D_8005E488 = 0;
        D_8005E48C = 0;
    }
    step = 1;
    status = 0;
    savedState = func_80017AA0();
    if (D_8005E2BC != 0) {
        input4 = ((GfxObj *)D_8005E3A8)->field_4;
        input8 = ((GfxObj *)D_8005E3A8)->field_8;
    } else {
        input4 = 0;
        input8 = 0;
    }
    value = func_800195F4();
    switch (value) {
    case 0:
        status = 0;
        if (input4 & 0x60) {
            func_80017A70(0U);
        }
        goto mainbody;
    case 1:
        if (D_8005E4B4 == 0) {
            status = 1;
            if ((input8 & 0x60) && (D_8005E4A0 >= 4)) {
                D_8005E4B4 = 1;
                D_8005E4A0 = 0;
                status = 0;
            }
        } else {
            status = 6;
            if (input4 & 0x60) {
                step = 2;
            }
        }
        goto mainbody;
    case 2:
        func_80022008();
        if (input8 & 0x60) {
            status = 5;
            if (D_8005E44E == 2) {
                D_8005E4B0 = 0;
                return 2;
            }
            goto mainbody;
        }
        status = 5;
        goto mainbody;
    case 3:
        status = 3;
        if (input8 & 0x60) {
            status = 0;
            func_80017AE8();
            list = D_8005E4A8;
            if (list[D_8005E444 - 1] == 0xFFFE) {
                if (D_8005E4B8 == 0) {
                    D_8005E4A0 = (u16)D_8005E4A0 -
                        func_8001929C(list, 0xFFFE,
                            D_8005E444 != 0 ? (s16)D_8005E444 : 1, 0);
                } else {
                    D_8005E4A0 = -1;
                }
            }
        }
        goto mainbody;
    case 7:
    case 8:
    case 9:
        status = D_8005E44E;
        if (func_80019CBC(arg2, arg3, &D_8005E4A4,
                          (s16)(status + 0xFFFBU)) != 0) {
            D_8005E4B0 = 0;
            return func_80019E50((D_8005E4A4 + 1) & 0xFFFF) & 0xFFFF;
        }
        goto mainbody;
    default:
    mainbody:
        D_8005E44E = (u16)value;
        func_8001A11C();
        func_8001A19C();
        textY = arg3;
        if (D_8005E474 == 0) {
            if (D_8005E4B4 == 1) {
                if (func_8001929C(D_8005E4A8, 0xFFFE,
                        D_8005E444 != 0 ? (s16)D_8005E444 : 1, 0) >= 4U) {
                    D_8005E4B8 = 1;
                    if (D_8005E4AC == 0) {
                        func_80017ACC();
                        D_8005E4A0 = (u16)D_8005E4A0 + 1;
                    }
                    if (func_80012CB4(1, &D_8005E4AC, 1) == 1) {
                        D_8005E4A2 = (u16)D_8005E4A2 + step;
                    }
                    if ((s16)D_8005E4A2 >= 0xE) {
                        func_80017AE8();
                        D_8005E4A8 = (u16 *)func_800193F0(D_8005E4A8, 1);
                        result = (u16)status;
                        cursorX = arg2 - 4;
                        cursorY = arg3 - 0x54;
                        D_8005E4AC = 0;
                        D_8005E4A2 = 0;
                        goto tailmerge;
                    } else {
                        textY = (s32)(s16)(arg3 - (u16)D_8005E4A2);
                        goto tail_189E4;
                    }
                } else {
                    D_8005E4A2 = 0;
                    goto tail_189E4;
                }
            } else {
                D_8005E4A2 = 0;
                goto tail_189E4;
            }
        } else {
            result = (u16)status;
            cursorX = arg2 - 4;
            cursorY = textY - 0x54;
            if (D_8005E4B4 == 1) {
                D_8005E4B8 = D_8005E4B4;
                D_8005E4A0 = 4;
                marker = func_800191B4(D_8005E4A8, 0xFFC, 1, 0);
                if (D_8005E4AC != 0) {
                    found = func_800191B4(D_8005E4A8, 0xFFFE, 4, 0);
                    if (found != NULL) {
                        if (found[1] != 0xFFFF) {
                            if ((marker != NULL) && ((u32)marker < (u32)found)) {
                                found = func_800191B4(marker, 0xFFFE, 1, 0);
                            }
                            D_8005E4A8 = found + 1;
                        }
                    } else if (marker != NULL) {
                        D_8005E4A8 = marker + 1;
                        if (*D_8005E4A8 == 0xFFFE) {
                            D_8005E4A8++;
                        }
                    }
                }
                D_8005E4AC = 1;
                marker = func_800191B4(D_8005E4A8, 0xFFC, 1, 0);
                found = func_800191B4(D_8005E4A8, 0xFFFE, 4, 0);
                if (found != NULL) {
                    if ((marker != NULL) && ((u32)marker < (u32)found)) {
                        D_8005E444 =
                            (u16)func_8001945C(D_8005E4A8, 0xFFC, 0);
                        result = (u16)status;
                        cursorX = arg2 - 4;
                        cursorY = arg3 - 0x54;
                        goto tailmerge;
                    } else {
                        {
                            u16 *cur;
                            s32 i;
                            s32 next;

                            i = 0;
                            value = 0;
                            cur = D_8005E4A8;
                            result = (u16)status;
                            cursorX = arg2 - 4;
                            cursorY = arg3 - 0x54;
                            do {
                                value = value + func_8001945C(cur, 0xFFFE, 0);
                                next = i + 1;
                                if (i < 3 && *cur != 0xFFFE) {
                                    while (*++cur != 0xFFFE) {
                                    }
                                }
                                cur++;
                                i = next;
                            } while (next < 4);
                            D_8005E444 = value;
                        }
                        goto tailmerge;
                    }
                } else {
                    found = func_800191B4(D_8005E4A8, 0xFFFF, 1, 0);
                    D_8005E444 = (u16)func_8001945C(D_8005E4A8,
                        (marker != NULL && (u32)marker < (u32)found) ? 0xFFC : 0xFFFF, 0);
                    goto tail_189E4;
                }
            }
            goto tailmerge;
        }
    tail_189E4:
        result = (u16)status;
        cursorX = arg2 - 4;
        cursorY = arg3 - 0x54;
    tailmerge:
        if ((D_8005E488 == 0) && ((s16)D_8005E4A0 >= 4)) {
            D_8005E4B4 = 0;
            func_80017ACC();
        }
        area1 = (DR_AREA *)ReservePacketWorkBytes(0xC);
        area2 = (DR_AREA *)ReservePacketWorkBytes(0xC);
        rect.x = arg2;
        rect.y = arg3 + (s16)gActiveRenderContext->field_2;
        rect.w = 0x140 - arg2;
        rect.h = (s16)height;
        SetDrawArea(area2, &rect);
        SetDrawArea(area1, (RECT *)gActiveRenderContext);
        addPrim(arg0, area1);
        func_80019564((s32)arg0, (s32)D_8005E4A8, arg2, textY);
        addPrim(arg0, area2);
        func_80017A70(savedState);
        func_8001A018(D_8005E454, D_8005E458);
        func_80019E80((u_long *)gActiveRenderContext->field_D8, (s16)cursorX, (s16)cursorY,
                      D_8005E464 != 0 ? D_8005E454 : -1);
        D_8005E484 = D_8005E482;
        return result;
    }
}
