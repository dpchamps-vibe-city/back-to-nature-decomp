/* Actor state/pause controls (ovl_19:800BAC40–800BAD50).
 * The two state writers serve different actor record formats through their
 * shared eight-byte prefix. The pause service changes only sprite flag 4;
 * it does not stop the scene's logic, sound or controller polling.
 * Interface: ovl19_actor_controls.h. Grouping: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl19_actor_controls.h"

void func_80015814(u16 *flags, s32 mask);
void func_80015828(u16 *flags, s32 mask);

void Ovl19SetActorControl(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter, s32 extra) {
    control->state = state;
    control->parameter = parameter;
    control->extra = extra;
}

void Ovl19SetActorState(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter) {
    control->state = state;
    control->parameter = parameter;
}

void Ovl19PauseActorSprites(void) {
    Ovl19SetActorSpritePauseMode(0);
}

void Ovl19ResumeActorSprites(void) {
    Ovl19SetActorSpritePauseMode(1);
}

void Ovl19SetActorSpritePauseMode(s32 resume) {
    u8 *sprite;
    s32 remaining;

    /* Install the operation before walking the groups and read it anew for
     * each sprite. The word view retains the original callback-slot access. */
    *(u32 *)&gOvl19ActorSpriteFlagOperation = (u32)(resume == 0 ? func_80015814 : func_80015828);

    sprite = gOvl19ActorSprites;
    for (remaining = 2; remaining >= 0; remaining--) {
        ((void (*)(u16 *, s32))*(u32 *)&gOvl19ActorSpriteFlagOperation)((u16 *)sprite, 4);
        sprite += 0x40;
    }
    sprite = gOvl19AuxActorSprites;
    for (remaining = 1; remaining >= 0; remaining--) {
        ((void (*)(u16 *, s32))*(u32 *)&gOvl19ActorSpriteFlagOperation)((u16 *)sprite, 4);
        sprite += 0x48;
    }
}
