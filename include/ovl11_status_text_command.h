#ifndef OVL11_STATUS_TEXT_COMMAND_H
#define OVL11_STATUS_TEXT_COMMAND_H

#include "common.h"

/* Start/poll shared status-text input. target 0/4 always requires input and
 * copies the 18-byte status scratch to 80075AD8/8007AFBC on completion.
 * Targets 1..3 start cancelable input when requireInput == 0, required input
 * otherwise; they do not copy scratch to either status destination here.
 * Other target values start nothing but still process an active text entry.
 *
 * resultSlot == -1 suppresses output; otherwise its unchecked value slot
 * receives 1 on cancellation, 0 on confirmation. Returns 1 for either of
 * those terminal results, 0 while starting/pending (and other statuses).
 * Scratch is not preloaded from a destination. Shares the non-reentrant
 * text-entry service and gOvl11StatusTextScratch with record-pair editing. */
s32 Ovl11StatusTextCommand(s16 target, s16 requireInput, s32 resultSlot);

#endif
