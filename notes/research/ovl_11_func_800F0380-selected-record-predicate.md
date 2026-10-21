# ovl_11_func_800F0380: selected-record predicate

## Verified behavior

`src/overlays/ovl_11/ovl_11_func_800F0380.c` is byte-exact with signature
`s32 ovl_11_func_800F0380(s16 arg0)`. Its original 0xF4-byte body ends at
`ovl_11_func_800F0474`. The signed-halfword argument is established by the
entry sign extension; the function returns only 0 or 1.

All accesses are through `D_8006C838`:

- Selector 1 returns 0 if the unsigned halfword at +0x44D0 is zero; selector
  3 does the same for +0x44D2.
- With selector bit 1 clear, the signed index at +0x99CA + selector*4 selects
  a 0xB8-byte record starting at +0x81BC. Return whether its word at +0x34
  has bit 0x200 set.
- With selector bit 1 set, the signed index at +0x99C8 + selector*4 selects
  a 0xB4-byte record starting at +0x7AB4. Return whether its leading unsigned
  halfword is 0x10A.

The existing `Ovl11Status99C8View` and `Ovl11PoolResetView` declarations
express these accesses without new layouts or globals. The leading type word
and flag bit are structural facts; their gameplay meaning is not established.
The callee itself does not check selector/index bounds. Its caller
`ovl_11_func_800F05C0` supplies selectors 0..3, but that does not establish a
contract for arbitrary signed-halfword arguments or prove every client valid.

## Relationships and limits

`ovl_11_func_800D0BC8` reads the same +0x44D0/+0x44D2 availability halfwords
for selectors 1/3 and the first signed halfword of the +0x99C8 index pairs.
The adjacent `ovl_11_func_800F0474` also consumes that first halfword and the
0xB4-stride pool (copying into each record at +4). The original F05C0 call at
0x800F060C uses this predicate before conditional flag clearing and pair
resetting, in its four-selector loop.

These shared data accesses, original call and local link adjacency support
the existing low-confidence status-reader/copy links in
`notes/file-groupings.md`. They do not establish original translation-unit
boundaries or justify merging the distant clusters.

## Evidence and verification

- Original instructions:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F0380/ovl_11_func_800F0380.s`.
- Caller instructions:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F05C0/ovl_11_func_800F05C0.s`.
- Container link order: `configs/splat/ovl_11.yaml`.
- Shared layouts: `include/game_types.h` (`Ovl11Status99C8View`) and
  `include/globals_override.h` (`Ovl11PoolResetView`, `Ovl11ResetB4`,
  `Ovl11ResetB8`).
- `psx_finalize_function` passed the exact diff, full configured-container
  build, modification-scope and clean-source gates. No register pinning,
  embedded assembly or per-file flag override was introduced.
