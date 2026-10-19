# ovl_11_func_80113310: local grid/texture run extension

## Verified anchor

The handoff reports byte identity and the full finalize gate passed for
source/evidence identity
`1b5eb8c7a5c2737d5cbf46d935a83513011d1f40cc4c99aac7d446d18f851450`.
Its changed files are `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80113310.c`. This notes-only review accepts
that gate; it does not rerun matching or change build inputs.

## New grouping evidence

- **Calls agree with gapless link order on both sides.** Original assembly
  in `build/ovl_11/functions/ovl_11_func_80113208.s` spans 0x108 bytes,
  ending exactly at 0x80113310. The target's original assembly in
  `build/ovl_11/functions/ovl_11_func_80113310.s` spans 0x3C0 bytes, ending
  exactly at 0x801136D0. It calls its immediate predecessor at 80113408,
  801134DC, 801135B0 and 801135D4, and its immediate successor at 8011336C.
  `configs/splat/ovl_11.yaml` records the same consecutive starts at file
  offsets 0x5B3E8, 0x5B4F0 and 0x5B8B0. This extends the already-recorded
  80113124/80113208 caller/helper pair through the target and 801136D0.
- **The local caller and helpers form a complementary grid operation.**
  The verified target resolves its offset position through 801136D0,
  classifies an empty cell of the current `D_8012D110` 10x10 byte plane,
  and applies selector 1 or 2 through 80113208. The existing whole-plane
  caller `src/overlays/ovl_11/ovl_11_func_80113124.c` reads the same plane
  and applies those same selectors. The verified
  `src/overlays/ovl_11/ovl_11_func_801136D0.c` supplies the 10x10 coordinate
  bounds and checks the same pointer-grid flag used by the texture helper.
  The earlier [pair evidence](ovl_11_func_80113124-grouping.md) explicitly
  left the target's membership unassessed; this review connects that
  caller/helper pair to the target and its coordinate helper.
- **Initialized-data order corroborates the local code connection.** In
  `build/ovl_11/asm/data/69960.data.s`, the texture helper's `D_801281D0`
  slice occupies 0x801281D0–0x801281EF; the target's `D_801281F0` offset
  pairs start immediately afterward. This supports the local relationship,
  but the offset table has distant readers and is not established as static.

## Membership inference and limits

Candidate same-original-TU run, **medium confidence**:
`ovl_11_func_80113124`, `ovl_11_func_80113208`,
`ovl_11_func_80113310` and `ovl_11_func_801136D0`. The premise is that the
contiguous, directly connected grid callers and helpers reflect one source
unit; separately linked adjacent units remain possible. Neither end is a
proven TU boundary. No original static ownership, declaration-order effect
or register-variable quirk is established.

This does not merge the distant `D_801281F0` readers 800BF4B4/800CE034,
the wider 45-column pointer-grid family, or the other `D_8012D110` users
into this candidate TU. The target's distant caller 800C580C (call at
800C6510 in `build/ovl_11/functions/ovl_11_func_800C580C.s`) and its
weighted-item helper 80113A20 likewise gain no TU membership from their
call edges alone. The adjacent 80113818/80113A20 constant-table cluster
remains a separate hypothesis.
