/*
 * Bit-indexed access to the shared state flag bank at byte offset 0x38.
 * Callers use numeric IDs; this module assigns no gameplay meaning to them.
 * Original functions: exe:8001AF44 and exe:8001AF70, in link order.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_flags.h"

s32 GetGameFlag(u32 flagId) {
    struct struct_8006C838_flags *flags;
    u32 wordIndex;

    flagId = flagId & 0xFFFF;
    wordIndex = flagId >> 5;
    flags = (struct struct_8006C838_flags *)&D_8006C838;
    return (flags->flags[wordIndex] >> (flagId & 0x1F)) & 1;
}

void SetGameFlag(u16 flagId, u16 enabled) {
    u32 *word;
    u32 wordIndex;
    u32 bitIndex;
    u32 mask;
    u32 byteOffset;
    char *bank;

    wordIndex = flagId >> 5;
    bitIndex = flagId & 0x1F;

    if (enabled) {
        bank = (char *)&D_8006C838;
        byteOffset = wordIndex << 2;
        bank += 0x38;
        word = (u32 *)(bank + byteOffset);
        mask = 1 << bitIndex;
        *word |= mask;
    } else {
        bank = (char *)&D_8006C838;
        byteOffset = wordIndex << 2;
        bank += 0x38;
        word = (u32 *)(bank + byteOffset);
        mask = 1 << bitIndex;
        *word &= ~mask;
    }
}
