/*
 * Horse-race entrant and odds setup, exe:80022014..8002238C.
 *
 * Three races have six entrants each, selected without replacement from
 * 40 horse IDs. The ovl_17 consumer displays their names beside "Odds" and
 * "Tickets" under "Race ... bets"; mode 3 substitutes the player's horse.
 * Private tables supply two attributes per horse and parameter ranges per
 * entrant. The two race parameters retain neutral names, not screen x/y.
 * Membership, boundaries and text evidence are in notes/file-groupings.md.
 */
#include "common.h"
#include "horse_race_setup.h"
#include "psyq/memory.h"
#include "random.h"

s32 HorseRaceContainsEntrant(s32 tableAddress, s16 horseId) {
    u32 race;
    u32 entrant;

    race = 0;
    tableAddress += 2;
    for (race = 0; race < 3; race++) {
        for (entrant = 0; entrant < 6; entrant++) {
            if (*(s16 *)((char *)tableAddress + race * 84 + entrant * 14) == horseId) {
                return 1;
            }
        }
    }
    return 0;
}

void InitHorseRaceEntries(void) {
    HorseRaceState *state = (HorseRaceState *)&gHorseRaceState;
    s16 *parameter0Cursor;
    s16 *oddsCursor;
    u32 race;
    u32 entrant;
    s16 horseId;
    s32 savedMode;
    s32 maxParameter0;
    s32 playerParameter1;

    savedMode = state->mode;
    memset(state, 0, 0x108);
    for (race = 0; race < 3U; race++) {
        s16 *horseIdCursor;
        entrant = 0;
        horseIdCursor = &gHorseRaceFirstHorseId;
        horseIdCursor = (s16 *)((u8 *)horseIdCursor + race * 84);
        do {
            *horseIdCursor = -1;
            horseIdCursor += 7;
            entrant++;
        } while (entrant < 6U);
    }
    /* Keep the field-anchor views: each expression addresses the same
     * stride-14 record but preserves the original symbolic base formation. */
    for (race = 0; race < 3U; race++) {
        for (entrant = 0; entrant < 6U; entrant++) {
            do {
                horseId = Rand(40);
            } while (HorseRaceContainsEntrant((s32)state, horseId));
            *(s16 *)((u8 *)&gHorseRaceFirstHorseId + race * 84 + entrant * 14) = horseId;
            *(u16 *)((u8 *)&gHorseRaceFirstAttribute0 + race * 84 + entrant * 14) =
                gHorseRaceHorseAttributes[horseId * 2];
            *(u16 *)((u8 *)&gHorseRaceFirstAttribute1 + race * 84 + entrant * 14) =
                gHorseRaceHorseAttributes[(horseId * 2) | 1];
            *(s16 *)((u8 *)&gHorseRaceFirstParameter0 + race * 84 + entrant * 14) =
                gHorseRaceEntrantParameterRanges[entrant * 4] +
                Rand((u16)(gHorseRaceEntrantParameterRanges[entrant * 4 + 1] -
                           gHorseRaceEntrantParameterRanges[entrant * 4]));
            *(s16 *)((u8 *)&gHorseRaceFirstParameter1 + race * 84 + entrant * 14) =
                gHorseRaceEntrantParameterRanges[entrant * 4 + 2] +
                Rand((u16)(gHorseRaceEntrantParameterRanges[entrant * 4 + 3] -
                           gHorseRaceEntrantParameterRanges[entrant * 4 + 2]));
        }
    }
    if (savedMode == 3) {
        Ovl17ModeView *gameState = (Ovl17ModeView *)D_8006C838;
        u32 playerParameter0 = gameState->unk9348;
        HorseRaceState *playerState = state;
        playerParameter1 = gameState->unk92B2;
        playerState->entries[2][2].horseId = -1;
        playerState->entries[2][2].parameter0 = playerParameter0;
        playerState->entries[2][2].parameter1 = playerParameter1;
        playerState->entries[2][2].attr[0] = 0;
        playerState->entries[2][2].attr[1] = 0;
    }
    race = 0;
    do {
        maxParameter0 = 0;
        for (entrant = 0; entrant < 6U; entrant++) {
            if ((u32)maxParameter0 < (u32)state->entries[race][entrant].parameter0) {
                maxParameter0 = state->entries[race][entrant].parameter0;
            }
        }
        entrant = 0;
        oddsCursor = (s16 *)((char *)&gHorseRaceFirstOdds + race * 84);
        parameter0Cursor = (s16 *)((u32)&gHorseRaceFirstParameter0 + race * 84);
        do {
            *oddsCursor = (s16)(Rand(4) +
                ((u32)((maxParameter0 - *parameter0Cursor) * (maxParameter0 - *parameter0Cursor)) / 1000 + 1));
            parameter0Cursor += 7;
            entrant++;
            oddsCursor += 7;
        } while (entrant < 6U);
        race++;
    } while (race < 3U);
}
