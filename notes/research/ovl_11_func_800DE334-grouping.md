# ovl_11_func_800DE334: adjacent queued-callback candidate pair

## Verified premise

The supplied handoff records byte-exact matching and a passed full finalize
gate for source/evidence identity
`dc74965c1d5796d6a7f6b0932aceada162fa5e746b74d7fbac87c21c3c365a67`,
origin agent, with changed file
`src/overlays/ovl_11/ovl_11_func_800DE334.c`. This documentation review
changes no build inputs and does not rerun matching.

## New membership evidence

DE334 and its immediate predecessor DE2EC form a **low-confidence candidate
same-TU callback pair**, within the ledger's broader flags/state-buffer
section:

- **Shared indirect caller and matching table/code order:** the ten-entry
  callback table `D_800B7F54` places DE2EC at index 3 (0x800B7F60) and
  DE334 at index 4 (0x800B7F64). `ovl_11_func_800C0EFC` invokes the selected
  entry through this table and removes the queued ID when it returns
  nonzero. Both verified sources return 1 after their work. This identifies
  consecutive callbacks for the same queue dispatcher, not just unrelated
  users of the main state buffer.
- **Gapless link adjacency:** the map gives DE2EC a 0x48-byte body at
  0x800DE2EC, ending exactly at DE334. DE334 occupies 0x6C bytes and ends
  at DE3A0. The splat offsets 0x264CC / 0x26514 / 0x26580 corroborate
  these boundaries. Thus the shared-caller relationship and callback-table
  order agree with original code order for DE2EC / DE334.
- **Supporting state-access idiom:** DE2EC keeps the `D_8006C838` base
  across calls while reading the s16 at +0x44BA; DE334 keeps that base in
  `$s0` across its conditional call while reading the s16 at +0x44C0.
  These nearby state fields support the callback pairing, but normal
  callee-saved allocation and a shared main-binary buffer are not themselves
  distinctive TU evidence.

DE334 runs `ovl_11_func_800CEBC8`, `ovl_11_func_8010C95C` and
`ovl_11_func_800CD45C`, then calls `ovl_11_func_800C1BE0` separately when
the state halfword is 0xC and when it is 0x12. The latter helper clears
`unkA` of the 37 `D_80076220` records. These calls explain its queued-update
role; they do not establish membership with the distant callees.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DE334.c`
- `src/overlays/ovl_11/ovl_11_func_800DE2EC.c`
- `src/overlays/ovl_11/ovl_11_func_800C0EFC.c`
- `src/overlays/ovl_11/ovl_11_func_800C1BE0.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DE2EC/ovl_11_func_800DE2EC.s`
- `build/ovl_11/asm/data/F4.rodata.s` (`D_800B7F54` entries 3 and 4)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DE334/ovl_11_func_800DE334.s`
- `build/ovl_11/ovl_11.map` (0x800DE2EC–0x800DE46C text entries)
- `configs/splat/ovl_11.yaml` (offsets 0x264CC–0x2664C)

## Limits

The same-TU inference is conditional on these consecutive queued callbacks
having been authored together. A table may reference external functions,
and adjacent callbacks can still straddle original TU boundaries. No private
static, GP ownership, declaration-order effect or register-variable quirk is
established. The reconstructed struct view is not an original-declaration
fingerprint.

This adds only DE334 to the local candidate pairing with DE2EC. It does not
merge the pair with the distant C0EFC input-state run, all other callback
table entries, the merely adjacent DE3A0, or the scattered users of
`D_8006C838` in the ledger's umbrella section. No original TU boundary or
data ownership is claimed.
