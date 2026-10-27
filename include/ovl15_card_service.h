#ifndef OVL15_CARD_SERVICE_H
#define OVL15_CARD_SERVICE_H

#include "common.h"
#include "game_types.h"

/* Overlay-15 memory-card services. Not reentrant: all operations share the
 * outstanding SDK command, gOvl15CardRetryCount and Ovl15CardFilename.
 * port is 0 or 1 (SDK channel 0 or 0x10); other values return 12.
 * File operations use Ovl15BuildCardFilename's slot mapping: 0..3, otherwise 0.
 *
 * These are menu statuses, NOT the SDK's McErr values:
 *   0 completed/usable, 2 operation error, 3 pending, 4 no card, 5 new card,
 *   8 invalid card, 9 file absent, 11 insufficient blocks, 12 invalid request,
 *   14 summary checksum mismatch, 15 full card with no matching filename.
 * Each operation returns only its documented subset below. No routine here
 * serializes game state or validates a full save image's checksums.
 */

/* Accept and query card type, retrying synchronously. Narrows port to s16.
 * Returns 4/5/8/12 on the corresponding errors. Otherwise returns 0/1 after
 * Accept success, or 6/7 after Accept reports unformatted: the lower of each
 * pair is an exhausted Mcx invalid response, the higher Mcx success.
 * The menu deliberately distinguishes these four type/format states. */
s16 Ovl15ProbeCard(s32 port);

/* Start/advance an Exist command. Returns 0/3/4/5/8/12. The retry counter
 * increments once per invocation, resets on idle, and gates errors at 31. */
s32 Ovl15PollCardPresence(s16 port);

/* Check filename existence and occupied 8192-byte blocks (file sizes rounded
 * up individually). An existing filename passes without checking its size.
 * A new file needs 4 free blocks, or 10 for slot 3. Returns 0/4/8/11/12/15;
 * does not create, reserve or delete anything. */
s32 Ovl15CheckSaveCapacity(s16 port, s16 slot);

/* Read 128 bytes at file offset 0x200, copy the typed summary, then verify
 * byte 127 against XOR(bytes 0..126). On checksum failure the summary is
 * already copied. Returns 0/4/5/8/9/12/14. Unlisted SDK results also enter
 * the copy/checksum path, rather than being converted to a generic error. */
s32 Ovl15ReadSaveSummary(s16 port, s16 slot, Ovl15SaveSummary *summary);

/* Destructive whole-card format, with bounded retries. Returns 0/2/4/12. */
s32 Ovl15FormatCard(s16 port);

/* Create a four-block file, including for slot 3. Returns 0/2/4/11/12. */
s32 Ovl15CreateSaveFile(s16 port, s16 slot);

/* Start/poll a 0x8000-byte transfer at file offset zero. data must remain
 * valid until completion; write reads it, read fills it. Returns 0/2/3/4/5/12.
 * Call repeatedly while status is 3. Each call samples Sync once and resets
 * the retry count; its local retry edges reclassify that same sample, not a
 * new SDK result. An outstanding different command is not treated as this
 * transfer's completion (a completed different command starts a new transfer;
 * a busy different command returns 12). SDK start-call returns are ignored. */
s32 Ovl15WriteSaveFile(s16 port, s16 slot, u32 *data);
s32 Ovl15ReadSaveFile(s16 port, s16 slot, u32 *data);

#endif
