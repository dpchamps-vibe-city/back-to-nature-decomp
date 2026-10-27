#ifndef OVL11_PROGRESS_GATE_H
#define OVL11_PROGRESS_GATE_H

#include "common.h"

/* Read-only 0/1 predicate; ovl_11 and shared work must be resident.
 * True when flag 25 is set and either flag C9 is set or the unsigned byte
 * at work+E642 is >=90. If includeFlag20 is nonzero, flag 20 independently
 * makes it true. Zero omits ONLY this last test, not the primary gate.
 * Flag IDs are hexadecimal; byte threshold 90 is decimal. No state changes.
 * Command dispatch, condition classification and event-slot selection share
 * this gate; its gameplay meaning and the byte's units are not established. */
s32 Ovl11TestProgressGate(s16 includeFlag20);

#endif
