#include "common.h"
#include "sound_commands.h"
#include "vblank_timer.h"

/* TU-owned GP globals (tentative definitions -> .comm, so cc1 emits the
 * target's %gp_rel accesses). Types match include/globals.h where present. */
u16 D_8005E2B8;
u16 D_8005E2BA;
u16 D_8005E444;
u16 D_8005E446;
s16 D_8005E44C;
s16 D_8005E478;
s16 D_8005E47A;
s16 D_8005E47E;
u16 D_8005E482;
u32 D_8005E448;
u32 D_8005E450;
s16 D_8005E454;
s32 D_8005E458;
s32 D_8005E464;
s16 D_8005E470;
s32 D_8005E474;
u16 D_8005E47C;
u16 D_8005E480;
s32 D_8005E488;
s32 D_8005E48C;
u16 D_8005E498;

/* Game callees. */
s16 GetPairedTpage(s32 tpage);
void func_80019FC4(s16 arg0);
void *func_80019070(s32 *ordering_table, u8 *packet, u32 glyph, s32 x,
                    s16 y, u8 red, u8 green, u8 blue, u32 palette,
                    s32 semitransparent);
s32 func_8001A284(s32 arg0);

u8 *func_80018B98(s32 *arg0, u8 *arg1, u16 *arg2, s16 arg3, s16 arg4,
                  s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    s32 terminator;
    s32 xProduct;
    s32 work0;
    s32 work1;
    s32 work2;
    s32 work3;
    s32 work4;
    s32 work5;
    s32 x;
    u16 remasked;
    u32 flag;
    u32 flagtmp;
    u16 glyph;

    if (arg8 == 0) {
        D_8005E47C = (u16)arg3;
        if (arg6 == 1) {
            D_8005E47E = 0;
            if (D_8005E446 == 0) {
                if (HasVSyncIntervalElapsed(D_8005E44C, &D_8005E448, 1) == arg6) {
                    if (D_8005E48C != 0) {
                        PlaySoundEffect(4);
                        D_8005E48C = 0;
                    } else {
                        D_8005E48C = arg6;
                    }
                    D_8005E444 = (u16)(D_8005E444 + 1);
                } else if (arg2[D_8005E444] >= 0xE01U ||
                           (arg2[0] & 0x4000) != 0) {
                    D_8005E444 = (u16)(D_8005E444 + 1);
                }
            }
        }
    }

    flagtmp = (u32)D_8005E446 - 7;
    flag = flagtmp < 3U;

    D_8005E478 = arg3;
    D_8005E47A = (u16)arg4;

    if (arg6 != 1) {
        goto inner;
    } else {
        goto outer_test;
    }

outer_body:
    work5++;
    D_8005E47E = (u16)work5;
    if (D_8005E446 == 1) {
        /* advance */
    } else if ((s16)D_8005E480 == (s16)work5) {
        /* advance */
    } else if (flag != 0) {
        /* advance */
    } else {
        D_8005E446 = 0;
    }

    do {
inner:
        work0 = *arg2;
        terminator = 0xFFFF;
        work2 = work0 & 0xFFFF;
        work1 = work0;

        work0 = terminator;
        if (work2 == work0) {
            if (arg6 != 0 && arg8 == 0 && flag == 0) {
                D_8005E446 = 2;
            }
            goto done;
        }

        if (work2 == 0xFFFE) {
            D_8005E47A = D_8005E2BA + ((u16)D_8005E47A + 0xC);
            D_8005E478 = (u16)D_8005E47C;
            if (arg9 != 0) {
                D_8005E478 = D_8005E498;
                arg2++;
                goto loop_latch;
            }
            goto advance;
        }

        if (work2 == 0xFFC) {
            if (D_8005E446 == 1) {
                goto advance;
            }
            D_8005E446 = 3;
            D_8005E480 = (u16)D_8005E47E;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFFB) {
            goto advance;
        }

        if ((u32)((work1 - 0xFF8) & 0xFFFF) < 3U) {
            goto advance;
        }

        if ((u32)((work1 - 0xE38) & 0xFFFF) < 0x1B8U) {
            glyph = *(volatile u16 *)arg2;
            arg2++;
            D_8005E454 = GetPairedTpage(glyph);
            goto loop_latch;
        }

        if (work2 == 0xFF7) {
            func_80019FC4(D_8005E454);
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF6) {
            D_8005E464 = 0;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF5) {
            D_8005E458 = 1;
            arg2++;
            goto loop_latch;
        }

        if (work2 == 0xFF4) {
            D_8005E464 = 1;
            arg2++;
            goto loop_latch;
        }

        if ((u32)((work1 - 0xE25) & 0xFFFF) < 0xBU) {
            D_8005E470 = (s16)(0xE2F - work1);
            arg2++;
            goto loop_latch;
        }

        remasked = work1 & 0xFFFF;
        if (work2 == 0xFF3) {
            D_8005E474 = 1;
            arg2++;
            goto loop_latch;
        }

        if (remasked == 0xFF2) {
            goto advance;
        }

        if (remasked == 0xFF1) {
            D_8005E488 = 1;
            arg2++;
            goto loop_latch;
        }

        if (work1 & 0x4000) {
            work4 = func_8001A284(*arg2);
            if (work4 == 0) {
                work4 = D_8005F0C8[*arg2 & 0xFFF];
                if (work4 == 0) {
                    goto advance;
                }
            }
            arg1 = func_80018B98(
                arg0, arg1, (u16 *)work4, D_8005E478, (s16)D_8005E47A,
                arg5, arg6, arg7, 1, 0);
            goto advance;
        }

        if ((work1 & 0xFFF) != 0xFFD) {
            xProduct = (D_8005E478 - arg3) * arg5;
            if (xProduct < 0) {
                xProduct += 0xFFF;
            }
            x = (s32)(s16)(arg3 + (xProduct >> 12));
            work3 = (D_8005E47A - arg4) * arg5;
            if (work3 < 0) {
                work3 += 0xFFF;
            }
            arg1 = (u8 *)func_80019070(arg0, arg1, *arg2, x,
                                         (s16)(s32)(s16)(arg4 + (work3 >> 12)),
                                         0x80, 0x80, 0x80, D_8005E450, arg7);
        }
        D_8005E478 = D_8005E2B8 + ((u16)D_8005E478 + 8);

advance:
        arg2++;
loop_latch:
        ;
    } while (arg6 != 1);
outer_test:
    work5 = (u16)D_8005E47E;
    if ((u16)D_8005E444 > (s16)D_8005E47E) {
        goto outer_body;
    }
done:
    D_8005E482 = arg2[-1];
    work1 = (s32)arg1;
    return (u8 *)work1;
}




