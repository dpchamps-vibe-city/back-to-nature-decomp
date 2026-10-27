#ifndef OVL11_STAGE_MODE2_RECORD_H
#define OVL11_STAGE_MODE2_RECORD_H

#include "common.h"
#include "game_types.h"

/* Single stage-mode-2 slot: F0 bytes beginning at gStageMode2Record.
 * Only IDs 15E/15F can be acquired; zero leading ID means free. This is not
 * the general object pool. Records embed a sprite source at +78, a timed
 * effect at +A8 and raw X/Y/Z position/increment words at +38/+48.
 * Dispatch uses twenty unchecked entry/step slots. State/effect timers count
 * update calls, not milliseconds. Shared sprite/work state is non-reentrant.
 * Signatures retain raw-address words where existing callers use that ABI.
 */

/* 8010B1C4: initialize an empty slot, returning its ID anchor; NULL on an
 * occupied slot or unsupported ID. Initialization clears F0 bytes, stamps
 * eight bytes from work +44B8 and installs initial position/state constants.
 * Does NOT load or initialize the sprite source. */
u16 *Ovl11AcquireStageMode2Record(s32 id);

/* 8010B64C: clear F0 writable bytes at recordAddress, making the slot free. */
void Ovl11ClearStageMode2Record(s32 recordAddress);

/* 8010B67C: refresh the singleton's source according to far-work mode 1/3
 * and ID 15E/15F; other modes clear its header word. May load sprite data.
 * Marks flag 08000000; unknown IDs clear that flag and the source header. */
void Ovl11RefreshStageMode2SpriteSource(void);

/* 8010B264: update the unsigned +16/+AC counters with signed-halfword
 * saturation tests, consume flags 26, reset the timed effect, and possibly
 * promote ID 15E to 15F from the stored/work stamp comparison. Returns -1
 * for zero ID or flag 400, otherwise 0. Does not run the state dispatcher. */
s32 Ovl11UpdateStageMode2Counters(Ovl11StageMode2RecordView *record);

/* 8010B4D8: run the selected nullable step callback. Unless flag 800 locks
 * selection, timer +2C >= 300 triggers state choice and resets that timer.
 * Then increment the timer. Returns -1 for zero ID, otherwise 0. */
s32 Ovl11UpdateStageMode2State(u16 *record);

/* 8010B57C: flag 01000000 forces state 17; flag 400 rejects with -1.
 * Call the requested nullable entry gate; if it does not return -1 and the
 * step slot exists, install state, clear +28/+2A/+2C and the +48 increment.
 * Return entry result (0 if absent), even when no new state was installed.
 * A callback may itself change state. state must index the twenty-slot bank.
 */
s32 Ovl11ChangeStageMode2State(u16 *record, s32 state);

/* 8010B198: add the +48 increment to the +38 position, then clear it. */
void Ovl11AccumulateStageMode2Direction(s32 recordAddress);

/* 8010B0E8: use mode +24/direction +22 to select the private animation pair,
 * then draw. Pair frame zero leaves the selected frame alone. Requires mode
 * 0..4, direction 0..3, initialized sprite/packet state. Skip when global
 * flag 800000 is set or source header +8C is zero. */
void Ovl11DrawStageMode2Record(u8 *record);

/* 8010C1FC: select the singleton's animation from a primary-object view;
 * updates its direction and can assign a frame directly from that direction. */
void Ovl11SelectStageMode2AnimationFromObject(Ovl11C7010Object *object);

/* Drawing adapters used by primary_object_draw as well as this lifecycle.
 * 8010C1C0 copies +100/+104/+108/+10C to work +92D4..+92E0 (fourth word
 * intentionally opaque). 8010C2EC draws and RESETS, not ticks, the +A8 effect;
 * global flag 800000 suppresses both operations. */
void Ovl11CopyObjectEffectPositionWords(Ovl11EffectPositionWords *object);
void Ovl11DrawObjectAndResetTimedEffect(u8 *record);

#endif
