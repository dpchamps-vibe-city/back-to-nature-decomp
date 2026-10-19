# ovl_11_func_800F67E0: state-set run's adjacent dispatcher

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_800F67E0`, source/evidence identity
`03d16f6735725d845755b275a0224419bb5b7337a606a6816c99e25a254f3e18`.
Its changed-file list names
`src/overlays/ovl_11/ovl_11_func_800F67E0.c`. This notes-only review reads
the verified source and original disassemblies; it does not rerun the gate.

## New grouping evidence

The dispatcher extends the existing medium-confidence `800F66F0` / `800F6734` /
`800F6778` candidate group, rather than establishing a separate TU:

- `build/ovl_11/functions/ovl_11_func_800F6778.s` has size 0x68 and its
  last instruction at `0x800F67DC`; the dispatcher's original assembly,
  `build/ovl_11/functions/ovl_11_func_800F67E0.s`, starts at `0x800F67E0`.
  Thus `0x800F6778 + 0x68 = 0x800F67E0`, extending the zero-gap run.
  `configs/splat/ovl_11.yaml` independently records consecutive offsets
  0x3E8D0, 0x3E914, 0x3E958 and 0x3E9C0 for the four members.
- The original assemblies for `800F66F0`, `800F6734` and `800F6778`
  (same `build/ovl_11/functions/` directory) store 6, 7 and 4 to
  `D_80126E40`. The dispatcher consumes exactly these entry states:
  6/7 select readiness checks and message IDs 0x378/0x379, while 4 joins
  its message-completion path. This is a producer/consumer relationship,
  not merely another reference to a widely shared global.
- The dispatcher's ready path at `0x800F684C–0x800F688C` repeats the
  immediate predecessor's distinctive sequence: OR 0x20000 into
  `D_8006C838 + 0x0C`, call `func_8001FABC(3)`,
  `func_800226D8(0)`, `func_80017B18(0)` and `func_8002261C(3, id)`.
  Its IDs 0x378/0x379 complement the predecessor's 0x37A, and its next
  states 3/5 join the same completion path as state 4.
- The dispatcher's closing path at `0x800F6970–0x800F6980` calls
  `func_8001AF70(0x71, 0)` and `func_8001AF70(0x70, 0)`, pairing with
  the two earlier setters' calls using the same IDs and second argument 1.

## Limits

Adjacency, state handoff and the repeated flag/call idiom support suspected
same-TU membership, not a proven original boundary. The dispatcher does not
directly call the three setters. These globals are also used outside the run;
their absolute-addressed accesses do not establish ownership or static linkage.
No declaration-order effect or shared register-variable quirk is established.
Calls to `800F69FC`, `800F77F0` and `800F787C` do not by themselves merge
those helpers' candidate groups into this one. The dispatcher's size is 0x1F0
and its exclusive end is `0x800F69D0`; the following function is not added on
adjacency alone.
