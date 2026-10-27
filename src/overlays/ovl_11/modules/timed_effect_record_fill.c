/* Fill the shared effect record through its original interior XYZ anchor.
 * The passed record controls only which +A8 timed effect is reset. */
#include "common.h"
#include "game_types.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_timed_effect_record_fill.h"

void Ovl11FillTimedEffectRecord(u8 *effectRecord, Ovl11EffectRecordPair *pair,
                              Vec3 *position) {
    ((s32 *)gOvl11TimedEffectPositionWords)[0] = position->x;
    ((s32 *)gOvl11TimedEffectPositionWords)[1] = position->y;
    ((s32 *)gOvl11TimedEffectPositionWords)[2] = position->z;
    gOvl11TimedEffectPositionWords[-4] = pair->second;
    gOvl11TimedEffectPositionWords[-5] = pair->first;
    Ovl11ResetTimedSpriteEffect((s16 *)(effectRecord + 0xA8));
}
