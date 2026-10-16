# ovl_11_func_800C6770 grouping evidence

Verified handoff identity: `2601bcd9e5a877fd467babedb0030014d17846543815148394c6cfa9cd666b8f` (origin: agent).
The supplied handoff reports byte identity and a passed full finalize gate;
this notes-only pass does not repeat or modify that verification.

## Shared snapshot writer/reader

- Verified source: `src/overlays/ovl_11/ovl_11_func_800C6770.c` restores
  `D_80128C58` into the six-byte item record at object offset `+0x8A` when
  the object's `+0x3C` state is `0x31`.
- Original reader assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C6770/ovl_11_func_800C6770.s`,
  `0x800C67C8–0x800C67E4`, copies a word and a halfword from that global
  into the object.
- Original writer assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CBB9C/ovl_11_func_800CBB9C.s`,
  `0x800CBBE0–0x800CBBFC`, copies the same six bytes from object `+0x8A`
  into `D_80128C58` before clearing/replacing the object's item record.
- A search of `build/ovl_11/asm/nonmatchings/` for `D_80128C58` finds these
  two functions as its named reader/writer sites. This complementary
  save/restore use supports a candidate same-TU pair, at low confidence.

## Limits

Both accesses are absolute; neither establishes a tentative definition,
GP-relative ownership, or original static linkage. The functions are not
adjacent, and the named-reference search does not exclude aliased accesses.
The data block in `build/ovl_11/asm/data/69960.data.s` reserves eight bytes
under `D_80128C58`; the observed six-byte copies do not establish the use
of the remaining bytes.

The candidate depends on the snapshot being private to one original TU,
which is not proven. It does not merge the pair into the nearby
`D_80128C4C / D_80128C54` cluster merely because the data addresses are close,
nor assign the intervening functions to the pair's TU.
