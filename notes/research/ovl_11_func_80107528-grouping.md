# ovl_11_func_80107528: dispatcher at the head of the guard run

Documentation-only review of the supplied byte-exact, passing-finalize handoff,
identity
`e9835132109db894823058c533f4d185ad61d29824f0c93d5f5840bb05e6fe3a`
(origin: agent; changed file: `src/overlays/ovl_11/ovl_11_func_80107528.c`).
The reviewed source has SHA-256
`6b7d6f8f69c42962c2e34811ddee98cec415b73b6281dbbef7ca219e957483f9`.
No solver experiments, build-input edits or new binary verification were
performed in this review.

## Membership evidence

The ledger already identified `ovl_11_func_80107528` as the shared caller of
four members of the candidate 0x801075C0–0x80107B54 button/state guard run,
but did not include the caller as a member. The reviewed dispatcher also
occupies the zero-gap position immediately before its first callee:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_80107528/ovl_11_func_80107528.s`
  records size 0x98, ending with the return delay slot at 0x801075BC.
  Thus 0x80107528 + 0x98 = 0x801075C0, the existing run head.
- `configs/splat/ovl_11.yaml` corroborates the order with file offsets
  0x4F708 (`80107528`) and 0x4F7A0 (`801075C0`), followed by `80107604`,
  `8010775C`, `8010780C` and `80107888`.
- The original assembly and verified C agree on direct calls to
  `801075C0`, `80107604`, `8010775C` and `80107888`. This is a dispatcher
  immediately preceding its contiguous callee family, not adjacency alone.

Together, call graph and zero-gap link order support extending the existing
candidate run backward to include `ovl_11_func_80107528`. The wider group's
confidence remains medium. The synthetic per-function splat splits are not
original translation-unit boundaries, and neither end of the extended run
is established as an original file boundary. The dispatcher references no
data globals and supplies no new shared-static or declaration-order evidence.
The stronger nested-function evidence for `8010780C`/`80107888` remains
specific to that pair; see
[the reviewed nested-guard evidence](ovl_11_func_80107888-grouping.md).

## Roles and retained context

The verified dispatcher switches on its first argument: cases 0 and 1 split
the u16 payload into high/low bytes for `801075C0` and `80107604`; case 2
passes it to `8010775C`; case 3 forwards it to `80107888`; other selectors
return zero.

The existing wider-run evidence remains unchanged: `801075C0` and `80107604`
share the 0x1D4-stride `D_8006C838 + 0x8000` record view (reads at +0x1A16
and +0x19EA/+0x19EC, respectively); `80107604` and `8010775C` both tail-call
`80107B54`. The latter is a nine-case selector dispatch choosing a range
pair. Original assembly paths follow the same per-function layout as the
dispatcher's path above; `src/overlays/ovl_11/ovl_11_func_80107604.c` and
`src/overlays/ovl_11/ovl_11_func_8010775C.c` expose these roles. These are
retained context, not new evidence from matching `80107528`.
