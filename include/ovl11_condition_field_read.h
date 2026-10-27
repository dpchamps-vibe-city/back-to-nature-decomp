#ifndef OVL11_CONDITION_FIELD_READ_H
#define OVL11_CONDITION_FIELD_READ_H

#include "common.h"

/* Read a state field for encoded condition evaluation, forwarding fieldId and
 * elementIndex unchanged to Ovl11ReadStateField. Returns its full signed
 * result; the condition-range caller separately keeps only the low byte.
 * No bounds checks, masking or writes are added here. Valid IDs/indices and
 * descriptor prerequisites are documented in ovl11_state_field_access.h. */
s32 Ovl11ReadConditionField(s32 fieldId, s32 elementIndex);

#endif
