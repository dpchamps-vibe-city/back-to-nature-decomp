#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800E0B40(Recon_ovl_11_func_8010CE80_A0View *arg0);
void ovl_11_func_800E0450(s32 arg0);
s32 ovl_11_func_80107DEC(Ov11Timer *arg0);
void ovl_11_func_800E03A0(u8 *arg0);
s32 ovl_11_func_800DEF30(UnkStruct800DF4F0 *arg0);
void ovl_11_func_800DE878(s32 arg0);
void ovl_11_func_80107DD0(s16 *arg0);
void ovl_11_func_800DE7C8(u8 *arg0);
s32 ovl_11_func_800E276C(s16 *arg0);
void ovl_11_func_800E1F70(s32 arg0);
void ovl_11_func_800E1EC0(u8 *arg0);

void ovl_11_func_800D1EB8(void) {
    Recon_ovl_11_func_8010CE80_A0View *entry;
    s32 *entryB4;
    s32 flags;
    s32 update_mask;
    s32 i;
    u8 *base;
    u8 *far_base;
    u8 *base2;
    u8 *far_base2;

    for (i = 0; i < 20; i++) {
        entry = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)&gTaggedRecordBase + i * 0xB8);
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if (*(u16 *)entry != 0 && (flags = entry->unk34, !(flags & 0x400))) {
            if (flags & 0x10000) {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            ovl_11_func_800E0B40(entry);
                            ovl_11_func_800E0450((s32)entry);
                        }
                    } else {
                        ovl_11_func_800E0B40(entry);
                        ovl_11_func_800E0450((s32)entry);
                    }
                    ovl_11_func_80107DEC((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                    ovl_11_func_800E03A0((u8 *)entry);
                } else {
                    ovl_11_func_80107DD0((s16 *)((u8 *)entry + 0xA8));
                }
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            ovl_11_func_800DEF30((UnkStruct800DF4F0 *)entry);
                            ovl_11_func_800DE878((s32)entry);
                        }
                    } else {
                        ovl_11_func_800DEF30((UnkStruct800DF4F0 *)entry);
                        ovl_11_func_800DE878((s32)entry);
                    }
                    ovl_11_func_80107DEC((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (!(entry->unk34 & 0x20000)) {
                    if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                        ovl_11_func_80107DD0((s16 *)((u8 *)entry + 0xA8));
                    } else {
                        ovl_11_func_800DE7C8((u8 *)entry);
                    }
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        entryB4 = (s32 *)((u8 *)&D_800742EC + i * 0xB4);
        far_base2 = D_8009AFF0;
        base2 = (u8 *)(&D_800742EC - 0x1EAD);
        if (*(u16 *)entryB4 != 0) {
            ovl_11_func_800E276C((s16 *)entryB4);
            update_mask = 0x20000;
            if (*(s16 *)(far_base2 + 0x5476) != *(s16 *)((u8 *)entryB4 + 0x30)) {
                ovl_11_func_80107DD0((s16 *)((u8 *)&D_800742EC + i * 0xB4 + 0xA8));
            } else {
                if (!(update_mask & *(s32 *)((u8 *)entryB4 + 0x34))) {
                    if (!(*(s32 *)(base2 + 0xC) & 0x08000000)) {
                        ovl_11_func_800E1F70((s32)entryB4);
                    }
                    if (!(*(s32 *)((u8 *)entryB4 + 0x34) & 0x02000000)) {
                        ovl_11_func_800E1EC0((u8 *)entryB4);
                        ovl_11_func_80107DEC((Ov11Timer *)((u8 *)&D_800742EC + i * 0xB4 + 0xA8));
                    }
                }
            }
        }
    }
}
