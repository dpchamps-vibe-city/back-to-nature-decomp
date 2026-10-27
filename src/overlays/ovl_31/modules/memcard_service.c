/*
 * Memory-card utility overlay (obj\gf_mcard.bin).
 *
 * The scene callback draws a debug-font menu and latches operation results
 * until any new button press acknowledges them. Operations use slot/channel
 * zero; Start requests scene zero. Directory sizes are reported in 8192-byte
 * blocks, rounding each file separately. Renaming attempts all four legacy
 * save names even when a preceding rename failed.
 *
 * Driver, five helpers and their interleaved literal/jump-table pool form one
 * translation unit. Membership and container boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/kernel.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libmcrd.h"
#include "psyq/stdio.h"
#include "psyq/libmcx.h"
#include "psyq/libapi.h"
#include "scene_selection.h"
#include "ovl31_memcard_utility.h"

s32 Ovl31FormatMemoryCard(void);
s32 Ovl31UnformatMemoryCard(void);
s32 Ovl31ReadMemoryCardDirectory(void);
s32 Ovl31ProbeMemoryCard(void);
s32 Ovl31RenameLegacySaveFiles(void);

void Ovl31UpdateMemoryCardUtility(void) {
    s32 index;
    s32 result;
    u32 buttons;

    FntPrint("\n\n<<Ez MemoryCard Util.>>\n\n");
    for (index = 0; index < 6; index++) {
        FntPrint(" %s \n", gOvl31MemoryCardMenuRows[index]);
    }
    FntPrint("\n");
    if (gOvl31MemoryCardResult == -1) {
        FntPrint("\n\n     <<<Error!>>>\n");
        if (gPad0Input->pressedButtons) gOvl31MemoryCardResult = 0;
        return;
    }
    if (gOvl31MemoryCardResult == 1) {
        FntPrint("\n\n     <<<Done.>>>\n");
        if (gPad0Input->pressedButtons) gOvl31MemoryCardResult = 0;
        return;
    }
    if (gOvl31MemoryCardResult == 2) {
        for (index = 0; index < gOvl31DirectoryFileCount; index++) {
            FntPrint("%s\n", gOvl31DirectoryEntries[index].name);
        }
        if (gOvl31DirectoryFileCount != 0) {
            FntPrint("\n");
        }
        FntPrint("%s\n", gOvl31DirectorySummary);
        if (gPad0Input->pressedButtons != 0) {
            gOvl31MemoryCardResult = 0;
        }
        return;
    } else if (gOvl31MemoryCardResult != 0) {
        FntPrint("STATUS : ");
        switch (gOvl31MemoryCardResult) {
        case -2:
            FntPrint("Broken\n");
            break;
        case -3:
            FntPrint("Unformat PDA\n");
            break;
        case -4:
            FntPrint("None\n");
            break;
        case -5:
            FntPrint("New\n");
            break;
        case -6:
            FntPrint("Unformat Card\n");
            break;
        case -7:
            FntPrint("PDA\n");
            break;
        case -8:
            FntPrint("Card\n");
            break;
        }
        if (gPad0Input->pressedButtons != 0) {
            gOvl31MemoryCardResult = 0;
        }
    } else {
        buttons = gPad0Input->pressedButtons;
        result = 0;
        /* Deliberately dispatch exact button words, not individual bits:
         * simultaneous presses do not select a command. */
        switch (buttons) {
        case 0x800:
            RequestSceneChange(0);
            break;
        case 0x10:
            result = Ovl31UnformatMemoryCard();
            break;
        case 0x80:
            result = Ovl31FormatMemoryCard();
            break;
        case 0x40:
            result = Ovl31ReadMemoryCardDirectory();
            break;
        case 0x20:
            result = Ovl31ProbeMemoryCard();
            break;
        case 0x100:
            result = Ovl31RenameLegacySaveFiles();
            break;
        }
        if (result != 0) {
            gOvl31MemoryCardResult = result;
        }
    }
}

s32 Ovl31FormatMemoryCard(void) {
    long command;
    long result;
    s32 status;

    status = 0;
    MemCardSync(0, &command, &result);
    result = MemCardFormat(0);
    if (result != 0) {
        if (result >= 0) {
            if (result < 3) {
                status = -1;
            }
        }
    } else {
        status = 1;
    }
    return status;
}

s32 Ovl31UnformatMemoryCard(void) {
    long command;
    long result;
    s32 status;

    status = 0;
    MemCardSync(0, &command, &result);
    result = MemCardUnformat(0);
    if (result >= -1) {
        if (result <= 0) {
            goto set_error;
        } else if (result == 1) {
            goto set_success;
        }
        goto done;
set_error:
        status = -1;
        goto done;
set_success:
        status = 1;
    }
done:
    return status;
}

s32 Ovl31ReadMemoryCardDirectory(void) {
    long command;
    long result;
    s32 index;
    s32 blocks;

    MemCardSync(0, &command, &result);
    MemCardGetDirentry(0, "*", gOvl31DirectoryEntries, &gOvl31DirectoryFileCount, 0, 15);
    blocks = 0;
    for (index = 0; index < gOvl31DirectoryFileCount; index++) {
        blocks += gOvl31DirectoryEntries[index].size / 8192 +
            ((gOvl31DirectoryEntries[index].size % 8192) ? 1 : 0);
    }
    sprintf(gOvl31DirectorySummary, "%d file(s) %d block(s)\n", gOvl31DirectoryFileCount, blocks);
    return 2;
}

/* These negative results select the driver's status labels; zero means no
 * latched result. Keep the two CardType paths separate: an accepted card and
 * an unformatted card map the same Mcx results to different UI statuses. */
s32 Ovl31ProbeMemoryCard(void) {
    long command;
    long result;
    s32 status;

    MemCardSync(0, &command, &result);
    MemCardAccept(0);
    status = MemCardSync(0, &command, &result);

    switch (result) {
    case 2:
        status = -2;
        break;
    case 4:
        McxCardType(0);
        status = McxSync(0, &command, &result);
        switch (result) {
        case 0:
            status = -3;
            break;
        case 1:
            status = -4;
            break;
        case 3:
            status = -5;
            break;
        case 2:
            status = -6;
            break;
        }
        break;
    case 0:
        McxCardType(0);
        status = McxSync(0, &command, &result);
        switch (result) {
        case 0:
            status = -7;
            break;
        case 1:
            status = -4;
            break;
        case 3:
            status = -5;
            break;
        case 2:
            status = -8;
            break;
        }
        break;
    case 3:
        status = -5;
        break;
    case 1:
        status = -4;
        break;
    }
    return (s16)status;
}

/* Each path is tried at most four times. The UI reports completion even if
 * all attempts fail: this routine intentionally ignores the final failures.
 * Explicit repeated operations retain the original call/literal order. */
s32 Ovl31RenameLegacySaveFiles(void) {
    s32 retries;

    sprintf(gOvl31RenameSourcePath, "bu%d%d:%s", 0, 0, "BISLPS-00000-FARM0");
    sprintf(gOvl31RenameDestinationPath, "bu%d%d:%s", 0, 0, "BISLPS-02489-FARM0");
    retries = 0;
    while (rename(gOvl31RenameSourcePath, gOvl31RenameDestinationPath) != 1) {
        if (retries < 3) {
            retries++;
        } else {
            break;
        }
    }
    sprintf(gOvl31RenameSourcePath, "bu%d%d:%s", 0, 0, "BISLPS-00000-FARM1");
    sprintf(gOvl31RenameDestinationPath, "bu%d%d:%s", 0, 0, "BISLPS-02489-FARM1");
    retries = 0;
    while (rename(gOvl31RenameSourcePath, gOvl31RenameDestinationPath) != 1) {
        if (retries < 3) {
            retries++;
        } else {
            break;
        }
    }
    sprintf(gOvl31RenameSourcePath, "bu%d%d:%s", 0, 0, "BISLPS-00000-FARM2");
    sprintf(gOvl31RenameDestinationPath, "bu%d%d:%s", 0, 0, "BISLPS-02489-FARM2");
    retries = 0;
    while (rename(gOvl31RenameSourcePath, gOvl31RenameDestinationPath) != 1) {
        if (retries < 3) {
            retries++;
        } else {
            break;
        }
    }
    sprintf(gOvl31RenameSourcePath, "bu%d%d:%s", 0, 0, "BISLPSP00000-FARMP");
    sprintf(gOvl31RenameDestinationPath, "bu%d%d:%s", 0, 0, "BISLPSP02489-FARMP");
    retries = 0;
    while (rename(gOvl31RenameSourcePath, gOvl31RenameDestinationPath) != 1) {
        if (retries < 3) {
            retries++;
        } else {
            break;
        }
    }
    return 1;
}
