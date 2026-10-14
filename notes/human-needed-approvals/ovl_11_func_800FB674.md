# ovl_11_func_800FB674 — human decision needed

- **Parked:** 2026-10-14T01:06:42.160Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_800FB674.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 1, 4] at 157/164 words, measured 2026-10-14T01:06:31, first scored from build/preparation/ovl_11_func_800FB674/f9d27bf8f9ad58b8e99e53af07478ff10a171ff85633c0ec1dd53624bb35118c/draft.c) rather than the source left on disk, which measured [0, 0, 3, 11] at 148/153 words, measured 2026-10-14T00:28:34

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800FB674 verdict MISMATCH — 157/165 words (95.2%).
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 1, allocation 4.
Next block: 9 (0x800FB7E8) — population 0, schedule 1, allocation 4. smallest open residual

This block's residual shape `move <reg>,<reg>@-3` is not new.
It was closed in ovl_11_func_800CEEF8, which carried it at [0, 4, 1, 1] (44/50 words). The edit that closed it:

  ...
  void *memset ();
  s32 ovl_11_func_800D589C (s16 arg0);
- s32 ovl_11_func_800E6EAC (s16 arg0, s16 arg1);
+ s32 ovl_11_func_800E6EAC (s16 arg0, s16 arg1, s16 arg2, s16 arg3);
  s32 ovl_11_func_80109188 (M2C_708e65b63435_Ovl11D04D4Obj *arg0, s32 arg1);
  s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
  ...
      temp_v1 = *(s32 *) ((u8 *) arg0 + 0x34);
      if (temp_v1 & 0x100000) {
-         if (ovl_11_func_800E6EAC(1, 0) == 1) {
+         if (ovl_11_func_800E6EAC(1, 0, 0, 0) == 1) {
              ovl_11_func_80109188((M2C_708e65b63435_Ovl11D04D4Obj *) &D_800759E4, 0x12);
              (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0xFFEFFFFF);

That is a proven answer for this shape in this codebase. Try it before modelling the compiler.
Also closed in ovl_11_func_800D9064, ovl_11_func_800E351C, ovl_11_func_800E516C, ovl_11_func_800F13D8, ovl_11_func_800F6C78, ovl_11_func_800F9BE4, ovl_11_func_80104148, ovl_11_func_801084E0, ovl_11_func_80112D10, ovl_11_func_8011D934, ovl_21_func_800BA5D0.
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80024A10 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u16 *func_8001AAB8 (s32 arg0, u16 *arg1);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80017D9C (s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void func_800136D4 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);

void ovl_11_func_800FB674(void) {
    s16 x, y;
    s32 repeat;
    s16 *temp_v0;
    s16 *display, *final_display, *glyph_display;
    s16 row, final_row, quotient, final_quotient;
    u16 terminator;
    s32 var_a0_2;
    s32 digit, minute;
    s32 call_object, final_object;
    s32 temp_s5;
    s32 temp_s7;
    s32 temp_t0;
    u32 temp_s1;
    u32 var_a0;

    repeat = 0;
        temp_t0 = D_8005E3C0->field_D8;
        temp_s5 = temp_t0 + 0x8C;
        temp_s7 = (((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C0) < 0xC;
        func_80022580((u32 *)(temp_t0 + 0x90), 1, 0xE4, 0x10, 0x4E, 0x20);
        y = 0x13;
        x = 0x114;
        func_80024A10(temp_s5, 0xE8, 0x13, ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BA);
        *(u16 *)func_8001A970((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BC) + 1, (s16 *)&D_8012A008, 2) = 0xFFFF;
        func_80017B3C(temp_s5, (s32) &D_8012A008, 0xFC, 0x13);
        *func_8001AAB8((s32) (((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE), (u16 *) &D_8012A008) = 0xFFFF;
        temp_s1 = func_80017A64();
    do {
        if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE) != 0) {
            var_a0 = 5;
            if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE) != 6) {
                var_a0 = temp_s1;
            }
        } else {
            var_a0 = 4;
        }
        func_80017A48(var_a0);
        func_80017B3C(temp_s5, (s32) &D_8012A008, x, y);
        func_80017A48(temp_s1);
        x = 0xE8;
        y = 0x21;
        temp_v0 = (s16 *)&D_8012A008;
        if (temp_s7 == 0) {
            *temp_v0 = 0xF;
        } else {
            *temp_v0 = 0;
        }
        temp_v0++;
        var_a0_2 = ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C0;
        *temp_v0++ = 0xC;
        *temp_v0++ = 0xFFD;
        if (temp_s7 == 0) {
            var_a0_2 -= 0xC;
        }
        do {
        temp_v0 = func_8001A970((s32) var_a0_2, temp_v0, 2);
        *temp_v0++ = 0xFFD;
        quotient = (s16) ((s16)(((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C2) / 10);
        temp_v0 = func_8001A970(0, func_8001A970((s32)quotient, temp_v0, 1), 1);
        do {
        display = (s16 *)&D_8012A008;
        final_display = display;
        glyph_display = final_display;
        row = y;
        y = 0;
        *(u16 *)temp_v0 = 0xFFFF;
        func_80017B3C(temp_s5, (s32)display, x, row);
        display = 0;
        final_display = 0;
        D_8012A008.field_0 = 0x66;
        ((u16 *)glyph_display)[1] = 0xFFFF;
        digit = (s16)((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C2;
        quotient = (s16)(digit / 10);
        digit = (digit - quotient * 10) / 5;
        func_80017D9C((s32 *) temp_s5, (u16 *)glyph_display, 0x110, row, digit);
        } while (repeat);
        x = 0;
        y = 0;
        } while (repeat);
    } while (repeat);
}
```
