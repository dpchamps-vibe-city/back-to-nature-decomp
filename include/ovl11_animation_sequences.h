#ifndef OVL11_ANIMATION_SEQUENCES_H
#define OVL11_ANIMATION_SEQUENCES_H

#include "common.h"
#include "game_types.h"

/* Per-update callbacks in the object handler table. Names list animation IDs,
 * not gameplay actions. Cursor +28 starts at 0; callbacks initialize it to 1,
 * step a private {s16 animation, s16 hold-count} playlist, and return 1 only
 * on a later call in the terminal state. Playback is gated by the frame-step
 * helper's selector/+7A flags. Objects and counters are caller-owned.
 * Most callbacks set lock 0x800 on entry, clear it on completion and arm the
 * +2C auto-advance timer at 300. The 3/8 callbacks do not set the lock; 052
 * does not arm that timer; 50 uses terminal state 4 rather than -1. */
s32 Ovl11RunAnimation0Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation052Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation50Sequence(Ovl11D04D4Obj *object);
/* The 41 sequence reuses +2C as its hold counter, submits mode 0x8031 with
 * argument 0x12 while active, and also clears flag 0x4000 on completion. */
s32 Ovl11RunAnimation41Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation70Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation60Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation3Sequence(Ovl11D04D4Obj *object);
s32 Ovl11RunAnimation8Sequence(Ovl11D04D4Obj *object);

#endif
