# ovl_15_func_80130FF0 — grouping evidence

Verified handoff identity:
`5e3f3d7d9bd16cf1648bf11893fbd4c2ba34d82cd78ed8c87c3722a42059f424`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## Membership in two existing candidate clusters

- `src/overlays/ovl_15/ovl_15_func_80130FF0.c` opens with the
  `D_80053350/func_8001AC10` cluster's text-update idiom: one
  `D_8005E3C0->field_D8` load, offsets +0x18/+0x14, and an updater base
  (`D_800533C8`) plus `D_80054BBC[0]`. It also shares the
  `D_80137584` result store, `D_8013759A >= 0x5B` guard and conditional
  `ovl_15_func_8012E15C` / `D_80137586 = 1` reset tail with the listed
  `src/overlays/ovl_15/ovl_15_func_8013010C.c`. The target additionally
  calls `ovl_15_func_80135A78` and `ovl_15_func_80135380` before resetting;
  it is a variant, not an identical body.
- Those state accesses also support the existing `D_80137828`
  buffer-pointer cluster's membership entry. Its initialiser,
  `src/overlays/ovl_15/ovl_15_func_8012E15C.c`, clears all three state
  fields and sets the buffer pointer. The target's tie is this shared
  state block and direct reset call, not a read of `D_80137828` itself.
- Link placement corroborates that tie: `configs/splat/ovl_15.yaml`
  places `ovl_15_func_80130F2C` at 0x3144, the reset wrapper
  `ovl_15_func_80130FC8` at 0x31E0 and the target at 0x3208. The wrapper's
  0x28-byte span ends exactly at the target, and
  `src/overlays/ovl_15/ovl_15_func_80130FC8.c` calls the same initialiser
  before writing `D_80137586 = -1`.

Original target assembly corroborates the prologue at 0x80130FF4–0x80131024
and the shared-state/reset sequence at 0x80131028–0x80131080 in
`build/ovl_15/asm/nonmatchings/ovl_15_func_80130FF0/ovl_15_func_80130FF0.s`.

Together these support candidate same-TU membership in the existing
medium-confidence clusters. They do not establish static linkage or exact
original TU boundaries, nor require merging the two ledger sections. The new
updater declaration alone is not the grouping premise.
