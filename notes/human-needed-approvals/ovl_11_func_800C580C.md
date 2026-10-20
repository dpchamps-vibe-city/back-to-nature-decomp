# ovl_11_func_800C580C — human decision needed

- **Parked:** 2026-10-20T01:18:10.989Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_800C580C.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 903, 15, 105] at 772/896 words, measured 2026-10-20T00:15:04, first scored from build/preparation/ovl_11_func_800C580C/d3ee42337a20184924166166d17b33bd5c72f02cef8563453d24e5a2f2718e22/draft.c) rather than the source left on disk, which measured [5, 1024, 5, 89] at 818/934 words, measured 2026-10-20T01:17:24

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800C580C verdict MISMATCH — 776/985 words (78.8%).
Instruction count delta versus target: -28.
Residual (steer by this, not the word count): control-flow 0, population 912, schedule 15, allocation 106.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 10 (0x800C5944) — population 1, schedule 1, allocation 0. the instruction populations differ here; nothing below can be read until they agree
Fixing it should also close block 17, 142.

This block's residual shape `jal <tgt>@-1` is not new.
It was closed in ovl_11_func_800D85F8, which carried it at [1, 106, 1, 8] (65/87 words). The edit that closed it:

  ...
  #include "game_types.h"
  
- typedef struct {
-     union {
-         u16 unk0;
-         s16 unk0_s;
-     } u0;
-     u16 unk2;
-     u8 unk4;
-     s8 unk5;
-     u16 unk6;
- } Ovl11D85F8Arg;
+ /* Item-state update for one action record: when either flag guard in the
+  * ovl_11_func_800D5868/800D589C pair is set, revalidate the record's byte at
+  * +4 against the ovl_11_func_800D5C3C lookup for the s16 index at +0 and
+  * either reset the record (copy the +2 next id back to +0) or refresh the
+  * ovl_11_func_800D5C90 result into +4 and set bit 15 of +6. The returned
+  * halfword is the resulting index, remapped by the D_80070CF2 mode. */
+ s32 ovl_11_func_800D589C(s16 arg0);
+ s32 ovl_11_func_800D5868(s16 arg0);
+ s32 ovl_11_func_800D5C3C(s16 arg0);
+ s32 ovl_11_func_800D5CE4(s16 arg0);
+ s32 ovl_11_func_800D5C90(s16 arg0);
  
- u16 ovl_11_func_800D85F8(Ovl11D85F8Arg *arg0) {
+ u16 ovl_11_func_800D85F8(Recon_ovl_11_func_800D85F8_A0View *arg0) {
      s16 temp_s0;
      s16 idx;
+     u16 temp_s3;
      u16 ret;
      s16 temp_v0;
  ...
  
      temp_s0 = arg0->u0.unk0_s;
+     temp_s3 = arg0->u0.unk0;
      if (ovl_11_func_800D589C(temp_s0) == 0 && ovl_11_func_800D5868(temp_s0) == 0) {
          return 0;
      }
  
-     idx = (s16)arg0->u0.unk0;
-     ret = arg0->u0.unk0;
+     idx = (s16)temp_s3;
      if (ovl_11_func_800D5868(idx) == 1) {
          temp_s0_2 = arg0->unk4;
-         if (temp_s0_2 == ovl_11_func_800D5C3C(idx)) {
-             if (arg0->u0.unk0 == 0x36) {
-                 return 0;
-             }
-             ret = ovl_11_func_800D5CE4(idx) & 0xFFFF;
-             temp_v0 = ovl_11_func_800D5C90(idx);
-             if (temp_v0 == -1) {
-                 arg0->unk4 = 0;
-                 arg0->unk5 = 0;
-                 arg0->unk6 = 0;
-                 arg0->u0.unk0 = arg0->unk2;
-             } else {
-                 arg0->unk4 = temp_v0;
-                 arg0->unk6 |= 0x8000;
-             }
+         if (temp_s0_2 != ovl_11_func_800D5C3C(idx)) {
+             return 0;
+         }
+         if (arg0->u0.unk0 == 0x36) {
+             return 0;
+         }
+         ret = ovl_11_func_800D5CE4(idx);
+         temp_v0 = ovl_11_func_800D5C90(idx);
+         if (temp_v0 == -1) {
+             arg0->unk4 = 0;
              arg0->unk5 = 0;
+             arg0->unk6 = 0;
+             arg0->u0.unk0 = arg0->unk2;
          } else {
-             return 0;
+             arg0->unk4 = temp_v0;
+             arg0->unk6 |= 0x8000;
          }
+         arg0->unk5 = 0;
      } else {
+         ret = temp_s3;
          arg0->unk4 = 0;
          arg0->unk5 = 0;

That is a proven answer for this shape in this codebase. Try it before modelling the compiler.
Also closed in ovl_11_func_800E6BDC, ovl_11_func_800FD33C, ovl_11_func_80108CD0, ovl_11_func_8010B67C, ovl_11_func_801180F0, ovl_11_func_80120414, ovl_11_func_801213D8, ovl_25_func_800B9480, ovl_30_func_8012F454, ovl_31_func_800B7FCC.
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 35 distinct measurements since the residual last improved on [0, 903, 15, 105]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 35 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"

typedef struct {
               u8 unk0[4];
               s16 unk4;
} M2C_7f0a9cc905ed_UnkStruct800D72E8;

typedef struct {
               u16 unk0;
               u16 unk2;
} M2C_b6c4596908ca_Ovl11CF314Arg1;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_b6c4596908ca_Ovl11CF314Arg2;

typedef struct {
               char pad0[0x38];
               u16 unk38;
               char pad3A[0x100 - 0x3A];
                s32 unk100;
                s32 unk104;
                s32 unk108;
} M2C_ad72053e5aa9_StructOvl11CE034A;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_ad72053e5aa9_StructOvl11CE034B;

typedef struct {
    u8 b[4];
} M2C_1d1631a2972a_Ov11_F43CCArgBlk;

typedef struct {
               s16 unk0;
               s16 unk2;
               s16 unk4;
               s16 unk6;
               s16 unk8;
               char pad0A[0x0C - 0x0A];
               s32 unkC;
               s32 unk10;
               s32 unk14;
               s32 unk18;
               s32 unk1C;
               char pad20[0x24 - 0x20];
               s32 unk24;
               s32 unk28;
               s32 unk2C;
} M2C_f3fe4d9631b5_UnkStruct800CBF40;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_f3fe4d9631b5_UnkStruct800CBF70Arg0;

typedef struct {
               char pad0[0x36];
               u16 flags;
               char pad38[0x40 - 0x38];
               u16 vals[(0x84 - 0x40) / 2];
               s16 value;
} M2C_583b0b68ff5a_UnkStruct800CD5BC;

typedef struct {
               s16 field_0;
               s16 field_2;
} M2C_42071334137b_Ov11Timer;

typedef struct {
              s32 unk0;
              s32 unk4;
              s32 unk8;
} M2C_cec44150eede_Ovl11Func800BDCE8Arg1;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} M2C_38c89582ddcb_M2C_4c1a205b3b09_ReconA0View;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} M2C_4c1a205b3b09_ReconA0View;

typedef struct {
    char pad_0[0x38];
    u16 unk38;
    char pad_3A[0x2];
    u16 unk3C;
    char pad_3E[0x2E];
    s32 unk6C;
    char pad_70[0xA0];
    s32 unk110;
    char pad_114[0x4];
    s32 unk118;
} M2C_1052de249052_Ovl119938View;

typedef struct {
               s16 unk0;
               s16 unk2;
               s16 unk4;
               s16 unk6;
               s16 unk8;
               char pad0A[0x0C - 0x0A];
               s32 unkC;
               s32 unk10;
               s32 unk14;
               s32 unk18;
               s32 unk1C;
               char pad20[0x24 - 0x20];
               s32 unk24;
               s32 unk28;
               s32 unk2C;
} M2C_1729124c6ef2_UnkStruct800CBF40;

typedef struct {
               u8 unk0[4];
               s16 unk4;
} M2C_029d3dcb50b2_UnkStruct800D72E8;

typedef struct {
               char pad_00[0x26];
               s16 unk26;
               s16 unk28;
               s16 unk2A;
               s16 unk2C;
               char pad_2E[0x34 - 0x2E];
               s32 unk34;
               char pad_38[0xB6 - 0x38];
               s16 unkB6;
} M2C_8d3156bb36aa_Struct_800DF010;

typedef struct {
    char pad_00[0x24];
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    char pad_2E[0x06];
    s32 unk34;
} M2C_708e65b63435_Ovl11D04D4Obj;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} M2C_93d07a715929_Ovl11SelectArg;

void *memset ();
s32 ovl_11_func_800CD08C (Recon_ovl_11_func_800CD08C_A0View *arg0);
s32 func_8001FABC (s16 arg0);
s32 ovl_11_func_800CD0F4 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1);
s32 ovl_11_func_800D58D0 (void *arg0);
void ovl_11_func_800CCF14 (M2C_7f0a9cc905ed_UnkStruct800D72E8 *arg0);
s32 ovl_11_func_800CB4E0 (void);
void ovl_11_func_800CF308 (s16 arg0);
s32 ovl_11_func_800CD3C4 (s32 arg0);
s32 ovl_11_func_800D622C (s8 arg0, s8 arg1, s16 arg2, s32 arg3);
void ovl_11_func_800C4A2C (Ovl11C4A2CObject *arg0);
void ovl_11_func_800C6770 (Ovl11C6770Object *arg0);
void ovl_11_func_800C70CC (Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C7270 (Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001B2CC (s32 arg0, s32 arg1);
void func_8001AF70 (u16 arg0, u16 arg1);
s32 ovl_11_func_80107C80 (s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void ovl_11_func_800CD670 (s16 arg0);
void func_800132F0 (s32 arg0, s32 arg1, s32 arg2);
void func_8001FE34 (s32 arg0);
s32 ovl_11_func_800F3E44 (s16 arg0);
s32 func_8002261C (s32 arg0, s32 arg1);
void ovl_11_func_800CBD78 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CF044 (s16 arg0, Ovl25SetupStatePair *arg1, VECTOR *arg2);
void ovl_11_func_800CF314 (u8 *arg0, M2C_b6c4596908ca_Ovl11CF314Arg1 *arg1, M2C_b6c4596908ca_Ovl11CF314Arg2 *arg2);
s32 ovl_11_func_800CDF4C (u8 *arg0);
void ovl_11_func_800C5738 (Ovl11C498CObject *arg0);
void ovl_11_func_800CE034 (M2C_ad72053e5aa9_StructOvl11CE034A *arg0, M2C_ad72053e5aa9_StructOvl11CE034B *arg1, s32 arg2);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800D666C (s16 arg0);
void ovl_11_func_800D7328 (s16 *arg0);
s32 ovl_11_func_800CB540 (u16 *arg0);
void ovl_11_func_800F3AF0 (u16 arg0, u16 arg1);
s32 ovl_11_func_800F2354 (s32 arg0, s32 arg1);
s32 ovl_11_func_800C8FA8 (Ovl11C8FA8Object *arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
M2C_f3fe4d9631b5_UnkStruct800CBF40 *ovl_11_func_800CBF70 (M2C_f3fe4d9631b5_UnkStruct800CBF70Arg0 *arg0, u16 *arg1, s16 arg2);
void ovl_11_func_800C55FC (Ovl11CE034Object *arg0);
s32 ovl_11_func_800CE53C (s16 arg0);
void ovl_11_func_800D7348 (u16 *dst, u16 *src);
void ovl_11_func_800C9D98 (void);
s32 ovl_11_func_800C6858 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CE514 (s16 arg0);
void ovl_11_func_800CCF58 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C4DE0 (Ovl11C7010Object *arg0);
s32 ovl_11_func_80113310 (Ovl11CE034Object *arg0);
void ovl_11_func_800CD5BC (M2C_583b0b68ff5a_UnkStruct800CD5BC *arg0, s32 arg1);
s32 ovl_11_func_800C4B6C (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C4C94 (Ovl11C7010Object *arg0);
void func_8001FAE8 (s32 arg0);
s32 ovl_11_func_800C5098 (Ovl11CD5BCObject *arg0);
void ovl_11_func_800C541C (Ovl11CD5BCObject *arg0);
void ovl_11_func_800C523C (Ovl11CD5BCObject *arg0);
s32 GetVal8005E3EC (void);
void func_8001B4D0 (s32 arg0, s32 arg1);
s32 func_800212A8 (s32 soundId, s32 lo, s32 hi);
s32 func_80021484 (s32 arg0);
void ovl_11_func_800BD358 (void);
s32 ovl_11_func_800C0824 (void);
s32 func_80013394 (void);
s32 ovl_11_func_800DD1D0 (s16 *arg0);
s32 func_8001AF44 (u32 arg0);
void ovl_11_func_800C498C (Ovl11C498CObject *arg0);
void ovl_11_func_800C47F0 (void);
s32 ovl_11_func_80107DEC (M2C_42071334137b_Ov11Timer *arg0);
void ovl_11_func_800CD6F4 (void *arg0);
s32 ovl_11_func_8011FFA8 (Ovl11SelectionObject *obj);
s32 ovl_11_func_800C6AFC (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CB114 (Ovl11CB114Object *arg0, s32 *arg1, s16 arg2);
s32 ovl_11_func_800BDB88 (s32 *arg0, s32 arg1);
s32 GetVal8005E524 (void);
s32 ovl_11_func_800C9D64 (void);
s32 ovl_11_func_800D7EF8 (s32 *arg0, u16 *arg1, u16 *arg2);
s32 ovl_11_func_8011E090 (VECTOR *pos, u16 *colOut, u16 *rowOut);
s32 ovl_11_func_800D5960 (s16 *arg0, u32 arg1, s16 arg2, s16 *arg3, s16 *arg4, s16 arg5);
s32 ovl_11_func_800D81BC (s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800C5048 (s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
s32 ovl_11_func_800F46A4 (Ovl11SpawnEffectRecord *arg0);
s32 ovl_11_func_800D8C6C (s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
s32 ovl_11_func_800F482C (Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D88D0 (s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
s32 ovl_11_func_800F474C (Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011E1E8 (s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D759C (s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D6A94 (s16 arg0);
s32 ovl_11_func_800D5750 (s16 arg0);
s32 ovl_11_func_800C69B8 (Ovl11C7010Object *arg0);
void ovl_11_func_800C6A24 (Ovl11C7010Object *arg0);
void func_80015840 (ObjectState *obj, s8 arg1);
void ovl_11_func_800C7010 (Ovl11C7010Object *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800CCFF8 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1, u8 arg2, s32 arg3);
void ovl_11_func_800C6E0C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6E8C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F0C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F6C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
u32 func_8001589C (SpriteSourceData *src);
s32 ovl_11_func_800C72F8 (Ovl11C7010Object *arg0, u16 *arg1);
s32 ovl_11_func_800C73FC (Ovl11C7010Object *arg0, u16 *arg1);
s32 ovl_11_func_800C74AC (u8 *arg0, u16 *arg1);
s32 ovl_11_func_800C7524 (Ovl11C7010Object *arg0, u16 *arg1);
s32 ovl_11_func_800CB850 (s32 arg0);
s32 ovl_11_func_800C3138 (u8 *arg0);
void *ovl_11_func_800BDE84 (u8 *arg0, s16 *arg1);
void ovl_11_func_800FEC2C (void);
void ovl_11_func_800F6778 (void);
void ovl_11_func_800FFA40 (void);
void ovl_11_func_800F9D5C (void);
void ovl_11_func_80107F38 (void);
void ovl_11_func_800F6734 (void);
void ovl_11_func_800FFE64 (void);
void ovl_11_func_800FFEF0 (void);
s32 ovl_11_func_800F4B5C (s16 arg0);
void ovl_11_func_800F66F0 (void);
void ovl_11_func_800C893C (Recon_ovl_11_func_800C7270_A0View *arg0);
void ovl_11_func_800C846C (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 ovl_11_func_800C8AD4 (Recon_ovl_11_func_800C7270_A0View *arg0, s16 arg1, s16 arg2);
void ovl_11_func_800C8A08 (Recon_ovl_11_func_800C7270_A0View *arg0);
void ovl_11_func_800C8334 (Recon_ovl_11_func_800C8334_A0View *arg0);
void ovl_11_func_800C83FC (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
void ovl_11_func_800C838C (Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1);
s32 ovl_11_func_800F3AC4 (u16 arg0);
void ovl_11_func_8011F0D8 (s32 arg0);
u16 ovl_11_func_800BF2F4 (s16 *arg0, s16 *arg1);
s32 ovl_11_func_80118C28 (s16 id);
void ovl_11_func_800C8764 (Recon_ovl_11_func_800C8334_A0View *arg0);
s32 ovl_11_func_800F24B0 (void);
void ovl_11_func_800C84DC (Recon_ovl_11_func_800C7270_A0View *arg0);
s32 ovl_11_func_800F3F7C (void);
void ovl_11_func_8011E588 (void);
s32 ovl_11_func_800FBE4C (void);
s32 ovl_11_func_800FAAD4 (void);
void *ovl_11_func_800BDCE8 (u8 *arg0, M2C_cec44150eede_Ovl11Func800BDCE8Arg1 *arg1, s32 arg2);
s32 ovl_11_func_800C8CC8 (M2C_15512f31231d_Func800C7878Arg0 *arg0);
s32 ovl_11_func_800D5810 (s16 arg0);
void ovl_11_func_800C9DC0 (void);
s32 ovl_11_func_800C9E3C (s32 arg0);
void ovl_11_func_800CB618 (void);
s32 ovl_11_func_800C9DE8 (s32 arg0);
s32 ovl_11_func_8010B57C (u16 *arg0, s32 arg1);
s32 ovl_11_func_800CEEF8 (M2C_38c89582ddcb_M2C_4c1a205b3b09_ReconA0View *arg0);
s32 ovl_11_func_800F4240 (M2C_4c1a205b3b09_ReconA0View *arg0);
s32 ovl_11_func_80112DA8 (Ovl11C7010Object *arg0);
void ovl_11_func_800C93A4 (Ovl11C7010Object *arg0);
u16 ovl_11_func_8010734C (s32 id);
s32 ovl_11_func_800C907C (Ovl11C8FA8Object *arg0, u16 arg1);
void ovl_11_func_800C8D9C (Recon_ovl_11_func_800C7270_A0View *arg0);
s32 ovl_11_func_800C7838 (u8 *arg0);
s32 ovl_11_func_800C8D24 (M2C_15512f31231d_Func800C7878Arg0 *arg0, u16 arg1);
s32 ovl_11_func_800C76EC (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C77D8 (Ovl11C7010Object *arg0, u16 arg1);
s32 ovl_11_func_800C97D0 (Recon_ovl_11_func_800C97D0_A0View *arg0);
void ovl_11_func_800C8E98 (Recon_ovl_11_func_800C7270_A0View *arg0);
s32 ovl_11_func_800C99C8 (void);
void ovl_11_func_800CBE14 (s32 arg0);
s32 ovl_11_func_800C9888 (u16 arg0, u16 arg1);
s32 ovl_11_func_800C9938 (M2C_1052de249052_Ovl119938View *arg0);
s32 ovl_11_func_800CADBC (Ovl11Func800CADBCArg0 *arg0, s32 arg1);
void ovl_11_func_800CBF40 (M2C_1729124c6ef2_UnkStruct800CBF40 *arg0);
s32 ovl_11_func_800D72E8 (M2C_029d3dcb50b2_UnkStruct800D72E8 *arg0, s16 arg1);
void ovl_11_func_800BD168 (void);
void func_80021B90 (s32 arg0);
s32 ovl_11_func_800DF010 (M2C_8d3156bb36aa_Struct_800DF010 *arg0, s32 arg1);
s32 ovl_11_func_800E2824 (u16 *arg0, s32 arg1);
s32 ovl_11_func_80109188 (M2C_708e65b63435_Ovl11D04D4Obj *arg0, s32 arg1);
void ovl_11_func_800D31EC (s16 arg0, s16 arg1);
s32 ovl_11_func_800D3104 (u16 *arg0, s32 arg1);
s32 ovl_11_func_800CEFCC (Ovl11CEFCCObject *arg0, Ovl11CollisionTag *arg1, VECTOR *arg2);
s32 ovl_11_func_800CF258 (s32 arg0);
void ovl_11_func_800D3200 (s32 arg0);
void ovl_11_func_80107DD0 (s16 *arg0);
void ovl_11_func_800E3C04 (void);
void ovl_11_func_800DBAB0 (void);
void ovl_11_func_800F1038 (u16 arg0, s32 arg1);
void ovl_11_func_800F6680 (void);
void SetVal8005E334 (s32 arg0);
void SetVal8005E2BC (s32 arg0);
void ovl_11_func_800F9D3C (void);
s32 ovl_11_func_800F6648 (void);
void ovl_11_func_800F6638 (void);
void ovl_11_func_800FAAAC (void);
void ovl_11_func_800FEBF8 (void);
void ovl_11_func_800FFA28 (void);
void ovl_11_func_800FFDCC (void);
void ovl_11_func_800F69D0 (void);
s32 ovl_11_func_800F77A8 (s16 arg0);
s32 ovl_11_func_80107390 (s32 arg0);
s32 ovl_11_func_80106F20 (s16 arg0, u16 arg1);
s32 ovl_11_func_801136D0 (s32 *arg0, u16 *arg1, u16 *arg2);
s32 ovl_11_func_80112EC4 (s32 arg0, s32 arg1, s16 arg2, s16 arg3);
u32 Rand (s32 arg0);
s32 ovl_11_func_80113A20 (s32 arg0);
void ovl_11_func_8011E574 (void);
void ovl_11_func_8011F52C (void);
s16 ovl_11_func_80120358 (M2C_93d07a715929_Ovl11SelectArg *arg0);
void func_8001B3CC (s32 arg0);
void ovl_11_func_80120850 (s32 arg0);
s16 ovl_11_func_80120414 (s32 *arg0);

s32 ovl_11_func_800F3EF0(Recon_ovl_11_func_800CD08C_A0View *); /* static */

s32 ovl_11_func_800C580C(Ovl11C580CObject *arg0) {
    VECTOR sp20;
    Ovl11CC7CCRequest request1;
    Ovl11CC7CCRequest request2;
    Ovl11CC7CCRequest request3;
    M2C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0;
    M2C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0_2;
    M2C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0_3;
    s16 temp_s0_4;
    s16 temp_s0_5;
    s16 temp_v1_4;
    s16 var_a1_3;
    s16 var_a2_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s8 var_a0;
    u16 *temp_s0_6;
    u16 *var_a0_2;
    u16 *var_a0_3;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v1_2;
    u8 temp_a2;
    u8 temp_v1;
    void *temp_a0;
    void *temp_v0_3;
    void *temp_v1_3;

    var_s3 = 0;
    if (arg0->unk6C & 0x1000) {
        temp_v0 = arg0->state;
        switch (temp_v0) {                          /* switch 1 */
        case 0x3D:                                  /* switch 1 */
        case 0x3E:                                  /* switch 1 */
        case 0x3F:                                  /* switch 1 */
        case 0x40:                                  /* switch 1 */
        case 0x41:                                  /* switch 1 */
        case 0x42:                                  /* switch 1 */
        case 0x43:                                  /* switch 1 */
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C(arg0);
            return 0;
        case 0x52:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            func_8001FABC(0x15);
            arg0->timer = 0x64;
            goto block_26;
        case 0x55:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            func_8001FABC(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 8);
            temp_s0 = (M2C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0);
            }
            ovl_11_func_800CCF14(temp_s0);
            arg0->unk6C &= 0xFFFF7FFB;
            ovl_11_func_800CB4E0();
            goto block_22;
        case 0x50:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            ovl_11_func_800CF308(0);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            ovl_11_func_800CB4E0();
            goto block_22;
        case 0x51:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            func_8001FABC(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 4);
            temp_s0_2 = (M2C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0_2);
            }
            ovl_11_func_800CCF14(temp_s0_2);
            arg0->unk6C &= 0xFFFF7FFB;
            ovl_11_func_800CB4E0();
            goto block_22;
        case 0x4E:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            var_a1 = 6;
            goto block_19;
        case 0x4F:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 2);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            ovl_11_func_800CB4E0();
            goto block_22;
        case 0x4D:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            var_a1 = 5;
block_19:
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, var_a1);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
block_22:
            var_v0_2 = ovl_11_func_800CD3C4(0);
            var_a0 = 0;
block_23:
            ovl_11_func_800D622C(var_a0, (s8) var_v0_2, 0, 0);
block_24:
            return 0;


        case 0x53:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            func_8001FABC(0xE);
            ovl_11_func_800C4A2C((Ovl11C4A2CObject *) arg0);
            arg0->unk6C &= 0xEFFFFFFF;
            ovl_11_func_800CCF14((M2C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84));
            ovl_11_func_800CB4E0();
block_26:
            var_v0_2 = ovl_11_func_800CD3C4(0);
            var_a0 = -1;
            goto block_23;
        case 0x54:                                  /* switch 1 */
            var_a1_2 = 0x2D;
            var_a2 = 1;
            var_a3 = -2;
            goto block_44;
        case 0x4C:                                  /* switch 1 */
            arg0->state = 0x31U;
            ovl_11_func_800C6770((Ovl11C6770Object *) arg0);
            ovl_11_func_800CD08C(arg0);
            arg0->unk6C &= ~0x4000;
            goto block_121;
        case 0x3B:                                  /* switch 1 */
            func_8001FABC(0x10);
            arg0->timer114 = 0;
            arg0->direction = 0U;
            arg0->unk6C |= 1;
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x56U, 0, -2);
            return 0;
        case 0x56:                                  /* switch 1 */
            arg0->unk6C = (arg0->unk6C | 0x1800) & 0xBFFFFFFF;
            arg0->attributes = (u16) (arg0->attributes | 0x100);
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x3CU, 0, D_80122F78 * 0xA);
            arg0->unk6C |= 0x40000000;
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, D_80122F78 * 0xA);
            func_8001B2CC(0, 3);
            return 0;
        case 0x58:                                  /* switch 1 */
            var_a1_2 = 0x57;
            var_a2 = 1;
            var_a3 = D_80122F78 * 0xA;
            goto block_44;
        case 0x45:                                  /* switch 1 */
            arg0->unk6C = (arg0->unk6C | 0x10) & 0xDFFFFFFF;
            goto block_24;
        case 0x49:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            temp_s0_3 = (M2C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x8A);
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D58D0(temp_s0_3);
            ovl_11_func_800CCF14(temp_s0_3);
            goto block_121;
        case 0x46:                                  /* switch 1 */
            if ((D_80128C50 & 0x80) && (ovl_11_func_800F3EF0(arg0) != 0)) {
                func_8001FABC(0x13);
                func_8001AF70(7U, 1U);
            }
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C(arg0);
            goto block_121;
        case 0x4B:                                  /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            ovl_11_func_80107C80(0x2F, 0U, NULL, 1);
            arg0->direction = (u16) ((arg0->direction + 2) & 3);
            ovl_11_func_800CD670(0x14);
            return 0;
        case 0x4A:                                  /* switch 1 */
            temp_v1 = arg0->phase;
            if (temp_v1 == 3) {
                ((struct Ovl11C580CWork *)&D_8007AFF0)->mode = 0xC;
                func_800132F0(0x1E, 0, 2);
                func_8001FE34(0x1E);
                arg0->unk6C |= 0x400;
                func_8001AF70(0x47U, 1U);
                return 0;
            }
            if (temp_v1 == 1) {
                func_8001AF70(0xAU, 1U);
            }
            var_a1_2 = 0x4A;
            temp_a2 = arg0->phase;
            var_a3 = *((s32 *) ((u8 *) &D_80123244 + (temp_a2 * 4)));
            var_a2 = temp_a2 + 1;
block_44:
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, var_a1_2, var_a2, var_a3);
            return 0;

        default:                                    /* switch 1 */
            ovl_11_func_800CD08C(arg0);
            return 0;
        }
    } else {
        temp_v1_2 = arg0->state;
        var_s2 = 0;
        switch (temp_v1_2) {                        /* switch 2 */
        case 0x6F:                                  /* switch 2 */
            func_8001FABC(9);
            ((struct Ovl11C580CState *)D_8006C838)->record->flags |= 0x100;
            D_80129634 = 3;
            if (ovl_11_func_800F3E44(arg0->item) != 0) {
                func_8002261C(3, 0x383);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
block_121:
            ovl_11_func_800CB4E0();
            return 0;
        case 0x3A:                                  /* switch 2 */
            if (arg0->unk6C & 0x80) {
                ovl_11_func_800CF044(*(s16 *)Ovl11SavedInteractionObject, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), (VECTOR *) ((u8 *) arg0 + 0x100));
            } else {
                ovl_11_func_800CF314((u8 *)&D_80075AD4, (M2C_b6c4596908ca_Ovl11CF314Arg1 *) ((u8 *) arg0 + 0xFC), (M2C_b6c4596908ca_Ovl11CF314Arg2 *) ((u8 *) arg0 + 0x100));
            }
            arg0->unk6C &= ~0x180;
            ovl_11_func_800CDF4C((u8 *) arg0);
            ovl_11_func_800C5738((Ovl11C498CObject *) arg0);
            return 1;
        case 0x59:                                  /* switch 2 */
        case 0x5A:                                  /* switch 2 */
        case 0x5B:                                  /* switch 2 */
        case 0x5C:                                  /* switch 2 */
        case 0x5D:                                  /* switch 2 */
        case 0x5E:                                  /* switch 2 */
        case 0x5F:                                  /* switch 2 */
        case 0x61:                                  /* switch 2 */
        case 0x62:                                  /* switch 2 */
        case 0x64:                                  /* switch 2 */
        case 0x65:                                  /* switch 2 */
        case 0x66:                                  /* switch 2 */
        case 0x69:                                  /* switch 2 */
        case 0x6A:                                  /* switch 2 */
        case 0x6B:                                  /* switch 2 */
        case 0x6E:                                  /* switch 2 */
            temp_s0_4 = arg0->item;
            if (arg0->state == 0x61) {
                func_8001FABC(0x13);
            }
            if (arg0->state == 0x62) {
                ovl_11_func_800CE034((M2C_ad72053e5aa9_StructOvl11CE034A *) arg0, (M2C_ad72053e5aa9_StructOvl11CE034B *)&sp20, 4);
                ovl_11_func_800F43CC(0x64, sp20, arg0->collisionId, 0);
                func_8001FABC(7);
            }
            if (arg0->state == 0x6E) {
                func_8001FABC(0x10);
                ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) arg0->item;
                if (arg0->item == 0xA4) {
                    ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) ((struct Ovl11C580CState *)D_8006C838)->fallback;
                }
                ovl_11_func_800F0FB0(4);
            }
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D666C(arg0->item);
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0x8A));
            if (arg0->state == 0x6A) {
                func_8001AF70(0x45U, 1U);
                temp_v0_2 = ((struct Ovl11C580CState *)D_8006C838)->count + 1;
                ((struct Ovl11C580CState *)D_8006C838)->count = temp_v0_2;
                if ((s16) temp_v0_2 < 2) {
                    ovl_11_func_800D622C(0, -5, 0, 0);
                }
            }
            if (arg0->state == 0x59) {
                request1.id = 0x8E;
                if (temp_s0_4 == 0x8C) {
                    request1.id = 0x8F;
                }
                if (temp_s0_4 == 0x8D) {
                    request1.id = 0x90;
                }
                if (temp_s0_4 == 0xE3) {
                    request1.id = 0xE4;
                }
                request1.count = 1;
                func_8001FABC(7);
                var_a0_2 = &request1.id;
block_93:
                ovl_11_func_800CB540(var_a0_2);
                return 0;
            } else if (arg0->state == 0x5A) {
                request2.id = 0x94;
                if (temp_s0_4 == 0x92) {
                    request2.id = 0x95;
                }
                if (temp_s0_4 == 0x93) {
                    request2.id = 0x96;
                }
                if (temp_s0_4 == 0xE7) {
                    request2.id = 0xE6;
                }
                request2.count = 1;
                func_8001FABC(7);
                var_a0_2 = &request2.id;
                goto block_93;
            } else if (arg0->state == 0x5B) {
                request3.id = 0x88;
                if (temp_s0_4 == 0x123) {
                    request3.id = 0x89;
                }
                if (temp_s0_4 == 0x124) {
                    request3.id = 0x8A;
                }
                if (temp_s0_4 == 0xE8) {
                    request3.id = 0xE5;
                }
                if (temp_s0_4 == 0x125) {
                    request3.id = 0x8A;
                }
                request3.count = 1;
                func_8001FABC(7);
                var_a0_2 = &request3.id;
                goto block_93;
            } else if (arg0->state == 0x5C) {
                ovl_11_func_800F3AF0(1U, 0U);
                ovl_11_func_800CB4E0();
            }
            break;
        case 0x33:                                  /* switch 2 */
            ovl_11_func_800F2354((s32) (*(s16 *) ((u8 *) D_8006C858 + 0x1002)), 0);
block_100:
            ovl_11_func_800D666C(arg0->item);
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            ovl_11_func_800CB4E0();
            goto block_152;
        case 0x34:                                  /* switch 2 */
            arg0->kind = 1;
            if (arg0->item == 0x6E) {
                arg0->kind = 2;
            }
            arg0->slot = (u16) *((u16 *) ((u8 *) &D_8012323C + (arg0->kind * 2)));
            goto block_100;
        case 0x45:                                  /* switch 2 */
            arg0->unk6C |= 0x10;
        default:                                    /* switch 2 */
block_151:
            goto block_152;
        case 0x32:                                  /* switch 2 */
            if ((ovl_11_func_800C8FA8((Ovl11C8FA8Object *) arg0) == 0) && (D_80128C54 & 0x400)) {
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) D_80128C4C_dispatch, 0xD);
                ovl_11_func_800CB4E0();
            }
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0xF0));
            arg0->unk6C &= 0xFFFF7FFF;
            goto block_151;
        case 0xA:                                   /* switch 2 */
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x15:                                  /* switch 2 */
            if (arg0->state == 0x15) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0xF0));
            goto block_121;
        case 0x2F:                                  /* switch 2 */
            func_8001FABC(9);
            ovl_11_func_800CBF70((M2C_f3fe4d9631b5_UnkStruct800CBF70Arg0 *) ((u8 *) arg0 + 0x100), (u16 *) ((u8 *) arg0 + 0x8A), (s16) arg0->direction);
            arg0->unk6C |= 0x40;
            /* fallthrough */
        case 0x9:                                   /* switch 2 */
            if (arg0->state == 9) {
                func_8001FABC(9);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            if (arg0->state == 9) {
                ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            }
            goto block_121;
        case 0x38:                                  /* switch 2 */
            func_8001FABC(9);
            ovl_11_func_800C55FC((Ovl11CE034Object *) arg0);
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            goto block_121;
        case 0xC:                                   /* switch 2 */
            temp_s0_6 = (u16 *) ((u8 *) arg0 + 0x8A);
            if (ovl_11_func_800CE53C(arg0->item) == 0) {
                ovl_11_func_800D7348((u16 *) ((u8 *) arg0 + (((s16) arg0->slot * 6) + 0xBA)), temp_s0_6);
                ovl_11_func_800C9D98();
                var_a0_3 = temp_s0_6;
            } else {
                ovl_11_func_800D666C(arg0->item);
                var_a0_3 = (u16 *) ((u8 *) arg0 + 0x8A);
            }
            ovl_11_func_800D7328((s16 *) var_a0_3);
            arg0->unk6C &= 0xEFFFFFFF;
            goto block_121;
        case 0xD:                                   /* switch 2 */
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x6:                                   /* switch 2 */
        case 0xE:                                   /* switch 2 */
        case 0x30:                                  /* switch 2 */
            ovl_11_func_800C6858((Ovl11C7010Object *) arg0);
            if (ovl_11_func_800CE514(arg0->item) == 1) {
                ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x2FU, arg0->direction + 0x17, -2);
            }
            var_v0 = 0;
            if (ovl_11_func_800CE53C(arg0->item) == 1) {
                arg0->unk6C |= 0x10000000;
                ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 0x67U, 0x1B, 0x1E);
                return 0;
            }
            /* Duplicate return node #165. Try simplifying control flow for better match */
            return var_v0;
        case 0x67:                                  /* switch 2 */
            arg0->unk6C |= 0x10000000;
            ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 0xCU, (*(u16 *) ((u8 *) &D_80071A00 + 0x38)) + 0x1D, -2);
            arg0->unk6C &= 0xFFFF7FFF;
            return 0;
        case 0x2C:                                  /* switch 2 */
            var_a1_3 = 0x2D;
            var_a2_2 = 0xB1;
            goto block_163;
        case 0x3:                                   /* switch 2 */
        case 0x31:                                  /* switch 2 */
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800C6770((Ovl11C6770Object *) arg0);
            ovl_11_func_800CCF58((Ovl11C7010Object *) arg0);
            ovl_11_func_800CB4E0();
            goto block_152;
        case 0x28:                                  /* switch 2 */
            ovl_11_func_800CCF14((M2C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x8A));
            ovl_11_func_800CB4E0();
            goto block_152;
        case 0x17:                                  /* switch 2 */
            var_s2 = 1;
            if (ovl_11_func_800C4DE0((Ovl11C7010Object *) arg0) == 1) {
                var_s2 = 0;
                var_s3 = 1;
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            goto block_152;
        case 0x19:                                  /* switch 2 */
            temp_v1_4 = *((struct Ovl11C580CWork *)&D_8007AFF0)->header;
            if ((temp_v1_4 == 0x37) || (temp_v1_4 == 0x43)) {
                var_s0 = 0x28;
                var_s3 = ovl_11_func_80113310((Ovl11CE034Object *) arg0);
                if (var_s3 != 0) {
                    var_s0 = 0x96;
                }
                ovl_11_func_800CD5BC((M2C_583b0b68ff5a_UnkStruct800CD5BC *) arg0, var_s0);
                var_s2 = 1;
            } else {
                var_s2 = 1;
                if (ovl_11_func_800C4B6C((Ovl11C7010Object *) arg0) == 1) {
                    var_s2 = 0;
                    var_s3 = 1;
                }
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            func_8001B2CC(0, 2);
            goto block_152;
        case 0x1F:                                  /* switch 2 */
            var_s2 = 1;
            if (ovl_11_func_800C4C94((Ovl11C7010Object *) arg0) == 1) {
                var_s2 = 0;
                var_s3 = 1;
            }
            func_8001FAE8((s32) D_80128C70);
            D_80128C70 = -1;
            goto block_151;
        case 0x68:                                  /* switch 2 */
            var_s2 = 0;
            if (ovl_11_func_800C5098((Ovl11CD5BCObject *) arg0) != 1) {
                var_s3 = 1;
            } else {
block_150:
                var_s2 = 1;
            }
            goto block_151;
        case 0x1D:                                  /* switch 2 */
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            ovl_11_func_800C541C((Ovl11CD5BCObject *) arg0);
block_149:
            func_8001B2CC(0, 2);
            goto block_150;
        case 0x1B:                                  /* switch 2 */
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            ovl_11_func_800C523C((Ovl11CD5BCObject *) arg0);
            goto block_149;
        case 0x39:
            return 1;
        }

block_152:
                if (var_s2 == 1) {
                    if (var_s3 == 0) {
                        ovl_11_func_800CB4E0();
                    }
                    temp_s0_5 = D_801231C0[(s16)((s16)(arg0->value - 1) / 5)][(s16)((s16)arg0->power / 20)];
                    ovl_11_func_800D622C((s8) temp_s0_5, ovl_11_func_800CD3C4(0), 0, 0);
                }
                var_v0 = 0;
                if (var_s3 != 1) {
                    temp_v0_3 = *(void **) ((u8 *) arg0 + 0x4C);
                    temp_a0 = (void *) ((u8 *) temp_v0_3 + 6);
                    if (temp_v0_3 != NULL) {
                        (*(void **) ((u8 *) arg0 + 0x4C)) = temp_a0;
                        if ((*(s16 *)((u8 *)temp_v0_3 + 6)) == 0x270F) {
                            ovl_11_func_800CB4E0();
                            (*(void **) ((u8 *) arg0 + 0x4C)) = NULL;
                            goto block_164;
                        }
                        if ((*(s16 *)((u8 *)temp_a0 + 4)) != 0) {
                            arg0->unk6C |= 0x1800;
                        }
                        temp_v1_3 = *(void **) ((u8 *) arg0 + 0x4C);
                        arg0->attributes = (u16) (arg0->attributes & 0xFFEF);
                        var_a1_3 = (*(s16 *)temp_v1_3);
                        var_a2_2 = (*(s16 *)((u8 *)temp_v1_3 + 2));
block_163:
                        ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, (u16) var_a1_3, (s32) var_a2_2, -2);
                        return 0;
                    }
block_164:
                    var_v0 = 1;
                    /* Duplicate return node #165. Try simplifying control flow for better match */
                    return var_v0;
                }
                return var_v0;
    }
}
```
