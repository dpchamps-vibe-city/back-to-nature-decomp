# ovl_11_func_800C7010: state-switch helper/dispatcher grouping

## Verified anchor

Supplied source/evidence identity:
`ce4e60553f71ccc027ae6aa9501d19d33b07bc354aefdd93d89a2e2a707288f9`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800C7010.c`. Its changed-file list
also includes `include/game_types.h`, `include/overlays/ovl_11.h`,
`include/sdk_types.h` and `notes/file-groupings.md`. This notes-only review
accepts that verification; it does not modify or re-verify build inputs.

## New membership evidence

- **Call graph corroborates immediate link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C6F6C, 800C7010, 800C70CC and
  800C7270 at offsets 0xF14C, 0xF1F0, 0xF2AC and 0xF450. The original
  assembly gives 800C7010 size 0xBC and 800C70CC size 0x1A4, so the target
  ends exactly at its caller, which ends exactly at the existing 800C7270
  member. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C70CC/ovl_11_func_800C70CC.s`,
  800C712C calls 800C7010, passing the same object and the address of its
  saved animation-selection argument at `sp+0x28`.
- **The adjacent caller ties the target to the existing helper run.**
  That same assembly calls 800C6E0C at 800C719C, 800C6E8C at 800C71B8,
  800C6F0C at 800C71E0 and 800C6F6C at 800C720C. Each receives the same
  object and `sp+0x28` cell. Thus the new function is a fifth helper of
  the same local dispatcher, not merely another user of a public type.
- **Original instructions corroborate the shared animation idiom.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7010/ovl_11_func_800C7010.s`
  reads `u16@+0x38`, `u16@+0x3C` and flags at `+0x6C`, stores a word to
  the selection cell and reloads its unsigned low byte before calling
  `func_80015840` on object `+0x260`. The verified source reuses
  `Recon_ovl_11_func_800C6F0C_A1View`, matching the word/byte idiom in
  `src/overlays/ovl_11/ovl_11_func_800C6F0C.c` and
  `src/overlays/ovl_11/ovl_11_func_800C6F6C.c`. Its non-0x100 branch also
  shares the caller's and `src/overlays/ovl_11/ovl_11_func_800C7270.c`'s
  `func_8001589C(+0x260)` test followed by a conditional low-byte copy
  from `+0x38` to `+0x265`.

## Function role

800C7010 selects an animation using object flags and state. With flag 0x100
set and state other than 0x39, it selects `+0x38 + 0x3F` and calls
`ovl_11_func_8010C1FC`. Without flag 0x100, it selects 0x1C or 0xA
according to flags 0x18000000, then conditionally
copies the low byte of `+0x38` to the sprite's `+5` byte (object `+0x265`).

## Inference and limits

Retain **medium confidence** for the existing candidate same-TU state-switch
family and include 800C7010 and its adjacent dispatcher 800C70CC. The premise
is that the contiguous helper/dispatcher arrangement, common argument cell
and repeated animation idioms reflect one local source unit. Separately
linked units remain possible: reconstructed typedef reuse is not original
static linkage, and neither end of this cluster establishes a TU boundary.

This evidence does not assign every intervening function in the broader
800C4F44–800C8D9C ledger span to one TU. Calls to distant
`ovl_11_func_8010C1FC` do not merge that callee's source unit into this group.
