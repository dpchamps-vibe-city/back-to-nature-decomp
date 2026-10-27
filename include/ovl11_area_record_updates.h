#ifndef OVL11_AREA_RECORD_UPDATES_H
#define OVL11_AREA_RECORD_UPDATES_H

/* ovl_11:800D1EB8, 800D2160, 800D2240, 800D2334.
 * Four ordered sweeps called by Ovl11RunAreaFrameUpdates. Require initialized
 * pools, stage state, handlers and sprite/ordering-table services. No arguments
 * or result; handlers, positions, effects, counters and GPU packet state change.
 * One call is one sweep, not elapsed time. Non-reentrant; pools must stay valid
 * across callbacks. Records are live when their first halfword is nonzero.
 * A stage mismatch resets the +A8 effect instead of presenting the record.
 * Work+0C bit 08000000 suppresses selected movement/timer steps, not all handlers
 * or drawing. Keep the separate sweeps: their gate order and policies differ. */

/* Twenty B8 tagged records, then ten B4 small records. Tagged flag 10000
 * selects its handler/presentation family. Work bit 40000000 permits tagged
 * movement across stages; record bit 20000 suppresses selected presentation.
 * Small-record handlers run before the stage test; bit 02000000 suppresses
 * their draw AND timer tick. */
void Ovl11UpdateTaggedRecordPools(void);

/* Three additional B4 small records. Unlike the ten-record sweep, the
 * matching-stage draw/timer has no 20000/02000000 suppression tests. */
void Ovl11UpdateSmallRecordTriplet(void);

/* Six B0 state records. Record flag 20000 skips the entire record. State
 * dispatch precedes the work freeze test; matching-stage draw is still allowed
 * while movement and effect ticking are frozen. */
void Ovl11UpdateStateObjectPool(void);

/* Mode-1/mode-2 stage singletons, in that order. Mode 1's flag 400 skips only
 * on calendar lookup row 8; mode 2's flag 400 always skips. On the matching
 * stage, flag 20000 suppresses presentation and work+5234 bit 100 additionally
 * suppresses mode 2. Eligible unfrozen movement updates the mode counters for
 * handler id 11 (hex), through the shared D266C service. */
void Ovl11UpdatePairedStageObjects(void);

#endif
