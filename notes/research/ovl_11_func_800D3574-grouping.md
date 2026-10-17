# ovl_11_func_800D3574 grouping evidence

Verified source/evidence identity supplied by the handoff:
`6f6bcf49c9133006a123be41fab2ae2019ebe2527f7362f3d44f065fbf8be4a1`.
The handoff reports byte identity and a passing full finalize gate; this
notes-only review does not repeat that verification.

## New membership evidence

The verified body in
`src/overlays/ovl_11/ovl_11_func_800D3574.c` supports extending the existing
D_800B8EC0 state-dispatch candidate group with `800D3468` and `800D3574`:

- **Shared ID-dispatch idiom:** its first switch and the switch in
  `src/overlays/ovl_11/ovl_11_func_800D3468.c` enumerate the same object IDs
  from the leading u16. Both reject unknown IDs through
  `SystemError(0x45, ...)`; the sprite helper uses subcodes 0–0xC and the
  state selector uses 0x64. The newly verified sprite-refresh body adds this
  common object-ID/error convention to the previously recorded call edges.
- **Same-object caller:**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D3048/ovl_11_func_800D3048.s`
  preserves the input pointer in `$s0` and passes it to `800D3574` at
  0x800D3068 and conditionally to `800D3468` at 0x800D30D8. This is the
  existing table-dispatch driver, not an unrelated caller of a common engine
  service.
- **Gapless link adjacency:** `build/ovl_11/ovl_11.map` places `800D3468`
  at 0x800D3468 with size 0x10C, ending exactly at `800D3574`.
  The latter has size 0x63C and ends at 0x800D3BB0. The first boundary
  corroborates the common-caller/shared-ID tie; the successor address alone
  is not used to add another member.

## Limits

This is medium-confidence candidate same-TU evidence, not a recovered
translation-unit boundary. It depends on the verified bodies and the
original caller/link layout represented by the assembly and map above.
There is no demonstrated shared static, declaration-order effect or
register-variable quirk. Shared engine globals, sprite banks and the
`func_80015704` call are not treated as private ownership evidence, nor is
the modern per-function `.rodata` split proof of an original file boundary.
No distant sprite-loader caller group is widened on that basis.
