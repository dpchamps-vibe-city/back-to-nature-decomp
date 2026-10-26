#ifndef OVL11_POOL_RECORD_TRANSITION_H
#define OVL11_POOL_RECORD_TRANSITION_H

#include "common.h"

/* Require a live pool pointer at shared work +DDD4 and room for the next
 * 0x18-byte record. Publish the selected pointer to gOvl11PublishedPoolRecord,
 * set its state word to 1 and initialize its X/Y/Z words. Current uses
 * (-5000,0,2300); next uses (-1200,0,2900), in the existing coordinate units.
 * Neither function allocates records or validates the pointer. */
void Ovl11PublishCurrentPoolRecord(void);
void Ovl11PublishNextPoolRecord(void);

/* Probe IDs 0x64/0x65 in the installed collision context. Hits publish the
 * current/next record; the next-record selector outside 1..6 completes the
 * gate even when a hit has just published that record. */
void Ovl11ProbePoolRecordTransitions(void);

/* Probe the active 0x02000000 gate and selected 0x800 record against volume
 * geometry and surface height. A gate hit clears its flag, deactivates the
 * next record's bit 0 and clears completion byte +E650. A selected-record hit
 * publishes/initializes the current record. All shared context must be live. */
void Ovl11ProbeActivePoolGate(void);

#endif
