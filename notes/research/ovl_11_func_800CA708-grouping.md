# ovl_11_func_800CA708: adjacent collision helper/movement caller

## Verified anchor

Supplied source/evidence identity:
`4ffd39517b99b9d8af973012e20b1173263de8211003d685dba500001c562aff`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CA708.c` as changed files. This notes-only
review accepts that verification without modifying or re-verifying build inputs.

## New membership evidence

- **Gapless helper/caller adjacency.** `configs/splat/ovl_11.yaml` places
  800CA708 and 800CAADC at offsets 0x128E8 and 0x12CBC.
  `build/ovl_11/functions/ovl_11_func_800CA708.s` gives the helper's size
  as 0x3D4: 0x800CA708 + 0x3D4 = 0x800CAADC. In
  `build/ovl_11/functions/ovl_11_func_800CAADC.s`, the immediate successor
  calls the helper at 800CAB20 before integrating movement.
- **Shared movement producer/consumer state.** The verified helper and
  original caller both retain `&D_80071A00` as their object base, copy its
  four-word position at +0x100, and add velocity from +0x110/+0x114/+0x118.
  The helper probes the prospective position and can replace those velocity
  components; the caller subsequently integrates them at 800CABD8–800CAC04
  and writes the resulting position at 800CACF4–800CAD00. The call therefore
  prepares the very state the adjacent function consumes, not just a shared
  global address.
- **Complementary transition handling.** The helper sets object +0x6C bit
  0x40000000 at 800CA884, zeros horizontal velocity, and stores -0x50 to
  vertical velocity at 800CA8EC–800CA8F8. The caller tests that bit at
  800CAC08–800CAC14 and clears it on one probe outcome at 800CAC34–800CAC54.
  When object bit 0x100 is set, the helper also sets bit 0x01000000 in
  `D_8006C838+0x92D0` at 800CA8B4–800CA8CC; the caller clears the same bit
  at 800CAC58–800CAC7C. These paired transitions corroborate the call/link
  relationship independently of reconstructed type names.

## Roles and limits

Record 800CA708 as a collision/side-slide probe adjusting player velocity
and dispatch state, and 800CAADC as the movement update applying the
helper-adjusted velocity. They are a **low-confidence candidate same-TU
pair within the existing D_80128C4C / D_80128C54 state-pair cluster**.
The new evidence is the narrower adjacent producer/consumer pair; the
helper's access to the dispatch pair was already recorded.

The conditional premise is that the gapless direct call and complementary
movement transitions reflect one original translation unit. Absolute global
accesses do not establish ownership or original static linkage. This does
not raise the wider cluster's confidence, locate its boundaries, or assign
other neighbours or callees to this pair. The helper's two offset tables
`D_80123250`/`D_80123270` have no other named references in the original
ovl_11 function assembly inspected here, so they supply no additional
membership tie. Its `D_80128C68` counter is reset by already-listed
800C9C60; that corroborates the existing state cluster without establishing
a new boundary.
