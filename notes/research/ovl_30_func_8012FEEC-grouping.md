# ovl_30_func_8012FEEC: translation-unit grouping evidence

Documentation-only review of the verified source
`src/overlays/ovl_30/ovl_30_func_8012FEEC.c` and the supplied passing-finalize
handoff, source/evidence identity
`41f21c91e0ebf1cc74710d6fb88f98b6c8eb14e81cf249f73ebcd16be40427cb`.
The source SHA-256 is
`1552adf547af62c0c8ecf00f3e5a4e3f7d0aca82a86e54023070f84c42c96812`,
also recorded by the exact experiment in
`build/experimentLedger/ovl_30_func_8012FEEC.jsonl`.
No solver work, build-input edits or new build were performed.

## New evidence: adjacent value-menu wrapper pair

The verified function supports a previously unrecorded medium-confidence
same-TU candidate with `ovl_30_func_8012FF50`:

- **Gapless mirrored code.** Both original functions occupy 0x64 bytes:
  `8012FEEC + 0x64 = 8012FF50`, then `8012FF50 + 0x64 = 8012FFB4`.
  `configs/splat/ovl_30.yaml` lists the pair consecutively at offsets
  0x2104 and 0x2168. Their instruction sequences have the same structure,
  differing in the header string, table address and entry count (apart
  from local branch destinations).
- **Shared menu protocol and state.** Both gate on `func_80013394`, print
  a header with `KanjiFntPrint`, initialize `D_80134014` from their incoming
  argument, call `ovl_30_func_80133454`, and return the resulting cursor;
  when gated off they return the incoming argument. The verified function
  passes `D_80134018` and 22 entries; its neighbour passes `D_801341D0` and
  23 entries. This is a shared-callee sibling tie, not a direct call
  between the pair or adjacency to their helper.
- **Table order corroborates code order.** The menu tables are contiguous
  20-byte-record arrays: `0x80134018 + 22*20 = 0x801341D0`, followed by
  `0x801341D0 + 23*20 = 0x8013439C`. The first table immediately follows
  the shared cursor word at 0x80134014. The original data layout thus
  agrees with the adjacent wrappers and their entry-count arguments.

Original instruction evidence:
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012FEEC/ovl_30_func_8012FEEC.s`
and
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012FF50/ovl_30_func_8012FF50.s`.
Original table evidence:
`build/ovl_30/asm/data/6188.data.s`, labels `D_80134014`, `D_80134018`
and `D_801341D0`. The neighbour remains an assembly stub in the current
source; its role here is established from original instructions.

## Limits

This hypothesis assumes the mirrored adjacent wrappers and matching data
order reflect source-local menu organization. They could instead have
been compiled from separate files using a common API. No private-static
ownership, declaration-order effect or TU-specific register quirk was
established; the ordinary matching register allocation is not such a quirk.
`D_80134014` and the helper are used by other menu callbacks, so those
shared references alone do not justify merging this pair with the distant
`80133454`/`80133974`/`80133A8C` group or the earlier lifecycle cluster.
The pair does not establish either outer file boundary or membership of
`8012FFB4` and the other intervening callbacks.
