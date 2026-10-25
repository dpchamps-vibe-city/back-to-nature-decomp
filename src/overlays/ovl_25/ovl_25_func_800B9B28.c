#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"


void ovl_25_func_800BA5BC (SpriteSourceData *arg0, u16 *arg1);
void ovl_25_func_800BA758 (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B9B28(Ovl25ObjectRecord *arg0) {
    SpriteSourceData *temp_s0;
    SpriteSourceData *temp_s0_2;
    s16 temp_v1;
    s32 var_a1;
    s32 var_s1;
    u16 *temp_s3;

    var_s1 = -1;
    temp_v1 = arg0->unk2;
    var_a1 = -1;
    switch (temp_v1) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (arg0->unk0) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            var_a1 = 4;
            break;
        case 1:                                     /* switch 2 */
        case 2:                                     /* switch 2 */
            var_a1 = 5;
            break;
        case 3:                                     /* switch 2 */
        case 4:                                     /* switch 2 */
        case 5:                                     /* switch 2 */
            var_a1 = 7;
            break;
        }
        break;
    case 1:                                         /* switch 1 */
    case 6:                                         /* switch 1 */
        var_a1 = 0xA;
        var_s1 = 0xB;
        break;
    case 2:                                         /* switch 1 */
    case 3:                                         /* switch 1 */
    case 4:                                         /* switch 1 */
        var_a1 = 0;
        var_s1 = 1;
        break;
    case 5:                                         /* switch 1 */
        var_a1 = 2;
        var_s1 = 3;
        break;
    case 7:                                         /* switch 1 */
        var_a1 = 6;
        if (arg0->unk0 != 0) {
            var_a1 = 0;
            var_s1 = 1;
        } else {
            var_s1 = 0xC;
        }
        break;
    case 8:                                         /* switch 1 */
        var_a1 = 8;
        if (*(s16 *)((u8 *)arg0 + 4) != 0) {
            var_a1 = 0xA;
            var_s1 = 0xB;
        } else {
            var_s1 = 9;
        }
        break;
    }
    temp_s0 = &arg0->record0;
    if (arg0->record0.field_4 != var_a1) {
        SelectSpriteAnimation((ObjectState *) temp_s0, var_a1 & 0xFF);
    }
    temp_s3 = arg0->source;
    ovl_25_func_800BA5BC(temp_s0, temp_s3);
    if (var_s1 != -1) {
        temp_s0_2 = &arg0->record1;
        if (arg0->record1.field_4 != var_s1) {
            SelectSpriteAnimation((ObjectState *) temp_s0_2, var_s1 & 0xFF);
        }
        ovl_25_func_800BA758(temp_s0_2, temp_s3);
    }
}
