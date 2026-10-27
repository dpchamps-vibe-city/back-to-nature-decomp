#ifndef OVL11_SCENE_SOUND_COMMANDS_H
#define OVL11_SCENE_SOUND_COMMANDS_H

#include "common.h"

/* Request soundBank via gObjectSoundBankRequest until far-work+2549C agrees.
 * beginFade's low halfword starts a decreasing screen fade before the check.
 * Once ready, sequenceMode 1 starts the sequence, -1 stops it immediately;
 * other modes leave it alone. Return 0 while requesting, 1 when ready.
 * The fourth object-command operand is ignored (the dispatcher and direct
 * scene caller supply it). No sound-bank loading is performed here.
 */
s32 Ovl11RequestSceneSoundBank(s16 soundBank, s32 beginFade,
                              s32 sequenceMode, s32 reserved);

/* Open/start sequences and play slot 0 using the current signed sequence id.
 * volume 0 selects 127 when header flag 1000 is clear or sequence is 5..9/E,
 * otherwise 63. A nonzero volume is passed through. Always return 1.
 * Trailing command operands are ignored, not removed from the caller ABI.
 */
s32 Ovl11StartSceneSoundSequence(s16 volume, s32 reserved1,
                                s32 reserved2, s32 reserved3);

/* mode 0 begins a master-volume fade-out (ticks 0 means 10), returning 0.
 * Other modes stop sequence slot 0 and return 1 once the volume ramp is idle;
 * mode 2 stops immediately. Does not service the volume ramp itself.
 */
s32 Ovl11StopSceneSoundSequence(s16 ticks, s16 mode,
                               s32 reserved2, s32 reserved3);

/* begin != 0 starts a fade and returns 0; otherwise return the fade-complete
 * predicate. ticks are update counts. Odd mode decreases, even increases;
 * modes 2/3 select the scene OT (others use the frame OT), and 2 also clears
 * scene command flags. Nonzero additiveBlend selects GPU blend mode 1 (add)
 * instead of 2 (subtract); it does not change the step count. Scene code must
 * still service the screen fade. Unknown modes retain parity-based selection.
 */
s32 Ovl11ScreenFadeCommand(s16 mode, s16 ticks, s32 begin, s32 additiveBlend);

#endif
