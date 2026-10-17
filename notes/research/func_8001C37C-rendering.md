# func_8001C37C — projected primitive emission

## Verified result

`src/func_8001C37C.c` is byte-exact: 936/936 relocated words, with residual
control-flow/population/schedule/allocation `[0, 0, 0, 0]`. The authoritative
`psx_finalize_function` gate passed the exact comparison, full configured
build, modification scope and clean-source checks. It also published the
signature through the function-context generator.

Documentation handoff identity:
`45416e0359323be7adbdfa9f1fef2e4589c34a85727a2add2396262a083a9ee1`.
This documentation changes no verified build input.

## Behaviour established by the matched source

The first argument is the base used to resolve relative offsets. The second
argument supplies the vertex offset at `+0`, primitive-stream offset at
`+0x10`, and signed primitive count at `+0x14`. Nonpositive counts do no work.

The stream is consumed as successive runs of these eight record classes,
in this order; the code is not an arbitrary-order switch dispatcher:

| Masked record flags (`flags & 0xFDFF0000`) | GPU packet | Input stride | Output bytes |
|---|---|---|---|
| `0x21010000` | `POLY_F3` | `0x10` | `0x14` |
| `0x31010000` | `POLY_G3` | `0x18` | `0x1C` |
| `0x25010000` | `POLY_FT3` | `0x1C` | `0x20` |
| `0x35010000` | `POLY_GT3` | `0x24` | `0x28` |
| `0x29010000` | `POLY_F4` | `0x10` | `0x18` |
| `0x39010000` | `POLY_G4` | `0x1C` | `0x24` |
| `0x2D010000` | `POLY_FT4` | `0x20` | `0x28` |
| `0x3D010000` | `POLY_GT4` | `0x2C` | `0x34` |

Each record supplies halfword vertex indices into the resolved `SVECTOR`
array. The GTE projects the first three vertices, rejects negative transform
flags and nonpositive NCLIP results, and writes packed screen coordinates.
Quads project a fourth vertex separately. The adjacent bounds helpers accept
when any supplied vertex X is in `[-19, 319]`; this is a vertex-X test, not
proof of general polygon/viewport intersection.

Accepted primitives use AVSZ3/AVSZ4 depth, add `0x23` when flag `0x8000`
is clear and one when flag `0x4000` is set, then link the packet into the
ordering table using `(depth / 4) * 4`. Packet command codes retain the
record's semitransparency bit. Each linked packet increments D_8005E4E8.

D_8005E3C0 supplies the packet cursor at `+0x118` and ordering-table base at
`+0x120`. The function caches that context in D_8005E4DC and publishes the
final cursor through `D_8005E4DC->field_118`.

## Grouping evidence and limits

The existing projected-primitive clipping cluster covers
`0x8001BFEC–0x8001D348`. The matched renderer adds a shared-global
fingerprint to the prior adjacency, direct calls and GTE idioms:

- `func_8001C0D4` clears D_8005E4E8 at `0x8001C0F4` and returns it at
  `0x8001C1A0`, both GP-relative halfword accesses.
- `func_8001C37C` increments that same counter in all eight accepted-packet
  paths. The first pair is at `0x8001C548`/`0x8001C554`; the last is at
  `0x8001D1CC`/`0x8001D1D8`, again GP-relative halfword accesses.
- The caller therefore resets/returns the renderer's emitted-packet count.
  This supports membership in the same rendering cluster; it does not prove
  the exact original translation-unit boundaries.

The original-word witnesses are the generated disassemblies under
`build/asm/nonmatchings/func_8001C0D4/` and
`build/asm/nonmatchings/func_8001C37C/`; the corresponding matched C remains
in `src/`. No broader cluster merge is inferred.

## Why the earlier park was a false positive

The preserved attempt was already exact when it was parked on
2026-10-17. Calls to SDK macros were permitted, but the patch scanner treated
six definitions in `include/psyq/inline_c_gas.h` as arbitrary embedded asm:
`gte_rtps`, `gte_avsz4`, `gte_rtpt_b`, `gte_nclip_b`, `gte_avsz3_b` and
`gte_avsz4_b`.

These are DMPSX-to-GAS compatibility forms of blessed SDK commands. Gate fix
`592d1529` recognizes their complete definitions against the independent
vendored SDK, using the encoding generator's shared substitution logic.
Replaying the original header patch now produces zero policy findings.
No per-function approval, register pin, new stub or compiler-flag override
was added for this renderer. The exact source was restored without a new
solver hypothesis; the later full finalization gate, not the historical
scratch match alone, establishes acceptance.
