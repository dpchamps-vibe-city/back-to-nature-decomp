#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800DA3AC(Ovl11D124Entry *arg0);
s32 ovl_11_func_800D5868(s16 arg0);
void ovl_11_func_800DA390(Ovl11GridResetEntry *arg0);
void ovl_11_func_800DAF24(u16 *arg0);
void ovl_11_func_800DA638(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

void ovl_11_func_800D7B8C(void) {
    u16 rows;
    u16 columns;
    Ovl11D124Entry *entry;
    s32 column;
    s32 isCrop;
    s32 layer;
    s32 row;
    s32 value;
    u16 tag;
    u16 nextTag;
    u16 flags;
    u16 *growth;
    Ovl11GrowthState *state;
    Ovl11TileItem *item;
    s32 work;
    s32 itemOffset;
    u16 *limit;

    for (layer = 0; layer < 2; layer++) {
        if (layer == 0) {
            rows = 25;
            columns = 45;
        } else {
            rows = 7;
            columns = 7;
            if (((Ovl11GrowthGlobals *) D_8006C838)->enabled == 0) {
                continue;
            }
        }
        {
            for (row = 0; row < rows; row++) {
                for (column = 0; column < columns; column++) {
                    entry = ovl_11_func_800DAF60(layer, column, (s16) row);
                    if (entry != NULL) {
                        itemOffset = (u16) entry->unk0 * sizeof(*item);
                        item = ((Ovl11GrowthGlobals *) D_8006C838)->items;
                        item = (Ovl11TileItem *) ((u8 *) item + itemOffset);
                        work = item->group;
                        work *= sizeof(*((Ovl11GrowthGlobals *) D_8006C838)->groups);
                        growth = (u16 *) ((u8 *) ((Ovl11GrowthGlobals *) D_8006C838)->groups + work);
                        state = (Ovl11GrowthState *) &entry->unk4;
                        if (layer == 0) {
                            ovl_11_func_800DA3AC(entry);
                        }
                        value = entry->unk0;
                        isCrop = ovl_11_func_800D5868(value);
                        if (layer == 0) {
                            if (((Ovl11GrowthGlobals *) D_8006C838)->season == 3) {
                                tag = (u16) entry->unk0;
                                if (tag == 0x40) {
                                    ovl_11_func_800DA390((Ovl11GridResetEntry *) entry);
                                } else if ((tag == 0x36) && (entry->unk4 != 0)) {
                                    entry->unk4 = 1;
                                    state->days = 0;
                                    state->flags = 0;
                                }
                            }
                            if (isCrop != 0) {
                                value = growth[0];
                                if (value != ((Ovl11GrowthGlobals *) D_8006C838)->season) {
                                    if (((Ovl11GrowthGlobals *) D_8006C838)->season == 3) {
                                        if ((u16) entry->unk0 != 0x36) {
                                            entry->unk0 = 0x168;
                                            entry->unk2 = 0x168;
                                            state->stage = 0;
                                            state->days = 0;
                                            state->flags = 0;
                                        }
                                        goto advanceGrowth;
                                    }
                                    if (value != 4) {
                                        if ((state->stage != 0) && (value == (((Ovl11GrowthGlobals *) D_8006C838)->season - 1))) {
                                            state->stage = 1;
                                        } else {
                                            entry->unk0 = 0x168;
                                            entry->unk2 = 0x168;
                                            state->stage = 0;
                                            state->flags = 0;
                                        }
                                        state->days = 0;
                                        goto normalizeEntry;
                                    }
                                    goto advanceGrowth;
                                }
                                goto advanceGrowth;
                            }
                            goto normalizeEntry;
                        }
                        if (isCrop == 0) {
normalizeEntry:
                            ovl_11_func_800DAF24((u16 *) entry);
                        } else {
advanceGrowth:
                            nextTag = (u16) entry->unk2;
                            if ((nextTag == 0x169) || (nextTag == 0x16B) || ((u16) entry->unk0 == 0x36)) {
                                ovl_11_func_800DAF24((u16 *) entry);
                                if (state->stage != growth[1]) {
                                    flags = (u16) state->flags;
                                    if (flags & 0x8000) {
                                        state->flags = flags & 0x7FFF;
                                    } else {
                                        state->days += 1;
                                    }
                                    limit = growth;
                                    limit += state->stage;
                                    work = limit[3];
                                    if ((u32) work <= (u8) state->days) {
                                        entry->unk2 = 0x168;
                                        state->days = 0;
                                        state->stage += 1;
                                    }
                                    if ((u16) entry->unk0 == 0x36) {
                                        ovl_11_func_800DA638((s16) row, (s16) column, (s16) layer, 0, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
