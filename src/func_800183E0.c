#include "common.h"
#include "include_asm.h"

INCLUDE_ASM("build/asm/nonmatchings/func_800183E0", func_800183E0);

/* Suspended at the user's request, 2026-10-21; not matched.
 * Corrected C attempt preserved below. Best measurement [0,42,10,24]
 * (355/466 compared words) used experimental -fno-gcse; that override and
 * its temporary allowlist entry were removed when reparking.
 * Findings, flag scope and remaining defects: notes/research/func_800183E0.md
 */

#if 0
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Callee prototypes (project convention: local declarations). */
u16 *func_800191B4(u16 *, u16, s32, s32);
s32 func_8001929C(u16 *, u16, s16, u16 *);
s32 func_800193F0(u16 *, s32);
s32 func_8001945C(u16 *, u16, s16);
void func_80019564(s32, s32, s32, s32);
s32 func_80019CBC(s16, s16, u16 *, s16);
s32 func_80019E50(s32);
void func_80019E80(u_long *, s16, s16, s16);
s32 func_80011F5C(s32);
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

s32 func_800183E0(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4,
                  s16 arg5) {
    RECT rect;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 s5;
    s32 s6;
    s32 s4;
    s32 f4;
    s32 f8;
    s32 s3;
    s32 s2;
    s16 a1var;
    s16 a3var;
    u16 *ptr;
    u16 *buf1;
    u16 *buf3;

    sp18 = (s32)arg5;
    if (D_8005E4B0 != arg1) {
        ptr = arg1;
        D_8005E4B0 = arg1;
        D_8005E4A8 = arg1;
        D_8005E444 = 0;
        if (*ptr != 0xFFFF) {
            do {
                if (*ptr & 0x4000) {
                    func_8001A284(*ptr);
                }
                ptr++;
                D_8005E444 = (u16)(D_8005E444 + 1);
            } while (*ptr != 0xFFFF);
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
    s4 = 1;
    s3 = 0;
    sp20 = func_80017AA0();
    if (D_8005E2BC != 0) {
        f4 = ((GfxObj *)D_8005E3A8)->field_4;
        f8 = ((GfxObj *)D_8005E3A8)->field_8;
    } else {
        f4 = 0;
        f8 = 0;
    }
    s2 = func_800195F4();
    switch (s2) {
    case 0:
        s3 = 0;
        if (f4 & 0x60) {
            func_80017A70(0U);
        }
        goto mainbody;
    case 1:
        if (D_8005E4B4 == 0) {
            s3 = 1;
            if ((f8 & 0x60) && (D_8005E4A0 >= 4)) {
                D_8005E4B4 = 1;
                D_8005E4A0 = 0;
                s3 = 0;
            }
        } else {
            s3 = 6;
            if (f4 & 0x60) {
                s4 = 2;
            }
        }
        goto mainbody;
    case 2:
        func_80022008();
        if (f8 & 0x60) {
            s3 = 5;
            if (D_8005E44E == 2) {
                D_8005E4B0 = 0;
                return 2;
            }
            goto mainbody;
        }
        s3 = 5;
        goto mainbody;
    case 3:
        s3 = 3;
        if (f8 & 0x60) {
            s3 = 0;
            func_80017AE8();
            if (D_8005E4A8[D_8005E444 - 1] == 0xFFFE) {
                if (D_8005E4B8 == 0) {
                    if (D_8005E444 != 0) {
                        a1var = (s16)D_8005E444;
                    } else {
                        a1var = 1;
                    }
                    D_8005E4A0 = (u16)D_8005E4A0 -
                                 func_8001929C(D_8005E4A8, 0xFFFE, a1var, 0);
                } else {
                    D_8005E4A0 = -1;
                }
            }
        }
        goto mainbody;
    case 7:
    case 8:
    case 9:
        s3 = D_8005E44E;
        if (func_80019CBC(arg2, arg3, &D_8005E4A4,
                          (s16)(s3 + 0xFFFB)) != 0) {
            D_8005E4B0 = 0;
            return func_80019E50((D_8005E4A4 + 1) & 0xFFFF) & 0xFFFF;
        }
        goto mainbody;
    default:
    mainbody:
        D_8005E44E = (u16)s2;
        func_8001A11C();
        func_8001A19C();
        sp1C = arg3;
        if (D_8005E474 == 0) {
            if (D_8005E4B4 == 1) {
                if (D_8005E444 != 0) {
                    a1var = (s16)D_8005E444;
                } else {
                    a1var = 1;
                }
                if (func_8001929C(D_8005E4A8, 0xFFFE, a1var, 0) >= 4U) {
                    D_8005E4B8 = 1;
                    if (D_8005E4AC == 0) {
                        func_80017ACC();
                        D_8005E4A0 = (u16)D_8005E4A0 + 1;
                    }
                    if (func_80012CB4(1, &D_8005E4AC, 1) == 1) {
                        D_8005E4A2 = (u16)D_8005E4A2 + s4;
                    }
                    if ((s16)D_8005E4A2 >= 0xE) {
                        func_80017AE8();
                        s6 = (u16)s3;
                        s5 = arg2 - 4;
                        D_8005E4A8 = (u16 *)func_800193F0(D_8005E4A8, 1);
                        s4 = arg3 - 0x54;
                        D_8005E4AC = 0;
                        D_8005E4A2 = 0;
                        goto tailmerge;
                    } else {
                        sp1C = (s32)(s16)(arg3 - (u16)D_8005E4A2);
                        s6 = (u16)s3;
                        goto tail_189E4;
                    }
                } else {
                    D_8005E4A2 = 0;
                    s6 = (u16)s3;
                    goto tail_189E8;
                }
            } else {
                D_8005E4A2 = 0;
                s6 = (u16)s3;
                goto tail_189E8;
            }
        } else {
            s6 = (u16)s3;
            s5 = arg2 - 4;
            s4 = sp1C - 0x54;
            if (D_8005E4B4 == 1) {
                D_8005E4B8 = D_8005E4B4;
                D_8005E4A0 = 4;
                buf1 = func_800191B4(D_8005E4A8, 0xFFC, 1, 0);
                if (D_8005E4AC != 0) {
                    ptr = func_800191B4(D_8005E4A8, 0xFFFE, 4, 0);
                    if (ptr != NULL) {
                        if (ptr[1] != 0xFFFF) {
                            if ((buf1 != NULL) && ((u32)buf1 < (u32)ptr)) {
                                ptr = func_800191B4(buf1, 0xFFFE, 1, 0);
                            }
                            D_8005E4A8 = ptr + 1;
                        }
                    } else if (buf1 != NULL) {
                        D_8005E4A8 = buf1 + 1;
                        if (buf1[1] == 0xFFFE) {
                            D_8005E4A8 = buf1 + 2;
                        }
                    }
                }
                D_8005E4AC = 1;
                buf1 = func_800191B4(D_8005E4A8, 0xFFC, 1, 0);
                ptr = func_800191B4(D_8005E4A8, 0xFFFE, 4, 0);
                if (ptr != NULL) {
                    if ((buf1 != NULL) && ((u32)buf1 < (u32)ptr)) {
                        s6 = (u16)s3;
                        s5 = arg2 - 4;
                        s4 = arg3 - 0x54;
                        D_8005E444 =
                            (u16)func_8001945C(D_8005E4A8, 0xFFC, 0);
                        goto tailmerge;
                    } else {
                        {
                            u16 *cur;
                            s32 i;

                            i = 0;
                            s2 = 0;
                            cur = D_8005E4A8;
                            s6 = (u16)s3;
                            s5 = arg2 - 4;
                            s4 = arg3 - 0x54;
                            s3 = 0xFFFE;
                            do {
                                s2 = s2 + func_8001945C(cur, 0xFFFE, 0);
                                if (i < 3 && *cur != s3) {
                                    cur++;
                                    while (*cur++ != 0xFFFE) {
                                    }
                                    cur--;
                                }
                                i++;
                                cur++;
                            } while (i < 4);
                            D_8005E444 = s2;
                        }
                        goto tailmerge;
                    }
                } else {
                    ptr = func_800191B4(D_8005E4A8, 0xFFFF, 1, 0);
                    a1var = 0xFFFF;
                    if ((buf1 != NULL) && ((u32)buf1 < (u32)ptr)) {
                        a1var = 0xFFC;
                    }
                    D_8005E444 =
                        (u16)func_8001945C(D_8005E4A8, (u16)a1var, 0);
                    goto tail_189E4;
                }
            }
            goto tailmerge;
        }
    tail_189E4:
        s6 = (u16)s3;
    tail_189E8:
        s5 = arg2 - 4;
        s4 = arg3 - 0x54;
    tailmerge:
        if ((D_8005E488 == 0) && ((s16)D_8005E4A0 >= 4)) {
            D_8005E4B4 = 0;
            func_80017ACC();
        }
        buf1 = (u16 *)func_80011F5C(0xC);
        buf3 = (u16 *)func_80011F5C(0xC);
        rect.x = arg2;
        rect.y = arg3 + D_8005E3C0->field_2;
        rect.w = 0x140 - arg2;
        rect.h = (s16)sp18;
        SetDrawArea((DR_AREA *)buf3, &rect);
        SetDrawArea((DR_AREA *)buf1, (RECT *)D_8005E3C0);
        addPrim(arg0, buf1);
        func_80019564((s32)arg0, (s32)D_8005E4A8, arg2, sp1C);
        addPrim(arg0, buf3);
        func_80017A70(sp20);
        func_8001A018(D_8005E454, D_8005E458);
        a3var = -1;
        if (D_8005E464 != 0) {
            a3var = D_8005E454;
        }
        func_80019E80((u_long *)D_8005E3C0->field_D8, (s16)s5, (s16)s4,
                      a3var);
        D_8005E484 = D_8005E482;
        return s6;
    }
}
#endif
