# ovl_11_func_800F291C: local queue-family membership

## Verified anchor

Supplied source/evidence identity:
`cd883f8041f65dd2970a878aa29ae3165f510f16ac2b3e587d0d1f4611f1b345`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800F291C.c`, with
`include/globals_override.h` as the other changed file. This notes-only review
accepts that gate without changing build inputs.

## New membership evidence

Extend the local queue family already recorded under the ledger's `ovl_11`
D_8006C838 flags/state-buffer cluster.

- **A specific shared reset/latch word, not merely the common buffer.**
  `src/overlays/ovl_11/ovl_11_func_800F2508.c` and its original target,
  `build/ovl_11/functions/ovl_11_func_800F2508.s`, initialize both queues'
  empty markers and clear the word at `D_8006C838+0x44FC` (800F2534).
  The verified 800F291C source tests and sets one-shot request bits in that
  same word. Its original target,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F291C/ovl_11_func_800F291C.s`,
  anchors `$s2` at `D_80070D34` (800F292C), reconstructs the buffer base by
  subtracting 0x44FC, and reads/writes the flags at `0($s2)` (for example
  800F3104 and 800F31E0). Thus the evaluator shares the queue initializer's
  actual reset state, not just an overlay-wide address idiom.
- **The caller and enqueue path agree with the local queue run.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F2620/ovl_11_func_800F2620.s`
  processes the two queues through 800F27B0/800F2880, then calls 800F291C at
  800F2700 while clearing the controller flag `D_80126C90` in its delay slot.
  The verified evaluator issues requests through 800F2538; that helper's
  original target at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F2538/ovl_11_func_800F2538.s`
  either calls 800F27B0 or appends to the second queue at `D_8007126C`.
  This connects the evaluator to both the established queue helpers and
  the controller flag documented in
  [the 800F2400 grouping review](ovl_11_func_800F2400-grouping.md).
- **Link placement corroborates those data/call links.**
  `configs/splat/ovl_11.yaml` places 800F2508 / 800F2538 / 800F2620 /
  800F2724 / 800F27B0 / 800F2880 / 800F291C in the existing contiguous
  queue run. The evaluator starts at file offset 0x3AAFC, immediately after
  800F2880 (0x3AA60, size 0x9C). Its original size 0xF04 ends at 0x3BA00,
  the start of its directly called predicate 800F3820.

## Inference and limits

The shared initializer/evaluator latch and corroborated local call/link run
support **low-confidence candidate same-TU membership** for 800F291C in the
existing local queue family. This assumes the initializer, queue helpers and
evaluator were emitted from one unit rather than adjacent units sharing public
state; neither private/static linkage nor an original TU boundary is proven.

No membership is newly asserted for the evaluator's callees, or for the whole
scattered D_8006C838 ledger cluster. The reconstructed header views, ordinary
callee-saved registers and neighbouring authorized register bindings are not
original-TU evidence.
