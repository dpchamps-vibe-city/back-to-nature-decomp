#ifndef OVL11_INTERACTION_RECORD_PREPARE_H
#define OVL11_INTERACTION_RECORD_PREPARE_H
#include "common.h"
#include "game_types.h"

/* Prepare the full record behind this six-byte prefix view. Requires storage
 * through +AB. Return 1 without mutation when its leading ID fails item flag
 * 8100. Flag 100000 takes the separate selection/request path, conditionally
 * clears that flag, and returns 2 even if the request was rejected. Otherwise
 * set 20000, clear 2000, zero halfwords +26/+28/+2A/+2C, reset direction at
 * +48 and timed effect at +A8, and return 0. The +0 ID read is deliberate:
 * the prefix's +2 field is NOT the ID used here. Shared selection effects
 * make this service non-reentrant. */
s32 Ovl11PrepareInteractionRecord(Ovl11InteractionRecord *record);
#endif
