# ovl_11_func_800EDD54 grouping evidence

Supplied verified source/evidence identity:
`4cafdeca64cbb786eae31d3eeba3c2d8535561fd3ece0d18c590ab7b8d82c55a`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_11/ovl_11_func_800EDD54.c`. This is a notes-only assessment,
not a new verification run.

## Candidate membership

Add the target to the existing `D_80129560` accessor and
`D_80076280`/`D_80076300` record-selector families. Its new local evidence
supports a low-confidence same-TU candidate with `ovl_11_func_800EDEB8`:

- The verified target optionally resolves two signed-halfword IDs through
  `D_80129560`, then selects each position triple from
  `D_80076280 + id * 0x1D4`, substituting `D_80071B00` for ID `0x29`.
  `src/overlays/ovl_11/ovl_11_func_800EDEB8.c` uses precisely the same
  table, stride and special-ID storage to apply position deltas. This is
  an exact shared-storage/selector tie, not just a similar arithmetic idiom.
  The declarations are in `include/globals_override.h`.
- `configs/splat/ovl_11.yaml` places the target at file offset `0x35F34`
  and that successor at `0x36098`. The original target assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EDD54/ovl_11_func_800EDD54.s`,
  records size `0x164`: `0x800EDD54 + 0x164 = 0x800EDEB8`. Thus the shared
  selector/storage pair is gapless in link order, adding a local witness
  absent from the earlier scattered selector-family membership.
- The target's immediate predecessor,
  `src/overlays/ovl_11/ovl_11_func_800EDAA4.c`, also reads and writes
  `D_80129560`; its splat entry is immediately before the target at
  `0x35C84`. This corroborates placement within the already-recorded
  accessor band, without establishing a larger TU boundary.

## Existing wider-family context

The selector-family introduction previously recorded the `0x29` special-base
selector and the shift/add multiply chain for stride `0x1D4` on the scattered
800E9778/800EDEB8/800E7C2C members. In that existing evidence,
800E7C2C uses `D_80076300 = D_80076280 + 0x80` and special base
`D_80071CCC`; its record inspection calls the 80107DD0/80107DE0 initializer
pair. The existing 800E4428 member shares the position-record table but
obtains its index through 800EEBB8 rather than testing the special ID.
These older observations remain data-family context, not new TU evidence
from this target. The new local pairing does not strengthen the distant ties.

## Role and limits

The target derives a cardinal direction between the selected records via
`ovl_11_func_800F06D8` (whose source compares position fields +0 and +8),
then forwards it to `ovl_11_func_800E9284` for the special ID or to
`ovl_11_func_800E5B84` followed by `ovl_11_func_800E5C60` otherwise.
These call edges support its role and accessor-family dependencies, not
same-TU membership with those distant callees.

The target and `800EDEB8` do not call one another. The position globals are
resident shared data; overlay-private `D_80129560` is not proof of C static
linkage. There is no new shared-static, GP-ownership, declaration-order or
register-variable witness. The local pairing is conditional on the shared
selector/storage cluster and link adjacency reflecting source organisation;
it does not prove an original file boundary or merge the wider accessor,
record-selector or record-reset families into one TU.
