# ovl_11_func_800F1678 grouping evidence

The documentation handoff reports a passed full finalize gate and verified
identity `743f7cf6a5022089c080566326080d74e6975cd3e773867fa8c97467305e6cce`.
Reviewed source: `src/overlays/ovl_11/ovl_11_func_800F1678.c`.
This documentation pass changes no build inputs.

## Evidence supporting the existing candidate run

- The verified predicate is called by `ovl_11_func_800F1078` on the byte
  at its input record's +0xC. That caller also invokes the run's predicates
  800F144C, 800F14D8, 800F1540, 800F15B4 and 800F1744 on other encoded
  fields. This corroborates membership in the already recorded predicate
  family, rather than relying on address adjacency alone.
- The target assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F1678/ovl_11_func_800F1678.s`
  has extent 0xCC, ending exactly at 800F1744. Its predecessor 800F15B4
  has extent 0xC4 and ends exactly at 800F1678. These are the existing
  gapless-run boundaries, not newly inferred outer TU boundaries.
- The newly verified source selects a signed record index through
  800F1EE4 and reads bit 3 of the halfword at
  `D_8006C838 + index * 0x1D4 + 0x9A06`. Its immediate successor
  `src/overlays/ovl_11/ovl_11_func_800F1744.c` uses the same base/stride
  family and reads halfwords at +0x99EA/+0x99EC. Together with their shared
  caller, this adds a record-layout/data tie within the existing link run.

## Limits

Keep the run's low-confidence candidate same-TU classification. The shared
external save-state base and related record accesses do not prove static
linkage, original declaration ownership, or a TU boundary. The matched
reconstruction's existing per-function flag override is not independent
proof of membership or a reason to merge neighboring groups.
