/* Complementary queries over the same 50-entry, 0x2C-stride volume format.
 * Both copy the two halfword-aligned bounding corners in the same way.
 * The point ABIs intentionally differ: word X/Z for interaction queries,
 * halfword XYZ for the flag-0x4000 lookup. See the public header and the
 * membership/boundary evidence in notes/file-groupings.md. */
#include "ovl11_interaction_volume_query.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} VolumeCorner;

void ovl_11_func_800F0FB0(s32 event);
void ovl_11_func_800BD358(void);

void *Ovl11ProbeInteractionVolume(u8 *volumes, Ovl11Func800BDCE8Arg1 *point,
                                s32 applyEffects) {
    u32 index;
    s32 offset;
    u8 *record;
    u8 *volume;
    VolumeCorner minimum;
    VolumeCorner maximum;
    u16 flags;
    u16 type;
    char *work;

    for (index = 0, work = (char *)&D_8006C838, volume = volumes + 0x10, record = volumes, offset = 0;
         index < 0x32; volume += 0x2C, record += 0x2C, index++, offset += 0x2C) {
        /* Preserve the original base/copy web: the foldable second spelling
         * computes the same offset but keeps the flag/type loads independent. */
        type = *(u16 *)(volumes + (offset * 2 - offset) + 0x12);
        flags = *(u16 *)(volumes + offset + 0x10);
        if (type != 0xFFFF && !(flags & 0x6008)) {
            minimum = *(VolumeCorner *)(record + 0x1E);
            maximum = *(VolumeCorner *)(record + 0x24);
            if (minimum.x < point->unk0 && point->unk0 < maximum.x &&
                minimum.z < point->unk8 && point->unk8 < maximum.z) {
                if (flags & 0x100) {
                    if (applyEffects != 0) {
                        ovl_11_func_800F0FB0(7);
                    }
                    continue;
                }
                if (flags & 0x80) {
                    if (applyEffects != 0) {
                        *(u16 *)(work + 0x51FE) |= 4;
                    }
                    continue;
                }
                if (type != 0xFFFE) {
                    ovl_11_func_800BD358();
                }
                return volume;
            }
        }
    }
    return 0;
}

void *Ovl11FindFlaggedVolume(u8 *volumes, s16 *point) {
    u32 index;
    s32 offset;
    u8 *record;
    u8 *volume;
    VolumeCorner minimum;
    VolumeCorner maximum;

    for (index = 0, volume = volumes + 0x10, record = volumes, offset = 0; index < 0x32;
         volume += 0x2C, record += 0x2C, index++, offset += 0x2C) {
        if (*(u16 *)(volumes + offset + 0x10) & 0x4000) {
            minimum = *(VolumeCorner *)(record + 0x1E);
            maximum = *(VolumeCorner *)(record + 0x24);
            if (minimum.x < point[0] && point[0] < maximum.x &&
                minimum.z < point[2] && point[2] < maximum.z &&
                minimum.y - 0x80 < point[1] && point[1] < maximum.y + 0x78) {
                return volume;
            }
        }
    }
    return 0;
}
