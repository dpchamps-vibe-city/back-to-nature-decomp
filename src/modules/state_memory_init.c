/*
 * Initial state-memory layout, original exe:0x80021DA8-0x80021FD0.
 * Startup clears both state buffers before binding static data and partitioning
 * the initial record pool. Scene setup can rebind just the static tables.
 * The following state accessors do not participate in this layout protocol.
 */
#include "common.h"
#include "state_memory_init.h"
#include "psyq/memory.h"
#include "asset_scratch.h"

void InitStateMemory(void) {
    u32 remainder;
    s32 roundedBytes;
    s32 bufferAddress;

    memset(&D_8006C838, 0, 0xE7B8);
    memset(&D_8007AFF0, 0, 0x256E4);
    BindStateDataTables(0);
    remainder = D_8001009C - D_80010098;
    roundedBytes = remainder >> 11;
    remainder &= 0x7FF;
    roundedBytes <<= 11;
    if (remainder) {
        roundedBytes += 0x800;
    }
    bufferAddress = D_80010098 + roundedBytes;
    ((s32 *)&D_8007AFF0)[1] = bufferAddress;
    D_8007AFF0 = bufferAddress + 0x49000;
    *(s32 *)((char *)D_8006C838 + 0x1C) = GetAssetScratchEnd() - 0x1008;
}

/* Each pool begins after the preceding pools' counts times 0x18 bytes.
 * The explicit pointer/count stores retain the original partition layout;
 * +0x8000 is an address anchor, not a separate allocation. */
void BindStateDataTables(s32 preservePools) {
    s32 *state;
    s32 *upperState;
    s32 *records;

    state = (s32 *)&D_8006C838;

    state[0x20 >> 2] = (s32)&D_8004B1A4;
    state[0x24 >> 2] = (s32)&D_80049B1C;
    state[0x28 >> 2] = (s32)&D_8004AFBC;
    state[0x2C >> 2] = (s32)&D_8004B044;
    state[0x34 >> 2] = (s32)&D_80054BC8;

    if (preservePools == 0) {
        upperState = state + (0x8000 >> 2);
        records = (s32 *)&gInitialPoolRecordData;

        upperState[0x5D8C >> 2] = (s32)records;
        upperState[0x5D90 >> 2] = (s32)((char *)records + 0x60);
        upperState[0x5D94 >> 2] = (s32)((char *)records + 0x150);
        upperState[0x5D98 >> 2] = (s32)((char *)records + 0x360);
        upperState[0x5D9C >> 2] = (s32)((char *)records + 0x480);
        upperState[0x5DA0 >> 2] = (s32)((char *)records + 0x5A0);
        upperState[0x5DA4 >> 2] = (s32)((char *)records + 0x6C0);
        upperState[0x5DA8 >> 2] = (s32)((char *)records + 0x840);
        upperState[0x5DAC >> 2] = (s32)((char *)records + 0x960);
        upperState[0x5DB0 >> 2] = (s32)((char *)records + 0xA08);
        upperState[0x5DB4 >> 2] = (s32)((char *)records + 0xA80);
        upperState[0x5DB8 >> 2] = (s32)((char *)records + 0xBB8);
        upperState[0x5DBC >> 2] = (s32)((char *)records + 0xCF0);
        upperState[0x5DC0 >> 2] = (s32)((char *)records + 0xDF8);
        upperState[0x5DC4 >> 2] = (s32)((char *)records + 0xED0);
        upperState[0x5DC8 >> 2] = (s32)((char *)records + 0x1038);
        upperState[0x5DCC >> 2] = (s32)((char *)records + 0x1068);
        upperState[0x5DD0 >> 2] = (s32)((char *)records + 0x1158);
        upperState[0x5DD4 >> 2] = (s32)((char *)records + 0x1248);

        *(u16 *)((u8 *)upperState + 0x5DD8) = 4;
        *(u16 *)((u8 *)upperState + 0x5DDA) = 0xA;
        *(u16 *)((u8 *)upperState + 0x5DDC) = 0x16;
        *(u16 *)((u8 *)upperState + 0x5DDE) = 0xC;
        *(u16 *)((u8 *)upperState + 0x5DE0) = 0xC;
        *(u16 *)((u8 *)upperState + 0x5DE2) = 0xC;
        *(u16 *)((u8 *)upperState + 0x5DE4) = 0x10;
        *(u16 *)((u8 *)upperState + 0x5DE6) = 0xC;
        *(u16 *)((u8 *)upperState + 0x5DE8) = 7;
        *(u16 *)((u8 *)upperState + 0x5DEA) = 5;
        *(u16 *)((u8 *)upperState + 0x5DEC) = 0xD;
        *(u16 *)((u8 *)upperState + 0x5DEE) = 0xD;
        *(u16 *)((u8 *)upperState + 0x5DF0) = 0xB;
        *(u16 *)((u8 *)upperState + 0x5DF2) = 9;
        *(u16 *)((u8 *)upperState + 0x5DF4) = 0xF;
        *(u16 *)((u8 *)upperState + 0x5DF6) = 2;
        *(u16 *)((u8 *)upperState + 0x5DF8) = 0xA;
        *(u16 *)((u8 *)upperState + 0x5DFA) = 0xA;
        *(u16 *)((u8 *)upperState + 0x5DFC) = 2;
    }
}
