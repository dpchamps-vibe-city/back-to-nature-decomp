/* Read-only interaction-ID query, ovl_11:80115FC8–80115FE0.
 * No arguments; returns exactly 0 for ID 20, otherwise 1. It changes no state.
 * The meaning of ID 20 and this entry's retail caller are not established.
 * Neither neighboring record handler calls this query or shares a private
 * helper/table with it. See notes/file-groupings.md for these evidence limits.
 * Retain external linkage; absence of a known caller does not prove static. */

#include "common.h"

s32 Ovl11InteractionIdIsNot20(void) {
    return gOvl11InteractionId != 0x14;
}
