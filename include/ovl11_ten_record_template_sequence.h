#ifndef OVL11_TEN_RECORD_TEMPLATE_SEQUENCE_H
#define OVL11_TEN_RECORD_TEMPLATE_SEQUENCE_H

#include "common.h"

/* ovl_11 ten-record choreography. Both signed value-slot indices are unchecked.
 * Copy the private two-by-five ID template and replace [1][4] with templateSlot's
 * unsigned low halfword. Values 20/3/14/7 select columns 0/1/2/3, 9 selects no
 * column, everything else column 4. Clear the selected row-0 record's flag 0x800
 * and replace its column with IDs 8/9; no-column clears ID 8's flag instead.
 * Record-action predicates can additionally disable columns 3 and 0.
 *
 * gOvl11ValueSlots[phaseSlot] == -1: initialize surviving records from the
 * parallel X/Z and signed frame tables, Y=0/animation=0, then increment phase
 * to zero. Otherwise call F07EC on each survivor with its target X/Z and a
 * LOCAL copy of the published phase; store the LAST survivor's resulting
 * phase. Updates do not accumulate phase from record to record.
 * Every call reapplies template/flag filtering and returns 1; record helper
 * results are discarded. Coordinates retain the state-record API's raw units
 * (not established as pixels). Shared record/value state must be initialized;
 * do not alias input phase/template slots if stable selection is required.
 * Original identity: ovl_11:800EB010. */
s32 Ovl11StepTenRecordTemplateSequence(s16 phaseSlot, s16 templateSlot);

#endif
