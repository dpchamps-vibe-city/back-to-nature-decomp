#ifndef OVL11_ACTION_GUARDS_H
#define OVL11_ACTION_GUARDS_H

#include "common.h"

/* ovl_11 must be resident. Search 31 ID-keyed lists and return the first
 * matching guard record's unsigned-halfword result code, or -1 if no record
 * matches. Area blocking takes precedence and returns 0x1CD. No list mutation.
 *
 * Resets the shared selector to UINT_MAX. Successful state predicates latch
 * their selector, restricting subsequent records to that first payload. Both
 * predicates of an eligible record are evaluated in order, even if the first
 * fails; state/flag queries therefore must not be reordered or short-circuited.
 * Non-reentrant: the selector is shared across the entire evaluation.
 * Original identity: ovl_11:0x80107390. */
s32 Ovl11FindActionGuardResult(s32 id);

#endif
