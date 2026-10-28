#ifndef OVL11_RECORD_ACTION_PREDICATE_H
#define OVL11_RECORD_ACTION_PREDICATE_H

#include "common.h"

/* ovl_11:800EDF3C. Resolve recordOrSlot through the shared value bank when
 * the low halfword of fromValueSlot is nonzero. Indexing is unchecked.
 * Return false for ID 0B (hex) if game flag 129 is set or work byte E647
 * is not FF; for ID 14, false only if flag 25 == 1 and season != 1.
 * All other cases return true. No state is changed.
 * The two unused command operands retain the original callers' explicit
 * a2/a3 setup (e.g. zeros at 800EB864/868), not invented behavior.
 * The implementation retains its authorized address-prefix/register recovery;
 * naming this interface does not expand that existing source-policy grant. */
s32 Ovl11IsRecordActionAllowed(s16 recordOrSlot, s32 fromValueSlot,
                              s32 unused2, s32 unused3);

#endif
