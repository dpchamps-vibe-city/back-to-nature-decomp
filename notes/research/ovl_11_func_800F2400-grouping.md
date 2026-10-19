# ovl_11_func_800F2400: local queue-controller membership

## Verified anchor

Supplied source/evidence identity:
`4066fff741c8220433f92eeb519a16cd574ac7ff29773028d20a5808b85c2074`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, with `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800F2400.c` as its changed files. This
notes-only review accepts that verification without changing build inputs.

## New membership evidence

Extend the local queue family already recorded under the ledger's
`ovl_11` D_8006C838 flags/state-buffer cluster, rather than creating another
section for its existing member 800F2724.

- **Shared active flag across gapless neighbours.** The verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F2400/ovl_11_func_800F2400.s`
  show 800F2400 testing and clearing the word `D_80126C90`.
  `src/overlays/ovl_11/ovl_11_func_800F24B0.c` and the original target
  `build/ovl_11/functions/ovl_11_func_800F24B0.s` show its neighbour testing
  the same word and setting it to 1 after a successful 800F2724 call.
  `configs/splat/ovl_11.yaml` places the two at offsets 0x3A5E0 and
  0x3A690; 800F2400's original size 0xB0 ends exactly at 800F24B0.
- **A second flag links the controller to the documented queue helper.**
  800F2400 clears `D_80126C94` when inactive; when the state getter returns
  2, it consumes a nonzero latch by clearing both words and calling
  800F3840. The original target
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F2724/ovl_11_func_800F2724.s`
  sets that same latch at 800F2784 when the queue-head halfword is 0x19B,
  before shifting the queue. The verified controller therefore connects
  the adjacent `D_80126C90`/`D_80126C94` words to the existing queue family;
  it does not merely share the overlay-wide D_8006C838 base.
- **Direct calls agree with the local link run.** The original targets call
  800F2724 at 800F2464 and 800F24CC. The splat configuration and
  `build/ovl_11/functions.csv` place both controllers immediately before
  the gapless 800F2508 / 800F2538 / 800F2620 / 800F2724 / 800F27B0 /
  800F2880 queue run. The ledger already records the queue initializer,
  advance, append and removal helpers and the intermediate caller links.

## Inference and limits

The complementary active/latch accesses, direct queue-helper calls and
contiguous placement support **low-confidence candidate same-TU membership**
for 800F2400 and 800F24B0 in that local queue family. This assumes the
controllers and queue helpers were emitted from one source unit, rather
than neighbouring units sharing public state. Neither original TU boundary
nor private/static linkage of the flags is established. Ordinary retention
of the flag-address high half in `$s0` is not a register-variable quirk.

This is a local extension, not evidence that the entire scattered
D_8006C838 ledger cluster was one TU. The call to 800F3840 alone does not
establish that callee's membership, and no new membership is asserted for
other surrounding functions. The new header declarations and reconstructed
GfxObj view are not themselves original-TU evidence.
