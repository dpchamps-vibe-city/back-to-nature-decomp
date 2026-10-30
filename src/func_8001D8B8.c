#include "common.h"
#include "game_types.h"
#include <sys/types.h>
#include <libgpu.h>
#include <libgs.h>

/* All textured TMD variants share the CLUT and texture-page fields.
 * Offsets below are already in page/CLUT units, not pixel coordinates. */
#define RELOCATE_TMD_TEXTURE(primitive) do { \
    s32 texturePage; \
    texturePage = (primitive)->tpage; \
    texturePage += pageXOffset; \
    texturePage += pageYOffset * 16; \
    clut = (primitive)->clut; \
    clutRow = (clut >> 6) + clutYOffset; \
    clut = (clut & 0x3F) + clutXOffset; \
    packedCode = clutRow << 6 | clut; \
    (primitive)->tpage = texturePage; \
    (primitive)->clut = packedCode; \
} while (0)

/* Relocate the texture and palette references in an unmapped TMD model. */
void func_8001D8B8(TmdModelData **model, s32 pageXOffset, s32 pageYOffset,
                   s32 clutXOffset, s32 clutYOffset) {
    TmdModelData *data;
    struct TMD_STRUCT *object;
    u8 *objectBase;
    u8 *packet;
    s32 objectIndex;
    s32 primitiveIndex;
    s32 primitiveCount;
    s32 packedCode;
    u16 clut;
    s16 clutRow;

    pageXOffset /= 64;
    pageYOffset /= 256;
    clutXOffset /= 16;
    data = *model;
    objectBase = data->objects;
    objectIndex = 0;
    if (data->objectCount > 0) {
        do {
            object = (struct TMD_STRUCT *)&data->objects[
                objectIndex * sizeof(struct TMD_STRUCT)];
            packet = objectBase + (u32)object->primtop;
            primitiveCount = object->primn;
            for (primitiveIndex = 0; primitiveIndex < primitiveCount; primitiveIndex++) {
                /* Adjacent flag/mode bytes, ignoring semi-transparency. */
                packedCode = *(u16 *)(packet + 2) & 0xFDFF;
                switch (packedCode) {
                case 0x2400: /* textured flat triangle */
                    RELOCATE_TMD_TEXTURE((TMD_P_TF3 *)packet);
                    packet += sizeof(TMD_P_TF3);
                    break;
                case 0x2004: /* gradated flat triangle */
                case 0x3101: /* unlit Gouraud triangle */
                case 0x3800: /* Gouraud quad */
                    packet += 0x18;
                    break;
                case 0x2800: /* flat quad */
                case 0x3000: /* Gouraud triangle */
                    packet += 0x14;
                    break;
                case 0x2501: /* unlit textured flat triangle */
                case 0x3400: /* textured Gouraud triangle */
                    RELOCATE_TMD_TEXTURE((TMD_P_TG3 *)packet);
                    packet += sizeof(TMD_P_TG3);
                    break;
                case 0x3004: /* gradated Gouraud triangle */
                case 0x3901: /* unlit Gouraud quad */
                    packet += 0x1C;
                    break;
                case 0x3501: /* unlit textured Gouraud triangle */
                case 0x3C00: /* textured Gouraud quad */
                    RELOCATE_TMD_TEXTURE((TMD_P_TG4 *)packet);
                    packet += sizeof(TMD_P_TG4);
                    break;
                case 0x3804: /* gradated Gouraud quad */
                    packet += 0x24;
                    break;
                case 0x2C00: /* textured flat quad */
                case 0x2D01: /* unlit textured flat quad */
                    RELOCATE_TMD_TEXTURE((TMD_P_TF4 *)packet);
                    packet += sizeof(TMD_P_TF4);
                    break;
                case 0x2804: /* gradated flat quad */
                    packet += 0x20;
                    break;
                case 0x3D01: /* unlit textured Gouraud quad */
                    RELOCATE_TMD_TEXTURE((TMD_P_TNG4 *)packet);
                    packet += sizeof(TMD_P_TNG4);
                    break;
                case 0x4001: /* flat line */
                    packet += 0xC;
                    break;
                case 0x2000: /* flat triangle */
                case 0x2101: /* unlit flat triangle */
                case 0x2901: /* unlit flat quad */
                case 0x5001: /* Gouraud line */
                    packet += 0x10;
                    break;
                }
            }
        } while (++objectIndex < data->objectCount);
    }
}

#undef RELOCATE_TMD_TEXTURE
