# ovl_11_func_800D539C grouping evidence

Verified source/evidence identity supplied by the handoff:
`a764646f6283048c02b410e76f39ba392cb806d000c85ae725d24435770dc679`.
The handoff reports byte identity and a passing full finalize gate; this
notes-only review does not repeat verification or change build inputs.

## New membership evidence

The verified body in
`src/overlays/ovl_11/ovl_11_func_800D539C.c` supports adding a candidate
handler-tail pair, `800D539C` / `800D55F8`, to the existing D_800B8EC0
state-dispatch group:

- **Common dispatchers:**
  `build/ovl_11/asm/data/104C.rodata.s` places `800D539C` at
  `D_800B8EC0[7]` and `800D55F8` at `[6]`. These are sibling handlers of
  the already-recorded `800D3048` / `800D3104` dispatchers. The verified
  `800D539C` also calls `800D3104` to enter state 0x13 or 0x14.
- **Gapless code adjacency:** `build/ovl_11/ovl_11.map` places
  `800D539C` at 0x800D539C with size 0x25C, ending exactly at
  `800D55F8` (size 0x148). Their dispatch-table order is reversed relative
  to code order, so this is common-dispatch corroboration of link adjacency,
  not an assumption that table order determines source order.
- **Adjacent per-handler data:** the verified body passes `D_80123C88`
  to `800D04D4`; the successor passes `D_80123C94` to the same helper at
  0x800D5700 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D55F8/ovl_11_func_800D55F8.s`.
  `build/ovl_11/asm/data/69960.data.s` shows consecutive 12-byte tables:
  `{0x00780010, 0x000F0000, 0x0000FFFF}` and
  `{0x005A0011, 0x000F0000, 0x0000FFFF}`. Data adjacency agrees with code
  adjacency and the shared step-helper idiom.
- **Shared handler convention:** both branch on the signed state at +0x28,
  set/clear flag 0x800 at +0x34 on entry/exit, store timer 0x12C on the
  state -1 path, and reset +0x28 to -1 when `800D04D4` completes. Their
  initialization paths also gate `func_8001FABC` on the same map comparison
  (`D_8007AFF0 + 0x25476` against object +0x30), with different event codes.
  The original target assembly corroborates the verified body at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D539C/ovl_11_func_800D539C.s`.

## Limits

This supports medium-confidence candidate same-TU membership, not a recovered
file boundary. The inference depends on the original code/data layout
represented by the assembly and map and on the shared dispatch/step idioms;
modern per-function object files are not original translation units.
No shared static, declaration-order effect or register-variable quirk was
established. The engine-owned object pools and far-buffer map field do not
prove ownership. This does not place the distant pool/state callees in the
same TU, widen the D_80128D78/D_80128D7A global cluster, or add other handlers
merely because they occur in D_800B8EC0.
