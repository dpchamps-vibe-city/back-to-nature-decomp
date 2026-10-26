#ifndef OVL11_AREA_RECORD_CLAIMS_H
#define OVL11_AREA_RECORD_CLAIMS_H

/* ovl_11 must be resident. object is the existing 0xF8-byte record view;
 * the action tail uses u16 input ID +AC, count sources +AE/+B0/+B2,
 * pending/result ID +B6 and action mask +B8. No validation is performed.
 *
 * Process the first set bit in priority order 1,2,4. Bit 1 consumes/derives
 * items from the paired cell grids; bit 2 tries cell-kind promotions; bit 4
 * consumes a pending item, then runs the area 5/4/1 record-action fallback
 * chain. Counts include Rand calls (divisors 50/10/100 are raw integer
 * scales); the selector can consume additional random values. Updates pools,
 * slot flags, pending IDs and action state through the existing helpers.
 * Earlier effects are not rolled back on failure. Always returns zero;
 * zero does not report success. Uses shared state and is not reentrant. */
s32 Ovl11DispatchAreaRecordAction(u8 *object);

#endif
