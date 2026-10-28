/*
 * card_diagnostics — interactive PlayStation/PDA communication diagnostics.
 *
 * The blocking scene entry alternates two display environments, polls pads
 * and draws fifteen commands until Select requests exit. Each command handler
 * uses the same phase protocol: 0 edits parameters from pad-0 button edges, 1 draws
 * them, 2 submits an SDK Mcx command and 3 draws its result/data. Only the
 * selected command runs; result text and the transfer bytes are shared.
 *
 * Hex editors keep separate cursors and four per-bit counters per page.
 * Counters advance while a supplied bit is set; the retail caller supplies
 * pressedButtons, not heldButtons. The repeat predicate emits at ticks 1,
 * 41, 81, then every 15 input occurrences. Address
 * bytes are packed in display order, most significant first. Original signed
 * cursor sentinels, byte wrapping and unchecked SDK-supplied lengths remain.
 *
 * The packed structs below express fixed-width text copies, not decoded card
 * records. The mode/day labels are targets of resident pointer tables; all
 * other help/result strings are emitted at their owning use sites. Neither
 * the unreferenced leading word nor the final three original bytes belongs
 * to this C pool. Public entry: ovl10_card_diagnostics.h.
 * Membership and boundary evidence: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libetc.h"
#include "scene_selection.h"
#include "pad_input_update.h"
#include "game_types.h"
#include "psyq/libmcx.h"
#include "psyq/libmcrd.h"
#include "psyq/stdio.h"
#include "ovl10_card_diagnostics.h"

/* The four environment structures are fields of one local so they pack at
   4-alignment (DRAWENV 0x5C, DISPENV 0x14), matching the target frame
   offsets 0x18/0x74/0x88/0xE4.  As four separate locals GCC 2.95 would
   8-align each BLKmode address-taken slot to BIGGEST_ALIGNMENT (8). */
typedef struct {
    DRAWENV draw0;
    DISPENV disp0;
    DRAWENV draw1;
    DISPENV disp1;
} PdaMenuEnvironments;

/* The input service ignores its work anchor, but the original scene passes
 * that resident address. Keep the common contract and the argument. */
void Ovl10ProcessPdaCommandInput(char *workAnchor);
void Ovl10DrawPdaCommandStatus(void);


s32 Ovl10RunPdaCommandMenu(void) {
    PdaMenuEnvironments environments;
    DRAWENV *currentDrawEnv = 0;

    gOvl10DeviceTransferBytes[0] = 1;
    gOvl10DeviceTransferBytes[1] = 4;
    gOvl10DeviceTransferBytes[2] = 1;
    gOvl10DeviceTransferBytes[3] = 0;
    gOvl10DeviceTransferBytes[4] = 0xD;
    gOvl10DeviceTransferBytes[5] = 0x80;

    SetDefDrawEnv(&environments.draw0, 0, 0, 0x280, 0xF0);
    SetDefDrawEnv(&environments.draw1, 0, 0xF0, 0x280, 0xF0);
    SetDefDispEnv(&environments.disp0, 0, 0xF0, 0x280, 0xF0);
    SetDefDispEnv(&environments.disp1, 0, 0, 0x280, 0xF0);
    FntLoad(0x3C0, 0);
    SetDumpFnt(FntOpen(4, 0x14, 0x270, 0xF0, 0, 0x400));

    environments.draw0.isbg = 1;
    environments.draw1.isbg = 1;
    environments.draw0.r0 = 0x3C;
    environments.draw0.g0 = 0x78;
    environments.draw0.b0 = 0x78;
    environments.draw1.r0 = 0x3C;
    environments.draw1.g0 = 0x78;
    environments.draw1.b0 = 0x78;
    SetDispMask(1);
    VSync(0);

    if (gOvl10MenuExitRequested == 0) {
        do {
            currentDrawEnv = (currentDrawEnv == &environments.draw0) ? &environments.draw1 : &environments.draw0;
            PollPadInputs();
            Ovl10ProcessPdaCommandInput(gOvl10InputWorkAnchor);
            Ovl10DrawPdaCommandStatus();
            FntFlush(-1);
            DrawSync(0);
            VSync(0);
            PutDispEnv((DISPENV *)((char *)currentDrawEnv + 0x5C));
            PutDrawEnv(currentDrawEnv);
        } while (gOvl10MenuExitRequested == 0);
    }

    RequestSceneChange(0);
    return 0;
}

s32 Ovl10DispatchPdaCommand(s32, s32);


typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} McData28;

void Ovl10ProcessPdaCommandInput(char *workAnchor) {
    s32 newPresses;
    s32 navigationButtons;

    newPresses = gPad0Input->pressedButtons;
    Ovl10DispatchPdaCommand(0, newPresses);
    if (gOvl10CommandNavigationLatch == 0) {
        if (newPresses & PADL2) {
            gOvl10CommandIndex = (gOvl10CommandIndex + 0xE) % 15;
        } else if (newPresses & PADL1) {
            gOvl10CommandIndex = (gOvl10CommandIndex + 1) % 15;
        }
    }
    if ((gOvl10CommandExecuteLatch == 0) && (newPresses & PADstart)) {
        if (Ovl10DispatchPdaCommand(2, 0) != 0) {
            gOvl10AcceptedCommandIndex = gOvl10CommandIndex;
        } else {
            gOvl10AcceptedCommandIndex = -1;
            *(McData28 *)gOvl10CommandResultText = *(McData28 *)"command can't reserve(busy)";
        }
    }
    if (newPresses & PADselect) {
        gOvl10MenuExitRequested = 1;
    }
    navigationButtons = newPresses & (PADL2 | PADL1);
    gOvl10CommandNavigationLatch = navigationButtons;
    gOvl10CommandExecuteLatch = newPresses & PADstart;
    if (navigationButtons != 0) {
        gOvl10CommandResultText[0] = 0;
    }
}

void Ovl10DrawPdaCommandList(void);
int Ovl10DispatchPdaCommand(int, int);

/* Width-only views preserve the packed status-message copies. */

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s8 unk10;
    s8 unk11;
} McCardRec18;

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s8 unk10;
    s8 unk11;
    s8 unk12;
} McCardRec19;

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s8 unkC;
    s8 unkD;
    s8 unkE;
} McCardRec15;

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s8 unk1C;
    s8 unk1D;
} McCardRec30;

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s8 unk20;
} McCardRec33;

void Ovl10DrawPdaCommandStatus(void) {
    s32 sp10;
    s32 sp14;
    s32 i;
    s32 ret;

    for (i = 0; i < 15; i++) {
        gOvl10CommandRowMarkers[i] = "~c888";
    }
    gOvl10CommandRowMarkers[gOvl10CommandIndex] = "~c585";
    Ovl10DrawPdaCommandList();
    FntFlush(-1);
    DrawSync(0);
    FntPrint("\n\n\n\n\n\n\n");
    switch (ret = McxSync(1, (long *)&sp10, (long *)&sp14)) {
    case 0:
        *(McCardRec18 *)&gOvl10CommandResultText = *(McCardRec18 *)"command executing";
        break;
    case 1:
        switch (sp14) {
        case 0:
            *(McCardRec18 *)&gOvl10CommandResultText = *(McCardRec18 *)"command succeeded";
            break;
        case 1:
            *(McCardRec19 *)&gOvl10CommandResultText = *(McCardRec19 *)"card not connected";
            break;
        case 2:
            *(McCardRec15 *)&gOvl10CommandResultText = *(McCardRec15 *)"command failed";
            break;
        case 3:
            if (sp10 == 0xB) {
                *(McCardRec30 *)&gOvl10CommandResultText = *(McCardRec30 *)"command failed (card changed)";
            } else {
                *(McCardRec33 *)&gOvl10CommandResultText = *(McCardRec33 *)"command succeeded (card changed)";
            }
            break;
        }
        break;
    }
    Ovl10DispatchPdaCommand(1, 0);
    FntPrint("\n");
    if (gOvl10CommandIndex == gOvl10AcceptedCommandIndex) {
        Ovl10DispatchPdaCommand(3, 0);
    } else {
        FntPrint("    press start to exec command \n");
    }
    FntPrint("\n    ~c585%s \n~c888", gOvl10CommandResultText);
}

void Ovl10DrawPdaCommandList(void) {
    FntPrint("  %s1: all info        ~c888 %s 6: get time       ~c888 %s11: show trans    ~c888\n", gOvl10CommandRowMarkers[0], gOvl10CommandRowMarkers[5], gOvl10CommandRowMarkers[10]);
    FntPrint("  %s2: current control ~c888 %s 7: get serial no  ~c888 %s12: hide trans    ~c888\n", gOvl10CommandRowMarkers[1], gOvl10CommandRowMarkers[6], gOvl10CommandRowMarkers[11]);
    FntPrint("  %s3: flush access    ~c888 %s 8: set exec flag  ~c888 %s13: set memory    ~c888\n", gOvl10CommandRowMarkers[2], gOvl10CommandRowMarkers[7], gOvl10CommandRowMarkers[12]);
    FntPrint("  %s4: exec appli      ~c888 %s 9: write decive   ~c888 %s14: get memory    ~c888\n", gOvl10CommandRowMarkers[3], gOvl10CommandRowMarkers[8], gOvl10CommandRowMarkers[13]);
    FntPrint("  %s5: get appli       ~c888 %s10: read device    ~c888 %s15: set led       ~c888\n", gOvl10CommandRowMarkers[4], gOvl10CommandRowMarkers[9], gOvl10CommandRowMarkers[14]);
    FntPrint("\n");
}

int Ovl10HandlePdaControl(int, int);
int Ovl10HandlePdaFlushAccess(int, int);
int Ovl10HandlePdaApplicationLaunch(int, int);
int Ovl10HandlePdaExecFlag(int, int);
int Ovl10HandlePdaDeviceWrite(int, int);
int Ovl10HandlePdaDeviceRead(int, int);
int Ovl10HandlePdaTransitionDisplay(int, int);
int Ovl10HandlePdaMemoryWrite(int, int);
int Ovl10HandlePdaMemoryRead(int, int);
int Ovl10HandlePdaLed(int, int);
int Ovl10HandlePdaAllInfo(int);
int Ovl10HandlePdaApplicationQuery(int);
int Ovl10HandlePdaTimeQuery(int);
int Ovl10HandlePdaSerialQuery(int);
int Ovl10HandlePdaHideTransitions(int);

int Ovl10DispatchPdaCommand(s32 arg0, s32 arg1) {
    switch ((u32)gOvl10CommandIndex) {
    case 1:
        return Ovl10HandlePdaControl(arg0, arg1);
    case 2:
        return Ovl10HandlePdaFlushAccess(arg0, arg1);
    case 3:
        return Ovl10HandlePdaApplicationLaunch(arg0, arg1);
    case 7:
        return Ovl10HandlePdaExecFlag(arg0, arg1);
    case 8:
        return Ovl10HandlePdaDeviceWrite(arg0, arg1);
    case 9:
        return Ovl10HandlePdaDeviceRead(arg0, arg1);
    case 10:
        return Ovl10HandlePdaTransitionDisplay(arg0, arg1);
    case 12:
        return Ovl10HandlePdaMemoryWrite(arg0, arg1);
    case 13:
        return Ovl10HandlePdaMemoryRead(arg0, arg1);
    case 14:
        return Ovl10HandlePdaLed(arg0, arg1);
    default:
        if (arg0 == 1) {
            FntPrint("    no parameter needed\n");
            return 0;
        }
        if ((u32) (arg0 - 2) < 2U) {
            switch ((u32)gOvl10CommandIndex) {
            case 0:
                return Ovl10HandlePdaAllInfo(arg0);
            case 4:
                return Ovl10HandlePdaApplicationQuery(arg0);
            case 5:
                return Ovl10HandlePdaTimeQuery(arg0);
            case 6:
                return Ovl10HandlePdaSerialQuery(arg0);
            case 11:
                return Ovl10HandlePdaHideTransitions(arg0);
            default:
                return 0;
            }
        } else {
            return 0;
        }
    }
}

void Ovl10DrawNoPdaCommandData(void) {
    FntPrint("    no data disp \n");
}


/* Targets of the resident link-mode and weekday pointer tables. Their
 * declaration order retains the tables' original reverse-initializer pool. */
const char gOvl10LinkModeBoth[] = "tx/rx";
const char gOvl10LinkModeTransmit[] = "tx";
const char gOvl10LinkModeReceive[] = "rx";
const char gOvl10LinkModeDisabled[] = "disable";
const char gOvl10WeekdayLabels[7][4] = {
    "sat", "fri", "thr", "wed", "tue", "mon", "sun"
};

int Ovl10HandlePdaAllInfo(int arg0) {
    char buf[16];
    unsigned char *p;

    if (arg0 == 2) {
        return McxAllInfo(0, (unsigned char *)gOvl10AllInfoBytes);
    }
    FntPrint("    current application : %d \n", gOvl10AllInfoBytes[0] | (gOvl10AllInfoBytes[1] << 8));
    FntPrint("    PDA appli flush access :%s \n", gOvl10AllInfoBytes[0xF] != 0 ? "SUPERIOR" : "INFERIOR");
    FntPrint("    current control  sound:%s  infred:%s \n", gOvl10AllInfoBytes[2] != 0 ? "ENABLE" : "DISABLE",
             gOvl10LinkModeNamePointers[gOvl10AllInfoBytes[3]]);
    FntPrint("    serial No. : %x \n", *(int *)&gOvl10AllInfoBytes[4]);
    p = (unsigned char *)gOvl10AllInfoBytes + 8;
    sprintf(buf, "%2x%02x", gOvl10AllInfoBytes[8], p[1]);
    FntPrint("    date: %s/", buf);
    FntPrint("%x/%x %s ", p[2], p[3], gOvl10InfoWeekdayNamePointers[p[4]]);
    sprintf(buf, "%2x:%02x:%02x", p[5], p[6], p[7]);
    FntPrint("%s \n", buf);
    return 0;
}


int Ovl10HandlePdaApplicationQuery(int arg0) {
    if (arg0 == 2) {
        return McxGetApl(0, &gOvl10ApplicationNumber);
    }
    FntPrint("    current application : %d \n", gOvl10ApplicationNumber);
    return 0;
}


int Ovl10HandlePdaTimeQuery(int arg0) {
    char buf[16];

    if (arg0 == 2) {
        return McxGetTime(0, (unsigned char *)gOvl10TimeBytes);
    }
    sprintf(buf, "%2x%02x", gOvl10TimeBytes[0], gOvl10TimeBytes[1]);
    FntPrint("    date: %s/", buf);
    FntPrint("%x/%x %s ", gOvl10TimeBytes[2], gOvl10TimeBytes[3], gOvl10TimeWeekdayNamePointers[gOvl10TimeBytes[4]]);
    sprintf(buf, "%2x:%02x:%02x", gOvl10TimeBytes[5], gOvl10TimeBytes[6], gOvl10TimeBytes[7]);
    FntPrint("%s \n", buf);
    return 0;
}

void Ovl10DrawNoPdaCommandData(void);

int Ovl10HandlePdaHideTransitions(int arg0) {
    if (arg0 == 2) {
        return McxHideTrans(0);
    }
    Ovl10DrawNoPdaCommandData();
    return 0;
}


int Ovl10HandlePdaSerialQuery(int arg0) {
    if (arg0 == 2) {
        return McxGetSerial(0, (unsigned long *)&gOvl10SerialNumber);
    }
    FntPrint("    serial No. : %x \n", gOvl10SerialNumber);
    return 0;
}

s32 Ovl10IsPdaMenuRepeatTick(s32);

int Ovl10HandlePdaMemoryRead(int arg0, int arg1)
{
    int i;
    int row;
    char buf[16];

    switch (arg0) {
    case 0:
        if (gOvl10MemoryReadCursorLatch == 0) {
            if (arg1 & 0x2000) {
                if (gOvl10MemoryReadCursor < 4) {
                    gOvl10MemoryReadCursor += 1;
                }
            } else if ((arg1 & 0x8000) != 0 && gOvl10MemoryReadCursor > 0) {
                gOvl10MemoryReadCursor -= 1;
            }
        }
        gOvl10MemoryReadCursorLatch = arg1 & 0xA000;
        if (arg1 & 0x10) {
            gOvl10MemoryReadUpperIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10MemoryReadUpperIncrementTicks) != 0) {
                gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] = (gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] + 0x10) & 0xFF;
            }
        } else {
            gOvl10MemoryReadUpperIncrementTicks = 0;
            if (arg1 & 0x80) {
                gOvl10MemoryReadUpperDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10MemoryReadUpperDecrementTicks) != 0) {
                    gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] = (gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] + 0xF0) & 0xFF;
                }
            } else {
                gOvl10MemoryReadUpperDecrementTicks = 0;
            }
        }
        if (gOvl10MemoryReadHeader[4] >= 0x81U) {
            if (gOvl10MemoryReadHeader[4] == 0x90) {
                gOvl10MemoryReadHeader[4] = 0;
            } else if (gOvl10MemoryReadHeader[4] == 0xF0) {
                gOvl10MemoryReadHeader[4] = 0x80;
            } else {
                gOvl10MemoryReadHeader[4] &= 0x7F;
            }
        }
        if (arg1 & 0x20) {
            gOvl10MemoryReadLowerIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10MemoryReadLowerIncrementTicks) != 0) {
                gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] = (gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] & 0xF0) +
                                         (((gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] & 0xF) + 1) & 0xF);
            }
        } else {
            gOvl10MemoryReadLowerIncrementTicks = 0;
            if (arg1 & 0x40) {
                gOvl10MemoryReadLowerDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10MemoryReadLowerDecrementTicks) != 0) {
                    gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] = (gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] & 0xF0) +
                                             (((gOvl10MemoryReadHeader[gOvl10MemoryReadCursor] & 0xF) + 0xF) & 0xF);
                }
            } else {
                gOvl10MemoryReadLowerDecrementTicks = 0;
            }
        }
        if (gOvl10MemoryReadHeader[4] >= 0x81U) {
            gOvl10MemoryReadHeader[4] -= 0x10;
        }
        return 0;
    case 1:
        FntPrint("    right:  pos right     left:  pos left \n");
        FntPrint("    triang: inc upper     squar: dec upper \n");
        FntPrint("    circle: inc lower     cross: dec lower \n");
        FntPrint("    start address :~c558");
        for (i = 0; i < 4; i++) {
            sprintf(buf, "%02x", gOvl10MemoryReadHeader[i]);
            if (i == gOvl10MemoryReadCursor) {
                FntPrint("~c588%s~c558", buf);
            } else {
                FntPrint(buf);
            }
        }
        sprintf(buf, "%02x", gOvl10MemoryReadHeader[4]);
        FntPrint("  ~c888len %s%s~c888 \n", (gOvl10MemoryReadCursor == 4) ? "~c588" : "~c558", buf);
        return 0;
    case 2:
        return McxGetMem(0, gOvl10MemoryReadBytes,
                         (gOvl10MemoryReadHeader[0] << 24) | (gOvl10MemoryReadHeader[1] << 16) |
                         (gOvl10MemoryReadHeader[2] << 8) | gOvl10MemoryReadHeader[3],
                         gOvl10MemoryReadHeader[4]);
    case 3:
        row = 0;
        do {
            FntPrint("    ");
            i = 0;
            if ((unsigned)(row * 0x10) < gOvl10MemoryReadHeader[4]) {
                do {
                    sprintf(buf, "%02x", gOvl10MemoryReadBytes[row * 0x10 + i]);
                    FntPrint("%s ", buf);
                    i++;
                    if (i >= 0x10) {
                        break;
                    }
                } while ((unsigned)(row * 0x10 + i) < gOvl10MemoryReadHeader[4]);
            }
            FntPrint("\n");
            if (gOvl10MemoryReadHeader[4] < (unsigned)(row * 0x10 + i)) {
                break;
            }
            row++;
        } while (row < 8);
        return 0;
    default:
        return 0;
    }
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s8 unk1C;
    s8 unk1D;
} __attribute__((packed)) McSaveData;


int Ovl10HandlePdaExecFlag(s32 arg0, s32 arg1) {
    long cmds;
    long result;

    switch (arg0) {
    case 0:
        if (arg1 & 0x20) {
            gOvl10ExecFlagEnabled = 1;
        } else if (arg1 & 0x40) {
            gOvl10ExecFlagEnabled = 0;
        }
        if (arg1 & 0x10) {
            s32 v = gOvl10ExecNumberIncrementTicks + 1;
            gOvl10ExecNumberIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0 && gOvl10ExecApplicationNumber < 0xF) {
                gOvl10ExecApplicationNumber++;
            }
        } else {
            gOvl10ExecNumberIncrementTicks = 0;
            if (arg1 & 0x80) {
                s32 v = gOvl10ExecNumberDecrementTicks + 1;
                gOvl10ExecNumberDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                    if (gOvl10ExecApplicationNumber >= 2) {
                        gOvl10ExecApplicationNumber--;
                    }
                }
            } else {
                gOvl10ExecNumberDecrementTicks = 0;
            }
        }
        return 0;
    case 1:
        FntPrint("    triang: inc appli no.     squar: dec appli no. \n");
        FntPrint("    circle: exec flag on      cross: exec flag off \n");
        FntPrint("    appli(block) number :~c558 %d ~c888\n", gOvl10ExecApplicationNumber);
        FntPrint("    exec flag :~c558 %s ~c888\n", gOvl10ExecFlagEnabled ? "on" : "off");
        return 0;
    case 2:
        MemCardAccept(0);
        MemCardSync(0, &cmds, &result);
        if ((u32)(result - 1) < 2U) {
            *(McSaveData *)gOvl10CommandResultText = *(McSaveData *)gOvl10DefaultCommandResultText;
            return -1;
        }
        return McxExecFlag(0, gOvl10ExecApplicationNumber, gOvl10ExecFlagEnabled);
    case 3:
        Ovl10DrawNoPdaCommandData();
        /* fallthrough */
    default:
        return 0;
    }
}

/* Copied as a complete packed status message when the card is absent. */
const char gOvl10DefaultCommandResultText[] = "~c855command failed (no card)";

int Ovl10IsPdaMenuRepeatTick(int);


int Ovl10HandlePdaApplicationLaunch(int arg0, int arg1) {
    char buf[16];
    int s0;

    switch (arg0) {
    case 0:
        if (gOvl10ApplicationCursorLatch == 0) {
            if (arg1 & 0x2000) {
                if (gOvl10ApplicationParameterCursor < 4) {
                    gOvl10ApplicationParameterCursor += 1;
                }
            } else if ((arg1 & 0x8000) && (gOvl10ApplicationParameterCursor > 0)) {
                gOvl10ApplicationParameterCursor -= 1;
            }
            if (arg1 & 0x1000) {
                gOvl10ApplicationParameterCursor = 0;
            } else if (arg1 & 0x4000) {
                if (gOvl10ApplicationParameterCursor == 0) {
                    gOvl10ApplicationParameterCursor = 1;
                }
            }
        }
        gOvl10ApplicationCursorLatch = arg1 & 0xF000;

        if (arg1 & 0x10) {
            gOvl10ApplicationUpperIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10ApplicationUpperIncrementTicks) != 0) {
                gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] = (gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] + 0x10) & 0xFF;
            }
        } else {
            gOvl10ApplicationUpperIncrementTicks = 0;
            if (arg1 & 0x80) {
                gOvl10ApplicationUpperDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10ApplicationUpperDecrementTicks) != 0) {
                    gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] = (gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] + 0xF0) & 0xFF;
                }
            } else {
                gOvl10ApplicationUpperDecrementTicks = 0;
            }
        }

        if (arg1 & 0x20) {
            gOvl10ApplicationLowerIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10ApplicationLowerIncrementTicks) != 0) {
                gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] = (gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] & 0xF0) +
                                         (((gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] & 0xF) + 1) & 0xF);
            }
        } else {
            gOvl10ApplicationLowerIncrementTicks = 0;
            if (arg1 & 0x40) {
                gOvl10ApplicationLowerDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10ApplicationLowerDecrementTicks) != 0) {
                    if ((gOvl10ApplicationParameterCursor == 0) && (gOvl10ApplicationParameters[0] == 0)) {
                        gOvl10ApplicationParameters[0] = -1;
                    } else {
                        gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] = (gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] & 0xF0) +
                                                 (((gOvl10ApplicationParameters[gOvl10ApplicationParameterCursor] & 0xF) + 0xF) & 0xF);
                    }
                }
            } else {
                gOvl10ApplicationLowerDecrementTicks = 0;
            }
        }

        if ((gOvl10ApplicationParameterCursor == 0) && (gOvl10ApplicationParameters[0] != -1)) {
            gOvl10ApplicationParameters[0] &= 0xF;
        }
        return 0;
    case 1:
        FntPrint("    triang: inc appri no.     squar: dec appli no. \n");
        FntPrint("    application number : %s%d \n", gOvl10ApplicationParameterCursor != 0 ? "~c558" : "~c588",
                 gOvl10ApplicationParameters[0]);
        FntPrint("    ~c888arg for appli :~c558 ");
        for (s0 = 0; s0 < 4; s0++) {
            sprintf(buf, "%02x", gOvl10ApplicationParameters[s0 + 1]);
            if (s0 + 1 == gOvl10ApplicationParameterCursor) {
                FntPrint("~c588%s~c558", buf);
            } else {
                FntPrint(buf);
            }
        }
        FntPrint("~c888\n");
        return 0;
    case 2:
        return McxExecApl(0, gOvl10ApplicationParameters[0], (gOvl10ApplicationParameters[1] << 24) |
                                                (gOvl10ApplicationParameters[2] << 16) |
                                                (gOvl10ApplicationParameters[3] << 8) |
                                                gOvl10ApplicationParameters[4]);
    case 3:
        Ovl10DrawNoPdaCommandData();
        return 0;
    default:
        return 0;
    }
}

/* First target of the current-control mode-name table (the other modes
 * share the transmit/receive labels above). */
const char gOvl10CurrentControlDisabled[] = "dis";


s32 Ovl10HandlePdaControl(s32 arg0, s32 arg1) {
    switch (arg0) {
    case 0:
        if (gOvl10ControlInfraredButtonLatch == 0) {
            if (arg1 & 0x10) {
                if (gOvl10ControlInfraredMode < 3) {
                    gOvl10ControlInfraredMode += 1;
                }
            } else if ((arg1 & 0x80) && (gOvl10ControlInfraredMode > 0)) {
                gOvl10ControlInfraredMode -= 1;
            }
        }
        gOvl10ControlInfraredButtonLatch = arg1 & 0x90;
        if (arg1 & 0x20) {
            gOvl10ControlSoundEnabled = 1;
        } else if (arg1 & 0x40) {
            gOvl10ControlSoundEnabled = 0;
        }
        return 0;
    case 1:
        FntPrint("    circle: sound enable      cross: sound disable \n");
        FntPrint("    triang: infred enable     squar: infred disable \n");
        FntPrint("    function allowance  sound ~c558%s~c888  infred ~c558%s~c888 \n", gOvl10ControlSoundEnabled != 0 ? "ENA" : "DIS",
                 gOvl10ControlModeNamePointers[gOvl10ControlInfraredMode]);
        return 0;
    case 2:
        return McxCurrCtrl(0, gOvl10ControlSoundEnabled, gOvl10ControlInfraredMode, 0);
    case 3:
        Ovl10DrawNoPdaCommandData();
        return 0;
    default:
        return 0;
    }
}


int Ovl10HandlePdaFlushAccess(int arg0, int arg1) {
    switch (arg0) {
    case 0:
        if (arg1 & 0x20) {
            gOvl10FlushWriteSuperior = 1;
        } else if (arg1 & 0x40) {
            gOvl10FlushWriteSuperior = 0;
        }
        break;
    case 1:
        FntPrint("    circle: flush write superior to spi \n");
        FntPrint("    cross:  spi superior to flush write \n");
        FntPrint("    PDA appli flush access : ~c558%s~c888 \n", gOvl10FlushWriteSuperior ? "SUPERIOR" : "INFERIOR");
        break;
    case 2:
        return McxFlashAcs(0, gOvl10FlushWriteSuperior);
    case 3:
        Ovl10DrawNoPdaCommandData();
        break;
    }
    return 0;
}


s32 Ovl10HandlePdaMemoryWrite(s32 arg0, s32 arg1) {
    char buf[16];
    s32 s3;
    int s2;
    int s7;

    switch (arg0) {
    case 0:
        if (gOvl10MemoryWriteCursorLatch == 0) {
            if (arg1 & 0x2000) {
                if (gOvl10MemoryWriteCursor < gOvl10MemoryWriteHeader[4] - 1) {
                    gOvl10MemoryWriteCursor += 1;
                }
            } else if ((arg1 & 0x8000) && (gOvl10MemoryWriteCursor >= -4)) {
                gOvl10MemoryWriteCursor -= 1;
            }
            if (arg1 & 0x1000) {
                if ((u32)gOvl10MemoryWriteCursor < 6U) {
                    gOvl10MemoryWriteCursor = -5;
                } else if (gOvl10MemoryWriteCursor == 6) {
                    gOvl10MemoryWriteCursor = -4;
                } else if ((u32)(gOvl10MemoryWriteCursor - 7) < 2U) {
                    gOvl10MemoryWriteCursor = -2;
                } else if ((u32)(gOvl10MemoryWriteCursor - 9) < 7U) {
                    gOvl10MemoryWriteCursor = -1;
                } else if (gOvl10MemoryWriteCursor < 0x10) {
                    /* leave the selection on a header entry */
                } else {
                    gOvl10MemoryWriteCursor -= 0x10;
                }
            } else if (arg1 & 0x4000) {
                switch (gOvl10MemoryWriteCursor) {
                case -5:
                    if ((u32)gOvl10MemoryWriteHeader[4] >= 6U) {
                        gOvl10MemoryWriteCursor = 5;
                    }
                    break;
                case -4:
                case -3:
                    if ((u32)gOvl10MemoryWriteHeader[4] >= 7U) {
                        gOvl10MemoryWriteCursor = 6;
                    }
                    break;
                case -2:
                    if ((u32)gOvl10MemoryWriteHeader[4] >= 8U) {
                        gOvl10MemoryWriteCursor = 7;
                    }
                    break;
                case -1:
                    if ((u32)gOvl10MemoryWriteHeader[4] >= 0xBU) {
                        gOvl10MemoryWriteCursor = 0xA;
                    }
                    break;
                default:
                    if (gOvl10MemoryWriteCursor + 0x10 < gOvl10MemoryWriteHeader[4]) {
                        gOvl10MemoryWriteCursor += 0x10;
                    }
                    break;
                }
                if (gOvl10MemoryWriteHeader[4] != 0 && gOvl10MemoryWriteCursor < 0) {
                    gOvl10MemoryWriteCursor = gOvl10MemoryWriteHeader[4] - 1;
                }
            }
        }
        s3 = -1;
        gOvl10MemoryWriteCursorLatch = arg1 & 0xF000;
        if (gOvl10MemoryWriteCursor < 0) {
            s3 = gOvl10MemoryWriteCursor + 5;
        }

        if (arg1 & 0x10) {
            s32 v = gOvl10MemoryWriteUpperIncrementTicks + 1;
            gOvl10MemoryWriteUpperIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                if (gOvl10MemoryWriteCursor < 0) {
                    gOvl10MemoryWriteHeader[s3] = (gOvl10MemoryWriteHeader[s3] + 0x10) & 0xFF;
                } else {
                    gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] = gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] + 0x10;
                }
            }
        } else {
            gOvl10MemoryWriteUpperIncrementTicks = 0;
            if (arg1 & 0x80) {
                s32 v = gOvl10MemoryWriteUpperDecrementTicks + 1;
                gOvl10MemoryWriteUpperDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                    if (gOvl10MemoryWriteCursor < 0) {
                        gOvl10MemoryWriteHeader[s3] = (gOvl10MemoryWriteHeader[s3] + 0xF0) & 0xFF;
                    } else {
                        gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] = gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] + 0xF0;
                    }
                }
            } else {
                gOvl10MemoryWriteUpperDecrementTicks = 0;
            }
        }

        if (s3 >= 0) {
            if ((u32)gOvl10MemoryWriteHeader[4] >= 0x81U) {
                if (gOvl10MemoryWriteHeader[4] == 0x90) {
                    gOvl10MemoryWriteHeader[4] = 0;
                } else if (gOvl10MemoryWriteHeader[4] == 0xF0) {
                    gOvl10MemoryWriteHeader[4] = 0x80;
                } else {
                    gOvl10MemoryWriteHeader[4] &= 0x7F;
                }
            }
        }

        if (arg1 & 0x20) {
            s32 v = gOvl10MemoryWriteLowerIncrementTicks + 1;
            gOvl10MemoryWriteLowerIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                if (gOvl10MemoryWriteCursor < 0) {
                    gOvl10MemoryWriteHeader[s3] = (gOvl10MemoryWriteHeader[s3] & 0xF0) +
                                     (((gOvl10MemoryWriteHeader[s3] & 0xF) + 1) & 0xF);
                } else {
                    gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] = (gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] & 0xF0) +
                                             (((gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] & 0xF) + 1) &
                                              0xF);
                }
            }
        } else {
            gOvl10MemoryWriteLowerIncrementTicks = 0;
            if (arg1 & 0x40) {
                s32 v = gOvl10MemoryWriteLowerDecrementTicks + 1;
                gOvl10MemoryWriteLowerDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                    if (gOvl10MemoryWriteCursor < 0) {
                        gOvl10MemoryWriteHeader[s3] = (gOvl10MemoryWriteHeader[s3] & 0xF0) +
                                         (((gOvl10MemoryWriteHeader[s3] & 0xF) + 0xF) & 0xF);
                    } else {
                        gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] =
                            (gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] & 0xF0) +
                            (((gOvl10MemoryWriteBytes[gOvl10MemoryWriteCursor] & 0xF) + 0xF) & 0xF);
                    }
                }
            } else {
                gOvl10MemoryWriteLowerDecrementTicks = 0;
            }
        }

        if ((u32)gOvl10MemoryWriteHeader[4] >= 0x81U) {
            gOvl10MemoryWriteHeader[4] -= 0x10;
        }
        break;

    case 1:
        s3 = gOvl10MemoryWriteCursor + 5;
        if (gOvl10MemoryWriteCursor >= 0) {
            s3 = -1;
        }
        FntPrint("    righ: pos right   left: pos left ");
        FntPrint("    up:   pos up      down: pos down\n");
        FntPrint("    trng: inc upper   sqar: dec upper");
        FntPrint("    crcl: inc lower   cros: dec lower\n");
        FntPrint("    start address :~c558");
        for (s2 = 0; s2 < 4; s2++) {
            sprintf(buf, "%02x", gOvl10MemoryWriteHeader[s2]);
            if (s2 == s3) {
                FntPrint("~c588%s~c558", buf);
            } else {
                FntPrint(buf);
            }
        }
        sprintf(buf, "%02x", gOvl10MemoryWriteHeader[4]);
        FntPrint("  ~c888len %s%s~c888 \n", s3 == 4 ? "~c588" : "~c558", buf);
        FntPrint("\n~c558");

        s7 = 0;
        do {
            FntPrint("    ");
            s2 = 0;
            if (s7 * 0x10 < (u32)gOvl10MemoryWriteHeader[4]) {
                do {
                    sprintf(buf, "%02x ", (s32)gOvl10MemoryWriteBytes[s7 * 0x10 + s2]);
                    if (s7 * 0x10 + s2 == gOvl10MemoryWriteCursor) {
                        FntPrint("~c588%s~c558", buf);
                    } else {
                        FntPrint(buf);
                    }
                    s2++;
                    if (s2 >= 0x10) {
                        break;
                    }
                } while ((u32)(s7 * 0x10 + s2) < (u32)gOvl10MemoryWriteHeader[4]);
            }
            FntPrint("\n");
            if ((u32)gOvl10MemoryWriteHeader[4] < (u32)(s7 * 0x10 + s2)) {
                break;
            }
            s7++;
        } while (s7 < 8);
        break;

    case 2:
        return McxSetMem(0, gOvl10MemoryWriteBytes,
                         (gOvl10MemoryWriteHeader[0] << 24) | (gOvl10MemoryWriteHeader[1] << 16) |
                             (gOvl10MemoryWriteHeader[2] << 8) | gOvl10MemoryWriteHeader[3],
                         gOvl10MemoryWriteHeader[4]);

    case 3:
        Ovl10DrawNoPdaCommandData();
        break;
    }
    FntPrint("~c888");
    return 0;
}


int Ovl10HandlePdaTransitionDisplay(int arg0, int arg1) {
    switch (arg0) {
    case 0:
        if (arg1 & 0x20) {
            gOvl10TransitionDirection = 1;
        } else if (arg1 & 0x40) {
            gOvl10TransitionDirection = 0;
        }
        if (arg1 & 0x10) {
            s32 v = gOvl10TransitionTimeoutIncrementTicks + 1;
            gOvl10TransitionTimeoutIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0 && gOvl10TransitionTimeout < 0xFF) {
                gOvl10TransitionTimeout++;
            }
        } else {
            gOvl10TransitionTimeoutIncrementTicks = 0;
            if (arg1 & 0x80) {
                s32 v = gOvl10TransitionTimeoutDecrementTicks + 1;
                gOvl10TransitionTimeoutDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0 && gOvl10TransitionTimeout > 0) {
                    gOvl10TransitionTimeout--;
                }
            } else {
                gOvl10TransitionTimeoutDecrementTicks = 0;
            }
        }
        return 0;
    case 1:
        FntPrint("    circle: PS to PDA    cross: PDA to PS \n");
        FntPrint("    triang: inc time     squar: dec time \n");
        FntPrint("    dir :~c558 %s ~c888\n", gOvl10TransitionDirection != 0 ? "PS to PDA" : "PDA to PS");
        FntPrint("    time out :~c558 %d ~c888s \n", gOvl10TransitionTimeout);
        return 0;
    case 2:
        return McxShowTrans(0, gOvl10TransitionDirection, gOvl10TransitionTimeout);
    case 3:
        Ovl10DrawNoPdaCommandData();
        return 0;
    default:
        return 0;
    }
}


int Ovl10HandlePdaLed(s32 arg0, s32 arg1) {
    switch (arg0) {
    case 0:
        if (arg1 & 0x20) {
            gOvl10LedEnabled = 1;
        } else if (arg1 & 0x40) {
            gOvl10LedEnabled = 0;
        }
        return 0;
    case 1:
        FntPrint("    circle: on   cross: off \n");
        FntPrint("    led :~c558 %s ~c888\n", gOvl10LedEnabled ? "on" : "off");
        return 0;
    case 2:
        return McxSetLED(0, gOvl10LedEnabled);
    case 3:
        Ovl10DrawNoPdaCommandData();
        /* fallthrough */
    default:
        return 0;
    }
}


int Ovl10HandlePdaDeviceRead(int arg0, int arg1) {
    char buf[16];
    int s1;
    int s2;
    int s3;

    switch (arg0) {
    case 0:
        if (gOvl10DeviceReadCursorLatch == 0) {
            if (arg1 & 0x2000) {
                if (gOvl10DeviceReadCursor < 0x1F) {
                    gOvl10DeviceReadCursor += 1;
                }
            } else if ((arg1 & 0x8000) && (gOvl10DeviceReadCursor >= 0)) {
                gOvl10DeviceReadCursor -= 1;
            }
            if (arg1 & 0x1000) {
                gOvl10DeviceReadCursor -= 0x10;
                if (gOvl10DeviceReadCursor < 0) {
                    gOvl10DeviceReadCursor = -1;
                }
            } else if (arg1 & 0x4000) {
                if (gOvl10DeviceReadCursor < 0) {
                    gOvl10DeviceReadCursor = 0;
                } else if (gOvl10DeviceReadCursor + 0x10 < 0x20) {
                    gOvl10DeviceReadCursor += 0x10;
                }
            }
        }
        gOvl10DeviceReadCursorLatch = arg1 & 0xF000;

        if (arg1 & 0x10) {
            s32 v = gOvl10DeviceReadUpperIncrementTicks + 1;
            gOvl10DeviceReadUpperIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] += 0x10;
            }
        } else {
            gOvl10DeviceReadUpperIncrementTicks = 0;
            if (arg1 & 0x80) {
                s32 v = gOvl10DeviceReadUpperDecrementTicks + 1;
                gOvl10DeviceReadUpperDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                    gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] += 0xF0;
                }
            } else {
                gOvl10DeviceReadUpperDecrementTicks = 0;
            }
        }

        if (arg1 & 0x20) {
            s32 v = gOvl10DeviceReadLowerIncrementTicks + 1;
            gOvl10DeviceReadLowerIncrementTicks = v;
            if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] =
                    (gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] & 0xF0) +
                    (((gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] & 0xF) + 1) & 0xF);
            }
        } else {
            gOvl10DeviceReadLowerIncrementTicks = 0;
            if (arg1 & 0x40) {
                s32 v = gOvl10DeviceReadLowerDecrementTicks + 1;
                gOvl10DeviceReadLowerDecrementTicks = v;
                if (Ovl10IsPdaMenuRepeatTick(v) != 0) {
                    gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] =
                        (gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] & 0xF0) +
                        (((gOvl10DeviceTransferBytes[gOvl10DeviceReadCursor + 1] & 0xF) + 0xF) & 0xF);
                }
            } else {
                gOvl10DeviceReadLowerDecrementTicks = 0;
            }
        }
        return 0;

    case 1:
        FntPrint("    up/down/right/left:  pos up/down/right/left \n");
        FntPrint("    triang/square: inc/dec upper     circle/cross: inc/dec lower \n");
        FntPrint("    device no : %s%x~c888 \n",
                 gOvl10DeviceReadCursor == -1 ? "~c588" : "~c558", gOvl10DeviceTransferBytes[0]);
        {
            for (s3 = 0; s3 < 2; s3++) {
                FntPrint(s3 != 0 ? "          " : "    fix :~c558 ");
                {
                    s1 = 0;
                    s2 = s3 << 4;
                    while (s1 < 0x10) {
                        sprintf(buf, "%02x ", gOvl10DeviceTransferBytes[s1 + s2 + 1]);
                        if (s1 + s2 == gOvl10DeviceReadCursor) {
                            FntPrint("~c588%s~c558", buf);
                        } else {
                            FntPrint(buf);
                        }
                        s1++;
                    }
                }
                FntPrint("\n");
            }
            FntPrint("~c888");
            return 0;
        }

    case 2:
        {
            for (s1 = 0; s1 < 0x80; s1++) {
                gOvl10DeviceTransferBytes[0x21 + s1] = 0;
            }
        }
        return McxReadDev(0, gOvl10DeviceTransferBytes[0], &gOvl10DeviceTransferBytes[1], &gOvl10DeviceTransferBytes[0x21]);

    case 3:
        {
            for (s3 = 0; s3 < 8; s3++) {
                FntPrint(s3 != 0 ? "          " : "    read: ");
                s2 = s3 << 4;
                {
                    int s1;
                    for (s1 = 0; s1 < 0x10; s1++) {
                        sprintf(buf, "%02x",
                                gOvl10DeviceTransferBytes[s2 + 0x21 + s1]);
                        FntPrint("%s ", buf);
                    }
                }
                FntPrint("\n");
            }
            return 0;
        }

    default:
        return 0;
    }
}


int Ovl10HandlePdaDeviceWrite(int arg0, int arg1) {
    char buf[16];
    int s1;
    int s2;
    int s3;

    switch (arg0) {
    case 0:
        if (gOvl10DeviceWriteCursorLatch == 0) {
            if (arg1 & 0x2000) {
                if (gOvl10DeviceWriteCursor < 0x9F) {
                    gOvl10DeviceWriteCursor += 1;
                }
            } else if ((arg1 & 0x8000) && (gOvl10DeviceWriteCursor >= 0)) {
                gOvl10DeviceWriteCursor -= 1;
            }
            if (arg1 & 0x1000) {
                gOvl10DeviceWriteCursor -= 0x10;
                if (gOvl10DeviceWriteCursor < 0) {
                    gOvl10DeviceWriteCursor = -1;
                }
            } else if (arg1 & 0x4000) {
                if (gOvl10DeviceWriteCursor < 0) {
                    gOvl10DeviceWriteCursor = 0;
                } else if (gOvl10DeviceWriteCursor + 0x10 < 0xA0) {
                    gOvl10DeviceWriteCursor += 0x10;
                }
            }
        }
        gOvl10DeviceWriteCursorLatch = arg1 & 0xF000;

        if (arg1 & 0x10) {
            gOvl10DeviceWriteUpperIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10DeviceWriteUpperIncrementTicks) != 0) {
                gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] += 0x10;
            }
        } else {
            gOvl10DeviceWriteUpperIncrementTicks = 0;
            if (arg1 & 0x80) {
                gOvl10DeviceWriteUpperDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10DeviceWriteUpperDecrementTicks) != 0) {
                    gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] += 0xF0;
                }
            } else {
                gOvl10DeviceWriteUpperDecrementTicks = 0;
            }
        }

        if (arg1 & 0x20) {
            gOvl10DeviceWriteLowerIncrementTicks += 1;
            if (Ovl10IsPdaMenuRepeatTick(gOvl10DeviceWriteLowerIncrementTicks) != 0) {
                gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] =
                    (gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] & 0xF0) +
                    (((gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] & 0xF) + 1) & 0xF);
            }
        } else {
            gOvl10DeviceWriteLowerIncrementTicks = 0;
            if (arg1 & 0x40) {
                gOvl10DeviceWriteLowerDecrementTicks += 1;
                if (Ovl10IsPdaMenuRepeatTick(gOvl10DeviceWriteLowerDecrementTicks) != 0) {
                    gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] =
                        (gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] & 0xF0) +
                        (((gOvl10DeviceTransferBytes[gOvl10DeviceWriteCursor + 1] & 0xF) + 0xF) & 0xF);
                }
            } else {
                gOvl10DeviceWriteLowerDecrementTicks = 0;
            }
        }
        return 0;

    case 1: {
        FntPrint("    up/down/right/left:  pos up/down/right/left \n");
        FntPrint("    triang/square: inc/dec upper     circle/cross: inc/dec lower \n");
        FntPrint("    device no : %s%x~c888 \n", gOvl10DeviceWriteCursor == -1 ? "~c588" : "~c558",
                 gOvl10DeviceTransferBytes[0]);
        for (s3 = 0; s3 < 2; s3++) {
            FntPrint(s3 != 0 ? "          " : "    fix :~c558 ");
            s1 = 0;
            s2 = s3 << 4;
            while (s1 < 0x10) {
                sprintf(buf, "%02x ", gOvl10DeviceTransferBytes[s1 + s2 + 1]);
                if (s1 + s2 == gOvl10DeviceWriteCursor) {
                    FntPrint("~c588%s~c558", buf);
                } else {
                    FntPrint(buf);
                }
                s1++;
            }
            FntPrint("\n");
        }
        for (s3 = 0; s3 < 8; s3++) {
            FntPrint(s3 != 0 ? "          " : "    ~c888var :~c558 ");
            s1 = 0;
            s2 = s3 << 4;
            while (s1 < 0x10) {
                sprintf(buf, "%02x ", gOvl10DeviceTransferBytes[s2 + s1 + 0x21]);
                if (s2 + s1 == gOvl10DeviceWriteCursor - 0x20) {
                    FntPrint("~c588%s~c558", buf);
                } else {
                    FntPrint(buf);
                }
                s1++;
            }
            FntPrint("\n");
        }
        FntPrint("~c888");
        return 0;
    }

    case 2:
        return McxWriteDev(0, gOvl10DeviceTransferBytes[0], &gOvl10DeviceTransferBytes[1], &gOvl10DeviceTransferBytes[0x21]);

    case 3:
        Ovl10DrawNoPdaCommandData();
        return 0;

    default:
        return 0;
    }
}

/* Counter values 1, 41, 81, 96, ... trigger an edit. Retain the two
 * signed quotient/product tests and the caller's reset-on-clear protocol. */
s32 Ovl10IsPdaMenuRepeatTick(s32 inputOccurrences) {
    s32 repeat = 0;
    s32 quotient;

    if (inputOccurrences < 0x51) {
        if (((inputOccurrences / 40) * 0x28) != (inputOccurrences - 1)) {
            repeat = 0;
            goto ret;
        }
        goto block_4;
    }

    quotient = (inputOccurrences - 0x50) / 15;
    inputOccurrences -= 0x50;
    repeat = 0;
    if ((quotient * 0xF) == inputOccurrences - 1) {
block_4:
        repeat = 1;
    }
ret:
    return repeat;
}
