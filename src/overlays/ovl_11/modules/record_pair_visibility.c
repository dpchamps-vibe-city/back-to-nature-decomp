/*
 * Select a pair of scene records and alternate which one is drawn.
 * Scene setup installs the pair; the scene update dispatcher applies flag
 * 0x72 each update. Bit 0x1000 is the renderer's hide bit, not an allocation
 * or destruction flag. Membership/boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_scene_record_visibility.h"
#include "game_flags.h"

void Ovl11InitSceneRecordVisibilityPair(void) {
    Ovl11SceneRecordFlagsView *record;
    s32 index;
    u16 slot;

    index = 0;
    record = (Ovl11SceneRecordFlagsView *)gOvl11SceneRecords;
    gOvl11SceneRecordVisibilityPair[0] = 0;
    gOvl11SceneRecordVisibilityPair[1] = 0;
    for (; index < 15; index++, record++) {
        slot = record->flags;
        if (slot & 0x10) {
            slot &= 1;
            gOvl11SceneRecordVisibilityPair[slot] = (u32)record;
            if (slot) {
                break;
            }
        }
    }
    ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[0])->flags &= 0xEFFF;
    ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[1])->flags |= 0x1000;
}

void Ovl11UpdateSceneRecordVisibilityPair(void) {
    if (gOvl11SceneRecordVisibilityPair[0] != 0) {
        if (GetGameFlag(0x72) == 1) {
            ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[1])->flags &= 0xEFFF;
            ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[0])->flags |= 0x1000;
        } else {
            ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[0])->flags &= 0xEFFF;
            ((Ovl11SceneRecordFlagsView *)gOvl11SceneRecordVisibilityPair[1])->flags |= 0x1000;
        }
    }
}
