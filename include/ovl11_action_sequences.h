#ifndef OVL11_ACTION_SEQUENCES_H
#define OVL11_ACTION_SEQUENCES_H

#include "common.h"
#include "game_types.h"

/* Update one object action sequence. State is retained in object +28, with
 * action ID +B6, selection index +BA and completion state +DC. These calls
 * can move the object, select/consume records, update sprites, flags and sound.
 * They require initialized area/grid, record-pool and sprite state and are not
 * reentrant. Do not share one object's action fields between active sequences.
 *
 * Grid variants return 1 when no target can be acquired, otherwise 0 (including
 * ordinary sequence completion, which resets +28). gridMode is forwarded to
 * grid search; 1 selects its mode-1 path. The alternate promotes a pending grid
 * cell after its key probe instead of performing the full completion sequence.
 */
s32 Ovl11StepGridTargetAction(Ovl11SetFieldsView *object, s32 gridMode);
s32 Ovl11StepGridTargetActionAlternate(Ovl11SetFieldsView *object, s32 gridMode);

/* mode == 1 selects the save-counter-18/Z600 placement path, otherwise the
 * save-counter-20/X400 path. Returns 1 on resource/selection failure or final
 * release, 0 while processing; update again only while the caller owns it. */
s32 Ovl11StepRecordSelectionAction(Ovl11StateArg0 *object, s32 mode);

/* Tagged selection uses selector mode 6 and code160/tagged handler dispatch;
 * seasonal selection uses mode 8 and spawn/seasonal records. The second word
 * is retained in the original ABI but unread. Return 1 only for initial
 * selection failure, otherwise 0; normal completion resets object +28. */
s32 Ovl11StepTaggedRecordAction(Ovl11SetFieldsView *object, s32 unused);
s32 Ovl11StepSeasonalRecordAction(Ovl11SetFieldsView *object, s32 unused);

/* Lookup mode 0..5 and signed key in mutable 16-byte XYZ/key rows. Return 1
 * and copy three raw coordinate words on a match, else 0 with output unchanged.
 * The padded fourth vector word is untouched. Modes 0/1 also use the object's
 * X/Z and pool-expansion state. Each call refreshes shared mode-0/mode-2 rows;
 * this is not a read-only table lookup. */
s32 Ovl11LookupActionPosition(Ovl11StateArg0 *object, s16 mode,
                            s32 *position, s16 key);

/* Set the shared movement-rank threshold, step the existing movement service,
 * preserve incoming flag 0x800, and return 1 once flag 0x2000 is clear (also
 * zeroing the direction vector), else 0. threshold is a raw movement value,
 * NOT a timer or elapsed-time duration. */
s32 Ovl11AdvanceActionMovement(Ovl11E644Object *object, s16 threshold);

#endif
