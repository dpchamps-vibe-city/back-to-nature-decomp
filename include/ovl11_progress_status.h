#ifndef OVL11_PROGRESS_STATUS_H
#define OVL11_PROGRESS_STATUS_H

#include "common.h"

/* Refresh the low 23 bits of gProgressDerivedFlags from shared progress,
 * counter and record state; preserve bits 23..31. Entry is the original
 * trampoline, with identical effects. Neither advances timers or requests.
 *
 * Bits 0/1: counters 0x5D/0x5F >=51; bits 2..4: all four counters in each
 * private ID group >=101. Bits 5..7: availability gates (state flag 0x800,
 * gate halfword and cooldown); bit 8: any of 37 record halfwords >50000,
 * unless state flag 0x200000. Bits 9..13 select the first incomplete stage;
 * bits 15..17 derive stage acknowledgement gates. Bit 14 and bits 18..22
 * are inverted results of the existing record/event queries. These names
 * do not assign speculative gameplay meanings to the stage/event IDs.
 * The refreshers retain external linkage but are internal to this protocol. */
void Ovl11RefreshProgressStatus(void);
void Ovl11RefreshProgressStatusEntry(void);

/* Refresh FIRST, then zero activeTimer, request and pendingTimer. Return the
 * shared work base as s32*, not newly allocated storage. Cooldown, stages,
 * state flags and game flags 0x37/0x38 are not cleared by this operation. */
s32 *Ovl11ResetProgressControl(void);

/* Store the signed request and set game flag 0x37. Active timer is 4 for
 * requests 0x13F..0x141, 2 for 0xF7..0xF9 or 1..25, otherwise 0.
 * No validation or derived-status refresh is performed. */
void Ovl11RequestProgressAction(s16 request);

/* Store 3 to pendingTimer only; this does NOT set game flag 0x38. */
void Ovl11ArmProgressPendingTimer(void);

/* One update-call tick, not elapsed-time measurement. Decrement/clamp the
 * active timer and clear flag 0x37 at expiry. With flag 0x38 set, decrement
 * pendingTimer; below zero, reset record state via existing services, advance
 * the first unrecorded stage (flags 0x3A..0x3E), and clear 0x38. Nonzero
 * cooldown also decrements/clamps. This does not refresh the derived word. */
void Ovl11UpdateProgressTimers(void);

/* operation 0: request !=0; 1: raw request as u16; 2: consume request.
 * On consume, 0x13F sets stage2=2/state flag 0x100000; 0x140 sets stage1=2/
 * flag 0x100; 0x141 sets stage1=3/flag 0x200. Those three refresh and return
 * zero. Other requests return their old u16 value without refreshing. Every
 * consume clears the request. Unsupported operations return zero unchanged.
 * All entrypoints share work memory and are not reentrant with its writers. */
u16 Ovl11HandleProgressRequest(s32 operation);

#endif
