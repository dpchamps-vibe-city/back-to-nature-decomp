/*
 * Manual type overrides for auto-generated globals.
 *
 * classifyGlobals.ts generates globals.h with scalar types for all D_ symbols.
 * When a symbol's true type is known (e.g., it's a struct), define it here.
 * classifyGlobals.ts will skip any symbol that appears in this file.
 *
 * For absolute-addressed symbols (outside GP range), use the _D_ pattern:
 *   extern struct MyType _D_ADDR[1] __asm__("D_ADDR");
 *   #define D_ADDR (*((struct MyType*)_D_ADDR))
 *
 * For GP-relative symbols (within GP range), use plain extern:
 *   extern struct MyType D_ADDR;
 */
#ifndef GLOBALS_OVERRIDE_H
#define GLOBALS_OVERRIDE_H

/* exe:8001B258 dispatches two slots; 8001B2CC selects 8-byte presets and
 * 8001B3EC walks three-byte actuator steps. All four state arrays are GP-owned. */
typedef struct {
    u8 priority;
    u8 reserved1;
    u8 reserved2;
    u8 reserved3;
    s32 stepsAddress;
} PadActuatorSequencePreset;
extern PadActuatorSequencePreset gPadActuatorSequencePresets[];
extern void (*gPadActuatorSequenceDispatch[])(s32);
extern u16 gPadActuatorSequenceTicks[2];
extern s16 gPadActuatorSequenceModes[2];
extern s32 gPadActuatorSequenceAddresses[2];
extern s16 gPadActuatorSequencePriorities[2];

/* exe:80011EF0 saves/sets these s32 dispatch IDs; 80011F08/80011F14 query them. */
extern s32 gPreviousSceneId;
extern s32 gCurrentSceneId;
/* exe:80011C24 runs callbacks while this is 1; 80011EF0 clears it to exit. */
extern s32 gSceneUpdateLoopActive;

/* exe:80013328 requests a one-shot PutDispEnv; 80012D30 consumes it. */
extern s32 gScreenFadeFeedbackDispEnvPending;
/* exe:80013438/80013444 store/query this word; 80012D30 latches and tests 1. */
extern s32 gScreenFadeDrawLatch;
/* exe:80012D30 toggles this phase, used for half-rate feedback increments. */
extern s32 gScreenFadeFeedbackPhase;
/* exe:80012D30/8001328C-80013400 share these GP-relative fade controls. */
extern s16 gScreenFadeMode;          /* 8005E294: mode 0/1/2/3. */
extern s32 gScreenFadeSkipUpdate;    /* 8005E298: skip one driver call. */
extern s16 gScreenFadeBlendMode;     /* 8005E2A0: low two bits select GPU ABR. */
extern s16 gScreenFadeCounter;       /* 8005E3CC: current update step. */
extern s16 gScreenFadeDenominator;   /* 8005E3CE: intensity divisor/target. */
extern s32 gScreenFadeOtSelector;    /* 8005E3D0: frame OT if zero, else scene. */

/* exe:80013450-80013668: signed word countdown, intensity step, mode and skip. */
extern s32 gWideScreenFadeUpdatesRemaining;
extern s32 gWideScreenFadeIntensityStep;
extern s32 gWideScreenFadeMode;
extern s32 gWideScreenFadeSkipDraw;
/* exe:800134C4 views these base labels as two POLY_F4 / two DR_MODE packets.
 * Keep the existing scalar base-label declarations; packet views are local. */
extern s32 gWideScreenFadeQuadStorage;
extern s32 gWideScreenFadeDrawModeStorage;

/* 800BE118: four pairs of 0x1C-byte X/Z bounds, halfword tags and exit X/Z. */
typedef struct {
    s32 minX;
    s32 minZ;
    s32 maxX;
    s32 maxZ;
    u16 source;
    u16 destination;
    s32 exitX;
    s32 exitZ;
} Ovl11TransitionBounds;
extern Ovl11TransitionBounds gOvl11RegionExitBounds[4][2];

/* 800C2CE8 compares this absolute-addressed unsigned halfword to 4. */
extern u16 D_80071A3C;
/* ovl_11:800C30AC increments per pass; 800C2CE8 also increments per replay tick. */
extern s32 gOvl11ObjectScriptTick;
/* ovl_11:800C19B0 brackets schedule reconstruction with 0, then restores 1. */
extern s16 gOvl11ObjectScriptLiveMode;

/* 800E9D28: original lh/lhu/lw/sb/sh/sw offsets through the shared
 * work-area bases, with 0xB4/0xB8 scans and a 0xC-byte index table. */
typedef struct { s16 pad0; s16 f2; char pad4[8]; s16 fC; } E9Ptr;
typedef struct { s16 id; char pad[10]; } E9Small;
/* 800E9D28 indexes four 0xC-byte records from the work-area +0xE4E0. */
typedef struct { char pad[0xE4E0]; E9Small records[4]; } E9SmallView;
typedef struct { s32 flags; char pad[0xB0]; } E9Flags;
/* 800E9D28 publishes the record address, reads id +12 and flags +30. */
typedef struct { char pad0[0x12]; u16 id; char pad14[0x1C]; s32 flags; char pad34[0x80]; } E9Record;
typedef struct { char pad[0x7AB8]; E9Record records[10]; } E9RecordsView;
/* 800E9D28 scans twenty 0xB8-byte flags from D_80074838 +0x1F0. */
typedef struct { s32 flags; char pad[0xB4]; } E9Flags2;
/* 800E9D28 reads the signed halfword at upper-base +0x1D2. */
typedef struct { char pad[0x1D2]; s16 health; } E9Health;
typedef struct {
    char pad0[0xC];
    s32 fC;
    char pad10[0x20];
    E9Ptr * f30;
    char pad34[0x441C];
    s32 f4450;
    char pad4454[0x66];
    s16 f44BA;
    s16 f44BC;
    char pad44BE[0xE];
    u8 f44CC;
    char pad44CD[0x2B];
    s32 f44F8;
    char pad44FC[0x58A];
    u16 f4A86;
    char pad4A88[0x738];
    s32 f51C0;
    char pad51C4[0x64];
    s32 f5228;
    char pad522C[0x8];
    s32 f5234;
    char pad5238[0x2];
    s16 f523A;
    char pad523C[0x256];
    u16 f5492;
    char pad5494[0x2636];
    u16 f7ACA;
    char pad7ACC[0x1C];
    s32 f7AE8;
    char pad7AEC[0x6E6];
    s16 f81D2;
    char pad81D4[0xFEE];
    s16 f91C2;
    char pad91C4[0x10C];
    s32 f92D0;
    char pad92D4[0x704];
    u16 f99D8;
    char pad99DA[0x10];
    s16 f99EA;
    s16 f99EC;
    char pad99EE[0x18];
    u16 f9A06;
    char pad9A08[0x64];
    u8 f9A6C;
    char pad9A6D[0x2D31];
    s16 fC79E;
    char padC7A0[0x1D74];
    s16 fE514;
    char padE516[0xFE];
    s32 fE614;
    s16 fE618;
    char padE61A[0x2];
    u16 fE61C;
    char padE61E[0x2];
    void * fE620;
    s32 fE624;
    s16 fE628;
    s16 fE62A;
    s32 fE62C;
    s32 fE630;
    s16 fE634;
    char padE636[0x2];
    s32 fE638;
    s16 fE63C;
    char padE63E[0x12E];
    s16 fE76C;
    s16 fE76E;
    char padE770[0x2];
    s16 fE772;
    char padE774[0x6];
    s16 fE77A;
    s16 fE77C;
    char padE77E[0x2];
    s32 fE780;
} E9State;
typedef struct { char pad[0x25388]; E9Ptr *f25388; } E9Other;


/* 800E9D28 uses absolute lhu of this input-mask halfword. */
extern u16 D_800712BE;
/* 800E9D28 stores the halfword 100 at this absolute address. */
extern s16 D_80071A16;
/* 800E9D28 loads this selected-record index with signed lh. */
extern s16 D_80071CCA;

/* 800CC7CC copies four position words to the spawn at owner +18. */
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} Ovl11CC7CCPosition;

/* 800CA708: direction/mode at +38/+3C, word flags +6C, four-word
 * position +100 and velocity +110; two tables of signed halfword pairs. */
typedef struct {
    char pad0[0x38];
    u16 direction;
    char pad3A[2];
    u16 mode;
    char pad3E[0x6C - 0x3E];
    s32 flags;
    char pad70[0x100 - 0x70];
    Ovl11CC7CCPosition position;
    s32 velocity[4];
} Ovl11PrimaryMotionObject;
typedef struct {
    s16 x, z;
} Ovl11SlideOffset;
extern Ovl11SlideOffset gOvl11MotionProbeOffsets[8];
extern Ovl11SlideOffset gOvl11MotionSlideVelocities[8];
/* 800CAADC selects four signed X/Z pairs at 80123100 or base -0x30. */
extern Ovl11SlideOffset gOvl11MotionSlideOffsets[4];
/* 800CAADC projects into two words at 80128B90 and passes this short vector. */
extern s32 gOvl11MotionScreenPoint[2];
extern s16 D_800A03A4[4];
/* 800CAADC reads flags +C/+10 and position halfwords +52C8/+52CC/+52D0. */
typedef struct {
    char pad0[0xC];
    s32 flagsC;
    s32 flags10;
    char pad14[0x51C8 - 0x14];
    Ovl11PrimaryMotionObject player;
} Ovl11MovementState;
/* 800CAADC copies three halfword coordinates at D_8007AFF0 +0x253B4. */
typedef struct {
    char pad0[0x253B4];
    u16 coordinates[3];
} Ovl11MovementCameraWork;

typedef struct {
    u16 id;
    u16 kind;
    u16 flags;
    u16 pad6;
    Ovl11CC7CCPosition position;
} Ovl11CC7CCSpawn;

typedef struct {
    char pad0[0x18];
    Ovl11CC7CCSpawn spawn;
} Ovl11CC7CCSpawnOwner;

/* 800CCB90 walks three 0x34-byte records at +2; retain the siblings' byte-array linkage view. */
extern u8 gOvl11MovingItemRecords[];

/* 800CC7CC/800CBF70 index four pairs of signed words; retain the byte-array linkage view. */
extern u8 gOvl11MovingItemDirectionVelocity[0x20];
/* 800CC7CC loads this shared entity selector with lhu. */
extern u16 D_80071AFE;

/* 800CE550 reads five signed halfwords at D_801232C8 with stride two. */
extern s16 gOvl11PositionHeightTolerances[5];
/* CF314 writes XYZ words here and the record's two preceding halfwords. */
extern u16 gOvl11TimedEffectPositionWords[];

/* 800D2DB8 loads this signed word and compares it with 1. */
extern s32 gOvl11MovementVolumeProbeEnabled;
/* 800D12B8 passes the timer word to HasVSyncIntervalElapsed and toggles gear. */
extern s32 gOvl11MovementSpeedTimer;
extern s32 gOvl11MovementSpeedGear;
/* D1C18 indexes four 0x120-byte direction slabs of 4x9x4 u16 weights. */
extern u8 gOvl11DirectionChoiceWeights[];

/* ovl_11 object-state callbacks: exclusive packed {s16 code,duration} tables
 * in text/member order at 80123BCC–80123CA0; scalar byte anchors retain the
 * existing address-taking view. State06 retains its 12-byte array view. */
extern u8 gOvl11State00Keyframes;
extern u8 gOvl11State02Keyframes;
extern u8 gOvl11State03Keyframes;
extern u8 gOvl11State04Keyframes;
extern u8 gOvl11State08Keyframes;
extern u8 gOvl11State09Keyframes;
extern u8 gOvl11State11Keyframes;
extern u8 gOvl11State12Keyframes;
extern u8 gOvl11State10Keyframes;
extern u8 gOvl11State13Keyframes;
extern u8 gOvl11State14Keyframes;
extern u8 gOvl11State15Keyframes;
extern u8 gOvl11State16Keyframes;
extern u8 gOvl11State17Keyframes;
extern u8 gOvl11State07Keyframes;
extern u8 gOvl11State06Keyframes[12];

/* ovl_11:0x80128810: counted collision queries since the last frame-driver reset.
 * Storage remains in the overlay's original data section, not in the C module. */
extern s32 gOvl11CollisionQueryCount;

/* exe:80011F2C/80011F38 store/load this byte with sb/lbu; .sdata initially holds 2. */
extern u8 gUpdateTimingMode;

/* Rand defines this u32 seed; ovl_11 801044E4 stores 1 with sw. */
extern u32 gRandomState;

/* 801044E4: word flags/pointer at +C/+1C and signed state halfwords. */
typedef struct {
    u8 pad_0[0xC];
    s32 flags;
    u8 pad_10[0xC];
    s32 *reset;
    u8 pad_20[0x4438 - 0x20];
    s16 field_4438;
    u8 pad_443A[0x44BA - 0x443A];
    s16 field_44BA;
    u8 pad_44BC[0x4502 - 0x44BC];
    s16 field_4502;
    u8 pad_4504[4];
    s16 field_4508;
    u8 pad_450A[0x51C8 - 0x450A];
    u8 field_51C8[0x5200 - 0x51C8];
} Ovl11StartupView;

/* ovl_11 80104A90: word flags/counters, signed state halfwords and u16 at +44CE. */
typedef struct {
    u8 pad_0[0xC];
    s32 flags;
    u8 pad_10[0x4438 - 0x10];
    s16 field_4438;
    u8 pad_443A[0x4450 - 0x443A];
    s32 field_4450;
    u8 pad_4454[0x4488 - 0x4454];
    s32 field_4488;
    u8 pad_448C[0x44BA - 0x448C];
    s16 field_44BA;
    u8 pad_44BC[4];
    s16 field_44C0;
    u8 pad_44C2[0x44CE - 0x44C2];
    u16 field_44CE;
    u8 pad_44D0[0x51C8 - 0x44D0];
    u8 field_51C8[0x5214 - 0x51C8];
    s32 field_5214;
    u8 pad_5218[0xE4C8 - 0x5218];
    s16 field_E4C8;
} Ovl11AreaInitView;

/* ovl_11 80104A90: three halfword sentinels and the far pointer/state slots. */
typedef struct {
    u8 pad_0[0x2479C];
    s16 field_2479C;
    u8 pad_2479E[8];
    s16 field_247A6;
    s16 field_247A8;
    u8 pad_247AA[0x25388 - 0x247AA];
    u8 *data;
    void *buffer;
    s32 field_25390;
    u8 pad_25394[0x2547C - 0x25394];
    s32 field_2547C;
    u8 pad_25480[4];
    s32 field_25484;
    s32 field_25488;
    s32 field_2548C;
    u8 pad_25490[8];
    s32 field_25498;
    s32 field_2549C;
    s32 field_254A0;
} Ovl11FarInitView;

/* ovl_11 80104A90 clears 0x10 bytes and publishes this buffer's address. */
extern u8 gAreaSetupBuffer[0x10];
/* ovl_11:800BC3E0 dispatches entry steps; 801048A0/80104A90 enter at 10. */
extern s32 gOvl11AreaEntryStep;

/* ovl_11:80126FF0 — eighteen 0x1C-byte item-menu layout rows. Each row
 * starts with pixel x/y, then eight triples of unsigned navigation indices.
 * Preserve both typed-navigation and halfword-coordinate views of one table. */
struct Ovl11ItemTransferLayoutRow {
    s16 x;
    s16 y;
    u8 destinations[24];
};
extern struct Ovl11ItemTransferLayoutRow gOvl11ItemTransferNavigation[] __asm__("gOvl11ItemTransferLayout");
extern s16 gOvl11ItemTransferLayout[][14];
/* Four signed x/y/width/height rectangles at ovl_11:801271E8. */
extern s16 gOvl11ItemTransferPanels[][4];
/* 800FAC0C adds the language byte offset to this label and its +0xC successor. */
extern u8 D_80051B7C[];
/* Item-transfer cursor, saved cursor during a transfer, and selected source
 * (FF means no source). Word widths follow FAC0C's lw/sw. */
extern s32 gOvl11ItemTransferCursor;
extern s32 gOvl11ItemTransferSavedCursor;
extern s32 gOvl11ItemTransferSelectedSlot;

/* ovl_11 800C5048/800C5098 store/load the counter, limit and result as words. */
extern s32 gOvl11CellActionProgress;
extern s32 gOvl11CellActionLimit;
extern s32 gOvl11CellActionScore;
/* ovl_11 800C5048 writes these halfwords; 800C5098 reads coordinates and modes. */
extern s16 gOvl11CellActionColumn;
extern s16 gOvl11CellActionRow;
extern s16 gOvl11CellActionMode;
extern s16 gOvl11CellActionOperation;
extern s16 gOvl11CellActionFilter;

/* ovl_11 800FFA64 reads its state with lh and advances it with sh. */
extern s16 gOvl11MenuChoiceState;
/* ovl_11 800FFA64 stores the menu return and compares it with signed -1 using lw. */
extern s32 gOvl11MenuChoiceResult;

/* 800C6A9C stores the sound helper's return value with sh. */
/* Action input stores PlaySoundEffect's handle; primary update stops that voice. */
extern s16 gOvl11ActiveItemActionSound;

/* 800BE4A8 reads signed halfwords at D_80071A00+FE/+100/+104/+108. */
typedef struct {
    u8 pad0[0xFE];
    s16 region;
    s16 x;
    u8 pad102[2];
    s16 y;
    u8 pad106[2];
    s16 z;
} Ovl11PlayerCollisionView;

/* 800CB618 indexes s16 triples at D_80071A00+0xC0; 800C9D98 supplies count 8. */
typedef struct {
    char pad[0xC0];
    s16 records[8][3];
} Ovl11CB618RecordView;

/* 80120A2C: phase and x/z pan; 801209D4 accumulates the vertical step at +4/+8. */
typedef struct {
    u16 phase;
    u8 pad2[2];
    s32 verticalStep;
    s32 verticalAccumulator;
    s32 panX;
    u8 pad10[4];
    s32 panZ;
} Ovl11MapCameraState;
extern s16 gOvl11MapCameraState;

/* 80120A2C writes camera words/halfwords and the exit flag through this far base. */
struct Ovl11MapCameraWork {
    u8 pad0[0x25394];
    s32 field_25394;
    s32 field_25398;
    u8 pad2539C[4];
    s32 field_253A0;
    u8 pad253A4[0x10];
    u16 field_253B4;
    u8 pad253B6[2];
    u16 field_253B8;
    u8 pad253BA[0x2548C - 0x253BA];
    s32 field_2548C;
};

/* 80121134 passes this UV/CLUT buffer to 80120CC4 and 80120F30. */
extern struct Ovl11TextureCoordinates gOvl11MapTextureScratch;

/* 800DB140/800DB5CC: leading u16 of eight 0x16-byte slots at this base. */
/* 800DB0E4 clears a four-byte prefix plus eight 0x16-byte animation slots. */
extern u8 gVramAnimationStoragePrefix[1];
extern u16 gVramAnimationSlotHead;

/* ovl_11 80118B84 loads this selection with lh and resets it with sh. */
extern s16 gOvl11CompactChoiceSelection;

/* ovl_11 800BF4B4/800CE034 read signed halfword pairs at four-byte strides. */
extern u8 gOvl11AreaGridCellOffsets[];

/* ovl_11 800F77F0 uses lh for this state threshold and sh for updates. */
/* 800F77F0: first sprite ID plus 0x400 while a batch read is pending. */
extern s16 gOvl11TextureStreamCursor;

/* ovl_11 80116878 loads this word with lw for a signed threshold comparison. */
extern s32 gSelectedThresholdBalance;

/* ovl_11 E0B40/E0C24 index complementary 20-word request/update tables. */
extern s32 (*gOvl11TaggedRecordRequestHandlers[20])(void *);
extern s32 (*gOvl11TaggedRecordStepHandlers[20])(void *);

/* E0F8C reads animation/frame halfwords: six rows, four descriptors per row. */
typedef struct {
    u16 animation;
    u16 frame;
} Ovl11TaggedRecordSpriteDescriptor;
extern Ovl11TaggedRecordSpriteDescriptor gOvl11TaggedRecordSpriteDescriptors[6][4];

/* E180C..E1CA8 step these private packed code/duration tables in member order. */
extern u8 gOvl11TaggedRecordHandler0Keyframes[8];
extern u8 gOvl11TaggedRecordHandler1Keyframes[16];
extern u8 gOvl11TaggedRecordHandler2Keyframes[12];
extern u8 gOvl11TaggedRecordHandler3Keyframes[12];
extern u8 gOvl11TaggedRecordHandler4Keyframes[12];
extern u8 gOvl11TaggedRecordHandler5Keyframes[12];
extern u8 gOvl11TaggedRecordHandler14Keyframes[8];

/* 8010B57C calls nullable entry gates; 8010B4D8 indexes raw step-handler
 * address words by the record's +26. Keep the original raw-word step view. */
extern s32 (*gOvl11StageMode2EntryHandlers[20])(u16 *);
extern s32 gOvl11StageMode2StepHandlers[20];
/* BD6C/BE24/BED4 pass these private packed code/duration streams to D04D4. */
extern u8 gOvl11StageMode2KeyframesA[8];
extern u8 gOvl11StageMode2KeyframesB[12];
extern u8 gOvl11StageMode2KeyframesC[8];

/* 800D3104 calls a nullable entry gate with the same u16-record pointer. */
extern s32 (*gOvl11ObjectStateEntryCallbacks[21])(u16 *);
/* 800D3048/800D3104 load 21 raw callback-address words, casting at dispatch. */
extern s32 gOvl11ObjectStateFrameCallbacks[21];

/* 8001B5DC loads this fog distance with lw; SetVal8005E2D8 stores s32. */
extern s32 gSceneFogNear;
/* 8001B5DC loads these colors with lbu; 8001BF74/8001BF88 store bytes. */
extern u8 gSceneBackRed;
extern u8 gSceneBackGreen;
extern u8 gSceneBackBlue;
extern u8 gSceneFarRed;
extern u8 gSceneFarGreen;
extern u8 gSceneFarBlue;
/* 1B530 clears 32 bytes; 1B5DC passes these addresses to GTE matrix macros.
 * Keep the existing scalar address views; no storage/layout change is implied. */
extern s32 gSceneLightMatrix;
extern s32 gSceneColorMatrix;

/* ovl_30 8012F084 reads a signed word through D_8007AFF0 + 0x2547C. */
struct Ovl30SettingsView {
    u8 pad[0x2547C];
    s32 selected;
};

/* ovl_30 8012F030 supplies this four-halfword rectangle to StoreImage/LoadImage. */
extern s16 gOvl30DebugDisplayRect[4];

/* Ovl30UpdateDebugMenu publishes a changed selection; Ovl11StepAreaModeState
 * consumes it when game flag 0x9C is set. */
extern s32 gDebugMenuRequestedSelection;
/* ovl_30 8012F084 clears this halfword with sh. */
extern s16 D_80133FBC;
/* ovl_30 8012F084 indexes 18 callbacks; one table member takes no arguments. */
extern s32 (*gOvl30DebugMenuHandlers[18])();
/* ovl_30 8012EFEC clears this word and 8012F084 tests it. */
extern s32 gOvl30DebugMenuPendingChange;
/* 80133A8C divides this signed menu-selection word by nine and updates it with sw. */
extern s32 gOvl30RecordMenuCursor;
/* 8012FEEC prints this NUL-terminated Shift-JIS rodata heading. */
extern char gOvl30MainDataTextPool[];
/* 8012FEEC passes this 20-byte record base to 80133454 with count 22. */
extern struct Ovl30ValueMenuEntry gOvl30MainDataMenu;
/* 8012FF50 prints this NUL-terminated Shift-JIS rodata heading. */
extern char gOvl30PlayerAndActionTextPool[];
/* 8012FF50 passes this 20-byte menu-record base to 80133454 with count 23. */
extern struct Ovl30ValueMenuEntry gOvl30PlayerDataMenu;
/* 80130690 prints this NUL-terminated Shift-JIS rodata heading. */
extern char gOvl30PetDataHeading[];
/* 80130690 passes this 20-byte menu-record base to 80133454 with count 48. */
extern struct Ovl30ValueMenuEntry gOvl30PetDataMenu;
/* 80132660 prints this NUL-terminated Shift-JIS rodata heading. */
extern char gOvl30SystemFlagHeading[];
/* 80132660 passes this 20-byte menu-record base to 80133A8C with count 12. */
extern struct Ovl30FlagMenuEntry gOvl30SystemFlagMenu;
/* 80133A8C copies the two string pointers in this rodata table to its local array. */
extern char *gOvl30RowKindMarkers[2];
/* 80133A8C passes these rodata formats and selection markers to KanjiFntPrint. */
extern char gOvl30HeaderRowFormat[];
extern char gOvl30CursorRowMarker[];
extern char gOvl30IdleRowMarker[];
extern char gOvl30ValueRowFormat[];
extern char gOvl30MenuBlankLine[];

/* 80130C20 indexes stride-B4 records; menu widths witness halfwords and words. */
typedef struct Ovl30AnimalRecord {
    u16 number;
    u8 pad2[0x16 - 2];
    u16 affection;
    u8 pad18[2];
    u16 birth[3];
    u8 pad20[4];
    u16 gestation;
    u16 action;
    u16 process_no;
    u16 process_id;
    u16 process_time;
    u16 elapsed[2];
    u8 pad32[2];
    u32 flags;
    s32 position[3];
    u8 pad44[4];
    s32 speed[3];
    u8 pad54[4];
    s32 target[3];
    u8 pad64[0xAC - 0x64];
    u16 grazing;
    u16 sick;
    u16 growth;
    u8 padB2[2];
} Ovl30AnimalRecord;
/* 80130C20 forms field addresses in this record array, retaining the legacy scalar alias. */
/* D3D60/D3E40 select ten 0xB4-byte records here; preserve the existing
 * three-word array-backed base lvalue and its declaration extent. */
extern s32 _gSmallTaggedRecordBase[3] __asm__("gSmallTaggedRecordBase");
#define gSmallTaggedRecordBase (*((s32 *)_gSmallTaggedRecordBase))
#define Ovl30_AnimalRecords ((Ovl30AnimalRecord *)_gSmallTaggedRecordBase)
/* 801306F4 indexes stride-B8 records and supplies these halfword/word field widths. */
typedef struct Ovl30AnimalRecordB8 {
    u16 number;
    u8 pad2[0x16 - 2];
    u16 affection;
    u8 pad18[2];
    u16 birth[3];
    u8 pad20[4];
    u16 gestation;
    u16 action;
    u16 process_no;
    u16 process_id;
    u16 process_time;
    u16 elapsed[2];
    u8 pad32[2];
    u32 flags;
    s32 position[3];
    u8 pad44[4];
    s32 speed[3];
    u8 pad54[4];
    s32 target[3];
    u8 pad64[0xAC - 0x64];
    u16 grazing;
    u16 sick;
    u16 growth;
    u16 growth_time;
    u16 pregnancy;
    u8 padB6[2];
} Ovl30AnimalRecordB8;
/* Twenty 0xB8 tagged records: D0C34 allocates, D0D7C finds the slot.
 * Retain the existing 12-byte backing/lvalue declaration, not a new pool extent. */
extern s32 _gTaggedRecordBase[3] __asm__("gTaggedRecordBase");
#define gTaggedRecordBase (*((s32*)_gTaggedRecordBase))
/* 801306F4 forms menu field addresses from the same record base. */
#define Ovl30_AnimalRecords749F4 ((Ovl30AnimalRecordB8 *)&gTaggedRecordBase)
/* 801306F4 reads this selector with lw and edits it as a four-byte value. */
extern s32 gOvl30CowSheepIndex;
/* 801306F4 installs these NUL-terminated Shift-JIS labels and print format. */
extern char gOvl30AnimalGrowthDaysLabel[];
extern char gOvl30AnimalDaysSinceShearingLabel[];
extern char gOvl30CowSheepHeading[];
/* 80130C20 reads this record selector with lw and edits it as a four-byte value. */
extern s32 gOvl30ChickenIndex;
/* 80130C20 installs these NUL-terminated Shift-JIS labels in its local menu. */
extern char gOvl30AnimalNumberLabel[];
extern char gOvl30AnimalAffectionLabel[];
extern char gOvl30AnimalBirthMonthLabel[];
extern char gOvl30AnimalBirthDayLabel[];
extern char gOvl30AnimalGrazingTimeLabel[];
extern char gOvl30AnimalSicknessDaysLabel[];
extern char gOvl30AnimalPregnancyDaysLabel[];
extern char gOvl30AnimalActionLabel[];
extern char gOvl30AnimalProcessNumberLabel[];
extern char gOvl30AnimalProcessIdLabel[];
extern char gOvl30AnimalProcessTimeLabel[];
extern char gOvl30AnimalInterruptTimeLabel[];
extern char gOvl30AnimalCurrentLocationLabel[];
extern char gOvl30AnimalFlagsLabel[];
extern char gOvl30AnimalXLabel[];
extern char gOvl30AnimalYLabel[];
extern char gOvl30AnimalZLabel[];
extern char gOvl30AnimalSpeedXLabel[];
extern char gOvl30AnimalSpeedYLabel[];
extern char gOvl30AnimalSpeedZLabel[];
extern char gOvl30AnimalDestinationXLabel[];
extern char gOvl30AnimalDestinationYLabel[];
extern char gOvl30AnimalDestinationZLabel[];
/* 80130C20 passes this NUL-terminated Shift-JIS format to KanjiFntPrint. */
extern char gOvl30ChickenHeading[];
/* 80131100 forms the same stride-B4 menu fields from this second record base. */
#define Ovl30_AnimalRecords75FE4 ((Ovl30AnimalRecord *)&gSmallRecordTripletBase)
/* 80131100 reads this selector with lw and edits it as a four-byte value. */
extern s32 gOvl30CoopChickenIndex;
/* 80131100 passes this NUL-terminated Shift-JIS format to KanjiFntPrint. */
extern char gOvl30CoopChickenHeading[];

/* 801320C8's 0xF8-stride menu addresses witness these halfword/word fields. */
typedef struct Ovl30RecordF8 {
    u8 pad0[0x16];
    u16 affection;
    u8 pad18[0xC];
    u16 gestation;
    u16 action;
    u16 process_no;
    u16 process_id;
    u16 process_time;
    u16 elapsed[2];
    u8 pad32[2];
    u32 flags;
    s32 position[3];
    u8 pad44[4];
    s32 speed[3];
    u8 pad54[4];
    s32 target[3];
    u8 pad64[0xAE - 0x64];
    u16 field_AE;
    u16 field_B0;
    u16 field_B2;
    u16 field_B4;
    u16 field_B6;
    u16 field_B8;
    u16 field_BA;
    u8 padBC[0xF8 - 0xBC];
} Ovl30RecordF8;
#define Ovl30_RecordsA638 ((Ovl30RecordF8 *)gObjectRecordPool)
/* 801320C8 loads this record selector with lw and edits it with width four. */
extern s32 gOvl30KorobokkuruIndex;
/* 801320C8 installs these NUL-terminated rodata labels and print format. */
extern char gOvl30HarvestTrainingLabel[];
extern char gOvl30WateringTrainingLabel[];
extern char gOvl30AnimalCareTrainingLabel[];
extern char gOvl30WorkDaysLabel[];
extern char gOvl30HeldItemLabel[];
extern char gOvl30TargetLivestockLabel[];
extern char gOvl30MoveSp0Label[];
extern char gOvl30MoveSp1Label[];
extern char gOvl30MoveSp2Label[];
extern char gOvl30MoveSp3Label[];
extern char gOvl30MoveSp4Label[];
extern char gOvl30KorobokkuruAndFlagTextPool[];
/* ovl_30 8012F084 maintains this word-sized active flag. */
extern s32 gOvl30DebugMenuWaitingForExit;
/* ovl_30 8012F084 loads this word-sized index; decrement uses a signed zero clamp. */
extern s32 gOvl30DebugPageIndex;
/* ovl_30 8012F084 passes this word to the selected callback and stores its result. */
extern s32 gOvl30DebugPageState;
/* ovl_30 8012F084 dispatches on this word-sized state. */
extern s32 gOvl30DebugMenuState;
/* ovl_30 8012F084 resets this word to -1 on a selection change. */
extern s32 gOvl30RecordMenuSelectedId;
/* ovl_30 8012F084 compares and copies this word-sized setting. */
extern s32 gOvl30DebugSettingValue;
/* ovl_30 8012F084 publishes the callback loaded from D_80133FC0. */
extern s32 (*gOvl30ActiveDebugMenuHandler)();

/* 8010021C wraps signed byte selectors; named views preserve legacy declarations. */
extern s8 Ovl11_D_801273E7 __asm__("D_801273E7");
extern s8 Ovl11_D_801273E8 __asm__("D_801273E8");
/* 8010021C reads the first u16 of 0x16-byte records at base +0x4AE0. */
typedef struct {
    u8 pad_0[0x4AE0];
    u16 records[80][11];
} Ovl11SelectionRecords;
/* 801035C0 copies eight IDs after each record's three-halfword header. */
typedef struct {
    u16 id;
    u16 value;
    u16 flags;
    u16 ids[8];
} Ovl11SelectionRecord;
typedef struct {
    u8 pad_0[0x4AE0];
    Ovl11SelectionRecord records[80];
} Ovl11SelectionRecordArray;
/* 80100644 loads this selector with lbu; 800FFDCC clears the same byte. */
extern s8 gOvl11SelectionRecordIndex;
/* 801007AC copies the selected record into this byte with sb. */
extern u8 gOvl11SelectionConfirmedRecord;
/* 80100644 installs void handlers called by 800FFF7C; named view preserves legacy declarations. */
extern void (*Ovl11_D_801273DC)(void) __asm__("gOvl11SelectionHandler");
/* 800FFF7C compares/latches the installed handler; 800FFDCC clears it. */
extern void (*gOvl11LastSelectionHandler)(void);
/* 800FFDCC clears the trailing state byte and halfword with sb/sh. */
extern s8 D_801273EB;
extern s16 D_80127424;

/* Shared work+0x10: record-frame, object-event, action-result and effect
 * services test independent bits with word loads; storage remains resident. */
extern s32 gSharedWorkFlags;

/* 8012135C scans 25 effect records at stride 0x18; named view preserves existing pool types. */
/* 801212C4/80121318 visit 25 effects at 0x18-byte stride; +14 is the free marker. */
extern u8 gOvl11SpriteEffects[0x258];

/* 800C3138 indexes four original signed heading words with an unsigned halfword selector. */
extern s32 gOvl11ReactionHeadingAngles[4];
/* 800C3B78 selects these signed word scales before rsin/rcos motion. */
extern s32 gOvl11CommandMotionScale;
extern s32 gOvl11ReducedCommandMotionScale;
/* 800C3138 stores the squared distance and resets the best-distance word (sw). */
extern s32 gOvl11ReactionDistanceSquared;
extern s32 gOvl11ReactionSearchSentinel;

/* ovl_11_func_800C35EC reads this absolute-addressed direction value with lhu. */
extern u16 gPrimaryObjectFacing;

/* ovl_17_func_800B8028 indexes 11 original callbacks; all entries ignore incoming arguments. */
extern void (*gOvl17RecordSceneStages[11])(void);
/* ovl_17_func_800B8028 reads this dispatch selector with lbu. */
extern u8 gOvl17RecordSceneStageIndex;

/* ovl_17_func_800B9FA8 copies 0x13DC bytes here, then uses the leading sprite header. */
extern u8 gOvl17SharedSpriteBank[0x13DC];
/* ovl_17_func_800B9FA8 passes this 0x30-byte object to func_80015704. */
extern u8 gOvl17SharedBankSpriteSource[0x30];

/* 80120CC4 indexes original 48-halfword texture codes with lhu and stride 2. */
extern u16 gOvl11MapTileTextureCodes[48];

/* ovl_11_func_800F4CA0 clears this s16; 800F62D8 reads its unsigned value with lhu. */
/* Item-interaction latch: the same storage as resident work+0x44D6. */
extern s16 gQueuedItemInteractionId;

/* Minimal SPU types for the D_8006C368 override below. Do not include
 * psyq/libspu.h here: its `void SpuGetAllKeysStatus` prototype reaches every
 * TU, but the original sound TU observes the call's return value (see
 * src/func_800212A8.c). Layout matches the PSY-Q 4.7 definitions exactly. */
typedef struct {
    short left;
    short right;
} SpuVolumeO;
typedef struct {
    SpuVolumeO volume;
    long reverb;
    long mix;
} SpuExtAttrO;
typedef struct {
    unsigned long mask;
    SpuVolumeO mvol;
    SpuVolumeO mvolmode;
    SpuVolumeO mvolx;
    SpuExtAttrO cd;
    SpuExtAttrO ext;
} SpuCommonAttrO;

/* ovl_11 data spans 0x230 bytes; 800C02B0 scans 40 records of stride 0xE. Named view preserves legacy scalar/array declarations in other TUs. */
extern u8 gOvl11WeatherParticles[0x230];

/* ovl_11_func_80120F30 indexes flag bytes; the original data span is 8 bytes. */
extern u8 gOvl11MapColumnUvOrientation[8];

/* ovl_11 grid at D_8007AFF0+0x23608: word header, UV bytes and u16 fields. */
typedef struct {
    u32 header;
    u8 u0, v0;
    u16 clut;
    u8 u1, v1;
    u16 tpage;
    u8 u2, v2;
    u16 pad_E;
    u8 u3, v3;
} Ovl11GridTextureEntry;

typedef struct {
    u8 pad_0[0x23608];
    Ovl11GridTextureEntry *entries[1];
} Ovl11GridTextureTableView;

/* 800D806C and 80113208 index the same pointer grid by row and column:
 * base+0x23608 is added before the scaled index (addu base,off then +idx*4). */
typedef struct {
    u8 pad_0[0x23608];
    Ovl11GridTextureEntry *entries[25][45];
} Ovl11GridTextureRowsView;

/* 8011E1E8 reads a signed mode at +25476 beside the 25x45 pointer grid. */
typedef struct {
    u8 pad_0[0x23608];
    Ovl11GridTextureEntry *entries[25 * 45];
    u8 pad_2479C[0x25476 - 0x2479C];
    s16 mode;
} Ovl11TextureStateView;

/* 80112DA8 tests signed halfwords at work-area +0x25474/+0x25476;
 * 80113818 reads/writes the words at +0x25480 and +0x254A0. */
typedef struct {
    u8 pad_0[0x25474];
    s16 field_25474;
    s16 field_25476;
    u8 pad_25478[8];
    s32 field_25480;
    u8 pad_25484[0x1C];
    s32 field_254A0;
} Ovl11TriggerWorkView;

/* 800B7F3C copies four bytes from D_8007AFF0+0x2479C to +0x25474;
 * 801048A0 and 800BB628 witness the leading and trailing halfwords. */
typedef struct {
    s16 field_0;
    s16 field_2;
} Ovl25SetupStatePair;

/* 800DC560 copies the state pair and accesses these far pointer/ID slots. */
typedef struct {
    u8 pad0[0x2479C];
    Ovl25SetupStatePair previous;
    u8 pad247A0[6];
    s16 music;
    u8 pad247A8[0x25388 - 0x247A8];
    u8 *header;
    u8 *buffer;
    u8 *record;
    u8 pad25394[0x25474 - 0x25394];
    Ovl25SetupStatePair current;
    u8 pad25478[4];
    s32 id;
    u8 pad25480[4];
    s32 normalId;
    s32 alternateId;
    u8 pad2548C[0x14];
    s32 state;
} Ovl11SetupWorkView;

/* 800DC560: flag words, music/time halfwords and three signed coordinates. */
typedef struct {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x4450 - 0x10];
    s32 control;
    u8 pad4454[0x44BA - 0x4454];
    u16 music;
    u8 pad44BC[0xE];
    s16 time;
    u8 pad44CC[0x52C8 - 0x44CC];
    s32 x;
    s32 y;
    s32 z;
} Ovl11SetupResidentView;

/* 800BDB88 tests the signed +25476 type and forwards the +25474 pair. */
typedef struct {
    u8 pad_0[0x25474];
    Ovl25SetupStatePair collision;
} Ovl11CollisionWorkView;

/* 800F4E84 compares signed tags at +0/+18 and probes positions at +8/+20. */
typedef struct {
    s16 type;
    u8 pad2[6];
    s32 position[3];
    u8 pad14[4];
} Ovl11PoolGateRecord;

/* 800F4E84 tests the +44F8 word and follows the pool pointer at +DDD4. */
typedef struct {
    u8 pad0[0x44F8];
    s32 flags;
    u8 pad44FC[0xDDD4 - 0x44FC];
    void *entry;
} Ovl11PoolGateState;

/* 800F4D28 tests lw +44F8 / lh +5252 and updates u16 +4 through +DDD4. */
typedef struct {
    u8 pad0[0x44F8];
    s32 flags;
    u8 pad44FC[0x5252 - 0x44FC];
    s16 selected;
    u8 pad5254[0xDDD4 - 0x5254];
    u16 *record;
} Ovl11SpawnProbeState;

/* 800D759C indexes 30 ten-byte UV/clut records here (lbu +0..7, lhu +8). */
extern u8 gOvl11MapCellUvRecords[300];
/* 800D759C reads a per-column flag with lbu; original data spans 48 bytes. */
extern u8 gOvl11MapColumnMirror[48];

/* 8011E1E8 indexes ten-byte records here: eight UV bytes and lhu at +8. */
extern u8 gOvl11Mode6TextureCoords[];

/* ovl_31 rodata: NUL-terminated "bu%d%d:%s" used by sprintf. */
extern char D_800B7F20[10];
/* ovl_31 rodata: NUL-terminated old FARM0 memory-card filename. */
extern char D_800B7F2C[18];
/* ovl_31 rodata: NUL-terminated new FARM0 memory-card filename. */
extern char D_800B7F40[18];
/* ovl_31 rodata: old FARM1 filename; ovl_11 has a different object at this address. */
extern char Ovl31_D_800B7F54[18] __asm__("D_800B7F54");
/* ovl_31 rodata: NUL-terminated new FARM1 memory-card filename. */
extern char D_800B7F68[18];
/* ovl_31 rodata: NUL-terminated old FARM2 memory-card filename. */
extern char D_800B7F7C[18];
/* ovl_31 rodata: NUL-terminated new FARM2 memory-card filename. */
extern char D_800B7F90[18];
/* ovl_31 rodata: NUL-terminated old FARMP memory-card filename. */
extern char D_800B7FA4[18];
/* ovl_31 rodata: NUL-terminated new FARMP memory-card filename. */
extern char D_800B7FB8[18];
/* ovl_31 data allocates 32 bytes for the first sprintf/rename path buffer. */
extern char gOvl31RenameSourcePath[32];
/* ovl_31 data allocates 32 bytes for the second sprintf/rename path buffer. */
extern char gOvl31RenameDestinationPath[32];

/* Complete per-pad input layout is in game_types.h. */
struct PadInputState;

/* Ovl11ProcessPendingStageMode reads with lh and resets with sh to -1. */
extern s16 gOvl11PendingStageMode;

/* 800CC0F8 projects to two signed words, then reads their low halfwords. */
extern s32 gOvl11MovingItemScreenPosition[2];
/* 800CC0F8 stores three halfwords and passes the vector to 8001DFD4. */
extern s16 gOvl11MovingItemProjectionInput[4];
/* ovl_11_func_800CC4E0 initializes this sprite via func_80015704. */
extern struct SpriteSourceData gOvl11MovingItemSpriteSource;
/* ovl_11_func_800CC4E0 stores a word flag when object id is 0x186. */
extern s32 gOvl11HeldActionSignal;
/* 800CDD7C clears three halfwords; 80120850 reads these coordinates. */
typedef struct {
    u16 x;
    u16 y;
    u16 z;
    u16 w;
} Ovl11EffectCoordinates;
extern Ovl11EffectCoordinates gOvl11HeldActionSpritePosition;
/* 800CDD7C writes this mode word; 80120850 reads its low byte. */
extern s32 gOvl11HeldActionSpriteHeader;
/* 800CDD7C resets this word; 80120850 increments it. */
extern s32 gOvl11HeldActionSpriteStep;
/* 80120850 wraps a signed phase modulo 4096, writes three projection
 * halfwords, then projects into this four-word workspace. */
extern s16 gOvl11HeldActionSpritePhase;
extern s16 gOvl11HeldActionProjectionInput[4];
extern s32 gOvl11HeldActionProjectionOutput[4];

/* Per-entry 8-byte table stored at offset 0xE4 of each D_80076220 record.
 * ovl_11_func_800F227C scans .unk0 (s16 threshold) and reads .unk2/.unk4/.unk6
 * of the first entry whose threshold is >= the requested value. */
typedef struct {
    s16 unk0;                 /* 0x00 */
    u8 unk2;                  /* 0x02 */
    u16 unk4;                 /* 0x04 */
    u16 unk6;                 /* 0x06 */
} struct_80076220_entry;

/* D_80076220 - absolute-addressed array of 0x1D4-byte entries.
 * ovl_11_func_800C1BE0 zeroes the u16 at offset 0xA of all 37 entries.
 * ovl_11_func_800C3CCC decrements the u16s at offsets 0x2 and 0x4 of all
 * 37 entries. ovl_11_func_800F227C writes unkC/unkE/unk22/unk2C/unk2E and
 * scans the 30-entry table at 0xE4. */
typedef struct {
    u8 unk0[2];               /* 0x00 */
    u16 unk2;                 /* 0x02 */
    u16 unk4;                 /* 0x04 */
    u8 unk6[0xA - 6];         /* 0x06 */
    u16 unkA;                 /* 0x0A */
    u16 unkC;                 /* 0x0C */
    u16 unkE;                 /* 0x0E */
    u8 unk10[0x1E - 0x10];    /* 0x10 */
    u16 unk1E;                /* 0x1E */
    u8 unk20[0x22 - 0x20];    /* 0x20 */
    u16 unk22;                /* 0x22 */
    s16 unk24;                /* 0x24 - read by ovl_11_func_800BFC20 */
    s16 unk26;                /* 0x26 - read by ovl_11_func_800BFC20 */
    u8 unk28[0x2C - 0x28];    /* 0x28 */
    u16 unk2C;                /* 0x2C */
    u16 unk2E;                /* 0x2E */
    u8 unk30[0xE4 - 0x30];    /* 0x30 */
    struct_80076220_entry unkE4[30]; /* 0xE4 - ends at 0x1D4 */
} struct_80076220;
/* 800C19B0: signed id/+A/+2E, reset halfword +10 and words +30..38,
 * u16 flags/index, and the 30-entry +E4 table; loop stride is 0x1D4. */
struct Ovl11EventInitRecord {
    s16 id;
    u8 pad2[8];
    s16 offset;
    u8 padC[4];
    s16 field10;
    u8 pad12[0x1E - 0x12];
    u16 flags;
    u16 flags20;
    u16 index;
    u8 pad24[8];
    s16 field2C;
    s16 field2E;
    s32 field30;
    s32 field34;
    s32 field38;
    u8 pad3C[0xE4 - 0x3C];
    struct_80076220_entry entries[30];
};
/* Existing first-record lvalue/array binding; pool walkers prove 37 records.
 * Keep the one-element declaration and original assembly-symbol identity view. */
extern struct_80076220 gObjectEventSlotStorage[1] __asm__("gObjectEventSlotBase");
#define gObjectEventSlotBase (*((struct_80076220*)gObjectEventSlotStorage))

/* 801315E0 takes width-two field addresses at these bases, stride 0x1D4. */
extern u16 D_80076222[];
extern u16 D_80076224[];
/* 801315E0 also addresses the adjacent halfwords independently. */
extern u16 D_80076226[] __asm__("D_80076224+2");
extern u16 D_8007622A[] __asm__("D_80076228+2");
extern u16 D_80076236[] __asm__("D_80076234+2");
extern u16 D_8007623E[] __asm__("D_8007623C+2");
extern u16 D_80076228[];
extern u16 D_8007622C[];
extern u16 D_80076234[];
extern u16 D_8007623A[];
extern u16 D_8007623C[];
extern u16 D_80076242[];
extern u16 D_8007624C[];
/* 801315E0 takes width-four vector addresses at these bases, stride 0x1D4. */
extern s32 D_80076250[];
extern s32 D_80076270[];
extern s32 D_80076290[];

/* 801315E0 selects a resident with lh +0x4458 and edits the same halfword. */
typedef struct {
    u8 pad0[0x4458];
    s16 resident;
} Ovl30ResidentSelection;
#define Ovl30_ResidentSelection ((Ovl30ResidentSelection *)D_8006C838)

/* 801315E0 passes these resident-record labels and heading to KanjiFntPrint. */
extern char gOvl30ResidentNameNumberLabel[];
extern char gOvl30ResidentSexLabel[];
extern char gOvl30ResidentFriendshipLabel[];
extern char gOvl30ResidentAffectionLabel[];
extern char gOvl30ResidentTalkIntervalLabel[];
extern char gOvl30ResidentDirectionLabel[];
extern char gOvl30ResidentControlDisableDcLabel[];
extern char gOvl30ResidentSDelayLabel[];
extern char gOvl30ResidentLsNumberLabel[];
extern char gOvl30ResidentActionLabel[];
extern char gOvl30ResidentPreviousActionLabel[];
extern char gOvl30ResidentLsTimeLabel[];
extern char gOvl30ResidentXFlagsLabel[];
extern char gOvl30ResidentGlobalLocationLabel[];
extern char gOvl30ResidentLocalLocationLabel[];
extern char gOvl30ResidentXLabel[];
extern char gOvl30ResidentYLabel[];
extern char gOvl30ResidentZLabel[];
extern char gOvl30ResidentSpeedXLabel[];
extern char gOvl30ResidentSpeedYLabel[];
extern char gOvl30ResidentSpeedZLabel[];
extern char gOvl30ResidentEXLabel[];
extern char gOvl30ResidentEYLabel[];
extern char gOvl30ResidentEZLabel[];
extern char gOvl30ResidentESpeedXLabel[];
extern char gOvl30ResidentESpeedYLabel[];
extern char gOvl30ResidentESpeedZLabel[];
extern char gOvl30ResidentBirthMonthLabel[];
extern char gOvl30ResidentBirthDayLabel[];
extern char gOvl30ResidentDestinationLabel[];
extern char gOvl30ResidentPreviousDestinationLabel[];
extern char gOvl30ResidentHeading[];

/* ovl_11_func_800F1F48 indexes this 40-word table and passes each entry to the u16 stream walker. */
/* F1F48 selects one flag-gated stream per event slot; 37 of 40 entries are used. */
extern u16 *gOvl11EventScheduleStreams[40];

/* ovl_11:800F02C8 reads five signed IDs with lh; the 12-byte label also
 * includes an unused trailing halfword whose meaning is not established. */
extern s16 gOvl11CounterCategoryRecordIds[];

/* ovl_11:800F0C70 reads seven unsigned pairs with lhu and writes
 * first / second-1 into the parallel 0xF8-stride records. */
typedef struct {
    /* 0x00 */ u16 first;
    /* 0x02 */ u16 second;
} Ovl11ParallelRecordPair;
extern Ovl11ParallelRecordPair gOvl11ParallelRecordPairDefaults[7];

/* ovl_11:800F0C70 uses lh for the index and lhu for each assigned field.
 * The override list terminates on index -1; its final pair is not read. */
typedef struct {
    /* 0x00 */ s16 recordIndex;
    /* 0x02 */ u16 first;
    /* 0x04 */ u16 second;
} Ovl11RecordPairAssignment;
extern Ovl11RecordPairAssignment gOvl11RecordPairDefaults[5];
extern Ovl11RecordPairAssignment gOvl11ConditionalRecordPairDefaults[5];
extern Ovl11RecordPairAssignment gOvl11RecordPairOverrideList[];

/* 800BEF64 scans 0x1D4-byte records, with lh at +A/+2E, lhu at +1E/+20 and lw position triples at +30/+60. */
typedef struct {
    u8 pad_0[0xA];
    s16 field_A;
    u8 pad_C[0x1E - 0xC];
    u16 field_1E;
    u16 field_20;
    u8 pad_22[0x2E - 0x22];
    s16 field_2E;
    s32 position[3];
    u8 pad_3C[0x60 - 0x3C];
    s32 alternatePosition[3];
    u8 pad_6C[0x1D4 - 0x6C];
} Ovl11ActorCollisionRecord;
/* 800BEF64 starts this absolute-addressed scan at record 1 and advances by 0x1D4. */
extern Ovl11ActorCollisionRecord D_800763F4[];
/* 800BEF64 tests bit 3 of this absolute-addressed word before scanning. */
/* CB4E0..CB6C8 operate on the same primary object and its +0x6C flags.
 * Preserve the existing three-word array-backed base lvalue, not a new object. */
extern s32 _gPrimaryObjectBase[3] __asm__("gPrimaryObjectBase");
#define gPrimaryObjectBase (*((s32 *)_gPrimaryObjectBase))
extern s32 gPrimaryObjectFlags;
/* EXE:80071A36 - E953C tests this u16 and modifies the same work+51FE cell. */
extern u16 gPrimaryObjectAuxFlags;

/* EXE:80076280 - coordinate-field anchor, 0x60 past the position-record
 * base at 80076220. E9778 and the resident editor read/write three s32 words
 * (X/height/Z); successive records are 0x1D4 bytes apart. Keep the incomplete
 * extern and original absolute address formation, not a new storage owner. */
extern s32 gObjectPositionRecordCoordinates[];

/* ovl_11_func_800E5078 copies 0x30-byte sprites at stride 0x1D4 from base-0x30. */
extern u8 gObjectEventSpriteSourceBase[];
/* 800E4DAC initializes this sprite from a SpriteDataHeader. */
extern struct SpriteSourceData gObjectArchiveSpecialSpriteSource;
/* 800E4DAC loads the first offset-table pointer, then signed words at +8/+0x18. */
extern s32 *D_80124FCC_spriteOffsets __asm__("D_80124FCC");
/* 800BD3C4 indexes the same offset-table pointers by item number. */
extern s32 *D_80124FCC_tables[] __asm__("D_80124FCC");
/* CDE3C reads the seventh pointer (+18), then offset word 1. Original data
 * contains two base tables, four named season tables, then the named fixed-ID
 * table at 80124FE4. Keep this aggregate-root view alongside the leaf aliases. */
typedef struct {
    s32 *baseTables[2];
    s32 *seasonTables[4];
    s32 *fixedIdOffsets;
} Ovl11SpriteOffsetRegistry;
extern Ovl11SpriteOffsetRegistry gOvl11SpriteOffsetRegistry __asm__("D_80124FCC");
/* 800BD3C4 reads [item] and [item + 1] as a start/end pair. */
extern s32 gOvl11IndexedSpriteBoundaries[];
/* 800BD3C4 stores the loaded item number at work-area +0x25478. */
typedef struct {
    u8 pad0[0x25478];
    s32 loadedItem;
} Ovl11LoadedItemView;
/* 800E499C relocates sprite data below scratch end; 800E4DAC uses these
 * 24 offsets, collected from archive tags by 800E4D08, relative to that base. */
extern u8 *gOvl11InstalledSpriteData;
extern s32 gOvl11InstalledSpriteOffsets[24];

/* 801081A0 fills four pairs; 801082B0 also writes slot 3 at +0xC/+0xE. */
typedef struct {
    s16 messageEntry;
    s16 variant;
} Ovl11ChoiceMessageSlot;
extern Ovl11ChoiceMessageSlot gOvl11ChoiceMessageSlots[4];
/* 80107F58 dispatches phases 0..6; input selects a slot index 0..3. */
extern s32 gOvl11ChoiceMessagePhase;
extern s32 gOvl11ChoiceMessageIndex;
/* 801088E4 resets the tick and last-uploaded block; 80108930 cycles 3 blocks. */
extern s32 gOvl11ChoiceImageTick;
extern s32 gOvl11ChoiceImageBlock;
/* 8010822C indexes four category messages; 801082F8 uses seven 6-byte rules. */
extern s16 gOvl11ChoiceCategoryMessages[4];
/* Action guards share this selector; state guards update it only on result 1. */
extern u32 gOvl11ActionGuardSelector;
/* Ovl11FindActionGuardResult scans 31 pointers to {s16 id,count; records*}. */
extern s16 *gOvl11ActionGuardLists[31];
/* 80107BE4: three pointers to area/room-keyed ID lists (lh/lw at +0/+2/+4/+8).
 * The pointed-to records and lists are private to the action-guard evaluator. */
typedef struct {
    s16 area;
    s16 room;
    s32 count;
    s16 *ids;
} Ovl11GuardAreaException;
extern Ovl11GuardAreaException *gOvl11ActionGuardAreaExceptions[3];
/* 801082F8: signed band count, unsigned base entry and signed variant;
 * seven records occupy 0x2A bytes, followed by two padding bytes in the blob. */
typedef struct {
    s16 bandCount;
    u16 baseEntry;
    s16 variant;
} Ovl11ChoiceVariantRule;
extern Ovl11ChoiceVariantRule gOvl11ChoiceVariantRules[7];

/* ovl_11:80128DB8 - private array of three 16-byte flat-light records
 * (GsF_LIGHT-shaped: s32 vx/vy/vz @0/4/8, u8 r/g/b @0xC/0xD/0xE), written by
 * Ovl11InitSceneLighting (absolute-addressed lui+lo in the target) and passed to
 * GsSetFlatLight. GsF_LIGHT itself lives in psyq/libgs.h, which this header
 * cannot include, so the .c casts the pointer at the call. */
typedef struct {
    /* 0x00 */ s32 vx;
    /* 0x04 */ s32 vy;
    /* 0x08 */ s32 vz;
    /* 0x0C */ u8 r;
    /* 0x0D */ u8 g;
    /* 0x0E */ u8 b;
    /* 0x0F */ u8 pad;
} Ovl11FlatLight;
extern Ovl11FlatLight _gOvl11SceneLights[3] __asm__("gOvl11SceneLights");
#define gOvl11SceneLights (_gOvl11SceneLights)

/* exe:8006C7B8. The bank-body transfer uses fixed-size chunks. lastChunk
 * is an inclusive quotient bound, so even an exact multiple gets a final
 * iteration; keep that behavior rather than rounding the chunk count up. */
typedef struct {
    s32 archiveIndex; /* 0x00 - index into gSoundArchiveOffsets */
    s32 readBuffer;   /* 0x04 - RAM address, retained signed-word ABI */
    s32 chunkBytes;   /* 0x08 */
    s32 lastChunk;    /* 0x0C */
    s32 chunkIndex;   /* 0x10 */
    s32 archiveByte;  /* 0x14 */
    s32 vabId;        /* 0x18 */
    s32 readPending;  /* 0x1C */
    s32 reserved20;
    s32 reserved24;
    s32 reserved28;
} SoundBankTransferState;
extern SoundBankTransferState gSoundBankTransferStorage[1] __asm__("gSoundBankTransfer");
#define gSoundBankTransfer (*((SoundBankTransferState*)gSoundBankTransferStorage))

/* exe:80061DE8. The view builder subtracts eye from target, applies roll,
 * and skips rebuilding while the dead matrix-install API's address is nonzero. */
struct CameraViewParameters {
    s32 eyeX;
    s32 eyeY;
    s32 eyeZ;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s32 rollAngle;
    s32 matrixOverrideAddress;
};
extern struct CameraViewParameters gCameraViewParameters;
/* func_8001B6A0 builds a MATRIX at E08, with row aliases E0E/E14. */
extern s16 gCameraViewMatrix[16];
extern s16 D_80061E0E[3];
extern s16 D_80061E14[3];

/* Boot's pointers to the two 0x38-byte pad-input records at gPadInputStateStorage
 * and +0x38. Boot owns these GP-relative words; setup/consumers use externs. */
extern struct PadInputState *gPad0Input;
extern struct PadInputState *gPad1Input;

/* 8012F454 accesses flag words at +0xC and +0x10 through this shared base. */
struct Ovl30DebugFlags {
    u8 pad[0xC];
    s32 flagsC;
    s32 flags10;
};
/* 8012FE68 writes the same +0x10 word using an unsigned expression. */
typedef struct {
    char padding[0x10];
    u32 bits;
} Ovl30UnsignedFlagWord;

/* 8012F454 passes these original debug-menu strings to FntPrint. */
extern char gOvl30FlagsSoundHeading[];
extern char gOvl30ModelNumberFormat[];
extern char gOvl30SoundEffectFormat[];
extern char gOvl30AlternateSoundEffectFormat[];
extern char gOvl30SoundSequenceFormat[];
extern char gOvl30CdAudioTrackFormat[];
extern char gOvl30FlagRowFormat[];
/* 8012F454 indexes the two status strings and the loaded menu-label table. */
extern char *gOvl30FlagStatusText[2];
extern char **gOvl30FlagRowLabels;
extern u32 gOvl30FlagRowCount;
/* 8012F454 uses lw/sw for the cursor, sound selectors and playback handle. */
extern s32 gOvl30FlagRowIndex;
extern s32 gOvl30SoundEffectIndex;
extern s32 gOvl30AlternateSoundEffectIndex;
extern s32 gOvl30SoundSequenceIndex;
extern s32 gOvl30CdAudioTrackIndex;
extern s32 gOvl30SoundTestVoice;

/* ovl_30_func_80131C38 reads/writes this selection index with lw/sw. */
extern s32 gOvl30GameFlagIndex;
/* ovl_30_func_80131C38 passes these original rodata strings to FntPrint. */
extern char gOvl30GameFlagOnFormat[];
extern char gOvl30GameFlagOffFormat[];
extern char gOvl30GameEventOnFormat[];
extern char gOvl30GameEventOffFormat[];

/* D_801349B0 - s32 selection index driven by ovl_30_func_80131F14.
 * Clamped to 0..0x24 and used as a modulo-0x25 record selector.
 * Absolute-addressed (overlay -G0), so a plain extern gives lui+lo. */
extern s32 gOvl30ObjectEventSlotIndex;

/* D_8012ECD8 - "P %d ADR %x\n" rodata format string whose address is
 * passed to FntPrint by ovl_30_func_80131F14 (absolute overlay rodata). */
extern char gOvl30ObjectEventSlotFormat[];

/* 801326C4 reads/writes this signed word as its 0..127 selector. */
extern s32 gOvl30DayCount0Cursor;
/* 801326C4 indexes unsigned bytes at +0xE640, bounded by the 0x80 selector. */
struct Ovl30DayCounters {
    u8 pad_0[0xE640];
    u8 counters[0x80];
};
/* 801326C4 passes these original rodata strings to FntPrint. */
extern char gOvl30EditValueFormat[];
extern char gOvl30DayCount0Format[];
/* 80132978 reads/writes this signed selector and indexes eight bytes at +0xE6C0. */
extern s32 gOvl30DayCount1Cursor;
struct Ovl30DayCounters1 {
    u8 pad_0[0xE6C0];
    u8 counters[8];
};
/* 80132978 passes this original Ke-Day-CNT1 row format to FntPrint. */
extern char gOvl30DayCount1Format[];
/* 80132C2C reads/writes this signed selector and indexes 128 bytes at +0xE6C8. */
extern s32 gOvl30NValueCursor;
struct Ovl30NValues {
    u8 pad_0[0xE6C8];
    u8 values[0x80];
};
/* 80132C2C passes this original Ke-N-VAL row format to FntPrint. */
extern char gOvl30NValueFormat[];

/* ovl_11_func_800E9914 tests bit 0x20 of this word with lw. */
extern s32 D_80070C88;
/* 800E5294 passes this pair to 80107DD0, which stores s16 -1 and 0. */
extern s16 gObjectWorkTimedSpriteEffect[2];
/* EXE:80076300 - E7C2C selects the ID/tick pair at this 0x1D4-stride anchor. */
extern s16 gPositionRecordTimedSpriteEffects[];

/* D_8006C838 - array of 0x3C-byte structs used for flags
 * Accessed with 0x3C stride but accessed in 4-byte s32 words
 * Struct size must be 4 bytes to get correct indexing (index * 0x3C as byte offset) */ 
struct struct_8006C838 {
    char data[0x3C];  /* 60 bytes per struct */
};
extern struct struct_8006C838 D_8006C838[];

/* 800CE654: unsigned cursor +0, current halfword +4, samples +6, bound 0x738. */
struct Ovl11RecordedInput {
    u32 index;
    u16 current;
    u16 samples[0x738];
};

/* 800CE654 reads/writes flags +0xC and follows the word pointer at +0x1C. */
struct Ovl11InputPlaybackState {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0xC];
    struct Ovl11RecordedInput *input;
};

/* 800DDDFC: resident flags, signed phase/mode and unsigned ID component. */
typedef struct {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0xD0 - 0x10];
    s16 phase;
    u8 padD2[0x4450 - 0xD2];
    s32 control;
    u8 pad4454[0x44BA - 0x4454];
    s16 index;
    u8 pad44BC[0x44CE - 0x44BC];
    u16 variant;
    u8 pad44D0[0x44F8 - 0x44D0];
    s32 status;
    u8 pad44FC[0x52C8 - 0x44FC];
    s32 position[3];
    u8 pad52D4[0xE4C8 - 0x52D4];
    s16 mode;
} Ovl11DDDFCState;

/* 800CCCC0 uses these word/halfword fields through the D_8006C838 base. */
struct Ovl11CCCC0State {
    u8 pad0[0x4450];
    s32 flags;
    u8 pad4454[0x44C0 - 0x4454];
    s16 time;
    u8 pad44C2[8];
    s16 angle;
    u8 pad44CC[0x12];
    s16 pending;
    u8 pad44E0[0x18];
    s32 status;
    u8 pad44FC[0x5242 - 0x44FC];
    s16 counter;
};

/* 80021DA8 initializes three base words; 800DE4C0 reaches the far work area. */
extern s32 D_8007AFF0[3];
#define D_8007AFF0 (D_8007AFF0[0])

/* 800C7AC0 reads/writes flags +0xC and a queued item u16 at +0x44D6. */
struct Ovl11InteractionState {
    char pad0[0xC];
    s32 flags;
    char pad10[0x44D6 - 0x10];
    u16 queuedItem;
};

/* 800C8808 compares a signed halfword through D_8006C838 at +0xE778. */
struct Ovl11C8808State {
    char pad0[0xE778];
    s16 selected;
};

/* 80111F88 reads/decrements signed timers and updates progress through this base. */
typedef struct {
    u8 pad0[0x44CE];
    s16 stage0;
    s16 stage1;
    s16 stage2;
    s16 complete;
    u8 pad44D6[2];
    s16 pendingTimer;
    s16 activeTimer;
    u8 pad44DC[0x44F8 - 0x44DC];
    s32 flags;
    u8 pad44FC[0x51C4 - 0x44FC];
    s16 cooldown;
    s16 request;
    u8 pad51C8[0x91DC - 0x51C8];
    s16 mode;
} Ovl11ProgressState;
/* 80111EE0 stores 3 at work+44D8; 80112160 reads work+51C6 directly. */
extern s16 gProgressPendingTimer;
extern s16 gProgressRequest;

/* 80111F88 passes this address to Ovl11ReadDefaultPosition (800CD0C0), which
 * writes the X, 0, Z words of a default work position. */
extern s32 gOvl11DefaultPositionWork;

/* 80119470 reads signed words at +44F8 and +51C0 through the shared base. */
struct Ovl11CallbackGateState {
    u8 pad0[0x44F8];
    s32 status;
    u8 pad44FC[0x51C0 - 0x44FC];
    s32 flags;
};

/* 800F3840 loads u8 at +0x44B8 and stores its increment at +0xE6CB. */
struct Ovl11F3840State {
    u8 pad_0[0x44B8];
    u8 field_44B8;
    u8 pad_44B9[0xE6CB - 0x44B9];
    u8 field_E6CB;
};

/* 80110944 reads the low halfword, signed +30, word +34 and u16 +B4. */
typedef struct {
    u16 number;
    u16 unused;
    u8 pad4[0x30 - 4];
    s16 area;
    u16 pad32;
    u32 flags;
    u8 pad38[0xB4 - 0x38];
    u16 pregnancy;
    u16 padB6;
} Ovl11AnimalSelection;

/* 80110944: shared +4AC0 block, 20 large/2 tracked/10 small selection bytes. */
extern u8 gRecordSelectionAvailability[32];

/* 80110944 scans seven stride-F8 entries at +DE00 and two selection slots. */
typedef struct {
    u8 pad0[0x26];
    s16 kind;
    u8 pad28[8];
    s16 area;
    u8 pad32[0xB8 - 0x32];
    u16 flags;
    s16 index;
    u8 padBC[0xF8 - 0xBC];
} Ovl11SelectionEntry;

typedef struct {
    u8 pad0[0x4AC0];
    u8 available[20];
    u8 selected[2];
    u8 smallAvailable[10];
    u8 pad4AE0[0x7AB4 - 0x4AE0];
    Ovl30AnimalRecord smallAnimals[10];
    Ovl30AnimalRecordB8 largeAnimals[20];
    u8 pad901C[0x99CA - 0x901C];
    s16 index0;
    s16 pad99CC;
    s16 index1;
    u8 pad99D0[0xDE00 - 0x99D0];
    Ovl11SelectionEntry entries[7];
} Ovl11SelectionState;
#define Ovl11_SelectionState ((Ovl11SelectionState *)D_8006C838)

/* 80110944 also walks the +8000 view in F8-byte steps, with signed displacements. */
typedef struct {
    u8 pad0[0x5E26];
    s16 kind;
    u8 pad5E28[8];
    s16 area;
    u8 pad5E32[0x5EB8 - 0x5E32];
    u16 flags;
    s16 index;
} Ovl11SelectionFarView;

/* 80107888 compares these signed halfwords through the shared state base. */
typedef struct {
    u8 pad0[0x44BA];
    s16 area;
    s16 room;
    u8 pad44BE[2];
    s16 time;
    u8 pad44C2[0x5252 - 0x44C2];
    s16 item;
    u8 pad5254[0xA6D8 - 0x5254];
    s16 savedArea;
    s16 savedRoom;
} Ovl11GuardState;

/* 800E8C14/800E8D00 read two signed tag/item pairs at work+524C/+5252.
 * The property command passes the second tag's address to GetRecordItemId. */
struct Ovl11TaggedItemState {
    u8 pad0[0x524C];
    s16 firstTag;
    s16 firstItemId;
    u8 pad5250[2];
    s16 secondTag;
    s16 secondItemId;
};

/* 800E9914 reads and clears a signed halfword through D_8006C838 + 0x51F0. */
struct Ovl11E9914State {
    u8 pad_0[0x51F0];
    s16 pending;
};

/* 800E4280: lw/sw flags at +0xC/+0x4450 and cleared words at +0x448C/+0x5214/+0x7A74. */
struct Ovl11E4280State {
    u8 pad_0[0xC];
    s32 flags_C;
    u8 pad_10[0x4450 - 0x10];
    s32 flags_4450;
    u8 pad_4454[0x448C - 0x4454];
    s32 field_448C;
    u8 pad_4490[0x5214 - 0x4490];
    s32 field_5214;
    u8 pad_5218[0x7A74 - 0x5218];
    s32 selector;
};

/* 800E7D4C: zero u16 at +44D4 suppresses alternate-area bulk removal. */
typedef struct {
    u8 pad[0x44D4];
    u16 alternateAreaAvailable;
} Ovl11GridRemovalWorkView;

/* 800E21E4: lh +44BA/+44BC, lw/sw +44F8 and ten lbu/sb cells at +4AD6. */
typedef struct {
    u8 pad0[0x44BA];
    s16 mode, day;
    u8 pad44BE[0x44F8 - 0x44BE];
    s32 flags;
    u8 pad44FC[0x4AD6 - 0x44FC];
    u8 pending[10];
} E21E4State;

/* 800B80D4 increments +0x448C; 800B8134 also updates the lw/sw flags at +0xC. */
typedef struct {
    u8 pad_0[0xC];
    s32 flags;
    u8 pad_10[0x448C - 0x10];
    s32 state;
} Ovl21MainStateView;

/* Selection-message inputs: lh mode at +0x44BA, lhu threshold at +0xB384
 * and byte variants at +0xE7AC. This overlaps gSelectionMessageStates. */
struct Ovl11StateSelection {
    u8 pad_0[0x44BA];
    s16 mode;
    u8 pad_44BC[0xB384 - 0x44BC];
    u16 threshold;
    u8 pad_B386[0xE7AC - 0xB386];
    u8 states[10];
    u8 counters[2];
};
/* 8011F1D0 indexes ten byte states and increments the two following bytes. */
struct Ovl11StateBytes {
    u8 states[10];
    u8 first;
    u8 second;
};
extern struct Ovl11StateBytes gSelectionMessageStates;
struct Ovl11StateCounters {
    u8 first;
    u8 second;
};
extern struct Ovl11StateCounters gSelectionMessageRepeatCounters;
/* 8011F1D0 separately addresses the byte at 0x8007AFEF. */
extern u8 D_8007AFEF;
/* Ovl11AdvanceSelectionMessageVariant adds a category's signed message base. */
extern s16 gOvl11SelectionMessageBases[10];

/* ovl_11's pool scans 99 four-byte cells; ages select IDs A3/A2/A1 at 10/20. */
typedef struct {
    u16 id;
    u16 age;
} Ovl11ItemCell;
extern Ovl11ItemCell gAgedItemCells[99];
/* 800CE96C clears 0x18E bytes, including two bytes beyond the cell array.
 * Preserve that witnessed reset span rather than widening the cell count. */
extern u8 gAgedItemCellsResetSpan[0x18E] __asm__("gAgedItemCells");
/* 8010C330 stores occupancy here only after a successful add. */
extern s16 gAgedItemLastAddCount;
/* Six 0xB0 state-object records. D2F48 allocates by zero tag; D2240 updates
 * all six, CF36C clears them and 1097F4 scans tag/selector/X/Z. Keep the
 * established partial layout; unexposed sprite/motion fields use byte views. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x2E];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[6];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ char pad_3C[4];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ char pad_44[0x6C];
} UnkStruct80075BC4;
extern UnkStruct80075BC4 gStateObjectPool[6];
/* CE96C's reset span and the generic record clients use these same bytes. */
#define gStateObjectPoolBytes ((u8 *)gStateObjectPool)

/* 800CE96C resets +0/+4 in these records; 800CFC20 tests flags at +34. */
struct Ovl11ResetB4 {
    u16 type;
    u16 pad_2;
    u16 id;
    u8 pad_6[0x34 - 6];
    s32 flags;
    u8 pad_38[0xB4 - 0x38];
};
struct Ovl11ResetB8 {
    u16 type;
    u16 pad_2;
    u16 id;
    u8 pad_6[0x34 - 6];
    s32 flags;
    u8 pad_38[0xB8 - 0x38];
};
struct Ovl11ResetB0 {
    u16 type;
    u16 pad_2;
    u16 id;
    u8 pad_6[0xB0 - 6];
};
/* 800CFC20 retires marked B8/B4 records and increments these counters.
 * The byte increment wraps before its upper clamp; keep its u8 width. */
struct Ovl11RecordRetirementWork {
    u8 pad_0[0x44CD];
    u8 byteCount;
    u8 pad_44CE[0x51C8 - 0x44CE];
    u8 object[0x5234 - 0x51C8];
    s32 flags;
    u8 pad_5238[6];
    u16 count;
    u8 pad_5240[0x7AB4 - 0x5240];
    struct Ovl11ResetB4 records[10];
};

/* 800CE96C: witnessed base-relative arrays and scalar reset fields.
 * 800CECB0 also reads/writes flags +44F8 and the counter +523C. */
struct Ovl11PoolResetView {
    u8 pad_0[0x4450];
    s32 flags;
    u8 pad_4454[0x44F8 - 0x4454];
    s32 flags_44F8;
    u8 pad_44FC[0x523C - 0x44FC];
    s16 counter_523C;
    u8 pad_523E[0x7AB4 - 0x523E];
    struct Ovl11ResetB4 records_7AB4[10];
    struct Ovl11ResetB8 records_81BC[20];
    u16 cells_901C[99][2];
    u8 pad_91A8[4];
    u16 field_91AC;
    u8 pad_91AE[0x91E0 - 0x91AE];
    s32 field_91E0;
    u8 pad_91E4[0x929C - 0x91E4];
    u16 field_929C;
    u16 pad_929E;
    u16 field_92A0;
    u8 pad_92A2[0x938C - 0x92A2];
    struct Ovl11ResetB0 records_938C[6];
    struct Ovl11ResetB4 records_97AC[3];
    s16 pairs_99C8[4][2];
    u16 field_99D8;
    u8 pad_99DA[0x99E6 - 0x99DA];
    u16 pendingCdAudioTrack;
    /* 800CECB0 branches on the signed halfword at +E4CA. */
    u8 pad_99E8[0xE4CA - 0x99E8];
    s16 field_E4CA;
};

/* Dated conditions: four calendar halfwords followed by two signed record
 * indices (800EF488 snapshots; 800EF6E4 assigns; 800EF0E8 classifies).
 * field_0/2/4 are year/season/day; field_6 is calendar state[3], not yet named. */
typedef struct {
    u16 field_0;
    u16 field_2;
    u16 field_4;
    u16 field_6;
    s16 index_8;
    s16 index_A;
} Ovl11EF488Record;
/* 800EF488 uses lhu at +0/+2/+4/+6 of this input state. */
extern u16 D_80070CF0_halfwords[4] __asm__("gCalendarYear");
/* 800EF488/800EF6E4 select the singleton for slot -1, otherwise five slots. */
extern Ovl11EF488Record gConditionDateSingleton;
extern Ovl11EF488Record gConditionDateSlots[5];
/* 800EF488 passes the signed halfword at +0x5492 to 80112904. */
typedef struct {
    u8 pad_0[0x5492];
    s16 selected;
} Ovl11EF488SelectionView;
/* 800EF488 indexes records at +0xE4D8 and reads their +8/+A signed indices. */
typedef struct {
    u8 pad_0[0xE4D8];
    Ovl11EF488Record records[5];
} Ovl11EF488RecordsView;
/* 800F00AC adds the signed halfword at +44DC to 25 unsigned counters
 * at +498C. Keep this narrow view's signedness distinct from the counters. */
typedef struct {
    char pad0[0x44DC];
    s16 additionalCounter;
} Ovl11ConditionCounterTotalWork;

/* 800EF0E8 reads lh +0x99EA and lhu +0x99EC after a signed index * 0x1D4. */
typedef struct {
    u8 pad_0[0x99EA];
    s16 field_99EA;
    u16 field_99EC;
} Ovl11EF0E8StatusView;

/* 800EF488 sets bit 3 in the u16 at +0x9A06 of each 0x1D4-stride record. */
typedef struct {
    u8 pad_0[0x9A06];
    u16 flags;
} Ovl11EF488FlagView;

/* ovl_11_func_80101964 reads the unsigned flag byte at D_8006C838+0x44CC. */
typedef struct {
    u8 pad_0[0x44CC];
    u8 flags;
} Ovl11IconFlagsView;

/* 8010B264 compares signed halfwords at +44B8/+44BA/+44BC and reads lh at +E4C8. */
struct Ovl11B264StateView {
    u8 pad_0[0x44B8];
    s16 field_44B8;
    s16 field_44BA;
    s16 field_44BC;
    u8 pad_44BE[0xE4C8 - 0x44BE];
    s16 field_E4C8;
};

/* ovl_11_func_800D93C8 reads the flag word at +0x10 and signed mode at +0x44BA. */
typedef struct {
    u8 pad_0[0x10];
    s32 flags;
    u8 pad_14[0x44BA - 0x14];
    s16 mode;
} Ovl11D93C8StateView;

/* ovl_11_func_800D93C8 indexes 8-byte cells by x + y*45, loading u16@0 and u8@4. */
typedef struct {
    u16 kind;
    u16 field_2;
    u8 value;
    u8 pad_5[3];
} Ovl11D93C8Cell;
typedef struct {
    u8 pad_0[0x55C4];
    Ovl11D93C8Cell cells[26][45];
} Ovl11D93C8MapView;
/* 800D9F1C reads kind/value at +0x78EC/+0x78F0 in a 7x7 stride-8 grid. */
typedef struct {
    u8 pad_0[0x78EC];
    Ovl11D93C8Cell cells[49];
} Ovl11D9F1CMapView;
/* ovl_11_func_800D93C8 selects these five-argument handlers; each writes three s32 outputs. */
/* 800D93C8 installs four s32/s32 resolvers; cells supply lhu/lbu inputs. */
extern s32 (*gOvl11CellSpriteResolver)(s32, s32, s32 *, s32 *, s32 *);

/* 800BFC20 reads five signed halfword record indices at overlay 11:80122E74. */
extern s16 gOvl11ConditionDateRecordIndices[];

/* 800BF8B8/800BFB28 choose and apply the next/current condition code.
 * Mixed signed/unsigned reads are intentional; retain the stored halfwords. */
typedef struct {
    u8 pad_0[0x44BA];
    u16 season;
    u16 day;
    u8 pad_44BE[0x548A - 0x44BE];
    s16 excludedSeason;
    s16 excludedDay;
    u8 pad_548E[0xE4C8 - 0x548E];
    u16 currentCondition;
    u16 nextCondition;
} Ovl11DayConditionWorkView;

/* ovl_11:800C090C fills 0xC8 bytes with -1; 800C0EA4/0EFC enqueue and
 * shift fifty signed event IDs. Absolute-addressed overlay state. */
extern s32 gOvl11ClockEventQueue[50];
extern s32 gOvl11ClockAdvanceActive;
extern s32 gOvl11ClockAdvanceTickEnabled;
extern s32 gOvl11ClockAdvanceMinutesRemaining;
/* 800C06B0 tests exactly 1 before the deferred refresh; 800C0A40 sets it. */
extern s32 gOvl11ClockAdvanceRefreshRequested;
/* ovl_11:800C0EFC indexes ten resident callbacks; nonzero dequeues the ID. */
extern s32 (*gOvl11ClockEventHandlers[10])(void);
/* ovl_11:800C0EFC clears this halfword when the queued callback completes. */
extern s16 D_8006C908;

/* ovl_11_func_80117178 scans ten 0xB4-byte records at +0x7AB4, reading u16 at +0 and flag bit 18 at +0x34. */
struct Ovl11Scan109Record {
    u16 type;
    u8 pad_2[0x32];
    u32 flags;
    u8 pad_38[0x7C];
};

struct Ovl11Scan109Globals {
    u8 pad_0[0x7AB4];
    struct Ovl11Scan109Record records[10];
};

/* ovl_11_func_800BE94C indexes four original-data halfwords by the +0x44BA selector. */
extern u16 gOvl11ItemCollisionSeasonMasks[4];
/* 800C0A4C walks ten 0xE-byte records; 800C0DB4 reads seven signed halfwords. */
extern s16 gOvl11ClockEventRules[10][7];
/* 800C1224/800C1280 search 14/18 entries; 800C141C reads the third halfword.
 * The bounds differ, not the identity or layout of the shared date table. */
typedef struct {
    s16 season;
    s16 day;
    s16 value;
} Ovl11CalendarDateEntry;
extern s16 gOvl11CalendarDates[18 * 3];
/* 800C0A4C: signed date/time pairs, flag words and elapsed halfword through the resident state. */
struct Ovl11ClockPair {
    s16 hour;
    s16 minute;
};
struct Ovl11ClockDate {
    s16 year;
    s16 season;
    s16 day;
    s16 weekday;
};
struct Ovl11ClockState {
    u8 pad_0[0xC];
    s32 flags;
    s32 flags_10;
    u8 pad_14[0x44BA - 0x14];
    s16 season;
    s16 day;
    s16 weekday;
    u8 pad_44C0[0xA];
    u16 elapsed;
};
/* 800C0A4C uses a u32 counter at +8 of D_8006C904, alongside its byte state. */
struct Ovl11ClockAnchor {
    u8 pad_0[8];
    u32 counter;
};

/* 800BE5A0 scans 0x14-byte X/Z rectangles: four signed words and a u16 region. */
typedef struct {
    s32 minX;
    s32 minZ;
    s32 maxX;
    s32 maxZ;
    u16 region;
    u16 reserved;
} Ovl11CollisionRegionBounds;
extern Ovl11CollisionRegionBounds gOvl11CollisionRegionBounds[25];
extern Ovl11CollisionRegionBounds gOvl11Region5Bounds[7];
extern Ovl11CollisionRegionBounds gOvl11Region5ExpandedBounds[9];
extern Ovl11CollisionRegionBounds gOvl11Region4Bounds[5];
extern Ovl11CollisionRegionBounds gOvl11Region4ExpandedBounds[10];
extern Ovl11CollisionRegionBounds gOvl11OverrideRegionBounds[4];

/* 800BEB28 reads the s16 at +91DC, flags at +91E0 and pool pointer at +DD8C. */
struct Ovl11CollisionDispatchState {
    char pad0[0x91DC];
    s16 region;
    char pad91DE[2];
    s32 flags;
    char pad91E4[0xDD8C - 0x91E4];
    void *pool;
};

/* ovl_11_func_800BE94C shares this base for the +0x20 pool and +0x44BA selector. */
struct Ovl11ItemCollisionGlobals {
    u8 pad_0[0x20];
    void *items;
    u8 pad_24[0x44BA - 0x24];
    s16 selector;
};

/* ovl_11_func_800E5078 reads a pointer at +0x30, then its signed halfword at +2. */
struct Ovl11CurrentObjectView {
    u8 pad_0[0x30];
    s16 *current;
};

/* 800CE834 reads/clears pending CD-audio track +99E6; D12A0 writes it. */
struct Ovl11UpdateState {
    u8 pad_0[0x10];
    s32 field_10;
    u8 pad_14[0xCC - 0x14];
    u8 field_CC;
    u8 pad_CD[0x44C0 - 0xCD];
    s16 field_44C0;
    s16 field_44C2;
    u8 pad_44C4[0x44F8 - 0x44C4];
    s32 field_44F8;
    u8 pad_44FC[0x99E6 - 0x44FC];
    s16 pendingCdAudioTrack;
};

/* 800CE834 compares the signed halfword at D_8007AFF0+0x25476 with 1. */
struct Ovl11UpdateWork {
    u8 pad_0[0x25476];
    s16 field_25476;
};

/* ovl_11:800C090C initializes these fields; 800C0A4C establishes the
 * date/time units and 06:00 day boundary. Widths and unknown fields retained. */
struct Ovl11StateInitView {
    u8 pad_0[0xD0];
    s16 field_D0;
    u8 pad_D2[2];
    s32 field_D4;
    u8 pad_D8[0x44B8 - 0xD8];
    s16 year;
    s16 season;
    s16 day;
    s16 weekday;
    s16 hour;
    s16 minute;
    u8 pad_44C4[4];
    s16 field_44C8;
    s16 field_44CA;
};

/* 800C19B0 reads lh +44CA and writes sh +CB70/+CB72 through D_8006C838. */
struct Ovl11EventInitState {
    u8 pad0[0x44CA];
    s16 time;
    u8 pad44CC[0xCB70 - 0x44CC];
    s16 fieldCB70;
    s16 fieldCB72;
};

/* 800C2044 loads the unsigned event time at D_8006C838+0x44CA. */
struct Ovl11CommandTimeState {
    u8 pad0[0x44CA];
    u16 time;
};

/* 800C19B0 loads the pointer at D_8009AFF0+5388, then reads lh +2. */
struct Ovl11EventInitWork {
    u8 pad0[0x5388];
    s16 *header;
};

/* ovl_11_func_800C0FC0 compares signed halfwords at these D_8006C838 offsets. */
struct Ovl11FlagDateView {
    u8 pad_0[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
    u8 pad_44BE[0x5246 - 0x44BE];
    s16 field_5246;
    s16 field_5248;
    u8 pad_524A[0xE778 - 0x524A];
    s16 field_E778;
    u8 pad_E77A[0xE798 - 0xE77A];
    s16 field_E798;
    s16 field_E79A;
};

/* ovl_11_func_800C0FC0 reads s16 at +0/+2, with a 0x1D4-byte indexed stride. */
extern s16 D_80076244[];
/* ovl_11_func_800C0FC0 reads this absolute-addressed signed-halfword pair. */
struct Ovl11FlagDatePair {
    s16 field_0;
    s16 field_2;
};
extern struct Ovl11FlagDatePair D_8007A414;

/* View of D_8006C838 for bit-flag access: a flat array of u32 flag words
 * starts at offset 0x38 (used by func_8001AF44 and its set/clear siblings).
 * Index range comes from a 16-bit flag id: (0xFFFF >> 5) + 1 words. */
struct struct_8006C838_flags {
    char pad_000[0x38];     /* 0x00-0x37 */
    u32 flags[0x800];       /* 0x38: one word per 32 flag ids */
};

/* View of D_8006C838 for func_80022DF8: s32 flag word at 0xC (bit 27) and
 * u8 state byte at 0xCC (script/timer status, also touched by func_8002261C). */
struct struct_8006C838_view {
    char pad_000[0x0C];     /* 0x00-0x0B */
    s32 field_0C;           /* 0x0C */
    char pad_010[0xBC];     /* 0x10-0xCB */
    u8 field_CC;            /* 0xCC */
};

/* View of D_8006C838 for ovl_11_func_800E99EC: two s16 time-like fields at
 * +0x44C0 and +0x44C2. Reached as a struct member so cc1 emits the base
 * address of D_8006C838 plus an immediate field offset instead of folding the
 * offset into %hi(D_8006C838+0x44C0) (the target keeps lui %hi(D_8006C838)
 * followed by lh 0x44C0/0x44C2). */
struct struct_8006C838_time {
    char pad_000[0x44C0];   /* 0x0000-0x44BF */
    s16 field_44C0;         /* 0x44C0 */
    s16 field_44C2;         /* 0x44C2 */
};

/* 801168C0 uses u16 +4A86, an object at +51C8, and s32 +5228 through one base. */
typedef struct {
    u8 pad0[0x4A86];
    u16 flags;
    u8 pad4A88[0x51C8 - 0x4A88];
    u8 object[0x60];
    s32 balance;
} Ovl11SelectedThresholdState;

/* 80114398 reads the u16 flag at D_8006C838+0x4A86 with lhu. */
struct Ovl11RecordFlagsView {
    u8 pad_0[0x4A86];
    u16 field_4A86;
};

/* ovl_11_func_800F9FF4 reads u16 at +0x44BC and s16 at +0x44BE from this base. */
struct Ovl11MenuDateView {
    u8 pad_0[0x44BC];
    u16 field_44BC;
    s16 field_44BE;
};

/* Clock HUD: zero-based season/day/weekday, hour, unsigned minute storage.
 * The time formatter explicitly narrows minutes to signed before division. */
struct Ovl11CalendarDisplayStateView {
    char pad_0[0x44BA];
    s16 season;
    s16 day;
    s16 weekday;
    s16 hour;
    u16 minute;
};

/* 800EEECC: lh at +0x44BA/+0x44BC and sb at +0xE668 from one state base. */
struct Ovl11ButtonUpdateState {
    char pad_0[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
    char pad_44BE[0xE668 - 0x44BE];
    s8 field_E668;
};

/* View of D_8006C838 for ovl_11_func_80111A60: the s16 at +0x44BA plus the
 * two time-like fields at +0x44C0/+0x44C2. Reached as a struct member so cc1
 * keeps lui %hi(D_8006C838) followed by lh 0x44BA/0x44C0/0x44C2 rather than
 * folding the offsets into %hi(D_8006C838+0x44BA). The +0x44BA halfword is
 * also named D_80070CF2 elsewhere; this view preserves the target base. */
struct struct_8006C838_button {
    char pad_000[0x44BA];   /* 0x0000-0x44B9 */
    s16 field_44BA;         /* 0x44BA */
    char pad_44BC[4];       /* 0x44BC-0x44BF */
    s16 field_44C0;         /* 0x44C0 */
    s16 field_44C2;         /* 0x44C2 */
    /* 800F4BB4 reads u16 +44D6 and decrements/clamps s16 +51DE. */
    char pad_44C4[0x44D6 - 0x44C4];
    u16 field_44D6;
    char pad_44D8[0x44F8 - 0x44D8];
    /* 800F4B5C gates queue submission on this word's bit 1. */
    s32 field_44F8;
    char pad_44FC[0x51DE - 0x44FC];
    s16 field_51DE;
};

/* 800DE9C8 reads +44BA/+E4C8 and +44C0; 800E1DA8 reads s16 at +52C6/+E4C8. */
struct Ovl11HandlerState {
    u8 pad0[0x44BA];
    s16 field_44BA;
    u8 pad44BC[4];
    u16 field_44C0;
    u8 pad44C2[0x52C6 - 0x44C2];
    s16 field_52C6;
    u8 pad52C8[0xE4C8 - 0x52C8];
    s16 field_E4C8;
};

/* 800DC388/800DD274: lh +44BA and lhu selectors +44CE/+44D0/+44D2/+44D4. */
struct Ovl11MessageState {
    char pad0[0x44BA];
    s16 base;
    char pad44BC[0x44CE - 0x44BC];
    u16 variant50;
    u16 variant60;
    u16 variant70;
    u16 variant80;
};

/* ovl_11:800EC354 compares its resolved pair to these two signed halfwords. */
typedef struct {
    char pad_000[0x44BA];
    s16 first;
    s16 second;
} Ovl11CurrentRecordPairView;
/* The same resolver's special record ID 29 reads the pair at 5246/5248. */
typedef struct {
    char pad_000[0x5246];
    s16 first;
    s16 second;
} Ovl11SpecialRecordPairView;

/* ovl_11:800E7954 clears word 448C and sets flag 400 in word 5234. */
typedef struct {
    char pad_000[0x448C];
    s32 field_448C;
    char pad_4490[0x5234 - 0x4490];
    s32 field_5234;
} Ovl11SceneModeCommandWork;

/* ovl_11:800E7798 sets bits in words 4450/5234 and writes word 7A74. */
typedef struct {
    char pad0[0x4450];
    s32 flags4450;
    char pad4454[0x5234 - 0x4454];
    s32 flags5234;
    char pad5238[0x7A74 - 0x5238];
    s32 field7A74;
} Ovl11ClockEventTransitionWork;

/* 800E5E98: lw/sw +4450, lh +44BA, lhu +44CE, and lw +44F8. */
struct Ovl11AreaSelectionCommandWork {
    char pad0[0x4450];
    s32 flags;
    char pad4454[0x44BA - 0x4454];
    s16 season;
    char pad44BC[0x44CE - 0x44BC];
    u16 variant;
    char pad44D0[0x44F8 - 0x44D0];
    s32 status;
};

/* 800E537C: state reset fields witnessed by sh/lhu/lw and a pointer load. */
struct Ovl11ResetState {
    char pad_0[0x4450];
    s32 field_4450;
    char pad_4454[0x51EA - 0x4454];
    s16 field_51EA;
    char pad_51EC[4];
    s16 field_51F0;
    char pad_51F2[0xC];
    u16 field_51FE;
    char pad_5200[4];
    u16 field_5204;
    char pad_5206[0x2E];
    s32 field_5234;
    char pad_5238[0x91DC - 0x5238];
    s16 field_91DC;
    char pad_91DE[0xDDD4 - 0x91DE];
    s16 *field_DDD4;
};

/* View of D_8006C838 for ovl_11_func_800E7660: the two s16 time-like fields
 * at +0x44C0/+0x44C2 plus the s16 status slot at +0x51EA. Reached as struct
 * members so cc1 keeps lui %hi(D_8006C838) followed by lh/sh with the field
 * offset as an immediate instead of folding each offset into the %hi. */
struct Ovl11PendingSelectionWorkView {
    char pad_000[0x44C0];   /* 0x0000-0x44BF */
    s16 field_44C0;         /* 0x44C0 */
    s16 field_44C2;         /* 0x44C2 */
    char pad_44C4[0xD26];   /* 0x44C4-0x51E9 */
    s16 pendingRecord;      /* 0x51EA, -1 means none */
};

/* Object archive loading clears this signed sound-bank request at work
 * +4438; E3AA4 sends a changed request other than -1 to the sound driver. */
struct Ovl11ObjectSoundRequestView {
    char pad_000[0x4438];
    s16 bankId;
};
/* E3AA4 copies descriptor +E through the shared far-work descriptor pointer
 * to far-work +25498; its broader interpretation remains neutral. */
typedef struct {
    char pad[0xE];
    s16 valueE;
} Ovl11ObjectSoundDescriptorView;

/* 800C2B04 reads the flag word at +0xC and signed time at +0x44CA. */
typedef struct {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x44CA - 0x10];
    s16 time;
} Ovl11C2B04GlobalState;

/* Current-object draw state at shared work +0xC/+0x4476..+0x4480.
 * The renderer uses the unsigned low halfwords of three word coordinates.
 * Keep the struct-member base/displacement addressing and existing widths. */
struct Ovl11CurrentObjectSpriteWork {
    char pad_000[0xC];      /* 0x0000-0x000B */
    s32 flags;             /* 0x000C */
    char pad_010[0x4466];   /* 0x0010-0x4475 */
    s16 drawSuppressed;     /* 0x4476: zero permits drawing */
    u16 xLow;              /* 0x4478 */
    u16 pad_447A;           /* 0x447A */
    u16 yLow;              /* 0x447C */
    u16 pad_447E;           /* 0x447E */
    u16 zLow;              /* 0x4480 */
};

/* Object-load entry: pointer +30, flags +4450/+5234, state +4476. */
struct Ovl11ObjectLoadEntryView {
    char pad0[0x30];
    u16 *currentObject;
    char pad34[0x4450 - 0x34];
    s32 workFlags;
    char pad4454[0x4476 - 0x4454];
    s16 spriteSourceState;
    char pad4478[0x5234 - 0x4478];
    s32 controlFlags;
};

/* 800E46A8 reads flags +0xC, an s16-record pointer +0x30 and s16 +0xE778. */
typedef struct {
    char pad_00[0xC];
    s32 flags;
    char pad_10[0x30 - 0x10];
    s16 *current;
    char pad_34[0xE778 - 0x34];
    s16 selectedId;
} Ovl11RecordSpriteWork;

/* View of D_8006C838 for ovl_11_func_800EE7BC: the u8 at +0x44CD, the s16 at
 * +0x91C2 and the u16 at +0xE7A0. Reached as struct members so cc1 keeps the
 * base address of D_8006C838 and uses each field offset as a displacement; for
 * the two offsets above 0x7FFF it keeps lui %hi(D_8006C838), ori 0x8000, addu
 * followed by the residual displacement 0x11C2 / 0x67A0. */
struct struct_8006C838_800EE7BC {
    char pad_000[0x44CD];   /* 0x0000-0x44CC */
    u8 field_44CD;          /* 0x44CD */
    char pad_44CE[0x4CF4];  /* 0x44CE-0x91C1 */
    s16 field_91C2;         /* 0x91C2 */
    char pad_91C4[0x55DC];  /* 0x91C4-0xE79F */
    u16 field_E7A0;         /* 0xE7A0 */
};

/* View of D_8006C838 for ovl_11_func_800D4030: the u16 at +0x99D8. Reached
 * as a struct member so cc1 keeps lui %hi(D_8006C838) as the base and adds
 * 0x8000 then uses 0x19D8 as the load displacement (the target keeps lui
 * %hi(D_8006C838), ori 0x8000, addu, lhu 0x19D8) instead of folding the
 * offset into %hi. */
struct struct_8006C838_800D4030 {
    char pad_000[0x99D8];   /* 0x0000-0x99D7 */
    u16 field_99D8;         /* 0x99D8 */
};

/* ovl_11_func_800FC358: lh at +0x4A84, text address at +0x51C8,
 * and signed lw at +0x5224, all through the same D_8006C838 base. */
struct struct_8006C838_800FC358 {
    char pad_0[0x4A84];
    s16 field_4A84;
    char pad_4A86[0x51C8 - 0x4A86];
    s16 field_51C8;
    char pad_51CA[0x5224 - 0x51CA];
    s32 field_5224;
};

/* 8010B994 passes four words at D_8006C838+0x52C8 by value to 800D05D0. */
typedef struct {
    char pad_00[0x52C8];
    s32 position[4];
} Ovl11B994PositionView;

/* View of D_8006C838 for ovl_11_func_800CBF70: the s32 at +0x52CC. Reached
 * as a struct member so cc1 keeps lui %hi(D_8006C838) as the base and uses
 * 0x52CC as the load displacement (the target keeps lui %hi(D_8006C838),
 * addiu %lo, lw 0x52CC) instead of folding the offset into %hi. */
struct struct_8006C838_800CBF70 {
    char pad_000[0x52CC];   /* 0x0000-0x52CB */
    s32 field_52CC;         /* 0x52CC */
};

/* View of D_8006C838 for ovl_11_func_800EE944: the s16 at +0xAE30. Reached
 * as a struct member so cc1 keeps lui %hi(D_8006C838), addiu %lo, ori 0x8000,
 * addu followed by lh 0x2E30 (the target keeps the 0x8000 split rather than
 * folding 0xAE30 into the %hi). */
struct struct_8006C838_800EE944 {
    char pad_000[0xAE30];   /* 0x0000-0xAE2F */
    s16 field_AE30;         /* 0xAE30 */
};

/* View of D_8006C838 for ovl_11_func_800E8F78: the s32 slots at +0x522C and
 * +0x5230 and the s32 flag word at +0x5234. The tail reaches these as struct
 * members so cc1 rematerialises lui %hi(D_8006C838) + addiu %lo instead of
 * reusing the D_80071B00 - 0x52C8 base kept live across the switch. */
/* 8011C298 clears the 25-entry counter table and updates the player state. */
typedef struct {
    u8 pad0[0x44F8];
    s32 status;
    u8 pad44FC[0x4A88 - 0x44FC];
    u16 count;
    u16 entries[25];
    s32 total;
    u8 pad4AC0[0x51C4 - 0x4AC0];
    s16 action;
    u8 pad51C6[0x51C8 - 0x51C6];
    u8 object[0x51E6 - 0x51C8];
    s16 level;
} Ovl11PurchaseState;

/* 800ED634 also clears the signed halfword at +0x51F6. */
struct struct_8006C838_800E8F78 {
    char pad_000[0x51F6];
    s16 field_51F6;
    char pad_51F8[0x522C - 0x51F8];
    s32 field_522C;         /* 0x522C */
    s32 field_5230;         /* 0x5230 */
    s32 field_5234;         /* 0x5234 */
};

/* 8011A25C: signed category/item IDs, interaction flags and raw position. */
typedef struct {
    u8 pad0[0x522C];
    s32 field_522C;
    s32 field_5230;
    s32 flags;
    u8 pad5238[0x524C - 0x5238];
    s16 categoryItemId;
    u8 pad524E[4];
    s16 selectedItemId;
    u8 pad5254[0x52C8 - 0x5254];
    s32 x;
    s32 y;
    s32 z;
} Ovl11TierUpgradeWork;
/* 8011A25C indexes these bank-2 message IDs by validation status minus one. */
extern s16 gOvl11UpgradeFailureMessages[6];

/* 80102B6C: lw/sw flags at +0x5234 and sh fields at +0x5252/+0x5254/+0x5256. */
struct Ovl11SpriteRequestState {
    u8 pad0[0x5234];
    s32 flags;
    u8 pad5238[0x5252 - 0x5238];
    u16 id;
    s16 value;
    s16 active;
};

/* View of D_8006C838 for ovl_11_func_800E65AC: the u8 at +0x9A6C. Reached as
 * a struct member with the base D_80071B00 - 0x52C8 kept live, so cc1 emits
 * addiu -0x52C8, ori 0x8000, addu followed by lbu 0x1A6C. */
struct Ovl11RecordAnimationStateView {
    char pad_000[0x9A6C];   /* 0x0000-0x9A6B */
    u8 animationState;      /* 0x9A6C */
};

/* View of D_8006C838 for ovl_11_func_800E65AC: the s16 at +0xDE22. Same
 * base/split form as the +0x9A6C view, ending in sh 0x5E22. */
struct Ovl11CompactRecordDirectionView {
    char pad_000[0xDE22];   /* 0x0000-0xDE21 */
    s16 direction;          /* 0xDE22 */
};

/* 800DF84C reads/writes u16 at D_8006C838 + index*0xF8 + 0xDEB6. */
typedef struct {
    u8 pad0[0xDEB6];
    u16 pending;
} Ovl11DF84CStateView;

/* 800E20F0 reads lh +44BA, lhu +44C0 and lh +E4C8 through D_8006C838. */
typedef struct {
    u8 pad0[0x44BA];
    s16 field_44BA;
    u8 pad44BC[4];
    u16 field_44C0;
    u8 pad44C2[0xE4C8 - 0x44C2];
    s16 field_E4C8;
} Ovl11E20F0StateView;

/* 8010CAD4 reads unsigned halfwords at D_8006C838 +0x44C0 and +0xE4C8. */
typedef struct {
    u8 pad0[0x44C0];
    u16 field_44C0;
    u8 pad44C2[0xE4C8 - 0x44C2];
    u16 field_E4C8;
} Ovl11CAD4StateView;

/* Direction-remap mode at D_8006C838+0xE4CC: 0/3 suppress rotation,
 * 1 forces it, other values use the primary-object latch. Keep this member
 * view: it preserves the original high-base plus signed-displacement access. */
struct Ovl11DirectionRemapState {
    char pad_000[0xE4CC];
    s16 mode;
};

/* 8011F958 stores a halfword at D_8006C838 + 0xE4D2 before resetting selection. */
struct Ovl11SelectionResetState {
    char pad0[0xE4D2];
    u16 savedSelection;
};

/* 8011F958 writes the word at D_8007AFF0 + 0x254A0 through that base. */
struct Ovl11SelectionResetMode {
    char pad0[0x254A0];
    s32 mode;
};

/* D_8007A670 - u8 base of the 0xF8-stride records reached by
 * ovl_11_func_800E65AC for arg3 == 1: the target passes (char *)D_8007A670 +
 * idx*0xF8 to ovl_11_func_800F06D8 (lui + addiu %lo(D_8007A670)).
 * Absolute-addressed. */
extern u8 gCompactObjectPositionCoordinates[];

/* D_80071B00 - s32 triple at 0x80071B00 (absolute, lui+%lo). Fields +0/+8 are
 * the two trackers written by ovl_11_func_800E8F78; p[0..2] are the three
 * values ovl_11_func_800E9778 and ovl_11_func_800EDEB8 copy to D_80129560.
 * Declared here (not per-file) so absolute addressing is derived once. */
extern s32 gObjectWorkPosition[3];

/* Four 4-byte selection slots: D08FC scans slots 0/1 at work+99C8,
 * slots 2/3 at work+99D0; D0BC8 tests the same leading -1 free marker.
 * The eight-halfword extent removes the old small-array label alias. */
extern s16 gRecordPairSlots[8];

/* 8005F0A8: DrawNumberText formats up to nine glyphs plus FFFF via WriteDecimalText,
 * then passes this scratch to DrawTextLine; retain the ten-halfword split-address view. */
extern s16 gTextNumberDrawBuffer[10];

/* D_80055988 - s16 array accessed with absolute addressing (lui+addiu+lh)
 * Index: (s16)arg. Array size of 5 ensures >8 byte declaration for absolute addressing */
extern s16 gMessagePortraitValueIndices[5];

/* 80049044: DrawGlyphSprite indexes six CLUT Y rows; setClut supplies X=0x380. */
extern u16 gTextPaletteClutRows[6];

/* D_80048B1C - table of 0x28-byte entries (CD file table). func_80014854
 * reads the s32 at offset 0x24 (last s32) of entry [arg] as a CD position.
 * Byte stride 0x28 = 40, absolute-addressed (owned elsewhere). */
typedef struct {
    char name[0x24];    /* 0x00-0x23 - filename / padding */
    u32 loc;            /* 0x24 - CD position, logical >> */
} CDLocTableEntry;      /* 0x28 */

extern CDLocTableEntry gCdArchiveEntries[];

/* cd_loader's GP-relative words: witnessed loads/stores and SDK CdlLOC use. */
extern s32 gCdReadLocation;
extern s32 gCdRangeReadBusy;
extern s32 gCdRangeFirstEntry;
extern s32 gCdRangeEndEntry;
extern s32 gCdRangeDestination;
extern s32 gCdRangeLeadingBytes;
extern s32 gCdRangeEntry;
extern s32 gCdRangeReadComplete;
extern volatile s32 gCdArchiveReadBusy;
extern s16 gCdReadEntry;
extern s32 gCdReadByteOffset;
extern s32 gCdReadDestination;
extern s32 gCdReadByteCount;
extern s32 gCdReadSector;
extern s32 gCdArchiveBaseSector;
extern s32 gCdByteRangeReadBusy;

/* D_80048B40 - view of the .loc fields of D_80048B1C as a flat s32 array.
 * Address 0x80048B40 = 0x80048B1C + 0x24 (offset of .loc within entry).
 * func_800145F0 accesses this with lui+addiu base (no extra offset addiu),
 * then indexes by arg*0x28. Array size forces >8 bytes for absolute addressing. */
extern s32 gCdArchiveFirstByteOffset[4];

/* 80049050: SetTextSpeedIndex selects VSync intervals 0/3/5 for text reveal.
 * Retain the existing five-halfword linkage view; only three entries are indexed. */
extern u16 gTextSpeedIntervals[5];

/* D_8005E43C - GP-relative update flag */
extern s32 gSpriteAnimationUpdateLatch;

/* Surface-query state, 8005E4F0..8005E528. Original GP accesses establish
 * surface_collision's tentative ownership. Triangle tests cache X/Z cross
 * products and queryY-averageY; the retained vertices supply plane height.
 * The historical bounds type is the query's three signed world coordinates.
 * See surface_collision.h and the original-address record in file-groupings. */
typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
} BoundsStruct_8001E878;
extern s32 gSurfaceCrossAB;
extern s32 gSurfaceCrossBC;
extern s32 gSurfaceCrossCA;
extern s32 gSurfaceQueryX;
extern s32 gSurfaceQueryZ;
extern s32 gSurfaceHeightDelta;
extern BoundsStruct_8001E878 *gSurfaceQueryPoint;
extern s32 gSurfaceHeightTolerance;
extern s32 gSurfaceTriangleHit;
/* EF8C setter and EAE4 scan snapshot the polygon-header reject mask. */
extern s32 gSurfacePolygonRejectMask;
/* EAE4 stores the vertex base address, remaining count and current polygon. */
extern s32 gSurfaceVertexBase;
extern s32 gSurfacePolygonsRemaining;
extern s32 gSurfacePolygonCursor;
/* EAE4 snapshots the public reject mask before its eight format runs. */
extern s32 gSurfaceActiveRejectMask;
/* E4C0 installs the cell-center extent used as (extent >> 1) + 600. */
extern s32 gSurfaceProximityExtent;
/* EAE4 publishes the accepted polygon address; EF98 returns its raw word. */
extern s32 gSurfaceHitPolygonAddress;

/* D_8005E540, D_8005E54C, D_8005E550, D_8005E554, D_8005E560 - GP-relative s32 scalars
 * Accessed by func_8001FF98 with sw %gp_rel; D_8005E54C by func_800214FC */
extern s32 gSoundBankSlotCursor;
extern s32 gSoundReadBuffer;
extern s32 gSoundLoadWriteIndex;
extern s32 gSoundLoadReadIndex;
extern s32 gSoundSequenceSlotCursor;

/* D_8005E44C - GP-relative s16 scalar (loaded with lh by func_80017AA0) */
extern s16 gTextSpeedInterval;

/* D_8005E47A - signed halfword (lh) — func_80019030 owns it */
extern s16 gTextPenY;

/* Text/slot state: original lhu/lh/lw widths, E46C's sprite-upload pointer,
 * and E4B0's u16-script comparison establish these shared canonical views. */
extern u16 gTextLetterSpacing;
extern s32 gTextSlotLoadDone;
extern s16 gTextUploadedSlot;
extern u8 *gTextSlotBuffer;
extern u16 gTextChoiceIndex;
extern u16 *gTextScriptTable;

/* Text-TU original lh/lhu/lw/sh/sw accesses and matching tentative definitions;
 * preserve signedness and scalar anchors, including the eight-byte number area. */
extern u16 gTextLineSpacing;
extern s32 gTextInputEnabled;
extern u32 gTextPendingSlot;
extern u16 gTextState;
extern u32 gTextTypeTimer;
extern u16 gTextPrevState;
extern u32 gTextPalette;
extern s16 gTextTpage;
extern s32 gTextSlotLoadAsync;
extern s16 gTextLoadedSlot;
extern s32 gTextSlotImageVisible;
extern s16 gTextPortraitSelector;
extern s32 gTextInstantPageMode;
extern s16 gTextPenX;
extern u16 gTextLineStartX;
extern s16 gTextRevealCount;
extern u16 gTextPauseIndex;
extern u16 gTextEndToken;
extern u16 gTextPublishedEndToken;
extern s32 gTextHoldAtEnd;
extern s32 gTextTypeSoundToggle;
extern s16 gTextNumberBuffer;
extern u16 gTextBlockOriginX;
extern u16 gTextBlockOriginY;
extern s16 gTextBlockWidth;
extern s16 gTextBlockHeight;
extern s16 gTextLineCount;
extern s16 gTextLineScroll;
extern s32 gTextPageTimer;
extern s32 gTextRendering;
extern s32 gTextScrolledFull;
/* 80049058: MapChoiceIndexToStatus returns window status codes 10..13 for choices 1..4. */
extern s32 gTextChoiceStatusCodes[4];
/* 8004908C: WriteSeasonNameText copies six glyphs per row: Spring, Summer, Fall, Winter. */
extern u16 gTextSeasonNames[];
/* 800490BC: WriteWeekdayNameText copies three glyphs per row, Sun through Sat. */
extern u16 gTextWeekdayNames[];

/* D_8005E444 - unsigned halfword (lhu) — func_80019030 owns it */
extern u16 gTextScriptIndex;

/* Camera-offset bytes: InitCameraOffsetState stores 2/1/0; setters mark dirty. */
extern s8 gCameraOffsetState;
extern s8 gCameraOffsetInitialized;
extern s8 gCameraOffsetDirty;
/* SetCameraOffsetParameters resets change state and stores radius/height/yaw. */
extern s32 gCameraOffsetChangeState;
extern s32 gCameraOffsetRadius;
extern s32 gCameraOffsetHeight;
extern s32 gCameraOffsetYaw;

/* D_8005E4A8 - pointer to u16 array (lw, used as base) — func_80019030 owns it */
extern u16 *gTextScriptCursor;

/* D_8005E025 - byte table accessed with absolute addressing (lui+addiu+lbu)
 * Array of 9 bytes ensures >8 byte declaration for absolute addressing */
extern u8 D_8005E025[9];

/* 8005F0C8: SetTextSourcePointer stores addresses in slots 0..10; text rendering
 * and scanning expand them via token & 0xFFF. Retain the existing masked-index view. */
extern s32 gTextSourceAddresses[4096];

/* D_80055994, D_800559BC - arrays of s32 pointers (byte table bases),
 * D_800559C4 - array of s32 function pointers.
 * All absolute-addressed (lui+addiu). Array size 3 ensures >8 bytes
 * to avoid GP-relative small-data addressing. */
extern s32 gTextEntryColumnBounds[3];
extern s32 gTextEntryRowBounds[3];
extern s32 gTextEntryColumnHandlers[3];



/* Boot and PollPadInputs address two consecutive 0x38-byte input records
 * here (exe:8005E870). Retain the existing partial first-record lvalue view
 * for actuator sequencers' byte stores; PadInputState is the full layout.
 * The array-backed binding preserves the existing absolute addressing. */
typedef struct {
    char pad[0x36];     /* 0x00-0x35 */
    u8 request0;       /* 0x36 */
    u8 request1;       /* 0x37 */
} PadInputActuatorView;
extern PadInputActuatorView _gPadInputStateStorage[1] __asm__("gPadInputStateStorage");
#define gPadInputStateStorage (*((PadInputActuatorView*)_gPadInputStateStorage))

/* exe:8001FCE4 initializes six words; 8001FD84 integrates/clamps the level
 * and sends its integer part to SsSetMVol. 8001FBF0 clears transferComplete. */
typedef struct {
    char reserved[4];       /* 0x00, initialized to zero, otherwise unused */
    s32 rampState;          /* 0x04: 0=idle, 1=fade in, 2=fade out */
    s32 level16;            /* 0x08: 16.16 master volume */
    s32 step16;             /* 0x0C: signed increment per update */
    s32 fullScale16;        /* 0x10: initialized to 127 << 16 */
    s32 transferComplete;   /* 0x14: completion latch for sound loading */
} MasterVolumeState;
extern MasterVolumeState gMasterVolumeState;

/* BindStateDataTables: absolute-addressed tables bound into state memory. */
extern s32 _D_8004B1A4[3] __asm__("D_8004B1A4");
#define D_8004B1A4 (*((s32*)_D_8004B1A4))
extern s32 _D_80049B1C[3] __asm__("D_80049B1C");
#define D_80049B1C (*((s32*)_D_80049B1C))
extern s32 _D_8004AFBC[3] __asm__("D_8004AFBC");
#define D_8004AFBC (*((s32*)_D_8004AFBC))
extern s32 _D_8004B044[3] __asm__("D_8004B044");
#define D_8004B044 (*((s32*)_D_8004B044))
extern s32 _D_80054BC8[3] __asm__("D_80054BC8");
#define D_80054BC8 (*((s32*)_D_80054BC8))
/* BindStateDataTables partitions 0x18-byte records starting here; retain the
 * existing partial array view rather than inferring the backing-data extent. */
extern char gInitialPoolRecordData[0x124C];

/* D_8006C088 / D_8006C0A8 - sound-driver score tables, [6 songs][1 seq track].
 * The [s][t] shape with t == 1 (SEQ-only) follows the libsnd SsSetTableSize
 * idiom; the true 2D type is required for func_8001FF98 to match (the nested
 * init loop over the second dimension must survive as a real loop). */
extern s32 gSoundSequenceStatesStorage[6][1] __asm__("gSoundSequenceStates");
#define gSoundSequenceStates (gSoundSequenceStatesStorage)
extern s32 gSoundSequencePlayParametersStorage[6][1] __asm__("gSoundSequencePlayParameters");
#define gSoundSequencePlayParameters (gSoundSequencePlayParametersStorage)

/* D_8005E3C0 - pointer stored by func_80011370, accessed GP-relative.
   Scalar declaration (4 bytes) keeps it within -G8 threshold for %gp_rel. */
typedef struct {
    /* 0x000 */ char    pad_0[0x2];
    /* 0x002 */ u16     field_2;        /* u16 read by func_800183E0 (rect.y base) */
    /* 0x004 */ char    pad_4[0xD8 - 0x4];
    /* 0xD8 */  s32     field_D8;
    /* 0xDC */  s32     field_DC;
    /* 0xE0 */  s32     field_E0;
    /* 0xE4 */  s32     field_E4;
    /* 0xE8 */  char    pad_E8[0x4];
    /* 0xEC */  s32     field_EC;
    /* 0xF0 */  s32     field_F0;
    /* 0xF4 */  s32     field_F4;
    /* 0xF8 */  s32     field_F8;
    /* 0xFC */  char    pad_FC[0x14];
    /* 0x110 */ s32     field_110;
    /* 0x114 */ char    pad_114[0x4];
    /* 0x118 */ s32     field_118;
    /* 0x11C */ char    pad_11C[0x4];
    /* 0x120 */ s32     field_120;
    /* 0x124 */ s32     field_124;
    /* Heap carve writes byte capacity/base; packet_work bounds the libgs cursor. */
    /* 0x128 */ s32     packetWorkCapacityBytes;
    /* 0x12C */ s32     packetWorkBase; /* byte address, existing s32 representation */
} struct_8005E3C0;
extern struct_8005E3C0 *gActiveRenderContext;

/* 800A3FB0/800A3FB4 are libgs HWD0/VWD0, declared below as SDK longs.
 * The text-entry backdrop reads their low s16 halves, not separate offsets.
 * Those explicit reads preserve the original unsplit lui/lh loads. */

/* ovl_11:800CFE40 uses three position halfwords and two marker-screen words. */
extern s16 gOvl11ObjectDrawPosition[4];
extern s32 gOvl11ObjectDrawMarkerScreen[2];
/* ovl_11:800D0198 has an independent screen/position scratch pair. */
extern s32 gOvl11OffsetObjectDrawMarkerScreen[2];
extern s16 gOvl11OffsetObjectDrawPosition[4];
/* func_8001C37C caches the render context and increments the GP-relative polygon count. */
extern struct_8005E3C0 *gProjectedPrimitiveContext;
extern s16 gProjectedPrimitiveCount;

/* 800F8F3C indexes 0x22-byte texture records: lh at 0/2/8/A, lhu at 18/1A. */
typedef struct {
    s16 x;
    s16 y;
    s16 x2;
    s16 y2;
    s16 width;
    s16 height;
    s16 width2;
    u16 height2;
    u16 pieceX;
    u16 pieceY;
    u16 pieceX2;
    u16 pieceY2;
    u16 clutX;
    u16 clutY;
    u8 pad_1C[2];
    /* 800F6C78 sets byte +0x1E and compares the u16 texture ID at +0x20. */
    u8 active;
    u8 pad_1F;
    u16 id;
} Ovl11TextureRecord;
/* Shared 72-record cache; resetters and stream/draw users share this view. */
extern Ovl11TextureRecord gOvl11TextureCache[] __asm__("gOvl11TextureCacheStorage");
/* 800F9210 passes this four-halfword scratch rectangle to the image routines. */
extern s16 gOvl11TextureTransferRect[4];
/* 800F7230 reads four lh counts and eight lhu overflow IDs. The fourth
 * retail count is 0x0D36; only request modes 0..2 have bounded list storage. */
extern s16 gOvl11TextureRequestCounts[];
extern u16 gOvl11ExtraTextureRequests[];
extern u16 D_8012965C[][17];
extern u16 D_8012965E[][17];
/* 800F7230 reads three stride-six request lists at +450A/+468A/+480A. */
typedef struct {
    u8 pad_0[0x450A];
    u16 lists[3][64][3];
} Ovl11TextureRequests;

/* ovl_11_func_800CA044 loads this word as the sprite-submission total. */
extern s32 D_8005E3BC;
/* ovl_11_func_800CA044 indexes this storage in 0x78-byte texture banks. */
extern u8 gOvl11PrimarySpriteTextureBanks[];
/* ovl_11_func_800CA044 uses signed halfword lookups with stride two. */
extern s16 gOvl11PrimarySpriteSubstituteIds[];
extern s16 gOvl11PrimarySpriteSubstituteValues[];
extern s16 gOvl11Mode1FSpriteSubstituteIds[];

/* ovl_19:800BF660 - header-initialized UI sprite source, shared by 800BAD50
 * and Ovl19DrawValueTable. Retain the opaque ObjectState-compatible view and
 * its original absolute addressing; animation/frame bytes are at +4/+5. */
extern u8 gOvl19UiSprite[];

/* 800B8C18 updates six 0x44-byte records; BAA60 draws the embedded sprite
 * at +0x10, reading animation +0x14 and palette-Y offset +0x40. */
struct Ovl23DecisionRecord {
    s16 lane;
    s16 unk2;
    s16 state;
    s16 direction;
    s16 unk8;
    s16 unkA;
    s32 xQ12;
    u8 pad10[4];
    u8 animation;
    u8 pad15[0x2B];
    s16 paletteY;
    u8 pad42[2];
};

/* 800B8C18 pairs 0x50-stride fields at +0x2E/+0x38 with records at +0x204. */
struct Ovl23DecisionView {
    struct {
        u8 pad0[0x2E];
        s16 unk2E;
        u8 pad30[8];
        s32 unk38;
        s32 unk3C;
        u8 pad40[0x10];
    } actors[6];
    u8 pad1E0[0x24];
    struct Ovl23DecisionRecord records[6];
};

/* ovl_23:7EA4/8084 dispatch five subphases and seven stages; keep their
 * existing scalar-base/incomplete-array views, with no storage definition. */
extern s32 gOvl23RecordRunSubphases;
extern s32 gOvl23RecordRunStages[];
/* ovl_23:7F00 advances this unsigned setup byte through bank-load stages 0..5. */
extern u8 gOvl23RecordRunSetupStage;
/* ovl_23:8290 reads six initial values and a four-choice halfword table. */
extern u16 D_800BBA7C[];
extern u16 D_800BBA88[];
/* ovl_23:97E0 writes six choices; 8644 uses the same +0x16 halfword array. */
extern u16 D_800BF892[];
/* ovl_23:8B94 drives this decision record and reads its +6 control halfword. */
struct Ovl23Func800BB0C8Arg;
extern struct Ovl23Func800BB0C8Arg D_800BFB08;
extern s16 D_800BFB0E;

/* 800B8644 indexes ten word offsets into the byte data at D_800BB860. */
extern s32 D_800BBA14[10];
extern u8 D_800BB860[];

/* 800B8354 reads six u16 pairs at +0/+2 with four-byte stride. */
extern u16 D_800BBA64[12];
/* 800B8354 indexes this u16 table by signed record +0x32 divided by 50. */
extern u16 D_800BBA90[];

/* 800B8354 writes +0x70/+0x72 relative to each 0x50-stride record base. */
struct Ovl23ScreenInitTail {
    u8 pad0[0x70];
    u16 unk70;
    u16 unk72;
};

/* 800B823C writes +4..+414; 800B8584 writes +0; 800B8754 increments s16 +416.
 * BB040 projects X relative to the signed Q12 camera word at +0x10. */
struct Ovl23ScreenHeader {
    s16 unk0;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 cameraXQ12;
    s16 unk14;
    /* 800B97E0 fills six u16 choices; 800B8644 reads indices 0, 1, 3, 5. */
    u16 unk16[6];
    u8 pad22[0x3F2];
    s16 unk414;
    s16 unk416;
};
extern struct Ovl23ScreenHeader Ovl23_D_800BF87C_header __asm__("gOvl23RecordRunWork");

/* 800B8FA4 updates six 0x50-byte actors at +0x24; BA73C draws the sprite
 * at +0x1C. BB0C8 writes state/direction/timer; texture-offset setup reads
 * palette X/Y at +0x4C/+0x4E. Both paths use this one record layout. */
struct Ovl23ActorDecision {
    s16 unk0;
    s16 unk2;
    s16 state;
    s16 direction;
    s16 timer;
    s16 unkA;
    u8 padC[8];
    s32 xQ12;
    s32 yQ12;
    u8 pad1C[4];
    u8 animation;
    u8 frame;
    u8 pad22[0x2A];
    s16 paletteX;
    s16 paletteY;
};
/* 800B9310 passes six 0x50-byte records at this base to 800BA73C. */
extern struct Ovl23ActorDecision D_800BF8A0[6];

struct Ovl23ActorDecisionView {
    u8 pad0[0x14];
    u16 unk14;
    u8 pad16[0xE];
    struct Ovl23ActorDecision actors[6];
    struct Ovl23DecisionRecord records[6];
    s16 unk39C;
    s16 unk39E;
    u8 pad3A0[0xC];
    s16 unk3AC;
    s16 unk3AE;
};
extern struct Ovl23ActorDecisionView Ovl23_D_800BF87C_decision __asm__("gOvl23RecordRunWork");

/* 800B8C18 retains the aggregate-relative state address across calls. */
struct Ovl23DecisionWindow {
    u8 pad0[0x206];
    s16 unk206;
    u8 pad208[4];
    s16 unk20C;
    s16 unk20E;
    s32 unk210;
};

/* 800B8C18 passes these 0x44-stride records to BB0D8/BA610/B9A00. */
extern struct Ovl23DecisionRecord D_800BFA80[6];

/* ovl_23:800BA494 reads four three-u16 weight rows at BBA9C (24 bytes). */
extern u8 gOvl23RecordRandomWeights[];
/* ovl_23:800BA3B4 reads four five-s16 trial rows at BBAB4 (40 bytes). */
extern s16 gOvl23RecordRandomTrialThresholds[];

/* 937C resets both rings; 9454/94D0 append; 954C renders the same nodes.
 * These views share the original D_800BF87C storage with the scene header,
 * decision records and sprite data. They define no new storage. */
typedef struct {
    s32 xQ12;
    s16 laneY;
    s16 state;
} Ovl23TrailNode;
typedef struct {
    u8 pad_000[0x418];
    Ovl23TrailNode decisionNodes[54];
    s16 decisionNext;
    u8 pad_5CA[2];
    Ovl23TrailNode actorNodes[12];
    s16 actorNext;
} Ovl23TrailWork;
extern Ovl23TrailWork gOvl23RecordTrails __asm__("gOvl23RecordRunWork");

/* 800B954C reads word +0x10, sprite +0x3B4 and the +0x418/+0x5CC record arrays; named base preserves legacy TU views. */
extern u8 Ovl23_D_800BF87C_draw[] __asm__("gOvl23RecordRunWork");
/* 800B9E04 reads actor +0x38/+0x3C and record +0x210 from one aggregate base. */
extern struct Ovl23DecisionView Ovl23_D_800BF87C_movement __asm__("gOvl23RecordRunWork");

/* 800B9B18 reads +0x39C/+0x39E/+0x3B2; 800B81AC initializes halfwords +0/+0x39C..+0x3B2. */
struct Ovl23MovementBounds {
    s16 unk0;
    u8 pad2[0x39A];
    s16 unk39C;
    s16 unk39E;
    s16 unk3A0;
    s16 unk3A2;
    s16 unk3A4;
    s16 unk3A6;
    s16 unk3A8;
    s16 unk3AA;
    s16 unk3AC;
    s16 unk3AE;
    s16 unk3B0;
    s16 unk3B2;
};
extern struct Ovl23MovementBounds Ovl23_D_800BF87C_bounds __asm__("gOvl23RecordRunWork");

/* D_800BFC30 - ovl_23 bss object state base (0x800BFC30). Referenced with
 * absolute addressing (lui + %lo) from ovl_23_func_800BB1B8, which passes
 * its address as an ObjectState and reads the ObjectState fields at +4/+5
 * through a base pointer biased by -0x3B4. */
extern u8 gOvl23SceneSpriteSource[];

/* ovl_23_func_800BB214 copies 0x13DC bytes here and uses the leading sprite header. */
extern u8 gOvl23SharedSpriteBank[0x13DC];
/* ovl_23_func_800BB214 initializes this 0x30-byte sprite object. */
extern struct SpriteSourceData gOvl23SharedBankSpriteSource;

/* ovl_23_func_800BB534 indexes four sprite-header offsets in original data. */
extern s32 gOvl23ActorSpriteHeaderOffsets[4];
/* ovl_23_func_800BB534 copies 0x29A8 bytes here and resolves sprite headers. */
extern u8 gOvl23ActorSpriteBank[0x29A8];

/* EXE:80071C90 - E4DAC initializes SpriteSourceData here; E9104 uses its
 * animation/result fields through the established ObjectState view and reads
 * s16 at -0x258 for reused frame selection. Keep the existing storage view. */
extern u8 gPrimaryObjectSpriteSource[];

/* ovl_11:801248BC - E9104 publishes this address at work+5214 and writes
 * animation/frame/adjusted operand/9999 at signed halfwords +6/+8/+A/+C. */
extern u8 gOvl11DeferredAnimationRecord[];

/* E9104: base-relative lhu/sh +51FE, pointer sw +5214 and lw/sw +5234. */
struct Ovl11AnimationCommandWorkView {
    u8 pad0000[0x51FE];
    u16 auxiliaryFlags;
    u8 pad5200[0x14];
    void *deferredRecord;
    u8 pad5218[0x1C];
    s32 flags;
};

/* E9104 writes signed halfwords +6/+8/+A/+C into its deferred record. */
struct Ovl11DeferredAnimationCommand {
    u8 pad0[6];
    s16 animationId;
    s16 frame;
    s16 adjustedOperand;
    s16 value0C;
};

/* 8010E6E8: eight signed-halfword base message IDs at ovl_11:80127D78. */
extern s16 gOvl11Mode6MessageIds[8];

/* 8010E6E8 retains this base for sh +51EE, lhu +5200 and words +522C..+5234. */
typedef struct {
    u8 pad0[0x51EE];
    s16 field_51EE;
    u8 pad51F0[0x5200 - 0x51F0];
    u16 field_5200;
    u8 pad5202[0x522C - 0x5202];
    s32 field_522C;
    s32 field_5230;
    s32 field_5234;
} Ovl11E6E8State;

/* 8010DA04: signed halfword lookup, eight entries in original 69960.data. */
extern s16 gOvl11ObjectMode6Messages[8];

/* 8010DA04 retains D_8006C838 as the base for these halfword/word fields. */
typedef struct {
    u8 pad_0[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
    u8 pad_44BE[0x5200 - 0x44BE];
    u16 field_5200;
    u8 pad_5202[0x5234 - 0x5202];
    s32 field_5234;
    u8 pad_5238[0x5252 - 0x5238];
    s16 field_5252;
} Ovl11DA04State;

/* D_800A0708 - 32-byte halfword table (u16[0x10]), absolute addressing.
 * func_80023A9C fills it with a walking halfword pointer (16 stores at
 * byte offsets 0..0x1E, stride 2) then writes entry 15 via a base+0x1E
 * displacement (the target's ori v0,0xFFFF proves unsigned typing);
 * func_80023B5C/func_80023C2C walk it by 2-byte units; a >8-byte element
 * keeps it out of the -G8 small-data range so it stays lui/lo_sum
 * addressed. Declared as an array so the target's array-index store
 * (base register + 30 displacement) is reachable. */
extern u16 _gTextEntryPreviewBuffer[0x10] __asm__("gTextEntryPreviewBuffer");
#define gTextEntryPreviewBuffer (*(u16 (*)[0x10])_gTextEntryPreviewBuffer)

/* D_8005E3B4 — pointer to s32, absolute addressing (func_800134C4 loads with
 * lui/lw self-clobber, then dereferences the loaded pointer). */
extern s32 *gFrameOrderingTable;

/* D_8005E438 - GP-relative u16, sprite tile/entry ID storage */
extern u16 gSpriteEntryTextureId;

/* Message-window request/status words: lw/sw through $gp in 8002261C..22DF8. */
extern s32 gMessageWindowEntry;
extern s32 gMessageWindowArchive;
extern s32 *gMessageWindowText;
extern s32 gMessageWindowStatus;
extern s32 gMessageWindowChoice;
/* 80022964 stores a word; 80022DF8 restores its low halfword to flag slot 3. */
extern s32 gMessageWindowSavedFlag3;
extern s32 gMessageWindowSavedWorkFlags;
extern s32 gMessageWindowQueuedArchive;
extern s32 gMessageWindowQueuedEntry;
extern s32 gMessageWindowAnimationStep;
/* 80022B98 draws the portrait only when this raw word equals one. */
extern s32 gMessagePortraitEnabled;
/* 800225C4 indexes the seven-entry phase dispatch table. */
extern s32 (*gMessageWindowHandlers[7])(void *);
/* 80022AF0 bounds the archive-table index to 0..4. */
extern s16 gMessageWindowArchiveIds[5];
/* 80021FE4 tests state+0xCD and 80022008 clears it. Retain the existing
 * nine-byte storage view; only its first byte has an established meaning. */
extern s8 _gMessageReadBufferBusy[9] __asm__("gMessageReadBufferBusy");
#define gMessageReadBufferBusy (*((s8*)_gMessageReadBufferBusy))
/* Retain existing partial word views; 800229F4 reads 0x2000 bytes into the
 * shared work area, and initializes the portrait's SpriteSourceData there. */
extern s32 _gMessageWindowReadBuffer[3] __asm__("gMessageWindowReadBuffer");
#define gMessageWindowReadBuffer (*((s32*)_gMessageWindowReadBuffer))
extern s32 _gMessagePortraitSource[3] __asm__("gMessagePortraitSource");
#define gMessagePortraitSource (*((s32*)_gMessagePortraitSource))

/* D_8005E70C - absolute s32 scalar (ovl_11_func_800E3C4C). Loaded 4 bytes
 * at +0 with an unsplit lui/lw pair, then passed both as the two arguments
 * of func_800145F0 and as the pointer argument of ovl_11_func_800E3CDC, so
 * it holds a pointer value; kept scalar so it stays within the -G8
 * threshold and the address stays unsplit. */
extern s32 gSpriteBankArchiveBuffer;

/* D_80010098, D_8001009C, D_8005E328 - absolute s32 scalars (func_80011370 / func_8001205C)
 *
 * Scalars, not arrays. Addressing mode does not follow from the declared size:
 * a translation unit that does not define the symbol addresses it absolutely
 * whatever its size. The size decides only whether cc1 splits the address, and
 * <= -G8 leaves the unsplit macro form that the assembler expands into the
 * single-register lui/lw pair these two targets use. Over-declaring either
 * past the threshold forces the two-register split form and stops the match. */
extern s32 D_80010098;
extern s32 D_8001009C;
/* Ovl11LoadAreaResourceSet records its first archive span; scratch queries reserve it. */
extern s32 gLoadedAreaResourceBytes;

/* D_80010078, D_80010088 - CD-ROM filenames in .rodata (func_800147BC)
 * "\\A_FILE.HDT;1" and "\\A_FILE.BIN;1". Accessed absolutely with
 * unsplit lui/addiu. char[] keeps the symbol <= -G8 so the address
 * is not split; this TU does not define them so they are absolute. */
extern char gCdArchiveIndexPath[];
extern char gCdArchiveDataPath[];

/* D_800100A0 - string embedded in func_80010000 at offset 0xA0 ("INIT ERROR\n")
 * Referenced by func_80015704 for FntPrint error messages.
 * s32 array of 3 forces >8 byte declaration for absolute addressing (lui+lw) */
extern s32 _D_800100A0[3] __asm__("D_800100A0");
#define D_800100A0 ((char*)_D_800100A0)

/* Raw SDK read buffers: PadInitDirect binds two 0x22-byte records in
 * func_80014064; func_80014250 selects a port and reads bytes directly.
 * Preserve the aggregate size and existing lvalue wrapper: initialization
 * takes its address, while decoding uses direct 2D indexing. */
extern u8 _gPadReadBuffers[2][0x22] __asm__("gPadReadBuffers");
#define gPadReadBuffers (_gPadReadBuffers)

/* Two eight-byte PadSetAct tables: func_80014064 binds them to SDK ports
 * 0 and 0x10; WritePadActuatorRequests updates them by zero-based port index.
 * Keep the 16-byte aggregate and existing absolute addressing. */
extern u8 gPadActuatorBuffers[2][8];

/* Pad startup sets 1; exe:80014494 clears both actuator tables when zero. */
extern s32 gPadActuatorsEnabled;

/* SDK port IDs (0 and 0x10), copied by PollPadInputs and exe:800140C8.
 * Retain the incomplete aggregate and existing two-byte object copy. */
typedef struct PadPortPair {
    s8 port0;
    s8 port1;
} PadPortPair;
extern PadPortPair gPadPortIds[];

/* Pad actuator alignment data passed to PadSetActAlign. */
extern u8 D_80048B14[];

/* D_80049370 - s32 array used by func_80021604, func_80020E58, and
 * func_800214FC. The aggregate size preserves split absolute address formation. */
extern s32 gSoundArchiveOffsets[3];

/* D_800495CC - per-sound-id s16 parameter table, 78 entries of 7 s16 fields
 * (0x800495CC..0x80049A0E, ending where D_80049A10 begins). Absolute-addressed
 * (lui+addiu) by func_800212A8 and neighbours; read as signed halfwords. */
extern s16 gSoundEffectToneParameters[78 * 7];

/* D_80049A70 - array of 4 s32 pointers to CD filename strings (func_800218C4).
 * Values point to "\\STR\\01.XA;1" through "\\STR\\04.XA;1" in rodata.
 * Absolute-addressed (lui+addiu). Array size 4 ensures >8 bytes. */
extern s32 gCdAudioFileNames[4];

/* PollPadInputs sets 1 after disconnect/find-pad, clears after successful
 * PadSetActAlign (exe:800140C8). Keep the existing eight-byte GP object. */
extern s32 gPadAlignmentPending[2];

/* PollPadInputs stores PadInfoMode(InfoModeCurExID) per port. The actuator
 * formatter (exe:80014494) distinguishes zero from nonzero; four-byte GP object. */
extern s16 gPadExtendedModeIds[2];

/* D_8005E5E8 — double-buffered DRAWENV/DISPENV pair (func_80011370).
 * Accessed at offsets 0x0..0x19A via absolute lui/addiu base (2*0x134 bytes
 * > -G8, so cc1 emits the split absolute lui/addiu address form).
 * Declared as env_struct_0x134[2] so element indexing stays a true ARRAY_REF:
 * expand_expr's get_inner_reference builds the element address base-first
 * (plus(base,offset)), which is what func_800128DC's addu base+offset with the
 * result in the base register needs. The three users only take &D_8005E5E8, so
 * the type change is address-neutral for them. */
typedef struct {
    char pad_0[0x17];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 800E3DC8 reads signed capacity at +11C and buffer word at +124. */
    char pad_1C[0x11C - 0x1C];
    s32 field_11C;
    char pad_120[4];
    s32 field_124;
    char pad_128[8];
    void *field_130;
} env_struct_0x134;

extern env_struct_0x134 gRenderContexts[2];
/* ovl_11:800E7954 clears exactly bytes 19..1B of both 134-byte contexts. */
typedef struct {
    char pad_000[0x19];
    s8 field_19;
    s8 field_1A;
    s8 field_1B;
    char pad_01C[0x134 - 0x1C];
} Ovl11SceneModeRenderContextView;
/* exe:8001DCB0 caches the computed ordering-table depth; 8001DFC8 reads it. */
extern s32 gProjectedQuadOtDepth;
/* exe:8001DCB0/8001DE4C preserve their render context across the quad gate. */
extern env_struct_0x134 *gProjectedQuadDepthContext;
extern env_struct_0x134 *gProjectedQuadFixedDepthContext;

/* 8005E644: interior DISPENV view with the render contexts' 0x134 stride.
 * Not owned by screen_fade (absolute lui/addiu base in target). */
extern env_struct_0x134 gRenderDisplayEnvView[];
/* 80011C24/8001B118 select context 0/1 and store its index at 8005E3A4.
 * Owned by boot/render code, read absolutely by screen_fade. */
extern s32 gActiveRenderContextIndex;

/* 8005E8E0 / 8005E910: packet arrays for UpdateAndDrawScreenFade,
 * indexed by gActiveRenderContextIndex.
 * Element strides 0x18 and 0xC. Declared as sized struct arrays so
 * get_inner_reference builds the element address base-first (the target's
 * lui(base) ... addu base+offset order); the .c casts elements to POLY_F4* /
 * DR_MODE* at use. */
typedef struct {
    char pad[0x18];
} packet_0x18;
typedef struct {
    char pad[0x0C];
} packet_0x0C;
extern packet_0x18 gScreenFadeQuadStorage[];
extern packet_0x0C gScreenFadeDrawModeStorage[];

/* 8005E930 / 8005E960: DrawScreenSubtractOverlay packet arrays indexed by
 * gActiveRenderContextIndex. Same shape as the fade packet pair:
 * element strides 0x18 and 0xC; absolute-addressed (lui/addiu base in
 * the target; this TU does not define either symbol). */
extern packet_0x18 gScreenSubtractQuadStorage[];
extern packet_0x0C gScreenSubtractDrawModeStorage[];

/* D_8005E5D8 — pointer toggled between two buffer bases (func_80011370).
 * Target uses absolute lui/lw addressing. Array size forces >8 bytes. */
extern s32 _D_8005E5D8[3] __asm__("D_8005E5D8");
#define D_8005E5D8 (*((s32*)_D_8005E5D8))

/* D_80049A80 — absolute-addressed u16 array (func_80021CD8).
 * Accessed with lui/addiu/lhu at byte offset index*6 (index*3 elements).
 * Array size forces >8-byte declaration for absolute addressing under -G8. */
extern u16 gCdAudioTrackSettings[6];
/* exe:800218C4 searches four 24-byte CdlFILE records; 80021B90 reads their
 * leading CdlLOCs with the same stride. Retain a word-storage view. */
extern s32 gCdAudioFileLocationStorage[24];
/* exe:8002194C/80021B90 use the phase; 80021B90 stores file/filter indices. */
extern s32 gCdAudioPollPhase;
extern s32 gCdAudioFileIndex;
extern s32 gCdAudioChannel;

/* D_8005E330 — GP-relative s32 (func_800223D4 reads/writes via lw/sw %gp_rel). */
extern s32 gSpriteGridTick;
/* exe:800223D4 initializes and draws this 0x30-byte source; overlays also use
 * it. Six overlay loaders copy 0x13DC bytes of header/tables to 800977F8. */
extern struct SpriteSourceData gSharedSpriteSource;
extern u8 gSharedSpriteData[0x13DC];
/* exe:8002470C/8002495C update byte phases; their GP-relative draw clients
 * divide by 30 to select four/two frames from the shared UI source. */
extern u8 gUiSpriteFourFramePhase;
extern u8 gUiSpriteTwoFramePhase;

/* 80055974: the grid reads indices 0..3; 8005597C is the next table's label.
 * The original declared extent is unknown; keep an incomplete array view. */
extern s16 gSpriteGridFrameIndices[];

/* D_8005E324 — GP-relative s16 (func_80021CD8 writes 1 via sh %gp_rel). */
extern s16 gCdAudioState;

/* D_8005E580 — GP-relative s16 (func_80021CD8 writes via sh; func_8002194C reads signed via lh %gp_rel). */
extern s16 gCdAudioVolumeCountdown;

/* D_8005E584 — GP-relative s32 (func_80021CD8 reads via lw %gp_rel). */
extern s32 gCdAudioStartSector;

/* D_8005E588 — GP-relative s32 (func_80021CD8 reads+writes via lw/sw %gp_rel). */
extern s32 gCdAudioRestartCount;

/* D_8005E58C — GP-relative s32 (func_80021CD8 writes via sw %gp_rel). */
extern s32 gCdAudioLastSector;

/* D_8005E594 — GP-relative s32 (func_80021CD8 reads via lw %gp_rel). */
extern s32 gCdAudioTrackIndex;

/* D_8005E538 — GP-relative s32, sound system flag (func_8001FEA4 clears, func_80020E58/func_800214FC read/write). */
extern s32 gSoundLoadPhase;

/* D_8005E53C — GP-relative s32, sound initialization flag (func_80020818). */
extern s32 gSoundSequencerStarted;

/* D_8005E558 — GP-relative s32, sound system flag (func_8001FEA4 clears, func_800200E4 reads/writes). */
extern s32 gSoundBufferConfigurationCount;

/* D_8005E55C — GP-relative s32, stereo/mono flag (func_80020818, func_80020A14, func_80020A40). */
extern s32 gSoundStereoEnabled;

/* D_8006C368 — SpuCommonAttr global, absolute-addressed (func_8001FEA4 initializes fields
 * and passes to SpuSetCommonAttr). Override replaces s32[3] from globals.h. */
extern SpuCommonAttrO _D_8006C368[1] __asm__("D_8006C368");
#define D_8006C368 (*((SpuCommonAttrO*)_D_8006C368))

/* D_8005EE28 — 0x200-byte sprite upload staging buffer used by
 * func_80017300. The next symbol begins at D_8005F028, fixing the extent.
 * The target forms its address with split lui/addiu pairs; the declaration
 * therefore has to retain the full aggregate size (> -G8), not the scalar
 * type inferred from the symbol boundary. */
extern u8 D_8005EE28[0x200];

/* 8005F0F8: Build*RecordListText appends eligible record names and suffixes here;
 * InsertRecordListText copies their output into the script and resets the FFFF sentinel.
 * Preserve the existing six-halfword split-address base view; it is not a capacity bound. */
extern u16 gTextRecordListBuffer[6];

/* D_80049078 — array of 3 function pointers (func_8001A574 dispatch table).
 * Each callee takes one s32 argument and returns s32. Absolute-addressed (12 bytes, > -G8). */
typedef s32 (*FuncPtr_80049078)(s32);
extern FuncPtr_80049078 _D_80049078[3] __asm__("D_80049078");
#define D_80049078 _D_80049078

/* 80049084: AppendRecordNameText appends {0x51, 0xFFFE, 0xFFFF} after each eligible name.
 * The original eight-byte region includes a trailing zero halfword. Retain the existing
 * five-halfword view: the target splits its address across 8001A850/8001A858. */
extern u16 gTextRecordNameSuffix[5];

/* D_800491E0 — four signed rotation angles loaded by func_8001D348 at offsets 0, 4, 8 and 12. */
extern s32 gProjectedClipAngles[4];

/* D_80061EC8 — camera/reference position as s32[3], accessed with absolute
 * addressing (lui+lw). func_8001C1C0 reads all three elements. */
extern s32 gProjectedClipOriginStorage[3] __asm__("gProjectedClipOrigin");
#define gProjectedClipOrigin (gProjectedClipOriginStorage)

/* D_80061EA8 — array of plane coefficients (s16), accessed with absolute
 * addressing (lui+lh). func_8001C1C0 reads 12 elements (4 planes × 3 components).
 * Array size 12 ensures >8 bytes for absolute addressing under -G8. */
extern s16 gProjectedClipNormalsStorage[12] __asm__("gProjectedClipNormals");
#define gProjectedClipNormals (gProjectedClipNormalsStorage)

/* D_80061E88 — 0x20-byte GTE MATRIX (3x3 s16 rotation + 3 s32 translation).
 * func_8001BB88 reads the GTE rotation into it (gte_ReadRotMatrix), and
 * func_8001BBD8 composes it in place (SetRotMatrix, three ldclmv/rtir12/stclmv
 * columns, SetTransMatrix, ldlv0/rtv0tr/stlvl, reload). The MATRIX view is
 * required by the target: under the generated `extern s32` scalar the composed
 * column field addresses fold to two-instruction `la sym+offset` forms
 * (lui v0,%hi; addiu v0,%lo) where the original emits `addiu v0,a1,offset`
 * against the once-loaded base. Layout mirrors psyq/libgte.h MATRIX. */
typedef struct {
    short m[3][3];
    long t[3];
} GteTransformStorage;
extern GteTransformStorage _gCapturedGteTransform[1] __asm__("gCapturedGteTransform");
#define gCapturedGteTransform (*((GteTransformStorage*)_gCapturedGteTransform))

/* D_80010000 - function pointer table at start of code segment (2 entries).
 * Array size 3 forces >-G8 declared size for split absolute addressing (lui/addiu). */
typedef void (*InitFunc)(void);
extern InitFunc D_80010000[3];

/* exe:1B074 sets enable, 1B100 clears it; 1B118 guards recursive rendering. */
extern s32 gAuxSpriteDisplayEnabled;
extern s32 gAuxSpriteDisplayRendering;

/* D_8005F2E8 - OT ring buffer (0x100 bytes = 0x40 u32 entries; func_8001AFE0
 * zeroes a larger 0x1300-byte sprite render region starting here). Declared
 * >-G8 so cc1 materializes the address as a SPLIT lui/addiu pair (two RTL
 * insns). Under the generated 4-byte s32 declaration the address was a single
 * unsplit `la` macro and CSE folded the OT base into each ClearOTagR use,
 * destroying the callee-saved $s2 web func_8001B074 keeps (move a0,s2 /
 * addiu a0,s2,128 across four calls). Size class only; this TU never defines it. */
extern u32 gAuxSpriteOrderingTables[0x40];

/* D_8005F2B8 - SpriteSourceData instance (0x30 bytes). Declared >-G8 so cc1
 * materializes its address as a SPLIT lui/addiu pair in RTL, matching how the
 * original TU compiled it (target lui s1,%hi / addiu s1,%lo). The declared size
 * class feeds the local-allocator quantity structure (HIGH temp merges into the
 * base quantity); with a 4-byte declaration the address is an unsplit `la` macro
 * and the register quantity differs from the original, shifting the callee-saved
 * assignment. This TU never defines it. */
extern u32 gAuxSpriteSource[0xC];

/* D_800605F0 - SpriteDataHeader instance. Same split-address declaration class
 * as D_8005F2B8/D_8005F2E8; target reaches it with lui s0,%hi / addiu s0,%lo. */
extern u32 gAuxSpriteHeader[0x10];

/* D_80128D00 - ovl_11 sprite source-data object (0x30 bytes).
 * The complete type is defined in game_types.h; forward-declare it here
 * because common.h includes this header before the shared types. */
extern struct SpriteSourceData gOvl11SharedSlotSpriteSource;

/* D_80070400 - ovl_11 sprite source-data array base (0x30-byte elements).
 * Walked as a contiguous array by ovl_11_func_800DA588 and indexed by
 * ovl_11_func_800F5BC0 as &D_80070400 + index. Same incomplete-type forward
 * declaration class as D_80128D00 above. */
extern struct SpriteSourceData gSpriteSourceBankBase;

/* InitCameraOffsetState computes a three-word offset, zeros its origin, and
 * copies the offset into a backup. Retain the existing array-backed lvalue
 * views used by callers; each object is 12 bytes and addressed absolutely. */
extern s32 _gCameraOffsetVector[3] __asm__("gCameraOffsetVector");
#define gCameraOffsetVector (_gCameraOffsetVector[0])
extern s32 _gCameraOffsetOrigin[3] __asm__("gCameraOffsetOrigin");
#define gCameraOffsetOrigin (*((s32*)_gCameraOffsetOrigin))
extern s32 _gCameraOffsetBackup[3] __asm__("gCameraOffsetBackup");
#define gCameraOffsetBackup (*((s32*)_gCameraOffsetBackup))

/* E878 retains three vertex pointers; E38C interpolates their triangle.
 * SurfaceVertex's three signed halfwords are defined in game_types.h. */
struct SurfaceVertex;
extern struct SurfaceVertex *gSurfaceHitVertices[3];

/* exe:8005E850 — snapshot of the last submitted VAB transfer chunk.
 * StepSoundBankBodyTransfer copies fields 0..0x1C before advancing.
 * Array size 8 (32 bytes) forces >-G8 declaration for split absolute
 * addressing (lui/addiu with real register) matching the target. */
extern SoundBankTransferState gSoundPreviousBankTransferStorage[1] __asm__("gSoundPreviousBankTransfer");
#define gSoundPreviousBankTransfer (*((SoundBankTransferState*)gSoundPreviousBankTransferStorage))

/* D_80054BC0 - s32 triple read by func_8001A284 (case 15) with the split
 * two-register form `lui v0,%hi` / `lw a0,%lo(v0)`: declared size 12 bytes
 * (> -G8) so cc1 emits the split two-register address rather than the
 * <=-G8 self-clobber macro pair. Not classified in globals.h. */
extern s32 D_80054BC0[3];

/* D_80054BBC - s32 quad read by func_80024108 / func_80023D08 (grid-cursor
 * TU): split two-register address (`lui r,%hi` / `lw r2,%lo(r)` with
 * base register != load destination). Declared array size 16 bytes (> -G8)
 * so cc1 emits the split two-register form rather than the <=-G8
 * self-clobber macro pair. Not classified in globals.h. */
extern s32 gTextBankOffsets[4];

/* D_8005175C / D_80051768 - s32 scalars whose addresses func_8001A284
 * materializes with the unsplit `la` form (expanded by ASPSX to a
 * self-clobbering lui/addiu pair). D_8005175C declared as an array so its
 * size exceeds -G8: func_80024108's target materializes &D_8005175C with
 * the SPLIT two-insn form (lui %hi / addiu %lo as separate defs), which
 * cc1 only emits for a symbol classified >small data. */
extern s32 D_8005175C[4];
/* 8011CEE0/8011CF10 add these address anchors to D_80054BC0[0] before
 * copying the primary/secondary value label; neither loads the anchor word. */
extern s32 gPrimaryValueLabelTextAnchor;
extern s32 gSecondaryValueLabelTextAnchor;
extern s32 D_80051768;

/* 800517E0 / 800517EE: Ovl11DrawChoicePromptVariant adds D_80054BC0[0] to
 * these compact Yes/No and wide Buy/Don't-buy text anchors, respectively.
 * Keep the existing incomplete arrays and split-address linkage views. */
extern u8 gYesNoChoiceTextAnchor[];
extern u8 gBuyDeclineChoiceTextAnchor[];

/* 800517C6: Ovl11DrawQuantityEditorBalance adds gTextBankOffsets[1] for Money
 * and an additional 0x42 for the G suffix. Retain the incomplete byte-array anchor. */
extern u8 gQuantityEditorMoneyTextAnchor[];

/* D_80061E28 - 0x20-byte u16 struct cleared with memset(&..,0,0x20) and
 * given u16 0x1000 writes at offsets 0x00/0x08/0x10 by func_8001B530.
 * Sibling objects at +0x20 (same 0x20 memset) are declared scalar s32 in
 * globals.h. Plain extern: within $gp range but never defined in this TU,
 * so cc1 emits the split absolute lui/%lo addressing the target shows. */
typedef struct {
    /* 0x00 */ u16 field_0;
    /* 0x02 */ u16 field_2;
    /* 0x04 */ u16 field_4;
    /* 0x06 */ u16 field_6;
    /* 0x08 */ u16 field_8;
    /* 0x0A */ u16 field_A;
    /* 0x0C */ u16 field_C;
    /* 0x0E */ u16 field_E;
    /* 0x10 */ u16 field_10;
    /* 0x12 */ u16 field_12;
    /* 0x14 */ u16 field_14;
    /* 0x16 */ u16 field_16;
    /* 0x18 */ u16 field_18;
    /* 0x1A */ u16 field_1A;
    /* 0x1C */ u16 field_1C;
    /* 0x1E */ u16 field_1E;
} struct_80061E28;
extern struct_80061E28 gSceneDefaultMatrix;

/* 80049068 / 80049070: EnterChoiceState's parallel token/state tables use
 * split absolute addressing (`lui %hi` / `addiu r,%lo` with the base in a
 * scratch register, not the self-clobber `la` pair). Declared as incomplete
 * arrays so cc1 leaves SYMBOL_REF_FLAG unset (size unknown > -G8) and emits
 * the split two-register address form matching the target. Not classified
 * in globals.h. */
/* EnterChoiceState searches the three FFA/FF9/FF8 choice-start tokens. */
extern u16 gTextChoiceStartTokens[];
/* EnterChoiceState stores 7/8/9 for the matched 2/3/4-choice start token. */
extern u16 gTextChoiceStates[];

/* D_800558A4 - two u16 attributes per horse ID. InitHorseRaceEntries reads
 * byte offsets 4*id and (4*id)|2; their exact meanings remain unresolved.
 * Absolute-addressed, owned by the nonmatching data file. */
extern u16 gHorseRaceHorseAttributes[80];

/* D_80055944 - two half-open race-parameter ranges per entrant position:
 * u16 entries [4k..4k+3] are (lo, hi, lo, hi), not screen coordinates.
 * Absolute-addressed, owned by the nonmatching data file. */
extern u16 gHorseRaceEntrantParameterRanges[24];

/* ovl_08: 800B8400 prints the original nine-byte "PADNO=%d" format. */
extern char gOvl08ActuatorTestFormat[9];
/* ovl_08: 800B8400 wraps this retained sequence-preset word in 0..10. */
extern s32 gOvl08ActuatorTestSequenceIndex;

/* ovl_08: 800B8500 phase indexes the three callbacks at 800B7E24. */
extern s32 gOvl08SceneSelectPhase;
/* ovl_08: 800B8504 selects one of the 32 rows at 800B8508. */
extern s32 gOvl08SceneSelectCursor;
/* ovl_08: original callback words, passed as an address-as-s32 table. */
extern s32 gOvl08SceneSelectHandlers;
/* ovl_08 KanjiFntPrint arguments; retain the original scalar address views. */
extern s32 gOvl08SceneSelectHeading;
extern s32 gOvl08SceneSelectSubheading;
extern s32 gOvl08SceneSelectRowFormat;
extern s32 gOvl08SceneSelectCursorMarker;
extern s32 gOvl08SceneSelectBlankMarker;
extern s32 gOvl08SceneSelectEmptyRow;
extern s32 gOvl08SceneSelectInstructions;

/* ovl_08: rows hold a scene ID (-1 disables selection) and a Shift-JIS
 * string address passed to KanjiFntPrint's %s. Original storage is retained. */
typedef struct {
    /* 0x00 */ s32 sceneId;
    /* 0x04 */ s32 labelAddress;
} Ovl08SceneSelectEntry;
extern Ovl08SceneSelectEntry gOvl08SceneSelectEntries[32];

/* ovl_10's format strings are emitted by modules/card_diagnostics.c,
 * rather than declared as unrelated address aliases. */

/* ovl_31 driver: original rodata strings passed to FntPrint. */
extern char D_800B7E48[];
extern char D_800B7E4C[];
extern char D_800B7E64[];
extern char D_800B7E78[];
extern char D_800B7E7C[];
extern char D_800B7E88[];
extern char D_800B7E90[];
extern char D_800B7EA0[];
extern char D_800B7EA8[];
extern char D_800B7EB0[];
extern char D_800B7EC0[];
extern char D_800B7EC8[];
/* ovl_31 driver: six 20-byte menu rows and the latched operation result. */
extern char gOvl31MemoryCardMenuRows[][20];
extern s32 gOvl31MemoryCardResult;

/* ovl_31 memory-card directory listing.  D_800B7EF0 is the "*" name
 * pattern and D_800B7EF4 the "%d file(s) %d block(s)\n" format;
 * D_800B88B0 is the DIRENTRY buffer MemCardGetDirentry fills (the loop
 * reads its +0x18 size word at stride 0x28) and D_800B8B10 the sprintf
 * output buffer.  All are absolute-addressed from the overlay. */
struct DIRENTRY;
extern char D_800B7EF0[];
extern char D_800B7EF4[];
extern struct DIRENTRY gOvl31DirectoryEntries[];
extern char gOvl31DirectorySummary[];
/* ovl_31:800B83B8 passes this word to libmcrd's long* output. */
extern long gOvl31DirectoryFileCount;
/* ovl_10 device editors: signed cursor -1 selects the device byte;
 * four held-button counters edit each byte's upper/lower nibble. */
extern s32 gOvl10DeviceReadCursor;
extern s32 gOvl10DeviceReadUpperIncrementTicks;
extern s32 gOvl10DeviceReadUpperDecrementTicks;
extern s32 gOvl10DeviceReadLowerIncrementTicks;
extern s32 gOvl10DeviceReadLowerDecrementTicks;
extern s32 gOvl10DeviceWriteCursor;
extern s32 gOvl10DeviceWriteUpperIncrementTicks;
extern s32 gOvl10DeviceWriteUpperDecrementTicks;
extern s32 gOvl10DeviceWriteLowerIncrementTicks;
extern s32 gOvl10DeviceWriteLowerDecrementTicks;
extern s32 gOvl10DeviceWriteCursorLatch;
extern s32 gOvl10DeviceReadCursorLatch;
extern unsigned char gOvl10DeviceTransferBytes[];
/* 95F0 packs four address bytes and a transfer length from five words. */
extern unsigned int gOvl10MemoryReadHeader[5];
extern unsigned char gOvl10MemoryReadBytes[];
extern s32 gOvl10MemoryReadUpperIncrementTicks;
extern s32 gOvl10MemoryReadUpperDecrementTicks;
extern s32 gOvl10MemoryReadLowerIncrementTicks;
extern s32 gOvl10MemoryReadLowerDecrementTicks;
extern s32 gOvl10MemoryReadCursor;
extern s32 gOvl10MemoryReadCursorLatch;
/* A394 edits header words with cursor -5..-1 and payload with >=0;
 * header[4] supplies the live transfer length. */
extern s32 gOvl10MemoryWriteCursor;
extern s32 gOvl10MemoryWriteHeader[5];
extern s32 gOvl10MemoryWriteUpperIncrementTicks;
extern s32 gOvl10MemoryWriteUpperDecrementTicks;
extern s32 gOvl10MemoryWriteLowerIncrementTicks;
extern s32 gOvl10MemoryWriteLowerDecrementTicks;
extern unsigned char gOvl10MemoryWriteBytes[];
extern s32 gOvl10MemoryWriteCursorLatch;

/* Six-byte item identity/detail/count record, alignment two. 800C6770
 * copies all six bytes; FB404 merges the count at +4, capped at 99. The
 * two detail bytes remain neutral: this menu copies but does not decode them. */
typedef struct {
    s16 id;
    u8 unk2[2];
    s16 unk4;
} Ovl11ItemState;
/* CBB9C saves the +8A held record before replacement; C6770 restores it. */
extern Ovl11ItemState gOvl11SavedHeldItem;
/* EXE 1A284 exposes fields 19/18/21/20; ovl_11 F3898..F3BA0 add/take them.
 * Preserve the pre-existing five-halfword array-backed lvalue views. These
 * are not four new allocations or evidence that the logical counters overlap. */
extern u16 gSaveCounter19Storage[5] __asm__("gSaveCounter19");
extern u16 gSaveCounter18Storage[5] __asm__("gSaveCounter18");
extern u16 gSaveCounter21Storage[5] __asm__("gSaveCounter21");
extern u16 gSaveCounter20Storage[5] __asm__("gSaveCounter20");
#define gSaveCounter19 (*((u16 *)gSaveCounter19Storage))
#define gSaveCounter18 (*((u16 *)gSaveCounter18Storage))
#define gSaveCounter21 (*((u16 *)gSaveCounter21Storage))
#define gSaveCounter20 (*((u16 *)gSaveCounter20Storage))
/* 801024D8 selects 48-byte groups of eight six-byte records. */
extern Ovl11ItemState D_80070D42_slots[][8] __asm__("D_80070D42");
/* 800F7AA0 passes this base to 800F8224's 64-record, six-byte compaction scan. */
extern Ovl11ItemState D_80071042[64];
/* 800F7AA0 saves/restores the menu cursor using sh/lhu across transfers. */
extern s16 gOvl11MenuGridSavedCursorSlot;
/* 801024D8 reads the table selector with lb. */
extern s8 D_801273EA_table __asm__("D_801273EA");

/* 800E8250 saves/restores a six-byte item and a 48-byte, word-aligned item list. */
typedef struct {
    s32 words[12];
} Ovl11ItemSnapshot;
/* Typed aliases coexist with legacy scalar/array declarations of these symbols. */
extern Ovl11ItemState Ovl11Item_80071A8A __asm__("D_80071A8A");
/* C9D98/C9DE8 operate on eight six-byte records; E8250 copies all 48 bytes. */
extern Ovl11ItemSnapshot gPrimaryObjectSecondaryItemList;
/* 800E8250 copies the six-byte held item here before detaching it. */
extern Ovl11ItemState gOvl11SnapshotHeldItem;
/* 800E8250 saves/restores eight six-byte secondary items with word copies. */
extern s32 gOvl11SnapshotSecondaryItems[12];

/* 800E8250: item-table pointer +20, flags +5234/+91E0, six-byte items,
 * and a pointer +DDD4 whose unsigned halfwords +4/+1C are updated. */
struct Ovl11ItemSlotSnapshotWork {
    u8 pad0[0x20];
    void *itemDefinitions;
    u8 pad24[0x5234 - 0x24];
    s32 flags;
    u8 pad5238[0x5252 - 0x5238];
    Ovl11ItemState heldItem;
    u8 pad5258[0x5288 - 0x5258];
    Ovl11ItemState secondaryItems[8];
    u8 pad52B8[0x91E0 - 0x52B8];
    s32 effectFlags;
    u8 pad91E4[0xDDD4 - 0x91E4];
    u16 *effectHalfwords;
};

/* Selection-menu search reads signed IDs in both six-byte item tables. */
typedef struct {
    s16 id;
    s16 detail;
    s16 count;
} SelectionSearchItem;
typedef struct {
    char pad0[0x450A];
    SelectionSearchItem storedItems[0x40];
    char pad468A[0xB5C];
    s16 capacityExponent;
    char pad51E8[0xA0];
    SelectionSearchItem secondaryItems[0x40];
} SelectionItemSearchView;
/* 80102A64 decrements the same +4 count as an unsigned halfword, then
 * interprets the truncated result as signed before clearing an exhausted ID. */
typedef struct {
    s16 id;
    s16 detail;
    u16 count;
} Ovl11SelectionItemCounter;

/* 80102368 reads lh +51E6 and scans signed IDs in six-byte records at +5288. */
struct Ovl11ItemGridState {
    u8 pad0[0x51E6];
    s16 exponent;
    u8 pad51E8[0x5288 - 0x51E8];
    Ovl11ItemState slots[8];
};

/* 800E7088 reads signed +51E6/+524C and scans six-byte records at +5258. */
struct Ovl11ItemInsertState {
    u8 pad0[0x51E6];
    s16 exponent;
    u8 pad51E8[0x524C - 0x51E8];
    Ovl11ItemState held;
    u8 pad5252[6];
    Ovl11ItemState slots[8];
};

/* Action sub-structure at 0x1C of ItemData (ovl_11_func_800D58D0): fn ptr at
 * +0 called as (s8, s8, s16, s32) -> s32, signed byte at +4, signed byte at
 * +5, s16 at +6. */
typedef struct {
    /* 0x00 */ s32 (*field_0)(s8 arg0, s8 arg1, s16 arg2, s32 arg3);
    /* 0x04 */ s8 field_4;
    /* 0x05 */ s8 field_5;
    /* 0x06 */ s16 field_6;
} ItemAction;

/* Shared item table entry (array behind the D_8006C858 pointer, stride 0x28).
 * Referenced by the ovl_11 item-cluster functions: the offset-0x00 word is
 * read both signed (lh: ovl_11_func_800D5868/800D589C/800D5750 bit tests
 * 0x40/0x8100/0x4000) and unsigned (lhu: e.g. 800C8764, 800CCF58, 800D6090),
 * so it is modelled as a union of both views; type byte at 0x02, s16 at
 * 0x10, status word at 0x18 (signed test + bitfield reads), and an action
 * sub-structure at 0x1C (s32 fn ptr at +0, bytes at +4/+5, s16 at +6). */
typedef struct {
    union {
        /* 0x00 */ u16 flags;
        /* 0x00 */ s16 field_00;
    } u0;
    /* 0x02 */ u8 type;
    /* 0x03 */ char pad_03[0x0E - 0x03];
    /* 0x0E */ s16 field_0E; /* 800D60F0: signed packed-property row index. */
    /* 0x10 */ s16 field_10;
    /* 0x12 */ s16 field_12; /* ovl_30_func_80132F18: lh/sh SELL value. */
    /* 0x14 */ char pad_14[0x18 - 0x14];
    /* 0x18 */ s32 field_18;
    /* 0x1C */ ItemAction action;
    /* 0x24 */ char pad_24[0x28 - 0x24];
} ItemData; /* 0x28 */

/* D_8006C858 - shared item table pointer (main-binary data, absolute addressed
 * from overlays that only declare it extern; stride 0x28 per ItemData). */
extern ItemData *gItemDataTable;

/* 800D60F0 loads item and four-word packed-property pointers at +20/+34. */
typedef struct {
    u8 pad_00[0x20];
    ItemData *items;
    u8 pad_24[0x34 - 0x24];
    s32 (*properties)[4];
} Ovl11PackedItemGlobals;

/* 800D603C reads item pointer +20, signed sale value +12 and flags +44F8. */
typedef struct {
    /* 0x00 */ char pad_00[0x20];
    /* 0x20 */ void *items;
    /* 0x24 */ char pad_24[0x44F8 - 0x24];
    /* 0x44F8 */ s32 statusFlags;
} Ovl11ItemSaleState;

/* 800CBE2C passes the halfword-aligned tag at +52C4 and reads u16 +929C. */
typedef struct {
    char pad0[0x52C4];
    Ovl25SetupStatePair collision;
    char pad52C8[0x929C - 0x52C8];
    u16 selectedRecord;
} Ovl11CBE2CState;

/* 800CB850 reads the unsigned selected record at D_8006C838 + 0x929C. */
typedef struct {
    u8 pad0[0x929C];
    u16 selectedRecord;
} Ovl11CB850State;

/* 800CBB9C: D_8006C838 pointer at +20, object at +51C8, u16 at +5200. */
typedef struct {
    char pad0[0x20];
    ItemData *items;
    char pad24[0x51C8 - 0x24];
    char object[0x38];
    u16 state;
} Ovl11CBB9CGlobals;

/* 800BE6E4: 0x28-byte items have u16@0, s8@3 and signed variant indices at +4. */
typedef struct {
    u16 flags;
    u8 type;
    s8 group;
    s16 variants[18];
} Ovl11TileItem;
/* 800BE6E4 reaches these pointer fields from D_80071DFC - 0x55C4. */
typedef struct {
    u8 pad_0[0x20];
    Ovl11TileItem *items;
    u8 pad_24[4];
    u16 (*properties)[4];
    u16 (*groups)[8];
} Ovl11TilePools;
/* 800D7B8C reads item/group pointers at +20/+2C, lh +44BA and lhu +44D4. */
typedef struct {
    u8 pad_0[0x20];
    Ovl11TileItem *items;
    u8 pad_24[4];
    u16 (*properties)[4];
    u16 (*groups)[8];
    u8 pad_30[0x44BA - 0x30];
    s16 season;
    u8 pad_44BC[0x44D4 - 0x44BC];
    u16 enabled;
} Ovl11GrowthGlobals;
/* 800BE6E4 reloads this pointer and reads u16 at signed-index*8 + 4. */
extern u16 (*D_8006C860)[4];

/* D_801247E8 - 51-entry s32 delta table (0xCC bytes, 0x801247E8-0x801248B4).
 * Read as adjacent words by ovl_11_func_800E3978 (a delta lookup) and by
 * ovl_11_func_800E351C/800E36CC (sprite-map offsets). Absolute-addressed. */
extern s32 gOvl11SpriteBlockOffsets[51];

/* 800EC064 scans ten flag words at stride 0xB4, starting at this address. */
extern s32 D_80074320;

/* D_800759E4 - shared field/stage-config struct (absolute-addressed from the
 * ovl_11 overlays, owned by the main EXE's data). Accessed at 0x00 (u16,
 * lhu by ovl_11_func_80108CD0), 0x34 (s32 flags word, func_800CF044 /
 * func_800EC064), and 0x30/0x38/0x3C/0x40 written by ovl_11_func_8010941C
 * per sub-state argument (0: 0x30=1,0x38=-0x708,0x40=0xC1C; 1:
 * 0x30=9,0x38=0xE6,0x40=-0x514). */
typedef struct {
    /* 0x00 */ u16 f00;
    /* 0x02 */ char pad_02[0x30 - 0x02];
    /* 0x30 */ u16 f30;
    /* 0x32 */ char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 f34;
    /* 0x38 */ s32 f38;
    /* 0x3C */ s32 f3C;
    /* 0x40 */ s32 f40;
} struct_800759E4;
extern struct_800759E4 gStageMode1RecordStorage[1] __asm__("gStageMode1Record");
#define gStageMode1Record (*((struct_800759E4 *)gStageMode1RecordStorage))

/* EFABC..EFF04 resolve/read/write/clamp these 48 descriptors, stride 0x14.
 * Access kinds 0/1 are u8, 2 u16, 3 s16, all others s32. */
typedef struct {
    /* 0x00 */ u8 *data;
    /* 0x04 */ u8 kind;
    /* 0x06 */ s16 elementStride;  /* bytes; zero selects a scalar */
    /* 0x08 */ s16 minimum;
    /* 0x0A */ u8 pad_0A[2];
    /* 0x0C */ s32 maximum;
    /* 0x10 */ s32 mask;           /* nonzero selects a boolean bit field */
} Ovl11StateField;
extern Ovl11StateField gOvl11StateFields[48];

/* ovl_11:801249F8 — five three-s16 code rows. The condition-triple
 * command reads +0/+2/+4 and writes them to the shared value slots.
 * Preserve the existing casted array view and absolute addressing. */
typedef struct {
    /* 0x00 */ s16 field_0;
    /* 0x02 */ s16 field_2;
    /* 0x04 */ s16 field_4;
} Ovl11Triple6;
extern Ovl11Triple6 _gOvl11ConditionCodeTriples[] __asm__("gOvl11ConditionCodeTriples");
#define gOvl11ConditionCodeTriples ((Ovl11Triple6 *)_gOvl11ConditionCodeTriples)

/* ovl_11:801249EC — condition-indexed byte offsets added to code 0x7C3.
 * Normal command indices are 0..9; original storage ends at 801249F8. */
extern u8 gOvl11ConditionCodeOffsets[];

/* ovl_11_func_800EB010 copies ten s16 IDs from rodata and walks these parallel ten-entry tables. */
struct Ovl11TenRecordIdTemplate {
    s16 values[2][5];
};
/* EB010 copies the ID template, then walks parallel X/Z/animation-frame rows. */
extern struct Ovl11TenRecordIdTemplate gOvl11TenRecordIdTemplate;
extern s32 gOvl11TenRecordTemplateX[10];
extern s32 gOvl11TenRecordTemplateZ[10];
extern s16 gOvl11TenRecordTemplateFrames[10];

/* ovl_11:800EB85C walks five record IDs and parallel X/Z arrays; the ID label
 * retains its original trailing alignment halfword outside the five entries. */
extern s16 gOvl11FiveRecordSequenceIds[5];
extern s32 gOvl11FiveRecordSequenceXZ[2][5];
/* ovl_11:800F0734 indexes four rows of five signed-halfword adjustments. */
extern s16 gOvl11RecordAdjustmentRows[4][5];

/* ovl_11:800ED7E0 copies five signed event-slot IDs (3,7,9,14,20). */
struct Ovl11CounterSlotCandidates {
    s16 slots[5];
};
extern struct Ovl11CounterSlotCandidates gOvl11SpecialCounterSlotCandidates;

/* Ovl11UpdateColorFlashCommand copies this private three-s16 RGB seed. */
struct Ovl11ColorTriple {
    s16 values[3];
};
extern struct Ovl11ColorTriple gOvl11ColorFlashBaseRGB;

/* 800EC52C copies 25 signed halfwords and adds values from a 5x5 unsigned table. */
struct Ovl11SceneScores {
    s16 values[5][5];
};
extern struct Ovl11SceneScores gOvl11SceneSequenceBaseScores;
extern u16 gOvl11SceneSequenceRandomRanges[5][5];
/* 800EC52C writes this word to 1000 before starting the item operation. */
extern s32 D_80070CB4;
/* 800EC52C uses signed word coordinates and a signed halfword at +44B8. */
typedef struct {
    u8 pad0[0x4478];
    s32 x;
    s32 y;
    s32 z;
    u8 pad4484[0x44B8 - 0x4484];
    s16 level;
} Ovl11SceneState;

/* ovl_11:0x80129560 — twenty s32 value slots. Ovl11CopyEntityValueToSlot
 * writes a zero-extended record halfword; other readers/writers use integer
 * values, selectors and coordinates. Retain the existing casted array view:
 * exposing it directly changes address formation order in three callers.
 * Overlay accesses are absolute, never GP-relative. */
extern s32 _gOvl11ValueSlots[] __asm__("gOvl11ValueSlots");
#define gOvl11ValueSlots ((s32 *)_gOvl11ValueSlots)

/* ovl_11:80129540..54 — camera-command endpoints, captured from scene
 * work +25394/+25398/+253B6. Height/distance are words; target Y narrows
 * to signed halfwords. 801295C6 caches the selected position-record index
 * in 800E880C and is invalidated by camera/snap commands. */
extern s32 gOvl11CameraCommandStartHeight;
extern s32 gOvl11CameraCommandStartDistance;
extern s16 gOvl11CameraCommandStartTargetY;
extern s32 gOvl11CameraCommandEndHeight;
extern s32 gOvl11CameraCommandEndDistance;
extern s16 gOvl11CameraCommandEndTargetY;
extern s16 gOvl11SelectedPositionRecord;

/* ovl_11:800E60A0 caches camera-target XYZ in words 0..2; word 3 is unused. */
extern s32 gOvl11CameraMoveActive;
extern s32 gOvl11CameraMovePosition[4];

/* D_80128422 - s16 slot index into the D_801284AC position table, read by
 * ovl_11_func_8011ECC4 (lh) and cleared by ovl_11_func_8011E574. Only ever
 * declared extern in the overlays, so it stays absolute-addressed. */
extern s16 gOvl11SelectionPanelLane;
/* 8011EE40 scans (ID, lane-slot) byte pairs until the 0xFF sentinel. */
extern u8 gOvl11SelectionLaneSlots[0x88];

/* D_801284AC - 8-byte-stride position records read by ovl_11_func_8011ECC4:
 * a u16 x at +0x00 and an s16 y at +0x02. The function indexes with byte
 * pointers to the base and to base+0x02 so both bases stay in GPRs; only ever
 * declared extern in the overlays, so it stays absolute-addressed. */
extern u8 gOvl11SelectionPanelLanes[];

/* D_8012D810 - ovl_11_func_8011EE98 copies 0x264 bytes from the load buffer;
 * its leading sprite header is passed to func_80015704. */
typedef struct {
    u8 data[0x264];
} Ovl11SelectionPanelSpriteBlock;
extern Ovl11SelectionPanelSpriteBlock gOvl11SelectionPanelSpriteBlock;

/* D_8012DA80 - table of 0x30-byte SpriteSourceData records blitted by
 * ovl_11_func_8011ECC4 at indices 0, 1 and 2. The override header cannot see
 * game_types.h (it is included first), so the element is a 0x30-byte view and
 * the consumer casts to SpriteSourceData *. Only ever declared extern in the
 * overlays, so it stays absolute-addressed. */
struct Ovl11SelectionPanelSpriteStorage {
    char data[0x30];
};
extern struct Ovl11SelectionPanelSpriteStorage gOvl11SelectionPanelSprites[];

/* ovl_11:801248F4 — Ovl11StepMessageCommand scans nine signed entries;
 * a match selects message 0x731 + index. Original accesses are absolute. */
extern s16 gOvl11MessageRemapEntries[9];

/* 8012490C: five bank-1 offsets to the opaque 18-byte payloads copied
 * by Ovl11LoadOrClearCode109Records; original overlay access is absolute. */
extern s32 gOvl11Code109PayloadOffsets[5];

/* D_80127A9C - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127A9C, passed by address to ovl_11_func_800D04D4 from
 * ovl_11_func_8010A0E4. Absolute-addressed from the overlay. */
extern u8 gOvl11ActionEntry8Keyframes;

/* D_80127AAC - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127AAC (0x10 bytes after D_80127A9C), passed by address to
 * ovl_11_func_800D04D4 from ovl_11_func_8010A254. Absolute-addressed from
 * the overlay. */
extern u8 gOvl11ActionEntry11Keyframes;

/* D_80127AB4 - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127AB4 (8 bytes after D_80127AAC), passed by address to
 * ovl_11_func_800D04D4 from ovl_11_func_8010A30C. Absolute-addressed from
 * the overlay. */
extern u8 gOvl11ActionEntry12Keyframes;

/* D_80127AC0 - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127AC0 (12 bytes after D_80127AB4), passed by address to
 * ovl_11_func_800D04D4 from ovl_11_func_8010A3C4. Absolute-addressed from
 * the overlay. */
extern u8 gOvl11ActionEntry13Keyframes;

/* D_80127AFC - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127AFC (0x10 bytes after D_80127AAC... 0x14 after D_80127AE8),
 * passed by address to ovl_11_func_800D04D4 from
 * ovl_11_func_8010ABAC. Absolute-addressed from the overlay. */
extern u8 gOvl11ActionEntry14Keyframes;

/* D_80127B0C - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127B0C (0x10 bytes after D_80127AFC), passed by address to
 * ovl_11_func_800D04D4 from ovl_11_func_8010AC64. Absolute-addressed from
 * the overlay. */
extern u8 gOvl11ActionEntry15Keyframes;

/* D_80127B1C - overlay-local 16-bit state-table base in ovl_11 data at
 * 0x80127B1C (0x10 bytes after D_80127B0C), passed by address to
 * ovl_11_func_800D04D4 from ovl_11_func_8010AD1C. Absolute-addressed from
 * the overlay. */
extern u8 gOvl11ActionEntry16Keyframes;

/* 8010E2C4 passes this byte anchor at 80127D48 to the packed-keyframe stepper. */
extern u8 gOvl11Mode0Keyframes;
/* 8010E37C passes the absolute 80127D50 keyframe-table anchor. */
extern u8 gOvl11Mode1Keyframes;
/* 8010E434 passes the absolute 80127D5C keyframe-table anchor. */
extern u8 gOvl11Mode2Keyframes;
/* 8010E4E4 passes the absolute 80127D68 keyframe-table anchor. */
extern u8 gOvl11Mode3Keyframes;
/* 8010E594 passes the absolute 80127D70 keyframe-table anchor. */
extern u8 gOvl11Mode4Keyframes;
/* 8010EA6C passes the absolute 80127D88 keyframe-table anchor. */
extern u8 gOvl11Mode5Keyframes;

/* 800E30FC passes this 12-byte state table at 801240B0 to 800D04D4. */
extern u8 gOvl11SmallRecordHandler3Keyframes[12];

/* D_801281D0 - overlay-local 10-byte (0xA-stride) record table in ovl_11
 * data at 0x801281D0, indexed by a signed s16 selector in
 * ovl_11_func_80113208 and copied field-by-field into a loaded object
 * record. Absolute-addressed from the overlay. */
extern u8 gOvl11AreaGridUvRecords;

/* One D_801281D0 record: four u/v byte pairs, then the clut halfword. */
typedef struct {
    u8 u0, v0;
    u8 u1, v1;
    u8 u2, v2;
    u8 u3, v3;
    u16 clut;
} Ovl11GridUvRecord;

/* 801295B0: ten number-character halfwords plus FFFF, filled and bound by
 * Ovl11BindIntegerText. Keep the existing scalar-anchor view of this scratch;
 * original accesses are absolute and the binding retains its address. */
extern s16 gOvl11IntegerBindingText;

/* 800F8B4C formats page digits plus FFFF into this 12-halfword buffer. */
extern s16 gOvl11MenuGridPageText[12];

/* 800F84F0 wraps this zero-based page index; 800F8B4C displays index+1. */
extern s16 gOvl11MenuGridPageIndex;
/* 800F84F0/7AA0 use these lh/sh slot/page indices; FF means no selection. */
extern s16 gOvl11MenuGridCursorSlot;
extern s16 gOvl11MenuGridSelectedSlot;
extern s16 gOvl11MenuGridSelectedPageIndex;
/* 800F7AA0 sets 0 to request a slot message; 800F87AC restores FF on close. */
extern s16 gOvl11MenuGridMessageState;
/* 800F84F0 selects links[direction][index]; 787C draws 2^(index+1) slots. */
extern s16 gMenuGridLayoutIndex;
/* 800F84F0/889C: 17 0x10-byte rows, lh x/y at +0/+2, lbu links at +4. */
struct Ovl11MenuNavigation {
    s16 x;
    s16 y;
    u8 links[4][3];
};
extern struct Ovl11MenuNavigation gOvl11MenuGridNavigation[];

/* D_80051B9A - base of a data table in the PS-X EXE, indexed by
 * D_80054BBC and passed to func_80017B3C by ovl_11_func_800FA6CC.
 * Only address-taken; absolute-addressed (split lui/%lo pair). */
extern u8 D_80051B9A[];

/* D_80051BF0 - base of a data table in the PS-X EXE, indexed by
 * D_80054BBC and passed to func_80017B3C by ovl_11_func_800F8B4C (and
 * ovl_11_func_8011775C). Only address-taken; absolute-addressed. */
extern u8 D_80051BF0[];

/* ovl_11_func_8011775C adds the relocation base to these byte-table addresses. */
extern u8 D_80051B90[];
extern u8 D_80051BE6[];
/* ovl_11_func_8011775C reads D_80070CF8 with lhu; retain a distinct typed view. */
extern u16 D_80070CF8_halfword __asm__("gClockHour");

/* D_800535E6 - base of a data table in the PS-X EXE, indexed by
 * D_80054BBC[0] and passed to func_80017B3C by ovl_11_func_800FC998.
 * Only address-taken; absolute-addressed (split lui/%lo pair). */
extern u8 D_800535E6[];

/* D_800535D0 - base of a data table in the PS-X EXE, indexed by
 * D_80054BBC[0] and passed as the source to func_8001ABF0 by
 * ovl_11_func_800FE3A0. Only address-taken; absolute-addressed (split
 * lui/%lo pair, the >8-byte incomplete-array form). */
extern u8 D_800535D0[];

/* D_8005378C / D_800537A2 - two data table bases in the PS-X EXE, each
 * indexed by D_80054BBC[0] and passed to func_80017B3C by
 * ovl_11_func_800FE3A0 when arg0 < 0 / arg0 == 5 respectively. Only
 * address-taken; absolute-addressed (split lui/%lo pair). */
extern u8 D_8005378C[];
extern u8 D_800537A2[];

/* 80051D18: Ovl15UpdateTwoChoiceMenu draws bank-adjusted MEMORY CARD slot 1 / slot 2 labels. */
extern u8 gMemoryCardSlotLabelsTextAnchor[];

/* 80051D80: Ovl15UpdateConfirmationMenu draws bank-adjusted Yes / No choices. */
extern u8 gSaveMenuConfirmationYesNoTextAnchor[];

/* D_80074124 - 7x7 table of 8-byte entries (ovl_11), written by
 * ovl_11_func_800D7B00. Each entry holds two s16 set to 0x167, two spare
 * u8, and an s16 set to 0. Reads in func_800BF630 etc. use the s16 @0
 * (0894 lhu) and u8 @4. Absolute-addressed from the overlays. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s16 unk6;
} Ovl11D124Entry;
extern Ovl11D124Entry gAlternateAreaGrid[7][7];
/* 800DA638 dispatches the two matched three-s16 texture setters. */
extern s32 (*gOvl11CellTextureSetters[2])(s16, s16, s16);
/* 800DA638 reads signed row/column bounds at index*4 and +2. */
typedef struct {
    s16 height;
    s16 width;
} Ovl11GridBounds;
extern Ovl11GridBounds gOvl11CellGridLayerBounds[2];
/* 800DA638 loads nine pairs of halfword offsets, then narrows sums to s16. */
extern u16 gOvl11CellNeighborOffsets[18];
/* 800DA638 accesses the first coordinate halfword in the data word at 80128DAC. */
extern u16 gOvl11CellProbeColumn;
/* 800DA638 addresses the second coordinate through the first +2; preserve
 * the existing assembler identity even with separately named halfwords. */
extern u16 gOvl11CellProbeRow __asm__("gOvl11CellProbeColumn+2");
/* 800DA638 stores the signed variant selector after the coordinate pair. */
extern s16 gOvl11CellProbeVariant;
/* 800DADB0 preserves work-base addressing for the alternate grid and gate. */
typedef struct {
    char pad_0000[0x44D4];
    u16 alternateGridEnabled;
    char pad_44D6[0x78EC - 0x44D6];
    Ovl11D124Entry alternateGrid[1];
} Ovl11GridCellSearchView;

/* 80110790: +20 points at 0x28-byte item rows (signed byte +3);
 * +2C at 0x10-byte mapped-type rows (unsigned halfword +2). */
typedef struct {
    u8 pad_00[0x20];
    void *itemTable;
    u8 pad_24[8];
    void *typeTable;
} Ovl11CandidateTypeLookupView;

/* 800C9F20 reads halfword coordinate pairs; original data spans 32 halfwords. */
extern u16 gOvl11CompanionScreenOffsets[32];

/* 800C4B6C indexes signed halfwords; original data spans six entries. */
extern s16 gOvl11CellResetTierCounts[6];
/* 800C4C94 indexes signed halfwords; original data spans six entries. */
extern s16 gOvl11CellPromotionTierCounts[6];
/* 800C4DE0 indexes signed halfwords; original data spans six entries. */
extern s16 gOvl11CellEffectTierCounts[6];
/* 800C55FC dispatches three pointers through the D7EF8/E090 callback pair. */
extern s32 (*gOvl11AreaCellConverters[])();

/* D_800742AC - word cleared by ovl_11_func_800DBE30 (`extern s32`) and reset
 * together with its following 0x3C bytes by ovl_11_func_800DB9A4, which memsets
 * 0x40 bytes from its address. Absolute-addressed from ovl_11. */
extern s32 gRecordSlotPhase;
/* DBE30 dispatches through the sole original table word, passing that word's
 * address; E3A3C ignores the extra argument. Preserve this callback call ABI. */
typedef s32 (*Ovl11RecordSlotHandler)(void *slot);
extern Ovl11RecordSlotHandler gOvl11RecordSlotHandlers[1];
/* DBE9C uses packed 12-byte copies to shift the five six-halfword records. */
typedef struct __attribute__((packed)) {
    s32 word0, word1, word2;
} Ovl11RecordSlotCopy;
typedef struct {
    u8 pad[0x7AA8];
    s16 lastTag;
} Ovl11RecordSlotTailView;
/* DBEF8 reads the active record pointer at +30 and its mode halfword +4. */
typedef struct {
    u8 pad[0x30];
    s32 *activeRecord;
} Ovl11ActiveRecordSlotView;

/* D_80071DFC - 25x45 table of 8-byte entries (ovl_11); row stride 0x168.
 * ovl_11_func_800D8FC8 walks all 25 rows x 45 entries and ovl_11_func_800DAF60
 * indexes it; absolute-addressed from the overlay. */
extern Ovl11D124Entry gMainAreaGrid[25][45];

/* ovl_27:BA914 selects four 0x8CC-byte ID tables and reads 1125 halfwords. */
extern u16 gOvl27SpriteFrameIds0[];
extern u16 gOvl27SpriteFrameIds1[];
extern u16 gOvl27SpriteFrameIds2[];
extern u16 gOvl27SpriteFrameIds3[];
/* ovl_27:BAC14 copies five packed six-byte records from this resident table. */
extern u8 gOvl27InitialWorkRecords[];
/* ovl_27:9124 constructs a terminated halfword string at this buffer base. */
extern s16 gOvl27TitleSummaryText;
/* ovl_27:9124 indexes this engine text bank by gTextBankOffsets. */
extern u8 D_80051AF6[];

/* View of D_8006C838 for ovl_27_func_800BA914: pointers at +0x20 (0x28-byte
 * records with an s8 at +3) and +0x2C (0x10-byte records with a u8 at +2).
 * Reached from the D_80071DFC base minus 0x55C4. */
typedef struct {
    char pad_00[0x3];
    s8 unk3;
    char pad_04[0x24];
} D8006C838Pool28;

typedef struct {
    char pad_00[0x2];
    u8 unk2;
    char pad_03[0xD];
} D8006C838Pool10;

typedef struct {
    char pad_000[0x20];
    D8006C838Pool28 *unk20;
    char pad_024[0x8];
    D8006C838Pool10 *unk2C;
} D8006C838Pools;

/* exe:800491C8. Signed numerator/denominator per rotation-matrix column.
 * The uniform setter writes all three pairs; the unused variant can differ. */
struct ProjectedRotationScaleRatios {
    /* 0x00 */ s32 xNumerator;
    /* 0x04 */ s32 xDenominator;
    /* 0x08 */ s32 yNumerator;
    /* 0x0C */ s32 yDenominator;
    /* 0x10 */ s32 zNumerator;
    /* 0x14 */ s32 zDenominator;
};
extern struct ProjectedRotationScaleRatios gProjectedRotationScale;

/* Indexed-selection record's existing partial view. Category is the u16
 * at +2; callers also use a signed +2 view and an index at +4. */
struct IndexedSelectionRecord {
    /* 0x00 */ char pad_0[0x2];
    /* 0x02 */ u16 category;
};
extern struct IndexedSelectionRecord *gIndexedSelectionRecord;
/* GP-owned selection/editor state: reset/install and cursor consumers at
 * 8002301C..80024030 establish these widths; cursor is deliberately signed. */
extern u16 gIndexedSelectionState;
extern s16 gIndexedSelectionCursor;
extern u16 gIndexedSelectionConfirmation;
extern u16 gTextEntryState;
extern s32 gTextEntryAllowCancel;
extern s16 gTextEntryAuxState;
extern u16 gTextEntryLength;
extern s32 gTextEntryHasCharacter;
extern u16 gTextEntryConfirmation;
extern u16 gTextEntryCursor;
extern s16 gTextEntryCharacterSet;
extern s32 gTextEntryAlternateLayout;
extern u16 *gTextEntryBuffer;
/* Two code-mapping callbacks at 559CC; 245C8 indexes the 28-halfword table. */
extern s32 gTextEntryCodeHandlers[];
extern u16 gTextEntryCodeTableSet1[28];
/* 80053888: DrawTextEntryGrid selects these symbol-grid labels for a nonzero
 * character set. Only its bank-adjusted address is used; retain the word-shaped anchor. */
extern s32 gTextEntrySymbolGridTextAnchor;

/* ovl_28: 800B9626 is 0=idle, 1=decrease, 2=increase; 800BAB0C is 0..255. */
extern s16 gOvl28SpriteColorRampMode;
extern s16 gOvl28SpriteColorRampLevel;
/* ovl_28: 800B9628 is the pixel rectangle shared by both ramp starters. */
typedef struct {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} Ovl28SpriteColorRampRect;
extern Ovl28SpriteColorRampRect gOvl28SpriteColorRampRect;
/* ovl_28: existing POLY_F4 (800BAB10) and DR_MODE (800BAB30) packet storage. */
extern u32 gOvl28SpriteColorRampPoly[6];
extern u32 gOvl28SpriteColorRampDrawMode[3];

/* VWD0 - PSY-Q libgs vertical display resolution (libgs.h). Forward-declared
 * here because libgs.h is not self-contained. */
extern long VWD0;

/* HWD0 - PSY-Q libgs horizontal display resolution (libgs.h). Forward-declared
 * here because libgs.h is not self-contained. */
extern long HWD0;

/* ovl_28: twenty 12-byte records at 800B9630; allocation starts at VWD0<<12,
 * drawing divides y by 4096, and stepping retires entries at y <= -14<<12. */
typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s16 frameIndex;
    /* 0x06 */ char pad_6[0x2];
    /* 0x08 */ s32 yQ12;
} Ovl28ScrollingSprite;
extern Ovl28ScrollingSprite gOvl28ScrollingSprites[20];
/* ovl_28: sprite source/header pairs, populated by the two archive loaders.
 * Original addresses: 9720/9750 and 9DC0/9DF0; no new C storage. */
extern u8 gOvl28ScrollingSpriteSource[];
extern u8 gOvl28ScrollingSpriteHeader[];
extern u8 gOvl28BackdropSpriteSource[];
extern u8 gOvl28BackdropSpriteHeader[];
/* ovl_28: 66-byte command stream at 800B936C and its 93AE..93BC state. */
extern u8 gOvl28SpriteCommands[66];
extern s16 gOvl28SpriteCommandIndex;
extern s16 gOvl28SpriteCommandTicks;
extern s16 gOvl28SpriteCommandDelay;
extern s32 gOvl28SpriteScrollStepQ12;
extern s32 gOvl28SpriteScrollEnabled;
extern s32 gOvl28SpriteSequenceComplete;
/* ovl_28: original seven-word phase table at 95F8; mutable slots at 9614/18. */
extern s32 gOvl28SpriteSceneHandlers;
extern void (*gOvl28ScrollingSpriteHandler)(void);
extern void (*gOvl28PresentationHandler)(void);
/* ovl_28: asset/display phases and modulo-108000 tick at 961C/9624/9620. */
extern s16 gOvl28SpriteAssetLoadPhase;
extern s32 gOvl28SpriteSceneTick;
extern s16 gOvl28SpriteSceneDisplayPhase;
/* ovl_28: presentation state at 93C0..93CE, selected from 25-ID rows at 93D0. */
extern s32 gOvl28PresentationEnabled;
extern s16 gOvl28PresentationPhase;
extern s16 gOvl28PresentationId;
extern s16 gOvl28PresentationX;
extern s16 gOvl28PresentationY;
extern s16 gOvl28PresentationSetIndex;
extern s16 gOvl28PresentationEntryIndex;
extern u16 gOvl28PresentationIds[][25];

/* D_80070CF2 - s16 global used in overlay 11 button-check functions */
extern s16 gCalendarSeason;
/* 800C0DB4 compares signed record halfwords against these lh-loaded globals. */
extern s16 gCalendarDay;
extern s16 gClockHour;
/* Preserve signed/unsigned halfword and combined hour/minute word views. */
extern s16 D_80070CF8_signedHalfword __asm__("gClockHour");
extern s16 gClockMinute;

/* D_800BAC04 - ovl_11 four-entry {s32 id, handler} dispatch table. The records
 * are {0, ovl_11_func_80111258}, {1, ovl_11_func_80111400}, {2,
 * ovl_11_func_801115A8}, {3, ovl_11_func_80111750}; ovl_11_func_801110CC
 * copies the whole table to a local (8 words, 32 bytes) and indexes it by the
 * s16 D_80070CF2, calling the handler at +4 with the s16 at arg0+0xB6.
 * Defined in the overlay data segment, so only an extern declaration belongs
 * here. */
typedef struct {
    s32 setIndex;
    s32 (*mapItemId)(s16);
} Ovl11SeasonItemSpriteMap;

typedef struct {
    Ovl11SeasonItemSpriteMap entries[4];
} Ovl11SeasonItemSpriteMaps;

extern Ovl11SeasonItemSpriteMaps gOvl11SeasonItemSpriteMaps;

/* D_80125528 - ovl_11 per-index pointer table (10 entries, D_80124FE8 ..
 * D_801254D0). Each entry points at an s32 record list whose word 0 is the
 * record count; ovl_11_func_801110CC reads word [var_a2 + 1] and adds the
 * D_800957F8 blob base. Defined in the overlay data segment, so only an
 * extern declaration belongs here. */
extern s32 *gOvl11SpriteRecordSets[];

/* D_800957F8 - ovl_11 sprite-data blob base (main RAM). func_80015704 headers
 * are resolved as D_800957F8 + record offset by ovl_11_func_800DA588,
 * ovl_11_func_800D688C and ovl_11_func_801110CC. Absolute-addressed from
 * overlay 11 code. */
extern u8 gLoadedSpriteDataBlob[];

/* D_80070C70 - s16 slot aliased with D_8006C838 + 0x4438. ovl_11_func_800E3AA4
 * compares it against D_8007AFF0 + 0x2549C, stores -1 in its early-out arm,
 * and ovl_11_func_800E6B18 writes its argument here. Absolute-addressed. */
extern s16 gObjectSoundBankRequest;

/* 800ED760 writes five word slots; 80107C80 reads each as two signed halfwords. */
extern s32 gActionResultOverrides[5];

/* 80070C92, work +445A: F0A58 copies the active 0x1E-byte record here;
 * E559C reads this partial packet view. Keep its witnessed unsigned size,
 * byte V coordinate and signed CLUT loads, distinct from the copy view. */
typedef struct {
    /* 0x00 */ s16 textureWordX;
    /* 0x02 */ u8 textureY;
    /* 0x03 */ u8 pad3[0x08 - 0x03];
    /* 0x08 */ u16 width;
    /* 0x0A */ u16 height;
    /* 0x0C */ u8 padC[0x18 - 0x0C];
    /* 0x18 */ s16 clutX;
    /* 0x1A */ s16 clutY;
} Ovl11ActiveRecordSpritePacketView;
extern Ovl11ActiveRecordSpritePacketView gActiveRecordSpriteDescriptor;

/* D_800A0384 - ovl_11_func_800C009C reads unsigned halfwords at +0x14/+0x16. */
typedef struct {
    u8 pad_0[0x14];
    u16 unk14;
    u16 unk16;
} Ovl11CameraOffsetView;
extern Ovl11CameraOffsetView D_800A0384;

/* Partial global-object views and external storage used by the integrated
 * reconstruction batch. Field offsets are witnessed; unknown extents remain
 * unsized. Parameter views live in game_types.h. */
typedef struct {
    char pad_0[0x4];
    s16 unk4;
} Recon_func_80023710_Pointee0View;

/* D5D38..D5F44: the +20 record header is also an 18-halfword index row.
 * This is a view of the same 0x28-byte record, not another allocation. */
typedef struct {
    char pad0[4];
    s16 propertyIndex[18];
} Ovl11RecordPropertyRow;

/* D5960..D5F44 share four pointer slots: headers, grids, flag records and
 * type properties. Keep void pointers and witnessed byte-stride arithmetic. */
typedef struct {
    /* 0x00 */ char pad_00[0x20];
    /* 0x20 */ void *recordHeaders;
    /* 0x24 */ void *cellGrids;
    /* 0x28 */ void *flagEntries;
    /* 0x2C */ void *typeProperties;
} Ovl11CellGridTables;

/* ovl_11:80126F8C — eighteen season/day/message records; both the marker
 * scan and the message scan traverse the entire table. Keep its existing
 * object-shaped view while documenting the independently witnessed extent. */
typedef struct {
    u8 season;
    u8 day;
    s16 message;
} Ovl11CalendarPanelEvent;

extern Ovl11CalendarPanelEvent _gOvl11CalendarPanelEvents[1] __asm__("gOvl11CalendarPanelEvents");
#define gOvl11CalendarPanelEvents (*((Ovl11CalendarPanelEvent *)_gOvl11CalendarPanelEvents))

/* ovl_11:80126FD4 — three marker styles: current day, event, saved date.
 * Animation/frame bytes and an unsigned horizontal pixel displacement. */
typedef struct {
    u8 animation;
    u8 frame;
    u16 xOffset;
} Ovl11CalendarMarkerStyle;
extern Ovl11CalendarMarkerStyle gOvl11CalendarMarkerStyles[];

/* D_80126E18 - ovl_11 table of 4 five-byte records, one unsigned byte per
 * field; indexed by D_80070CF2 only after it is bounded to < 4, so the table
 * is exactly 0x14 bytes. Fields are read as bytes (lbu) at offsets 0x0..0x4. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} Recon_ovl_11_func_800F6118_D80126E18Entry;
extern Recon_ovl_11_func_800F6118_D80126E18Entry D_80126E18[];

extern s32 gGraphicsHeapBase;

extern s16 gOvl11LoadedSpriteBankSelector;

extern u8 D_8007BFF8[];
/* CCFF8 loads unsigned halfword byte offsets for its embedded sprite record. */
extern u16 gOvl11ItemSpriteRecordOffsets[];

/* ovl_11:6680 resets the shared menu/interaction phase to 0xFF;
 * 66F0/6734/6778 install phases serviced by 67E0. */
extern s32 gOvl11InteractionPhase;
/* ovl_11:8A3C animates the two menu-grid indicators every nine draws;
 * 6680 clears both halfwords. Preserve signedness of the counter. */
extern u16 gOvl11MenuGridIndicatorFrame;
extern s16 gOvl11MenuGridIndicatorTicks;

extern s32 gOvl11ItemTransferMenuState; /* 0 inactive, 1 browse, 2 inspect, 3 close */

/* ovl_11 800FB648 tests this word flag with lw; 800FB608/800FB628 set and clear it. */
extern s32 gOvl11ClockHudEnabled; /* ovl_11:80127208, zero hides the clock HUD */

/* ovl_11: status-page lifecycle state, 0 idle and 1..6 fade/setup/display. */
extern s16 gOvl11StatusPageState;

/* Eight-page selector; signed comparisons implement wrap and empty-page skips. */
extern u16 gOvl11StatusPageIndex;

/* D_80127210 - first visible row of the seven-row scrolling list drawn by
 * ovl_11_func_800FD034. Signed: scrolling wraps at both ends, and the
 * negative test reads it with lh. */
extern s16 gOvl11StatusRowScroll;

extern u16 gOvl11StatusAnimationTick;

/* D_8012CDC8 - 14-halfword scratch list (0x1C bytes, sh at +0x1A) written by
 * ovl_11_func_800FB908 and passed by address to func_80017B3C from
 * ovl_11_func_800FC1AC. Absolute-addressed from ovl_11 code. */
extern u16 gOvl11StatusHeadingText[14];

/* D_80127308 - 8-entry function-pointer table indexed by the s16 D_8012720E
 * and called from ovl_11_func_800FC1AC; the entries name
 * ovl_11_func_800FC358/800FC998/800FCB80/800FCD0C/800FCEA0/800FD33C/800FD5E8/
 * 800FDD74. Absolute-addressed from ovl_11 code. */
extern void (*gOvl11StatusPageDrawers[8])(void);

/* D_80127214 - 6-halfword threshold table (0x0000, 0x1770, 0x4650, 0x8CA0,
 * 0xFFFF, 0x0000) read by ovl_11_func_800FE068 (entries 1..3, absolute
 * addressing) and ovl_11_func_800FE3A0/ovl_11_func_800FE14C. */
extern u16 gOvl11StatusLevelThresholds[];
/* 800FE14C loads five original 32-bit RGB colors at (abs(level)-1)*4. */
extern s32 gOvl11StatusLevelLightColors[5];
/* 800FE14C uses this parallel five-word RGB table for the gradient endpoints. */
extern s32 gOvl11StatusLevelDarkColors[5];

extern s16 gOvl11StatusSmallRecordCount;

extern s16 gOvl11StatusTaggedRecordCountA;

extern s16 gOvl11StatusTaggedRecordCountB;

/* D_80127220 - 5-entry table of 4-byte records walked by
 * ovl_11_func_800FC6A4: u8 at +0x00 (lbu), u8 at +0x01 (lbu), s16 at +0x02
 * (lh). Absolute-addressed from ovl_11 code; the separately labelled
 * D_80127222/D_80127226/D_8012722A halfwords are the unk2 field of entries
 * 0/1/2. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
} Ovl11D80127220Entry;
extern Ovl11D80127220Entry gOvl11StatusPoolCounts[];
/* 800FB908 writes +2/+6/+A/+E; 800FC5F4 walks four pairs of signed halfwords. */
extern s16 gOvl11StatusFourCounters[];

/* 800FB908 accesses these fields through D_8006C838, retaining its base. */
typedef struct {
    u8 pad0[0x44CD];
    u8 field_44CD;
    u16 field_44CE;
    u16 field_44D0;
    u16 field_44D2;
    u16 field_44D4;
    u8 pad44D6[0x4500 - 0x44D6];
    u16 field_4500;
    u16 field_4502;
    u16 field_4504;
    u16 saveCounter21; /* work+4506 is gSaveCounter21 (80070D3E). */
    u16 field_4508;
    u8 pad450A[0x498C - 0x450A];
    u16 values[170];
    u16 records[80][11];
    u8 pad51C0[0x91AC - 0x51C0];
    u16 field_91AC;
    u8 pad91AE[0x91C2 - 0x91AE];
    u16 field_91C2;
    u8 pad91C4[0x929C - 0x91C4];
    u16 field_929C;
    u8 pad929E[0x92B2 - 0x929E];
    u16 field_92B2;
    u8 pad92B4[0xDBBA - 0x92B4];
    u16 field_DBBA;
    u8 padDBBC[0xE776 - 0xDBBC];
    s16 field_E776;
    s16 field_E778;
    u8 padE77A[6];
    s32 field_E780;
} Ovl11FB908State;

/* D_80127234 - 4-entry table of 0xC-byte records walked by
 * ovl_11_func_800FC754: s32 unk0 at +0x00 (lw, passed as first argument),
 * s32 unk4 at +0x04 (lw, nonzero selects the entry), u8 unk8 at +0x08 and
 * u8 unk9 at +0x09 (lbu). Absolute-addressed from ovl_11 code; written by
 * ovl_11_func_800FB908 at +0x00/+0x04/+0x09. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ char pad_A[0x2];
} Ovl11D80127234Entry;
extern Ovl11D80127234Entry gOvl11StatusNamedRecords[];

/* D_80127328 - 6-entry table of 8-byte records walked by
 * ovl_11_func_800FC8C8: s32 flag mask at +0x00 (lw, ANDed against the
 * D_8006C838+0x44F8 status word) and u8 argument at +0x04 (lbu). Two s32
 * words per entry (0x04/0x13, 0x08/0x12, 0x10/0x15, 0x20/0x11, 0x40/0x10,
 * 0x80/0x14 in the data section). Absolute-addressed from ovl_11 code. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ char pad_5[0x3];
} Ovl11D80127328Entry;
extern Ovl11D80127328Entry gOvl11StatusFlagIcons[];

/* D_80127274 - 21-entry table of 6-byte records walked by
 * ovl_11_func_800FC998: s16 at +0x02 (lh) passed to ovl_11_func_800FCAF4
 * and u8 at +0x04 (lbu) passed to func_80015EE8. Absolute-addressed from
 * ovl_11 code. */
typedef struct {
    /* 0x00 */ char pad_0[0x2];
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ char pad_5;
} Ovl11D80127274Entry;
extern Ovl11D80127274Entry gOvl11StatusValueGrid[];
/* 800FD194/800FDE98 index original byte/s16 tables; CE28 is an interior
 * word anchor in the five 0x30-byte sprite sources, not separate storage. */
extern u8 gOvl11StatusRecordPageIcons[];
extern s16 gOvl11StatusLevelEntryBases[];
extern s32 gOvl11StatusRecordSpriteSource;

/* ovl_11: base of shared formatted-number scratch, passed to 8001A970
 * and 800FC544. The scalar label does not specify the whole buffer extent. */
extern s16 gOvl11NumberTextScratch;
/* 800FD5E8 writes a two-halfword text header at scratch +0/+2. */
struct Ovl11NumberTextHeader {
    s16 prefix;
    s16 marker;
};

/* 800FFA28 declares this s16 selection; 800FFB08 reads it as unsigned. */
extern s16 gOvl11MenuChoiceRow;
/* 800FFB08 adds D_80054BBC[0] bytes to this menu text base. */
extern u8 D_80053A22[];

/* 800FD5E8: Ovl11DrawPairedValuePanel draws bank-adjusted Earnings/expenses; +0x28 is This month/accounts. */
extern u8 gStatusEarningsTextAnchor[];

/* 800FD7B4 adds D_80054BBC[0] bytes to this empty-pair label base. */
extern u8 D_80053770[];
/* 800FDD74 adds D_80054BBC[0] bytes to this text base for func_80017B3C. */
extern u8 D_80053780[];
/* 800FDD74 walks five unsigned halfwords at D_8006C838+0x5208. */
struct Ovl11FDD74State {
    u8 pad_0[0x5208];
    u16 entries[5];
};
/* 800FD7B4 indexes nine signed halfword text offsets as a 3-by-3 table. */
extern s16 gOvl11PairedValueLabelOffsets[3][3];
/* 800FD7B4: lh at +0x44BA, four eight-byte word pairs at +0x49C4; caller 800FD5E8 selects 0..3. */
struct Ovl11PairedValues {
    char pad_0[0x44BA];
    s16 field_44BA;
    char pad_44BC[0x49C4 - 0x44BC];
    s32 pairs[4][2];
};

/* 800FD33C: Ovl11DrawStatusSevenRecordPage draws bank-adjusted Help as the page header. */
extern u8 gStatusHelpTextAnchor[];
/* 80100B50 adds D_80054BBC[0] bytes to this text base and its +0x12 successor for func_80017B3C. */
extern u8 D_8005396A[];
/* 800FEFEC adds D_80054BBC[0] bytes to this text base before func_80017B3C. */
extern u8 D_80053B22[];
/* 800FED0C: Ovl11UpdateOptionsMenu draws bank-adjusted How to operate / Configuration. */
extern u8 gOptionsConfigurationPromptTextAnchor[];
/* ovl_11:8012CEE8 — twelve packed settings bytes saved on menu open
 * and committed at work +E4CC on close. Keep packed-copy and halfword views. */
typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl11MenuSettings;
extern Ovl11MenuSettings gOvl11OptionsSettings_menu __asm__("gOvl11OptionsSettings");
/* Scalar reset and array-backed selector views share each original symbol.
 * The array lvalue retains the halfword through the remainder subtraction. */
extern s16 gOvl11OptionsRootRow;
extern s16 gOvl11OptionsRootRow_selection[1] __asm__("gOvl11OptionsRootRow");
extern void (*gOvl11OptionsPageHandlers[])(void);
extern s32 gOvl11OptionsMenuState;
extern s16 gOvl11OptionsListColumn;
extern s16 gOvl11OptionsListRow;
extern s16 gOvl11OptionsListRow_selection[1] __asm__("gOvl11OptionsListRow");
extern s16 gOvl11OptionsSettingChoice;
extern s16 gOvl11OptionsSettingRow;
/* First four halfwords: three-way mode, actuators, stereo and clock HUD.
 * The final two halfwords are copied unchanged; their roles are not assumed. */
extern s16 gOvl11OptionsSettings_options[6] __asm__("gOvl11OptionsSettings");
extern void (*gOvl11OptionsSoundSetters[2])(void);
extern void (*gOvl11OptionsClockSetters[2])(void);
/* 800FF324 adds D_80054BBC[0] bytes to these text bases and label offsets. */
extern u8 D_80053CB8[];
extern u8 D_80053D6A[];

/* 800FD3CC: Ovl11DrawStatusSevenRecordRow copies bank-adjusted Days left after a count. */
extern u16 gStatusDaysLeftTextAnchor[];
/* 800FD3CC: Ovl11DrawStatusSevenRecordRow draws bank-adjusted Free when the count is unavailable. */
extern u16 gStatusFreeTextAnchor[];

/* ovl_11_func_800FC358 reads each display counter with lh. */
extern s16 gOvl11StatusScore;
extern s16 gOvl11StatusFlagScore;
extern s16 gOvl11StatusOccupiedRecordCount;
/* ovl_11_func_800FC358 passes this buffer to func_80015EE8; 800FC544 uses 0x30-byte rows. */
/* 800FBE88 copies 0x2D74 sprite-header bytes here before indexing its offsets. */
extern u8 gOvl11StatusSpriteBank[];
/* 800FBE88 walks five lw offsets into D_8012A048 (original data: five words). */
extern s32 gOvl11StatusSpriteOffsets[5];
extern u8 gOvl11StatusSpriteSources[];

extern u8 gOvl11StatusLevelIconFrames[];

extern u8 gOvl11StatusIndicatorSpriteSource[];

/* D_8012CE58 - 0x30-byte scratch/display buffer in ovl_11 bss passed by
 * address to func_80015EE8 by ovl_11_func_800FC998. Absolute-addressed. */
extern u8 gOvl11StatusValueGridSpriteSource[];

extern u8 gOvl11StatusLevelSpriteSource[];

/* Shared 0x60-byte selection record; panels reuse its parallel arrays. */
extern u8 gOvl11PrimarySelectionRecord[];
/* 80115308 copies nine halfwords from the record at D6E8+4. */
typedef struct {
    s32 header;
    u16 values[9];
} Ovl11NameRecord;
extern Ovl11NameRecord gOvl11QuantityPurchaseNameRecord;
/* 80114604 copies nine halfwords from D638+4 and passes D63C to 800231AC. */
extern Ovl11NameRecord D_8012D638;
extern u16 D_8012D63C[];
/* 80114604 reads the u16 amount at resident-state +4502. */
typedef struct {
    u8 pad0[0x4502];
    u16 amount;
} Ovl11SaleGlobals;
/* 80114604 reads signed affection +16 and flags +34 in stride-B8 records. */
typedef struct {
    u8 pad0[0x16];
    s16 affection;
    u8 pad18[0x34 - 0x18];
    u32 flags;
    u8 pad38[0xB8 - 0x38];
} Ovl11SaleAnimal;
typedef struct {
    u8 pad0[0x81BC];
    Ovl11SaleAnimal animals[20];
} Ovl11SaleAnimalGlobals;
/* 80115308 passes this text to 800231AC. */
/* Interior nine-halfword name view at QuantityPurchaseNameRecord+4. */
extern u16 gOvl11QuantityPurchaseNameText[];
/* 80115308 reads the unsigned halfword at resident-state +0x4508. */
typedef struct {
    u8 pad0[0x4508];
    u16 amount;
} Ovl11FeedGlobals;

/* D_80051D34 - base of the shared D_80051xxx table region referenced by
 * ovl_11_func_80117F70: `&D_80051D34 + *D_80054BC0` (offset 0) and
 * `&D_80051D34 + 0x98 + *D_80054BC0`; the same base minus 0x5D8 forms a
 * third pointer. Address taken only (lui/addiu). Never defined in this TU. */
extern u8 D_80051D34[];

/* D_80128264 - ovl_11 s32 table indexed by the ovl_11_func_80117F70
 * sub-state (lw at base + index*4). Absolute-addressed. */
extern s32 gOvl11ResourceOptionTextOffsets[];

/* 80116980 reads/writes these two-byte menu indices with lh/lhu/sh. */
extern s16 gOvl11RecordListCursor;
extern s16 gOvl11RecordListScroll;
/* 8012823C contains the three-argument draw routines 8002462C..800246D4. */
extern void (*gOvl11RecordListIconDrawers[4])(s32, s16, s16);
/* 80116980 reads halfwords at +44E0 and +8270 through this common base. */
typedef struct {
    u8 pad0[0x44E0];
    u16 text[1];
    u8 pad44E2[0x8270 - 0x44E2];
    u16 status;
} Ovl11MenuGlobals;

/* ovl_11:8012D608 is shared text-format scratch. 80116F4C fills 0x18
 * halfwords; 8011CC08/8011CD2C/8011D474 format and append text here.
 * The independent name record at 8012D638 bounds this 0x30-byte span.
 * Retain the existing absolute-addressed scalar-first-word C view. */
extern s16 gOvl11TextFormatScratch;

/* D_8012D538 - 0xC-byte ovl_11 record (pointer at +4, s16 fields at
 * +8/+A); the +8 halfword is the separately named D_8012D540. Initialized
 * by ovl_11_func_80117F70 (pointers) and ovl_11_func_8011A9DC. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} Ovl11D8012D538;
extern Ovl11D8012D538 gOvl11QuantityPrompt;

/* D_8012D5A8 - 0x60-byte ovl_11 record of the D_8012D548 family (s16 fields
 * at +0x28/+0x46/+0x5C and s8 at +0x3C), reset through
 * ovl_11_func_8011D400 by ovl_11_func_80117F70. Absolute-addressed. */
extern u8 gOvl11SecondarySelectionRecord[];

/* D_8012D7D8 - 0x18-byte ovl_11 state block initialized by
 * ovl_11_func_80117F70 (pointer at +0, six s16 fields at +4..+E, s32 at
 * +0x10). Absolute-addressed. */
typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
} Ovl11D8012D7D8;
extern Ovl11D8012D7D8 gOvl11ResourcePanelDescriptor;

/* D_8012D7CC / D_8012D7D0 / D_8012D7D4 - ovl_11 s32 state words written by
 * ovl_11_func_80117F70. Absolute-addressed. */
extern s32 gOvl11ResourcePanelState;
extern s32 gOvl11ResourcePanelResumeState;
extern s32 gOvl11ResourceOptionIndex;
/* 80119008 stores the selected ID with sh and reads it with lh/lhu. */
/* 80119008 snapshots secondTag for the later A4 item request and slot match. */
extern s16 gOvl11A4RequestItemId;
/* 801180F0 passes this state word by address and clears the adjacent word. */
extern s32 gOvl11InteractionStep;
/* ovl_11:80118CB0 stores word 1 here during the interaction reset callback;
 * no wider meaning is established, so retain the neutral data identity. */
extern s32 D_80128210;
extern s32 gOvl11InteractionResumeStep;
/* ovl_11 801189C8 indexes these callback-bank anchors; retain scalar views. */
extern s32 gOvl11InteractionDrawCallbacks;
extern s32 gOvl11InteractionUpdateCallbacks;
extern s32 gOvl11InteractionResetCallback;
/* 80118C28 scans fourteen signed {item ID, interaction ID} pairs, stride 4. */
typedef struct {
    s16 itemId;
    s16 interactionId;
} Ovl11ItemInteractionEntry;
extern Ovl11ItemInteractionEntry gOvl11ItemInteractionMap[14];

/* ovl_11 80118A68 installs this ID; 8011C6CC compares it with 14. */
extern s32 gOvl11InteractionId;
/* ovl_11 80118A68 indexes no-argument callbacks; element 1 is also the reset word. */
extern s32 (*gOvl11InteractionBeginCallbacks[])(void);
/* Independently addressed signed halfword at QuantityPrompt +8. */
extern s16 gOvl11QuantityPromptCount;
/* 801180F0 reads signed halfword pairs with a four-byte table stride. */
extern s16 gOvl11ResourceOptionCosts[][2]; /* money, resource units */
/* 801180F0 stores the next message id and next state as signed words. */
extern s32 gOvl11ResourceResultMessage;
extern s32 gOvl11ResourceResultResumeState;
/* 801180F0 reads and updates the unsigned halfword at work-area +0x4500. */
typedef struct {
    u8 pad0[0x4500];
    u16 amount;
} Ovl11ResourceGlobals;

/* D_8012D7B8 - 0x14-byte ovl_11 record initialized by ovl_11_func_80117A40
 * (pointer at +0, s16 fields at +4/+6/+8/+A, s32 at +0x10) and passed as five
 * words to ovl_11_func_8011B840 by ovl_11_func_80117BB4. */
typedef struct {
    /* 0x00 */ u8 *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE; /* 8011977C initializes this message pair with sh. */
    /* 0x10 */ s32 unk10;
} Ovl11D8012D7B8;
extern Ovl11D8012D7B8 gOvl11CatalogPanelDescriptor;

/* D_8012D7A8 - 8-entry s16 table of ovl_11 record indices written by
 * ovl_11_func_80117A40 and read by ovl_11_func_80117F14/80117BB4. */
extern s16 gOvl11CatalogPanelMessages[];

/* D_8012D79C - s32 ovl_11 state/case word written by ovl_11_func_80117A40
 * and read by ovl_11_func_80117BB4. Address taken (lui/addiu). */
extern s32 gOvl11CatalogPanelState;
/* 80117BB4 stores a selected id with sw, then reloads it with lw/lhu. */
extern s32 gOvl11CatalogSelectedId;

/* D_8012D7A0 - s32 ovl_11 state word written by ovl_11_func_80117A40 and
 * read by ovl_11_func_80117BB4. */
extern s32 gOvl11CatalogPanelResumeState;

/* D_80051C36 - base of a table in the D_80051xxx region; the s32 read from
 * D_80054BC0[0] is added to its address (plus offsets 0x12/0x2C/0x38/-0x50)
 * to form the five pointers stored into the ovl_11 D_8012D548 record
 * (ovl_11_func_80117A40). Address taken only. */
extern u8 D_80051C36[];

/* D_80051AA8 - base of a table in the D_80051xxx region; the s32 read from
 * D_80054BC0[0] is added to its address to form two pointers stored into the
 * ovl_11 D_8012D548 record (ovl_11_func_8011B210). Address taken only. */
extern u8 D_80051AA8[];

/* 8011B2B0 adds D_80054BBC[1] bytes to these text-table addresses. */
extern u8 D_80051B76[];
extern u8 D_80051AE6[];
extern u8 D_80051B22[];
extern u8 D_80051B38[];
extern u8 D_80051B56[];
extern u8 D_80051B64[];
/* 8011B2B0 indexes this u16 table with func_80012A34(3), using lhu. */
extern u16 D_801282DC[];

/* D_80051E26 - base of a table in the D_80051xxx region; the s32 read from
 * D_80054BC0[0] is added to its address (plus offsets 0/0x1A/0x32/0x4C and
 * -0x60C) to form the four pointers stored into the ovl_11 D_8012D548 record
 * and the two pointers stored into D_8012D538 by ovl_11_func_80113FF0.
 * Address taken only (split lui/addiu). Never defined in the overlays. */
extern u8 D_80051E26[];

/* 80113CD0 adds D_80054BBC[1] byte offsets to these text-table addresses. */
extern u8 D_80051E92[];
extern u8 D_80051EB8[];
extern u8 D_80051ED0[];
extern u8 D_80051F1C[];

/* ovl_11_func_80114194 takes these table addresses and adds D_80054BBC[1]. */
extern u8 D_80051DD6[];
/* 80118744 adds D_80054BBC[1] bytes to this text base before 8011DD48. */
extern u8 D_80051DBE[];
extern u8 D_80051CB0[];

/* 80114398 adds D_80054BBC[1] bytes to these text-table addresses. */
extern u8 D_80051C7A[];
extern u8 D_80051CF4[];
extern u8 D_80051D0C[];

/* D_8005181A - base of the shared D_80051xxx table region (== D_80051E26 -
 * 0x60C); the s32 read from D_80054BBC[1] is added to its address (plus
 * offsets 0x276/0x138/0x156/0x170/0x196) to form the five pointers stored
 * into the ovl_11 D_8012D548 record by ovl_11_func_8011ADF4, and its address
 * is taken by ovl_11_func_8011DD48. Address taken only; typed s32 to match
 * the pre-existing local view in ovl_11_func_8011DD48. */
extern s32 gQuantityPromptDefaultTextAnchor;

/* 8011AFB4 adds D_80054BBC[1] bytes to this address and its +0x1E/+0x3E/+0x60 successors. */
extern u8 D_80051A10[];

/* D_80051A90 - base of the shared D_80051xxx table region (== D_8005181A +
 * 0x276); the s32 read from D_80054BBC[1] is added to its address (plus
 * offsets 0/-0x1C2/-0x1A8/-0x192/-0x17A) to form the five pointers stored
 * into the ovl_11 D_8012D548 record by ovl_11_func_8011AC30, and plus
 * offsets 0/-0x252/-0x238/-0x21E by ovl_11_func_8011AA64. Address taken
 * only; typed s32 to match the sibling view of D_8005181A. */
extern s32 D_80051A90;

/* D_8012D52C - container-wide s32 busy/finished state flag of ovl_11, reset
 * by the 0x80114184 reset-stub family (see notes/file-groupings.md). */
extern s32 gOvl11InteractionDrawMode;
/* ovl_11 801173B8 dispatches and stores this signed word state. */
extern s32 gOvl11CellStatusPhase;
/* ovl_11 801173B8 loads this signed id, comparing it with -1. */
extern s32 gOvl11CellStatusSelection;
/* 801176BC indexes this ten-halfword item-ID table; only indices 0..8 are accepted. */
extern u16 gOvl11CellStatusItemIds[10];
/* ovl_11 801173B8 tests this signed halfword against 6. */
extern s16 gCalendarWeekday;
/* ovl_11 801173B8 tests this signed word against 10. */
extern s32 gSpendableBalance;

extern s16 gOvl11SelectionPanelState;
/* 8011E5B0 passes this eight-byte rectangle to StoreImage and LoadImage. */
extern s16 gOvl11SelectionPanelSaveRect[4];

/* ovl_27 800B944C tests bit zero of this absolute-addressed word. */
extern s32 D_8006C840;
/* Ovl27DrawTiledBackdrop advances these signed-halfword phases modulo 960.
 * Preserve the second phase's independent table-relative address view. */
extern s16 gOvl27BackdropFirstPhase;
/* ovl_27:800C4A4E, immediately before the four title label offsets. */
#define gOvl27BackdropSecondPhase (((s16 *)gOvl27TitleLabelOffsets)[-1])
/* Fixed/tiled title draws use the eighth stride-0x30 pool source (800C4BD0). */
extern u8 gOvl27BackdropSpriteSource[];

extern s32 gOvl17RecordSceneSubphases[5];

/* 800B7E78 dispatches and advances this unsigned state byte with lbu/sb. */
extern u8 gOvl17SceneLoadStep;

/* BA630 expands the three six-byte groups to halfword roll thresholds. */
extern u8 gOvl17RecordRollThresholds[];

/* ovl_17_func_800B8470 increments this byte and compares its signed value to 30. */
extern s8 gOvl17BackdropScrollTick;

/* 800B80B0 loads this signed halfword and uses -1 as its inactive value. */
extern s16 D_800BDB10;
/* 800B80B0 reads mode at +0xE614 and updates words at +0xC/+0x448C. */
typedef struct {
    u8 pad_0[0xC];
    s32 flags;
    u8 pad_10[0x448C - 0x10];
    s32 state;
    u8 pad_4490[0xE614 - 0x4490];
    s32 mode;
} Ovl17CompletionView;

/* ovl_17: six 0x50 record views and adjacent controls share this raw base. */
extern u8 gOvl17RecordWorkBase[];

/* 800B9628 reads s32 values at +0x38 with stride 0x50 and s16 selections/count at +0x2BC/+0x2D4. */
typedef struct {
    s32 field0;
    u8 pad[0x4C];
} Ovl17Rec;

typedef struct {
    u8 pad_000[0x38];
    Ovl17Rec recs[6];
    u8 pad_218[0x2BC - 0x218];
    s16 sel[6];
    u8 pad_2C8[0x2D4 - 0x2C8];
    s16 count;
} Ovl17SelView;

/* BA54C's control view. 8898 toggles +0, counts ticks and advances the
 * 12-fraction-bit scroll position; BA808 divides it by 4096 for screen x.
 * 9C48 evaluates the base/bias/gain curve. The
 * offset-named constants retain uncertain broader meaning (946C/9594). */
typedef struct {
    s32 toggle;
    s8 phase;
    u8 pad_5;
    s16 tickPeriod;
    s16 valueBase;
    s16 valueBias;
    s16 valueGain;
    s16 field_E;
    s16 field_10;
    s16 field_12;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    u8 pad_1A[2];
    s32 tickCounter;
    s16 scrollSpeed;
    s16 targetScrollSpeed;
    s32 scrollPosition;
    u8 pad_28[0x2BC - 0x28];
    s16 selectionIndices[6];
    s16 soundVoice;
    u8 pad_2CA[6];
    s16 soundTriggerPercent;
    s16 primaryValueThreshold;
    s16 selectionCount;
} Ovl17RecordWorkInitView;
#define OVL17_RECORD_WORK (*(Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)

/* 800B8D88 reads a word at +0x5228 and a signed mode halfword at +0xE514. */
typedef struct {
    u8 pad_0[0x5228];
    s32 value;
    u8 pad_522C[0xE514 - 0x522C];
    s16 mode;
} Ovl17SummaryView;
/* 800B8F28 reads +0x5228/+0xE514, writes +0xE614/+0xE618 and increments +0x448C. */
typedef struct {
    u8 pad_0[0x448C];
    s32 state;
    u8 pad_4490[0x5228 - 0x4490];
    s32 value;
    u8 pad_522C[0xE514 - 0x522C];
    s16 mode;
    u8 pad_E516[0xE614 - 0xE516];
    s32 enabled;
    s16 total;
} Ovl17SummaryResultView;
/* 800B8F28 shares D_8006C838+0x8000 for mode and the enabled word. */
typedef struct {
    u8 pad_0[0x6514];
    s16 mode;
    u8 pad_6516[0x6614 - 0x6516];
    s32 enabled;
} Ovl17SummaryUpperView;
/* 800B8F28 compares this signed halfword with 2. */
extern s16 D_800BDB04;

/* 800B8D88 selects one of three four-byte text entries in data/3634.data.s. */
extern u16 gOvl17RecordModeHeadingText[6];
/* 800B8D88 draws this halfword text and the preceding text at byte offset -0x2E. */
extern u16 gOvl17RecordPanelLabelText[];

/* ovl_17 data/3634.data.s reserves ten halfwords; 800B9940 formats text here. */
extern u16 gOvl17NumberTextBuffer[10];
/* One horse-race entrant at D_8006C838 + 0xE516 (D_8007AD4E).
 * Ovl17DrawHorseRaceBetsPanel labels +0xA "Odds" and +0xC "Tickets" and
 * multiplies them for winnings. The ID selects a horse name at 800556A8;
 * mode 3 uses the player's horse name at 80075AD8 for entrant [2][2].
 * Parameter/attribute meanings are unresolved; these are not screen x/y. */
typedef struct {
    s16 horseId;
    s16 parameter0;
    s16 parameter1;
    u16 attr[2];
    s16 odds;
    s16 ticketCount;
} HorseRaceEntry;
/* 8002206C clears 0x108 bytes: mode@0, then three races of six 14-byte entries. */
typedef struct {
    s16 mode;
    HorseRaceEntry entries[3][6];
    u8 unkFE[0x108 - 0xFE];
} HorseRaceState;
extern s16 gHorseRaceState[0x84];
/* 8002206C's separate symbolic bases for the stride-14 entrant-field walks. */
extern s16 gHorseRaceFirstHorseId;
extern s16 gHorseRaceFirstParameter0;
extern s16 gHorseRaceFirstParameter1;
extern u16 gHorseRaceFirstAttribute0;
extern u16 gHorseRaceFirstAttribute1;
extern s16 gHorseRaceFirstOdds;
/* ovl_17 view of D_8006C838: race mode at +0xE514 (3 uses race 2 with the
 * player's horse), the entrant table, and the player's parameters at
 * +0x92B2/+0x9348 copied by Ovl17ResetHorseRaceEntrants. The cast base goes
 * through a register, so far fields split as (base + 0x8000) + low offset. */
typedef struct {
    u8 pad_0[0x92B2];
    u16 unk92B2;
    u8 pad_92B4[0x9348 - 0x92B4];
    u16 unk9348;
    u8 pad_934A[0xE514 - 0x934A];
    s16 mode;
    HorseRaceEntry entries[3][6];
} Ovl17ModeView;
/* 800B9940 reads mode at +0x6514 and value halfwords from +0x6520 of D_80074838. */
typedef struct {
    u8 pad_0[0x6514];
    s16 mode;
    u8 pad_6516[0xA];
    u16 values[0xD70];
} Ovl17WorkView;
/* 800B9940: six 0x50-byte rows have a u16 name index at +0x40; selector is +0x2BC.
 * 800B8294 clears flags at +0x28, stores the data pointer at +0x3C,
 * and sets bit 0 on row two. 800BA704 resets +0x2A..+0x38 and copies a
 * horse-race entry's parameters and attributes into +0x34/+0x36 and +0x42/+0x44. */
typedef struct {
    u8 pad_0[0x28];
    u8 flags;
    u8 pad_29;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s32 unk30;
    s16 parameter0;
    s16 parameter1;
    s32 unk38;
    u8 *data;
    u16 nameOffset;
    u16 attr[2];
    u8 pad_46[0xA];
} Ovl17NameRow;
typedef struct {
    Ovl17NameRow rows[6];
    u8 pad_1E0[0xDC];
    s16 selected;
} Ovl17RowsView;
/* ovl_17_func_800B9940 draws text at this base and byte offsets +0xA/+0x1A. */
extern s16 gOvl17RecordColumnLabelText[];
/* ovl_17_func_800B9940 reads six indexed halfwords with lhu. */
extern u16 gOvl17RecordRowLabelText[6];
/* ovl_17_func_800B9940 reads signed/unsigned halfwords at a computed byte offset. */
extern s16 D_8007AD5A[];

/* gOvl17RecordDrawTick - ovl_17 unsigned counter read by Ovl17DrawRecordSpriteQuads
 * and compared against ((x + 1) / 10) * 10 - 1; the target divides with
 * multu and srl 3, so the value is unsigned. Plain extern (never defined in
 * this TU), absolute-addressed (-G0). */
extern u32 gOvl17RecordDrawTick;

/* BA698 initializes six 6-byte triples; BA704/8294 install their addresses
 * in display rows. The original label spans 0xF0, only its first 0x24 is used. */
extern u8 gOvl17RowSlotTargets[];
/* ovl_17 data/3634.data.s holds four-byte text offsets; 800B8294 stores their low halfwords. */
extern s32 gOvl17RecordRowNameOffsets[6];
/* 800B8294 initializes a sprite here and writes a signed halfword at +0x1E4. */
extern u8 D_800BD930[];

extern u8 gOvl17DisplaySpriteSource[];
/* Ovl17DrawRecordBackdrop: 30 eight-byte props then a spriteKind=255 sentinel. */
typedef struct {
    s16 spriteKind;
    s16 pad_2;
    s32 xQ12;
} Ovl17BackdropEntry;
extern Ovl17BackdropEntry gOvl17BackdropProps[];
/* ovl_17:800BB5D4 holds 14 halfword animation IDs; the selector uses their low byte. */
extern u16 gOvl17BackdropPropAnimations[];
/* ovl_17_func_800BA2C8 loads five 32-bit offsets from this original table. */
extern s32 gOvl17SpriteHeaderOffsets[5];
/* ovl_17_func_800BA2C8 copies 0xC28 bytes here before resolving sprite headers. */
extern u8 gOvl17RecordSpriteBank[0xC28];
/* ovl_17_func_800BA2C8 stores blob-relative pointers at these word globals. */
extern u8 *gOvl17SpriteHeaderVariantA;
extern u8 *gOvl17SpriteHeaderVariantB;
/* ovl_17_func_800BA2C8 passes this object to the sprite initializer. */
struct SpriteSourceData;
extern struct SpriteSourceData gOvl17SceneSpriteSource;

/* ovl_17 indicator sprite embedded at work+0x28C. The rectangle drawer
 * reads animation/frame through the work base; the quad drawer updates it.
 * Plain external storage, absolute-addressed (-G0). */
extern u8 gOvl17ThresholdIndicatorSprite[];

/* ovl_17 animation drivers visit six 0x50-byte records at BD848+28;
 * the named data label spans only the first A0 bytes of that shared view. */
extern u8 gOvl17AnimatedRecordBase[];
/* ovl_17_func_800B861C passes this record to 800BAC24 and stores a byte at -0xC4. */
extern u8 D_800BD910[];

/* ovl_23: +0x10 sprite field of six 0x44 records; the companion is -0x1D4. */
extern u8 gOvl23ActorSpriteSourceBase[];
/* ovl_23 data: six anchor Y halfwords and six inclusive (minimum, maximum) pairs. */
extern s16 gOvl23RecordAnchorY[6];
extern s16 gOvl23RecordYBounds[12];

extern s32 gOvl27TitlePhaseCallbacks[];

/* ovl_27:800C4A50: four word-sized text offsets selected by
 * Ovl27DrawTextBankLabel's unchecked signed-halfword index. Keep the extern
 * array view and split absolute address formation; no local allocation. */
extern s32 gOvl27TitleLabelOffsets[];

/* ovl_27:0x800C4A60: four 8-byte page sources, ids 1/3/0/2.
 * Ovl27CopySelectedPage copies 0x1000 bytes to the bound work buffer +6,
 * then records the source id. Overlay data, absolute-addressed (-G0). */
struct Ovl27PageSource {
    s32 id;
    void *data;
};
extern struct Ovl27PageSource gOvl27PageSources[];


/* D_800711C4 - main-EXE progress-counter record, absolute-addressed from the
 * ovl_11 overlays (-G0). ovl_11_func_800F3BCC increments a u16 entry selected
 * by arg and folds the return into the s32 at 0x34; ovl_11_func_800F3EF0
 * folds the pending half of the u16 array (entries 1..25 at 0xFE..0x12E) into
 * the counter half (entries 0..24 at 0x00..0x30) with a 999 clamp, clears the
 * pending entries, and folds the s32 at 0x130 into the s32 at 0x34. */
extern u16 gItemValueCounterWork[0x9A];
/* F3E44 stages at work+FC: count@0, 25 halfword categories@2, value@34;
 * F3EF0 merges and clears the same record. This is a view, not new storage. */
typedef struct {
    u16 acceptedCount;
    u16 categoryCounts[25];
    s32 totalValue;
} Ovl11PendingItemValueCounters;

/* 80108864 selects the two offset pairs forwarded to sprite-image upload. */
typedef struct {
    s16 x;
    s16 y;
    s16 type2X;
    s16 type2Y;
} Ovl11ChoiceImageOffsets;
extern Ovl11ChoiceImageOffsets gOvl11ChoiceImageOffsets;
/* 80108A24 scans nine cells; 80108B8C promotes a selected cell from 1 to 2. */
extern u8 gChoiceCellStates[9];
/* 80111A60/B74 pass this same record and two 16-pixel source rows to the
 * EXE color-row service. Only the first 0x14 bytes of the 0x20 label are used. */
extern struct ColorRowBlend gOvl11ColorRowBlend;
extern unsigned long gOvl11ColorRowSources[16];
extern s32 gOvl11ColorRowProgress;
/* 80111D28 indexes four seasonal sunset hours: 18, 19, 18, 17. */
extern s16 gOvl11SunsetHours[];
/* 80111D48 reads low halfwords at +0/+4/+8; 80111D94 interpolates three words. */
extern s32 gOvl11SceneTripletStart[3];
/* 80111D94 supplies this adjacent three-word endpoint to InterpolateThreeWords. */
extern s32 gOvl11SceneTripletEnd[3];
/* 80111D48 resets and 80111D94 advances the 0..1200 interpolation position. */
extern s32 gOvl11SceneTripletProgress;
/* exe:8001FA98 post-increments this sole GP-owned signed counter. */
extern s32 gSequenceNumber;

/* 80109D08 clears/passes this record pointer, then forwards its +0x38 vector. */
extern s32 *gOvl11ActionFoundRecord;
/* 801096FC writes a word through this output passed by 80109D08. */
extern s32 gOvl11ActionFoundRecordCategory;

/* D_8012D080 - ovl_11 absolute-addressed pointer cell (outside $gp range).
 * ovl_11_func_801098B0 passes its address as the out pointer of the
 * D_80075BC4 record scan and clears it to 0 when the scan fails;
 * ovl_11_func_80109E04 reads it as the current cluster record. */
extern s32 *gOvl11ActionProbedRecord;

/* D_8012D084 - ovl_11 absolute-addressed s32 state cell. Cleared to 0 by
 * ovl_11_func_801090A4 at the end of every handler step; written by
 * ovl_11_func_8010A850 (three sites). Accessed with lui + %lo from ovl_11
 * code, i.e. outside the $gp range. */
extern s32 gOvl11ActionMotionVariant;

/* ovl_11:800BA894, nineteen raw callback addresses indexed by object+0x26.
 * 801090A4 invokes the selected script each frame; 80109188 only tests its
 * presence before resetting accepted state/script fields. */
extern s32 gOvl11StageMode1StepCallbacks[];
/* ovl_11:800BA848, nineteen transition checks called by 80109188 before
 * accepting a requested state. Return -1 rejects the script-field reset. */
struct Ovl11D04D4Obj;
extern s32 (*gOvl11StageMode1TransitionCallbacks[19])(struct Ovl11D04D4Obj *);
/* 80108BE8/80109550 index four animation/frame pairs per mode-1 state. */
typedef struct {
    u16 animation;
    u16 frame;
} Ovl11StageMode1AnimationPair;
extern Ovl11StageMode1AnimationPair gOvl11StageMode1AnimationPairs[][4];

/* D_80070D30 and D_800719F8 - the ovl_11 file-status flag word and its
 * status sibling (reached as &D_800719F8 - 0xCC8). Both are >8-byte-view
 * symbols accessed with absolute addressing (lui + %lo) from ovl_11 code. */
extern s32 gProgressStateFlags;
extern s32 gProgressDerivedFlags;

/* 800F291C reads these signed/unsigned state fields through one buffer base. */
typedef struct {
    u8 pad0[0xC];
    s32 field_C;
    u8 pad10[0x44B8 - 0x10];
    union {
        s32 word;
        struct { s16 low; s16 high; } halves;
    } field_44B8;
    s16 field_44BC;
    u8 pad44BE[0x44D4 - 0x44BE];
    u16 field_44D4;
    u8 pad44D6[0x498C - 0x44D6];
    u16 field_498C[(0x51C0 - 0x498C) / 2];
    s32 field_51C0;
    u8 pad51C4[0x5246 - 0x51C4];
    s16 field_5246;
    s16 field_5248;
    u8 pad524A[0x5492 - 0x524A];
    s16 field_5492;
    u8 pad5494[0x929C - 0x5494];
    u16 field_929C;
    u8 pad929E[0x9F68 - 0x929E];
    u16 field_9F68;
    u8 pad9F6A[0xA13A - 0x9F6A];
    s16 field_A13A;
    u8 padA13C[0xA30E - 0xA13C];
    s16 field_A30E;
    u8 padA310[0xA6B8 - 0xA310];
    u16 field_A6B8;
    u8 padA6BA[0xA88A - 0xA6BA];
    s16 field_A88A;
    u8 padA88C[0xAA60 - 0xA88C];
    u16 field_AA60;
    u8 padAA62[0xAE06 - 0xAA62];
    s16 field_AE06;
    u8 padAE08[0xAFDA - 0xAE08];
    s16 field_AFDA;
    u8 padAFDC[0xB384 - 0xAFDC];
    u16 field_B384;
    u8 padB386[0xB72A - 0xB386];
    s16 field_B72A;
    u8 padB72C[0xB8FE - 0xB72C];
    s16 field_B8FE;
    u8 padB900[0xBCA6 - 0xB900];
    s16 field_BCA6;
    u8 padBCA8[0xBE7C - 0xBCA8];
    u16 field_BE7C;
    u8 padBE7E[0xC04E - 0xBE7E];
    s16 field_C04E;
    u8 padC050[0xC3F6 - 0xC050];
    s16 field_C3F6;
    u8 padC3F8[0xC79E - 0xC3F8];
    s16 field_C79E;
} Ovl11EventStateView;

/* 800F291C loads a signed halfword at flag anchor +690A. */
typedef struct {
    s32 flags;
    u8 pad4[0x690A - 4];
    s16 field_690A;
} Ovl11EventFlagView;
/* 800F291C reads/sets the leading word and retains this address across calls. */
extern Ovl11EventFlagView gOvl11EventRequestOnceFlags;
/* F2508 resets the same one-shot word through shared work +44FC. */
typedef struct {
    char pad[0x44FC];
    s32 requestOnceFlags;
} Ovl11EventQueueWorkView;

/* D_80127F80 - ovl_11 table of four halfwords scanned by
 * ovl_11_func_80112284 through an advancing s16 pointer.
 * Accessed with absolute addressing from ovl_11 code. */
extern s16 gOvl11StatusCounterGroupAIds;

/* D_80127F88 - second ovl_11 table of four halfwords, scanned by
 * ovl_11_func_80112318 through an advancing s16 pointer. Sits just past
 * the D_80127F80 table. Accessed with absolute addressing from ovl_11
 * code. */
extern s16 gOvl11StatusCounterGroupBIds;

/* D_80127F90 - third ovl_11 table of four halfwords, scanned by
 * ovl_11_func_801123AC through an advancing s16 pointer. Sits just past
 * the D_80127F88 table. Accessed with absolute addressing from ovl_11
 * code. */
extern s16 gOvl11StatusCounterGroupCIds;

/* D_80127FE8 - base of the ovl_11 16-byte-stride halfword table read by
 * ovl_11_func_80113A20. The table index is the halfword at D_8012D110 and
 * the selected 16-byte record is walked as eight s16 values. Accessed with
 * absolute addressing from ovl_11 code. */
extern s16 gOvl11AreaGridRewardWeights[];

/* D_80128088 - second base of the same 16-byte-stride halfword table,
 * selected by ovl_11_func_80113A20 when its argument equals 0x37. Sits
 * 0xA0 bytes past D_80127FE8. Accessed with absolute addressing. */
extern s16 gOvl11Area37RewardWeights[];

/* D_80128128 - ovl_11 eight-halfword table walked by
 * ovl_11_func_80113A20 alongside the selected 16-byte record. Accessed
 * with absolute addressing from ovl_11 code. */
extern s16 gOvl11AreaGridRewardItems[];

/* D_80128138 - second ovl_11 eight-halfword table (0x10 bytes past
 * D_80128128) selected by ovl_11_func_80113A20 when its argument equals
 * 0x37. Accessed with absolute addressing from ovl_11 code. */
extern s16 gOvl11Area37RewardItems[];

/* D_80074838 - large ovl_11 work-area base. Referenced with absolute
 * addressing from ovl_11 code; the horse-race region this overlay clears
 * sits at +0x6520 (three races of six 14-byte entrant records, also reached
 * as D_8007AD4E). */
extern u8 D_80074838[0x8000];

/* 800F05C0 masks word flags at work+0x1F0 in 0xB8-byte records. */
typedef struct {
    s32 flags;
    u8 pad4[0xB8 - 4];
} Ovl11ClearFlagRecord;

typedef struct {
    u8 pad0[0x1F0];
    Ovl11ClearFlagRecord records[1];
} Ovl11ClearFlagWorkView;

/* 800F05C0 reads the second signed halfword of four work+0x19C8 pairs. */
typedef struct {
    u8 pad0[0x19C8];
    s16 pairs[4][2];
} Ovl11ClearPairWorkView;

/* 800D09E0 reads signed slot/kind halfwords at work+0x19C8/+0x19CA. */
struct Ovl11D09E0SelectionView {
    u8 pad0[0x19C8];
    s16 slot;
    s16 kind;
};

/* ovl_11_func_800C1D68: five lhu thresholds at D_8006C838+0x9F68..0xBE7C. */
struct Ovl11RecordThresholds {
    u8 pad0[0x9F68];
    u16 field_9F68;
    u8 pad9F6A[0xA6B8 - 0x9F6A];
    u16 field_A6B8;
    u8 padA6BA[0xAA60 - 0xA6BA];
    u16 field_AA60;
    u8 padAA62[0xB384 - 0xAA62];
    u16 field_B384;
    u8 padB386[0xBE7C - 0xB386];
    u16 field_BE7C;
};

/* 80112A84 selects one of five label pointers consumed by 800FB908's text row. */
extern s32 *gOvl11CounterRecordLabels[5];

/* D_8007A3F0 - address inside the D_80074838 work area (base + 0x5BB8),
 * referenced absolutely (lui + %lo) by ovl_11_func_801129EC, which passes
 * its address to ovl_11_func_80112B60. Only the address is taken, so the
 * element type is not witnessed; u8 keeps the base/offset relation. */
extern u8 D_8007A3F0;

/* D806C indexes these {columnCount,rowCount} records; original words are
 * {45,25},{7,7}. Absolute-addressed, owned by ovl_11's existing data section. */
typedef struct {
    s16 columnCount;
    s16 rowCount;
} Ovl11CellQueryBounds;
extern Ovl11CellQueryBounds gOvl11CellQueryLayerBounds[2];

/* D_801287CC - s16 threshold table (13 halfwords) walked with a stride-2
 * pointer by ovl_11_func_80121204. Absolute-addressed (lui + %lo) from
 * ovl_11 code. */
extern s16 gOvl11DateMessageThresholds[13];

/* 801164F8 reads a signed numerator/denominator/level at work+51DA/DC/DE. */
typedef struct {
    char pad0[0x51DA];
    s16 numerator;
    s16 denominator;
    s16 level;
} Ovl11OfferMessageState;
/* 801164F8 alone indexes five ratio rows by four level columns; message IDs. */
extern u16 gOvl11OfferConditionMessages[5][4];

/* The quantity editor snapshots/commits six final-halfword fields at work-area
 * +0xE522, with selector@E514, available value@5224 and balance@49C8. */
typedef struct {
    char pad_000[0x49C8];
    s32 field_49C8;
    char pad_49CC[0x5224 - 0x49CC];
    s32 field_5224;
    char pad_5228[0xE514 - 0x5228];
    s16 unkE514;
    s16 pad_516[6];
    s16 rows[1][6][7];
} Ovl11QuantityWorkView;

/* ovl_11:8012CF10: six pending row quantities copied by Begin/CommitQuantityEditor.
 * Reset/comparison use signed halfwords; commit copies their raw bits as unsigned.
 * Retain the existing seven-halfword base view and absolute addressing. */
extern s16 gOvl11QuantityEditorQuantities[7];
/* ovl_11:8012CF20: pending money balance, edited at 50 per item and committed to work+5224. */
extern s32 gOvl11QuantityEditorBalance;
/* ovl_11:80127428: inactive/select/edit/commit/cancel/message-wait states 0..5. */
extern s32 gOvl11QuantityEditorState;
/* ovl_11:8012742C: selection wraps over six editable quantity rows and the finish row. */
extern s32 gOvl11QuantityEditorSelectedRow;

/* ovl_11:8012CF1C / 8012CF24: selection input snapshots the row quantity and balance;
 * ServiceQuantityEditor restores them on row-edit cancellation. Reset clears both.
 * Preserve the existing signed-halfword/word views and absolute addressing. */
extern s16 gOvl11QuantityEditorEditStartQuantity;
extern s32 gOvl11QuantityEditorEditStartBalance;

/* ovl_11:8012CF30: balance/row rendering and the race-number text source reuse
 * this 12-halfword glyph scratch. Decimal writers append FFFF before drawing. */
extern s16 gOvl11QuantityEditorTextBuffer[12];

/* 80104148 writes a glyph at +0 and a 0xFFFF terminator at +2 in this buffer. */
typedef struct {
    s16 glyph;
    u16 terminator;
    s16 rest[10];
} Ovl11GlyphTextBuffer;
extern Ovl11GlyphTextBuffer D_8012CF30_glyph __asm__("gOvl11QuantityEditorTextBuffer");

/* 80051BF4: Ovl11DrawQuantityEditorRow draws bank-adjusted Odds and Tickets (+0xA) labels. */
extern u8 gRaceBettingColumnLabelsTextAnchor[];
/* 80051C0E: DrawQuantityEditorRows displays Race [text slot 0] bets and OK (+0x18), bank-adjusted. */
extern u8 gRaceBettingHeaderTextAnchor[];
extern u8 D_800556A8[];
/* 80104148 loads a signed halfword at index*4 from this text-offset table. */
extern s16 D_80127430[][2];
/* 80104148 reads selector@E514 and 14-byte entries at E516 (name@0, value@A). */
typedef struct {
    u8 pad_0[0xE514];
    s16 selector;
    s16 entries[3][6][7];
} Ovl11HorseRaceView;
/* 80104148 shares a cached work-area +0x8000 base across bank selection:
 * selector@+0x6514 and six 14-byte records per bank at +0x6516. */
typedef struct {
    u8 pad_0[0x6514];
    s16 selector;
    s16 entries[3][6][7];
} Ovl11HorseRaceUpperView;

/* The sprite-rotation unit resets and walks ten 0x18-byte state records. */
struct Ovl11RotationState;
extern struct Ovl11RotationState gOvl11SpriteRotations[10];

/* ovl_11_func_8010C6A0 scratch state. D_8007A638 is seven 0xF8-byte
 * records (0x6C8 bytes) zeroed by that function and populated one record at
 * a time; D_80127CD0 and D_80127CB4 are the two 7-entry parallel arrays it
 * walks alongside them. All are absolute-addressed (outside $gp). */
extern u8 gObjectRecordPool[0x6C8];
extern s32 gOvl11ObjectInitialRecordOffsets[7];
extern s32 gOvl11ObjectInitialAreaRoomPairs[7];

/* 800B806C dispatches 19 no-argument state handlers listed in original ovl_21 data at 800BCB6C. */
/* 800B7E3C indexes five original code-pointer words and forwards the table
 * address as an ignored legacy argument; retain its word-view dispatch. */
extern s32 gOvl21ScenePhaseTable[5];
extern void (*gOvl21SceneStateHandlers[19])(void);

/* ovl_21_func_800B7E98 selects and advances this state with lw/sw. */
extern u32 gOvl21SceneLoadStep;

/* 800B836C indexes four u16 triples at stride 6, loading +2/+4 with lhu. */
extern u16 D_800BCBBC[4][3];
/* 800B836C indexes u16 triples by selector*12 + variant*6; original data has 24 units. */
extern u16 D_800BCBD4[4][2][3];
/* 800B836C loads lhu at selector*4 + variant*2; original data has eight units. */
extern u16 D_800BCC04[4][2];

/* 800B9F3C loads u16 values at (entity * 3 + target % 3) * 2. */
extern u16 D_800BCC24[18];

/* 800B96B0 indexes this halfword table with stride 2 and loads with lhu. */
extern u16 D_800BCC14[];
/* 800B96B0 stores a halfword at D_8006C838+E62A and words at +448C/+E62C. */
typedef struct {
    u8 pad_0[0x448C];
    s32 counter;
    u8 pad_4490[0xE62A - 0x4490];
    u16 selection;
    s32 flag;
} Ovl21ResultView;

/* ovl_21:800BCC48, six world-coordinate X/Z halfword pairs shared by
 * Ovl21InitSceneRecords and Ovl21DrawPrimarySceneSprite. Both load with lhu
 * before storing/narrowing into signed position vectors; keep the bit widths.
 * Absolute extern-only accesses in the -G0 overlay, not newly owned storage. */
typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 z;
} Ovl21SceneRecordOffset;
extern Ovl21SceneRecordOffset gOvl21SceneOffsetPairs[];

/* D_800BCCC4 - ovl_21 table of s16 selector values (0x21/0x1F/0x1E/0x20 at
 * +0), indexed by a signed halfword read from the D_800C0448 state block.
 * ovl_21_func_800B871C loads it with lh, so it is signed. Absolute-addressed
 * (extern-only, lui + %lo) in the -G0 overlay build. */
extern s16 gOvl21FirstGroupMessages[];

/* D_800BCCCC - ovl_21 table of s16 selector values (0x21/0x1F/0x1E/0x20 at
 * +0), indexed by a signed halfword read from the D_800C0448 state block by
 * ovl_21_func_800B88A0, which loads it with lh, so it is signed. Identical in
 * content to D_800BCCC4 but a distinct symbol. Absolute-addressed
 * (extern-only, lui + %lo) in the -G0 overlay build. Not classified in
 * globals.h. */
extern s16 gOvl21SecondGroupMessages[];

/* D_800BCCD4 - ovl_21 table of s16 selector values (0x25/0x23/0x22/0x24 at
 * +0), indexed by a signed halfword read from the D_800C0448 state block.
 * ovl_21_func_800B90C4 loads it with lh, so it is signed. Absolute-addressed
 * (extern-only, lui + %lo) in the -G0 overlay build. */
extern s16 gOvl21ResultMessages[];

/* D_800BCC60 - ovl_21 table of 25 4-byte voice-bit masks (0x1, 0x2, 0x4,
 * ... doubling to 0x800000, then a 0 terminator). ovl_21_func_800B93F0
 * indexes it by a signed halfword read from the D_800C0448 state block and
 * passes the loaded word to SpuGetKeyStatus as its voice_bit argument, so the
 * element type is u32. Absolute-addressed (extern-only, lui + %lo) in the -G0
 * overlay build. Not classified in globals.h. */
extern u32 gOvl21SceneVoiceMasks[];

/* 800B8B3C stores this data object's address at state +0xC; no typed access. */
extern u8 D_800BCA78[];

/* D_800BCAD8 - ovl_21 data block at 0x800BCAD8 (0x80 bytes, ending where
 * D_800BCB58 begins). ovl_21_func_800B990C takes its address and stores it
 * into the D_800C0448 state block; no field is read through it, so an opaque
 * byte object suffices. Absolute-addressed (extern-only, lui + %lo) in the
 * -G0 overlay build. Not classified in globals.h. */
extern u8 D_800BCAD8[];

/* D_800C0448 - leading state-pair view of the larger ovl_21 scene block.
 * 800B9538 writes the first halfword; 800B95EC clears it and increments the
 * second with lhu. Other views below/game_types.h cover the six records and
 * trailing selectors/counts. Absolute extern addressing; no new ownership. */
extern u16 gOvl21SceneControl[2];
/* 800BA698 walks 0x4C-byte slices from each 0x108-byte record: active s32
 * at +0x34 and unsigned X/Z at +0x40/+0x44, also seeded/stepped by 9F3C/A300.
 * Biased traversal view (0x34 before the active word), not a full record. */
typedef struct {
    u8 pad_00[0x34];
    s32 active;
    u8 pad_38[0x40 - 0x38];
    u16 x;
    u16 pad_42;
    u16 z;
} Ovl21ProjectileSearchView;
/* 800B8134 loads the signed handles at D_800C0448 +0x984/+0x986 with lh. */
typedef struct {
    u8 pad_0[0x984];
    s16 handles[2];
} Ovl21ExitStateView;
/* 800B90C4 indexes the +0x640 selectors by slot; the array member keeps the
 * base-first address form. It also clears the halfword at +0x988. */
typedef struct {
    u8 pad_0[0x640];
    s16 selectors[2];
    u8 pad_644[0x988 - 0x644];
    s16 field_988;
} Ovl21SelectorView;
/* 800B836C's six 0x108-byte records and table selectors at +0x640/+0x642. */
struct Ovl21InitState;
#define D_800C0448_init (*(struct Ovl21InitState *) gOvl21SceneControl)

/* 800BB648 loads fifteen 32-bit sprite-header offsets from original data. */
extern s32 gOvl21UiSpriteOffsets[15];
/* ovl_21:800BB328 copies 0x13DC bytes and uses the leading sprite header. */
extern u8 gOvl21SceneSpriteData[0x13DC];
/* ovl_25:800B9E84 uses its own unnamed data at the same RAM address. */
/* ovl_25:9E84 passes this four-halfword anchor to the projected sprite service. */
extern u8 gOvl25SequenceAnchor0[8];
/* ovl_21:800BB328 binds this 0x30-byte source to the scene sprite header. */
extern struct SpriteSourceData gOvl21PrimarySceneSprite;
/* ovl_21:800BB648 retains 0x233C bytes for its fifteen source headers. */
extern u8 gOvl21UiSpriteData[0x233C];
/* 800BB648 initializes this 0x30-byte sprite and its adjacent records. */
extern struct SpriteSourceData gOvl21UiSpriteAnchor;

/* ovl_21:800C0458, record-table +0x10 object-state anchor. The drawing
 * traversal walks six records at stride 0x108; the pause service walks
 * three slot sources at +0x40 with stride 0x4C in each record.
 * An interior label, not a separate allocation; absolute extern access. */
extern u8 gOvl21RecordObjectBase[];
/* ovl_21:800BA670 loads the signed-halfword primary-sprite offset selector. */
extern s16 gOvl21PrimarySpriteOffsetIndex;

/* 800CA24C writes three projection-input halfwords and passes them to 8001DFD4. */
extern s16 gOvl11PrimaryDrawWorldPoint[4];
/* 8001DFD4 writes two screen-coordinate words, read by 800CA24C at +0/+4. */
extern s32 gOvl11PrimaryDrawScreenPoint[2];
/* 800CA24C tests this word before consulting the per-index halfword table. */
extern s32 gOvl11SharedSlotSpriteEnabled;
/* 800CA24C indexes this signed-halfword table with the object's signed +FE. */
extern s16 D_80125F24[];

/* D_800BCD18 - ovl_21 four-halfword (SVECTOR-shaped) projection workspace.
 * ovl_21_func_800BAFFC writes vx/vy/vz and passes its address as the SVECTOR
 * argument of func_8001DFD4. Absolute-addressed; classified in globals.h as
 * an s16 scalar would be wrong width, so the aggregate lives here. */
extern s16 gOvl21SpriteProjectPosition[4];

/* D_800BCD20 - ovl_21 eight-byte projection result buffer. func_8001DFD4
 * writes the two sxy words at +0/+4 through an s32* and
 * ovl_21_func_800BAFFC reads their low halfwords. Absolute-addressed;
 * classified in globals.h as an s32 scalar would be wrong width, so the
 * buffer lives here. */
extern s32 gOvl21SpriteProjectScreen[2];
/* ovl_25 uses the same RAM address as an unrelated load-stage byte; retain
 * its existing word-array view separately from ovl_21's projection output. */
extern s32 D_800BCD20[2];

/* D_800C0AFC - ovl_21 object-state base (0x800C0AFC). ovl_21_func_800BAF70
 * passes its address to func_80015840 and reads the two bytes at +4/+5
 * through a base register biased to D_800C0AFC - 0x6B4. An opaque byte
 * object suffices; the overlay build is -G0, giving absolute lui/addiu
 * addressing. Not classified in globals.h. */
extern u8 gOvl21FrameRectSprite[];

/* ovl_21:800C0DD8 - two-pointer scene geometry descriptor, initialized by
 * 800BB8B8 and consumed by Ovl21DrawSceneGeometry. Preserve opaque storage. */
extern u8 gOvl21SceneGeometry[];

/* E516C initializes ten 0x30-byte sprite sources; E4568 draws them by
 * entity index and E5230 clears their +14 header-address/free marker. */
typedef struct {
    /* 0x00 */ char pad_00[0x14];
    /* 0x14 */ s32 sourceHeaderAddress;
    /* 0x18 */ char pad_18[0x18];
} Ovl11EntitySpriteSlot; /* 0x30; preserve the existing word-address view. */
extern Ovl11EntitySpriteSlot gOvl11EntitySpriteSources[10];

/* 800E499C sets the VRAM offsets used by image upload and source setup.
 * X is 384 (or 0 for archive flag 0x20); Y is 0. Absolute-addressed. */
extern s16 gOvl11ArchiveTextureX;
extern s16 gOvl11ArchiveTextureY;
/* 800E499C stores the relocated sprite-data byte count here. */
extern s32 gOvl11InstalledSpriteDataBytes;
/* 800E499C loads this pointer and reads the current object's signed id at +2. */
extern s16 *Ovl11CurrentObjectId __asm__("D_8006C868");

/* D_8006C904 - byte state written by the func_800226F0/func_80022B20/
 * func_80022D70 cluster (values 0/4/6) and read by
 * ovl_11_func_800E6AB0 as a plain zero/non-zero test. The original read is
 * `lbu`, so the byte is unsigned: the generated scalar default is s8 and
 * would emit `lb`. Absolute-addressed (lui + %lo). */
extern u8 _gMessageWindowPhase[9] __asm__("gMessageWindowPhase");
#define gMessageWindowPhase (*((u8*)_gMessageWindowPhase))

/* ovl_11:80123A00 - row-major 3x3 order codes (equal/less/greater ranks),
 * plus the existing trailing zero halfword. Keep the incomplete extern's
 * original address-formation behavior; overlay data, not module storage. */
extern u16 gOvl11RecordPairOrderCodes[];

/* ovl_11:801239D0 - 800CFD9C copies three 16-byte rows' X/Y/Z words
 * into code-109 records at +38/+3C/+40; the fourth word is not read.
 * Keep the incomplete extern and split two-register address formation.
 * Storage belongs to the overlay data image, not to this C unit. */
typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
    /* 0x0C */ s32 reserved;
} Ovl11Code109Position; /* 0x10 */
extern Ovl11Code109Position gOvl11Code109Positions[];

/* ovl_11:800CF258 scans 17 item-ID/mask rules; 800CF428 also compares
 * mode and copies the three coordinate words into spawned records.
 * The mask is ANDed with the signed-halfword result of 800D2E20.
 * This aggregate remains extern; no source ownership is inferred. */
typedef struct {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ s16 mode;
    /* 0x04 */ u16 enabledMask;
    /* 0x06 */ char pad_06[0x08 - 0x06];
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 z;
    /* 0x14 */ char pad_14[0x18 - 0x14];
} Ovl11ItemPlacementRule; /* 0x18 */
extern Ovl11ItemPlacementRule gOvl11ItemPlacementRules[];

/* 800CF428 reads three coordinate words from each of three 0x10-byte records. */
extern s32 gOvl11Timed154SpawnPositions[][4];
/* 800CF748 copies one of two four-word positions for areas 0x1F/0x1B. */
extern s32 gOvl11Area108SpawnPositions[8];
/* 800CF428 reads and updates the item-spawn bit mask with lh/lhu/sh. */
extern s16 gOvl11ItemSpawnRuleMask;

/* D_80123940 - ovl_11 table of 0x18-byte records walked by
 * ovl_11_func_800CF848 with an advancing s16 pointer (stride 0x18). Field
 * +0x0 is the sprite id read both signed (ba/compare) and unsigned (the
 * argument to ovl_11_func_800CE744), +0x2 is compared against a far s16 and
 * +0x4 gates an extra check; +0x8/+0xC/+0x10 are copied to a spawned
 * record's +0x38/+0x3C/+0x40. Absolute-addressed (lui + %lo, split
 * two-register form): declared as an aggregate wider than -G8, and owned by
 * the ovl_11 data image (this TU only declares it). */
extern s16 gOvl11AreaItemSpawnRules[];

/* D_8009AFF0 - main-RAM base whose s16 at +0x5476 ovl_11_func_800CF848
 * compares against the D_80123940 record's +0x2 field. Only the base and
 * that one far offset are witnessed; an incomplete byte array keeps the
 * base/offset relation. Absolute-addressed (lui + %lo). */
extern u8 D_8009AFF0[];

/* 8010C7F0 reads words at D_8006C838 +0xC and +0x10. */
struct Ovl11C7F0Flags {
    u8 pad_0[0xC];
    s32 flagsC;
    s32 flags10;
};

/* 8010C7F0 compares the signed halfword at D_8009AFF0 +0x5476. */
struct Ovl11C7F0NearState {
    u8 pad_0[0x5476];
    s16 area;
};

/* ovl_11:80128818 - two-pointer scene geometry descriptor, initialized by
 * 800DBFEC/80104CB8 and shared by drawing and collision queries.
 * Preserve its existing opaque storage view. */
extern u8 gOvl11SceneGeometry[];

/* ovl_19:800BF700 - two-pointer scene geometry descriptor, initialized by
 * 800BBB48 and shared by Ovl19DrawSceneGeometry/Ovl19AdjustPositionToSurface.
 * Keep the existing opaque storage view; consumers cast it as needed. */
extern u8 gOvl19SceneGeometry[];

/* ovl_19_func_800B7F34 selects and advances this state with lbu/sb. */
extern u8 gOvl19LoadStep;

/* ovl_19:800BCB74 - disc positions. Ovl19LoadSceneGeometry reads the first
 * three and transfers the two intervening byte ranges. Overlay data remains
 * externally defined; this is not a new allocation or tentative definition. */
extern s32 gOvl19SceneDiscPositions[];

/* gOvl21SceneDiscPositions (800BC894) - s32 CD-position table. ovl_21
 * func_800BB8B8 reads [0], [1] and [2] and passes [1]-[0] and [2]-[1] as
 * the sector counts of two func_80014BCC transfers, exactly as its
 * byte-identical twin ovl_19_func_800BBB48 does with D_800BCB74. Defined in
 * the overlay data segment, so only an extern declaration belongs here. */
extern s32 gOvl21SceneDiscPositions[];

/* D_800BBED0 - ovl_25 s32 {start,end} CD-position table. ovl_25
 * func_800BB2B8 reads [0], [1] and [2] and passes [1]-[0] and [2]-[1] as the
 * sector counts of two func_80014BCC transfers, exactly as its
 * sibling ovl_19_func_800BBB48 does with D_800BCB74; then it additionally
 * publishes the D_8009F78C copy and a 0x39 value to the D_8007AFF0 far
 * buffer. Defined in the overlay data segment, so only an extern declaration
 * belongs here. */
extern s32 gOvl25SceneDiscPositions[];
/* ovl_25 AA5C reads unsigned position halfwords; 83A0/8478 reinterpret
 * the same initial-record table through their existing byte/record views. */
extern u16 gOvl25InitialSceneRecords[];
/* ovl_25 83A0/8478 copy two eight-byte records from BCC70 through their
 * existing byte/record views, separately from the six-record BCC10 table. */
extern s16 gOvl25InitialAuxRecords[];
/* 83A0/8478 copy four signed halfwords per eight-byte initial-table record. */
typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
} Ovl25SeedRecord;

/* D_8009F78C - main-RAM copy destination. ovl_19_func_800BBB48 memcpys
 * 0xBDC bytes here from the D_8007AFF0 work area (absolute lui/addiu). The
 * TU only declares it, so an opaque byte object is enough. */
extern u8 gLoadedAreaHeaderCopy[];

/* ovl_11 800BCF54/800BD374 use this word as the anchor for stride-0x10 records. */
extern s32 gOvl11ResourceDescriptors;
/* BD488/BD5A8 index adjacent archive byte boundaries, then load their difference. */
extern s32 gOvl11OffsetSpriteBoundaries[];
extern s32 gOvl11RecordSpriteBoundaries[];
/* BD358 stores -1; no retail reader of this retained marker is established. */
extern s32 gOvl11ResourceIndex;
/* BD168 initializes this byte-backed sprite source from the staging header. */
extern u8 D_80071C60[];

/* D_800C0454 - ovl_25 ProjectedPrimitiveSource descriptor whose address
 * ovl_25_func_800BAC28 hands to func_8001C0D4. Only its address is taken, so
 * an opaque byte object is enough; the overlay build is -G0, giving absolute
 * lui/addiu addressing. */
extern u8 gOvl25SceneGeometry[];

/* D_800C030C - ovl_25 signed halfword state word. ovl_25_func_800B81F4
 * loads it (lh) and, when it is not -1, passes it to func_8001FAE8.
 * Absolute-addressed (lui + lh %lo) from ovl_25 code; the object lives in
 * the overlay data segment, so only an extern declaration belongs here. */
extern s16 gOvl25SceneEffectVoice;

/* ovl_25_func_800BAFCC copies 0x1C88 streamed bytes here and indexes sprite headers. */
extern u8 gOvl25ObjectSpriteData[];
/* 800BAFCC initializes full 0x30-byte sources. The sprite-set service
 * explicitly casts their flag/ObjectState views; these are first-record anchors. */
extern struct SpriteSourceData gOvl25AuxiliarySpriteBase;
extern struct SpriteSourceData gOvl25PrimarySpriteBase;
extern struct SpriteSourceData gOvl25SequenceSpriteBase;

/* D_800C027C - ovl_25 object-state base (0x800C027C). ovl_25_func_800B9E20
 * resets it with func_80015840(&base, 0), resets the record 0x30 bytes on as
 * +1, and derives the raw u16 buffer 104 bytes below (0x800C0214) for the
 * ovl_25_func_800BA5BC/ovl_25_func_800BA758 pair. Absolute-addressed
 * (lui + %lo) from -G0 overlay code, so only an extern declaration belongs
 * here; the caller views the base as ObjectState. */
/* Accessed as gOvl25PrimarySpriteBase + 1, not separate storage. */

/* D_800C02DC - ovl_25 object-state base (0x800C02DC), the record 0x90 bytes
 * above D_800C024C. ovl_25_func_800B9D4C resets it with func_80015840 and
 * also hands it to the ovl_25_func_800BA5BC transform, while deriving a
 * 0x78-byte record 0x498 bytes below (0x800BFE44) for the three stacked u16
 * vertex values. Absolute-addressed (lui + %lo) from -G0 overlay code, so
 * only an extern declaration belongs here. */
/* Accessed as gOvl25PrimarySpriteBase + 3, not separate storage. */

/* D_800BFE54 - ovl_25 base of a six-entry run of 0x78-byte Ovl25ObjectRecord
 * records. ovl_25_func_800B941C walks it (lui/addiu %hi/%lo) and hands each
 * entry to ovl_25_func_800B9B28. Absolute-addressed from -G0 overlay code;
 * the record type is defined in game_types.h, forward-declared here. */
extern struct Ovl25ObjectRecord gOvl25SceneRecordBase;

/* 800B813C indexes these three original callbacks; each ignores incoming arguments. */
extern void (*gOvl25SceneModeHandlers[3])(void);
/* 7EB4 dispatches the five original pointer words at ovl_25:800BCBD8,
 * forwarding the table base as an ignored argument to its phase handlers. */
extern s32 gOvl25ScenePhaseTable[5];
/* 7F3C increments/resets only the byte at ovl_25:800BCD20. This is NOT
 * ovl_21's unrelated projection buffer at the same RAM address. */
extern u8 gOvl25SceneAssetLoadStep[1];

/* 800B813C reads the s16 selector@0; 800B8848 accesses s16@4CC; 800B97E4 reads s16@C,4D4,4DE..4E4. */
typedef struct {
    s16 selector;
    u8 pad_2[2];
    s32 paused; /* 800B8728 loads/stores the pause flag as a word at +4. */
    s32 unk8;
    s16 unkC;
    s16 unkE;
    u8 pad_10[0x10];
    s16 unk20;
    u8 pad_22[0x4C8 - 0x22];
    s16 sceneEffectVoice; /* +4C8: pause/start/end effects; teardown stops it. */
    s16 recordEffectVoice; /* +4CA: record-0 update starts/stops/sets volume. */
    s16 unk4CC;
    s16 unk4CE;
    s16 unk4D0;
    s16 unk4D2;
    s16 tierUpdateDivisor;       /* +4D4: divisor for per-call values/Q12 deltas. */
    s16 unk4D6;
    s16 unk4D8;
    s16 unk4DA;
    s16 unk4DC;
    s16 kind5TierAmount;         /* +4DE: divided by 30 before repeated decay. */
    s16 kind5TierDecayPercent;   /* +4E0: percentage removed on each decay step. */
    s16 kind4TierAmount;         /* +4E2: divided by 30 for the kind-4 increment. */
    s16 otherKindTierPercent;    /* +4E4: scales the tier value for other nonzero kinds. */
} Ovl25StateView;
/* 82B4/8340/83A0/8478 and the mode handlers address the same halfword
 * work anchor. Keep its established typed view without adding storage. */
extern s16 gOvl25SceneWorkWords[];
extern Ovl25StateView D_800BFE44_view __asm__("gOvl25SceneWorkWords");
/* ovl_25 95B0 reads unsigned weights at category*24 + (recordId-1)*8 + choice*2.
 * Its -1 category intentionally reaches the preceding original table words. */
extern u16 gOvl25State5ChoiceWeights[][3][4];
/* ovl_25 977C reads signed thresholds at (recordId-1)*5 + choice. */
extern s16 gOvl25RecordChoiceThresholds[];
/* 800B8DF4 reads signed halfwords at +E,+20,+4CA..4DC and a word at +8 in the view above. */
/* 800B8DF4 compares this signed halfword with state +20. */
extern s16 D_800BCC40;
/* ovl_25_func_800B8848 passes these halfword text streams to func_8001AC10; original data terminates with 0xFFFF. */
extern u16 D_800BCB34[];
extern u16 D_800BCB8E[];
/* ovl_25_func_800B8848 stores a byte flag; matched neighbours declare it unsigned. */
/* ovl_25: the six callbacks at BA7F0..BA9F4 write this signed phase;
 * 853C indexes the six-address table with the same work-area halfword. */
extern s16 gOvl25EventPhase;
extern void (*gOvl25EventPhaseHandlers[6])(void);
/* ovl_25:9E84 advances 0/1/2; 860C and the event-phase service also write it. */
extern u8 gOvl25SequenceCyclePhase;
/* ovl_25_func_800B8848 writes the first three halfwords; original data allocates four. */
extern s16 gOvl25SequenceAnchor4[4];
/* ovl_25:800B9A10/9A84 select these private s16 values at 40/30/20/10.
 * The delta words initially contain -2; no runtime meaning is inferred. */
extern s16 gOvl25TierValue40;
extern s16 gOvl25TierValue30;
extern s16 gOvl25TierValue20;
extern s16 gOvl25TierValueBelow20;
extern s16 gOvl25TierDelta40;
extern s16 gOvl25TierDelta30;
extern s16 gOvl25TierDelta20;
extern s16 gOvl25TierDelta10;
/* ovl_25:800BBA7C reverses/clamps a signed 0..511 interpolation index. */
extern s16 gOvl25BackgroundCycleDescending;
extern s16 gOvl25BackgroundCycleIndex;
/* ovl_25:800BBA7C passes each contiguous RGB triple to InterpolateThreeWords. */
extern s32 gOvl25BackgroundCycleLowRgb[3];
extern s32 gOvl25BackgroundCycleHighRgb[3];
/* 800B9E84 reads/writes unsigned state and flag bytes; its counter is D_800BCD21 + 2. */
extern u8 gOvl25SequenceRecordStage;
extern u8 gOvl25SequenceEffectActive;
/* 800B9E84 passes these coordinate buffers to BA5BC; signed x comparisons use the first halfword. */
extern s16 gOvl25SequenceAnchor1[4];
extern s16 gOvl25SequenceAnchor2[4];
extern s16 gOvl25SequenceAnchor3[4];
/* 800B9E84 uses signed division by 3/5 and signed comparisons on this halfword. */
extern s16 gOvl25SequenceStep;
/* ovl_25:A5BC/A758 alone write four projection halfwords and receive two
 * signed screen-coordinate words from ProjectPointToScreen. Extern under -G0. */
extern s16 gOvl25BiasedProjectionInput[4];
extern s32 gOvl25BiasedProjectionResult[2];
extern s16 gOvl25ProjectionInput[4];
extern s32 gOvl25ProjectionResult[2];
/* 800B9E84 compares this signed halfword against 0x690. */
extern s16 D_800BFE64;
/* 800C0394/800C0424 are records 2/5 of gOvl25SequenceSpriteBase;
 * the service accesses them through that canonical base, not interior aliases. */

/* D_800C0124 - ovl_25 base of a two-entry run of the same 0x78-byte
 * Ovl25ObjectRecord records, walked by ovl_25_func_800B941C after the
 * D_800BFE54 run and handed to ovl_25_func_800B9C90. Same absolute-addressed
 * incomplete-type declaration class as D_800BFE54 above. */
extern struct Ovl25ObjectRecord gOvl25AuxiliaryRecordBase;

/* D_800BFE60 - ovl_25 signed level/threshold word. ovl_25_func_800B9D4C
 * loads it once (lw) and selects one of five reset levels by comparing it
 * against 0x28000, 0x1E000, 0x14000 and 0xA000. Absolute-addressed
 * (lui + lw %lo) from -G0 overlay code; the overlay only declares it, so a
 * scalar extern keeps the base name. */
extern s32 D_800BFE60;

/* D_801227F8 - ovl_11 pair table indexed by an s32 argument in
 * ovl_11_func_800BD1BC; entries are {start, end} s32 pairs. Defined in the
 * overlay data segment, so only an extern declaration belongs here. */
extern s32 gOvl11CopiedSpriteBoundaries[];

/* D_80122804 - ovl_11 pair table sibling of D_801227F8, indexed by an s32
 * argument in ovl_11_func_800BD238; entries are {start, end} s32 pairs.
 * Defined in the overlay data segment, so only an extern declaration belongs
 * here. */
extern s32 gOvl11SceneSpriteBoundaries[];

/* D_8007F7F8 - main-RAM destination buffer written by ovl_11_func_800BD1BC's
 * memcpy. Address comes from undefined_syms_auto.txt. */
extern s32 D_8007F7F8;

/* D_8007AFF8 - main-RAM base 8 bytes past D_8007AFF0, handed to
 * func_80015704 as a SpriteDataHeader by ovl_11_func_8010B67C, which then
 * compares an s16 at +0x1FFF8+0x5476. Absolute-addressed (lui + %lo); the
 * overlay only declares it, so a scalar extern keeps the base name. */
extern s32 D_8007AFF8;

/* D_80137586 - ovl_15 private state byte in the overlay data segment.
 * ovl_15_func_8012E15C clears it and ovl_15_func_801300E4 stores -1 to it
 * (sb $v0, %lo). Absolute-addressed (lui + %lo) from -G0/overlay code; the
 * TU family only declares it, so a scalar extern keeps the base name. */
extern s8 gOvl15MenuExitRequest;

/* D_80137587 - ovl_15 private state byte in the overlay data segment,
 * incremented/decremented and clamped by ovl_15_func_80136990 (lbu/sb for
 * the +=/-= and lb for the signed comparisons). Absolute-addressed
 * (lui + %lo) from -G0/overlay code; the TU family only declares it, so a
 * scalar extern keeps the base name. */
extern s8 gOvl15MenuCursor;

/* D_80137588 - ovl_15 private state byte in the overlay data segment,
 * incremented/decremented and clamped by ovl_15_func_801370B4.
 * Absolute-addressed (lui + %lo) from -G0/overlay code; the TU family only
 * declares it, so a scalar extern keeps the base name. */
extern s8 gOvl15ConfirmationChoice;

/* D_80137584 - ovl_15 private state byte in the overlay data segment; the
 * shared-prologue cluster (ovl_15_func_8012EEEC/8013063C/80131618 and the
 * D_80052FC4 family) stores a state code to it (sb $v0, %lo) after
 * ovl_15_func_80136990. Absolute-addressed (lui + %lo) from -G0 overlay
 * code; only extern declarations belong here. */
extern s8 gOvl15MenuState;
/* 8012E520 compares this previous-state byte with lb and updates it with sb. */
extern s8 gOvl15LastMenuState;

/* D_8013758C - ovl_15 private state s16 in the overlay data segment; the
 * same cluster stores the ovl_15_func_80136990 result to it (sh $v0, %lo).
 * Absolute-addressed (lui + %lo) from -G0 overlay code. */
extern s16 gOvl15SelectedCard;

/* ovl_15_func_801301A0 reads this card index with lh and updates it with sh. */
extern s16 gOvl15MonitoredCard;

/* D_8013758E - ovl_15 private state s16 in the overlay data segment, read by
 * ovl_15_func_80130C8C and passed to ovl_15_func_801367F8 and by the
 * ovl_15_func_80130C2C/80130D3C family. Absolute-addressed (lui + %lo) from
 * -G0 overlay code; only extern declarations belong here. */
extern s16 gOvl15SelectedSlot;

/* D_8013759E - ovl_15 private mode/state s16 in the overlay data segment,
 * read by the D_8013759E/A0/A2/A4 state run (ovl_15_func_801344E8 selects its
 * scan table with `lh D_8013759E`; ovl_15_func_80133F4C/801342A0 also read it).
 * Absolute-addressed (lui + %lo) from -G0 overlay code; the TU family only
 * declares it, so a scalar extern keeps the base name. */
extern s16 gOvl15SlotCategory;

/* D_80053350 - text-updater array base in the PS-X EXE used by the shared
 * ovl_15 text-draw prologue (ovl_15_func_8012ED18 family add D_80054BBC[0]
 * to its address with the split lui/addiu form; also ovl_15_func_80130C8C).
 * Never defined in the overlay, so an incomplete array extern keeps the base
 * name. */
extern u8 D_80053350[];

/* D_80053530 - ovl_15 updater-array base in the PS-X EXE used by the shared
 * text-draw prologue (ovl_15_func_8012EEEC/8013063C/80131618 add
 * D_80054BBC[0] to its address with the split lui/addiu form). Never defined
 * in the overlay, so an incomplete array extern keeps the base name. */
extern u8 D_80053530[];

/* ovl_15_func_80130FF0 adds D_80054BBC[0] bytes to this text-updater base. */
extern u8 D_800533C8[];
/* ovl_15_func_80130FF0 reads this state with lh and compares it against 0x5B. */
extern s16 gOvl15MenuStateTimer;
/* ovl_15_func_8012F4F4 clears this state with sh; 8012F6C0 reads it with lh. */
extern s16 gOvl15FormatPromptStep;

/* ovl_15_func_80130B04 adds D_80054BBC[0] bytes to this text-updater address. */
extern u8 D_80053596[];
/* ovl_15_func_8012F300 adds D_80054BBC[0] bytes to this text-updater address. */
extern u8 D_80053566[];
/* ovl_15_func_8012F7D4 adds D_80054BBC[0] bytes to this text-updater address. */
extern u8 D_8005316C[];

/* 8012F3FC/801326E8: card write and transfer summaries draw bank-adjusted OK to overwrite / this diary. */
extern u8 gCardOverwriteConfirmTextAnchor[];
/* ovl_15_func_8012F158 adds D_80054BBC[0] to this text-updater address. */
extern u8 D_80052FC4[];
/* ovl_15:801315B0 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052FFA[];
/* ovl_15:80131778 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_8005306A[];
/* ovl_15:801317E8 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_800530C8[];
/* ovl_15:80131A34 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_800532FE[];
/* ovl_15:80131B5C adds the text-bank byte offset before drawing this prompt. */
extern u8 D_800523AC[];
/* ovl_15:80131D88 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_8005341E[];
/* ovl_15:80131DF8 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_800533F6[];
/* ovl_15:80131E68 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052F4E[];
/* ovl_15:80131F5C adds the text-bank byte offset before drawing this prompt. */
extern u8 D_8005204A[];
/* ovl_15:8013237C adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052150[];
/* ovl_15:80132408 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052178[];
/* ovl_15:80132494 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_800521F6[];
/* ovl_15:80132520 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052280[];
/* ovl_15:801325AC adds the text-bank byte offset before drawing this prompt. */
extern u8 D_8005230A[];
/* ovl_15:801328C4 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80053506[];
/* 80132934: Ovl15CardTransferConfirmExit draws bank-adjusted Aborting exchange / Is that OK. */
extern u8 gCardTransferAbortConfirmTextAnchor[];
/* 80130F2C: Ovl15CardReadConfirmExit draws bank-adjusted Aborting load / Is that OK. */
extern u8 gCardReadAbortConfirmTextAnchor[];
/* ovl_15:801329F8 adds the text-bank byte offset before drawing this prompt. */
extern u8 D_80052022[];
/* 8012E15C clears these halfword controls before each menu session. */
extern s16 D_80137590;
/* Card operations 36558/36660/367F8 count retries; signed limits use casts. */
extern u16 gOvl15CardRetryCount;
/* 8012E15C clears this 0x80-byte scratch area. Summaries use the header view. */
extern u8 D_801377A0[];

/* ovl_15_func_8012E15C initializes two halfword card states here. */
extern s16 gOvl15CardStates[];
/* ovl_15_func_8012F158 writes three halfword save-slot states at stride 2. */
extern s16 gOvl15SaveSlotStatus[];

/* ovl_15_func_80134A34 copies an aligned word pair and two 18-byte
 * halfword-aligned names into each 0x3C-byte record at D_801376E0. */
struct Ovl15SaveWordPair {
    s32 words[2];
};
struct Ovl15SaveName {
    /* 80136C70 sanitizes unsigned glyphs and stores the 0xFFFF terminator. */
    u16 units[9];
};
struct Ovl15SaveHalfwordPair {
    s16 units[4];
};
struct Ovl15SaveHeader {
    struct Ovl15SaveWordPair words;
    struct Ovl15SaveName name_8;
    struct Ovl15SaveName name_1A;
    struct Ovl15SaveHalfwordPair fields_2C;
    s32 field_34;
    s32 field_38;
};
extern struct Ovl15SaveHeader gOvl15SaveSlotHeaders[];
/* 80137544 selects one of four filename addresses, then copies it with strcpy. */
extern s32 gOvl15CardFilenameTable[];
/* ovl_15_func_80134A34 reads the word pair at +0 and names at +0x28/+0xD10. */
struct Ovl15SaveHeaderSource {
    struct Ovl15SaveWordPair words;
    u8 pad_8[0x20];
    struct Ovl15SaveName name_28;
    u8 pad_3A[0xD10 - 0x3A];
    struct Ovl15SaveName name_D10;
};
extern struct Ovl15SaveHeaderSource D_80070CF0_headerSource __asm__("gCalendarYear");

/* ovl_15 transfer buffer and its metadata view (+0x200), not two allocations.
 * Ovl15WriteSaveChecksums (0x80135AE0) covers records 5..255 at 0x80 stride;
 * metadata stores their XOR at +0x38 and its own XOR at +0x7F. The base is
 * also reused by Ovl15LoadMenuSpriteResources. Keep these external:
 * target accesses are absolute (lui + %lo); storage stays in the data segment. */
extern u8 gOvl15TransferBuffer[];
extern u8 gOvl15SaveMetadata[];

/* 801373A0 uploads images at transfer-buffer +0x2D74, copying the preceding
 * sprite records separately. This is an interior label, not another buffer. */
extern u8 gOvl15LoadedSpriteImages[];

/* 801373A0 copies 0x2D74 bytes here before initializing five sprite sources. */
extern u8 gOvl15MenuSpriteData[];

/* 801373A0 reads five byte offsets into the copied sprite-data block. */
extern s32 gOvl15MenuSpriteOffsets[];

/* 801373A0 initializes five 0x30-byte SpriteSourceData records here. */
extern struct SpriteSourceData gOvl15MenuSpriteSources[];

/* Calendar panel state, ovl_11:80126F7C..80126F8C. The panel has phases
 * 0=inactive, 1=interactive, 2=closing. Message state FF enables navigation;
 * selected day is a zero-based signed word. The hook flag gates an empty
 * compatibility callback. All remain external overlay data, not GP-owned. */
extern s32 gOvl11CalendarPanelState;
extern s32 gOvl11CalendarMessageState;
extern s32 gOvl11CalendarSelectedDay;
extern s32 gOvl11CalendarCloseHookEnabled;

/* ovl_11: 123E54/56 are the background-cycle direction and 0..511 index. */
extern s16 gOvl11BackgroundCycleDescending;
extern s16 gOvl11BackgroundCycleIndex;
/* ovl_11: 123E58/64 begin consecutive s32 RGB triplets (0,99,127) and
 * (0,165,213). Retain the original scalar address views used to pass each
 * triplet to the three-component interpolator; no storage is defined here. */
extern s32 gOvl11BackgroundCycleLowRgb;
extern s32 gOvl11BackgroundCycleHighRgb;

/* D_801287F8 - ovl_11 pointer slot written by ovl_11_func_800BD938, which
 * stores D_8005E3B0 + 0x4290 into it (absolute lui + sw %lo). Defined in the
 * overlay data segment, so only an extern declaration belongs here. */
extern u16 *gOvl11PaletteBankPointer;

/* DCC5C writes three position halfwords before projecting this vector. */
extern s16 gOvl11SceneProjectionInput[4];
/* DCC5C culls against the two signed screen-coordinate words. */
extern s32 gOvl11SceneProjectionOutput[2];

/* ovl_11:80128E08 - fifteen 0x30-byte scene render records. Byte storage
 * keeps partial views compatible; each client casts to its witnessed layout. */
extern u8 gOvl11SceneRecords[15 * 0x30];
/* DCC5C's render view: retain unsigned halfword loads, with signed position
 * conversion at projection time. +2C points to an Ovl11VertexBlend when flag
 * 4000 is set; void* avoids depending on that shared parameter type here. */
typedef struct {
    u8 primitive[8];
    u16 positionX, positionY, positionZ;
    u8 reservedE[2];
    u16 baseAngleX, baseAngleY, baseAngleZ;
    u8 reserved16[2];
    u16 deltaAngleX, deltaAngleY, deltaAngleZ;
    u8 reserved1E[2];
    u16 offsetAngleX, offsetAngleY, offsetAngleZ;
    u8 reserved26[2];
    u16 flags;
    void *blend;
} Ovl11SceneRecordRenderView;

/* 800DD8AC/800DDA08 select 0x30-byte records. Renderer 800DCC5C passes
 * +0x10/+0x12/+0x14 to RotMatrixZXY; both ramp updates compare signed angles. */
typedef struct {
    /* 0x00 */ u8 pad[0x10];
    /* 0x10 */ s16 rotationX;
    /* 0x12 */ s16 rotationY;
    /* 0x14 */ s16 rotationZ;
    /* 0x16 */ u8 pad2[0x12];
    /* 0x28 */ u16 flags;
    /* 0x2A */ u8 pad3[0x6];
} Ovl11SceneRecordRotationView;
/* ovl_11:800DD8AC/800DD904 select and ramp the record's Z angle. */
extern Ovl11SceneRecordRotationView *gOvl11SceneZRotationRecord;
extern s32 gOvl11SceneZRotationRampState;
/* ovl_11:800DDA08/800DDA60 select and ramp the record's Y angle. */
extern Ovl11SceneRecordRotationView *gOvl11SceneYRotationRecord;
extern s32 gOvl11SceneYRotationRampState;

/* ovl_11:800DCECC/800DCF10 walk three 0x34-byte vertex-blend descriptors.
 * Ovl11VertexBlend in game_types.h is the shared entry view. */
extern u8 gOvl11VertexBlendPool[3 * 0x34];

/* ovl_11:80129178 / 80129184: 800DD374 indexes three scene records;
 * 800DD4D8 and its nested helper step their blends and transfer visibility. */
extern u8 *gOvl11SceneAnimationCycle[3];
extern s32 gOvl11SceneAnimationCycleState;

/* ovl_11:801291A8: two scene-record addresses selected by flags 0x10/1;
 * setup/update alone use this pair to alternate the renderer's hide bit. */
extern u32 gOvl11SceneRecordVisibilityPair[2];

/* D_800BFC90 - ovl_23 sound/state block. Absolute-addressed (lui + %lo) from
 * ovl_23 code; ovl_23_func_800B8104 reads a signed halfword at +0 and compares
 * it against -1, while ovl_23_func_800B9454 stores a word at +0 and halfwords
 * at +4/+8/+0xA (+0x1B4). Declared as the halfword this TU observes; the
 * object lives in overlay RAM, so only an extern declaration belongs here. */
extern s16 D_800BFC90;

/* ovl_19 initialiser tables. Absolute-addressed (lui + %lo) from ovl_19 code;
 * they live in the overlay data segment, so only extern declarations belong
 * here. D_800BCEF0 is a u16 table of 0..5 at 0x800BCEF0; D_800BCF0C is a table
 * of 8-byte {s16 x4} records; D_800BF4C0 is the flat s16 display/state array
 * that the ovl_19 initialiser walks. D_800BCEE8 is a u16 table of four
 * halfwords read with an (entry[4] + 1) index by ovl_19_func_800B847C;
 * D_800BCEFC is the flat s16 source array that function copies 8-byte records
 * out of. D_800BCF28 is a u16 weight table read by ovl_19_func_800BA33C as six
 * groups of four halfwords, indexed by (category << 3) + (flag << 2) in
 * halfwords (category = ovl_19_func_800BA33C's threshold class, flag = whether
 * arg->+0xC is non-zero). */
/* exe:8007AE58 - Ovl19DrawValueTable reads its first label address here. */
extern s32 D_8007AE58;
/* ovl_19:800BCD68 - 0x12C-byte halfword-label pool, indexed by byte offsets. */
extern u16 gOvl19ValueLabelText[0x96];
/* ovl_25 has a different object at 800BCD68: 800BAFCC reads sprite-header
 * offsets here. Preserve its prior declaration; it is NOT the ovl_19 text. */
extern u16 D_800BCD68[0x96];
/* 800B8C74 indexes this byte table with the signed selection at state +0x8. */
extern u8 D_800BCF24[];
extern s16 gOvl19SceneWork[];
/* ovl_19 800B89C8 loads/stores this selection as a signed halfword. */
extern s16 D_800BD070;
/* 800B8C74: state at +4, signed selection at +8, byte flag at +C,
 * indexed signed values at +160 and signed handle at +204. */
typedef struct {
    u8 pad_0[4];
    s16 state;
    u8 pad_6[2];
    s16 selection;
    u8 pad_A[2];
    u8 flag;
    u8 pad_D[0x153];
    s16 values[0x52];
    s16 handle;
} Ovl19SelectionState;
/* ovl_19 800B9124 increments/resets this signed word and tests its remainder modulo 30. */
extern s32 D_800BD078;
/* ovl_19 800B95D4 anchors its two 0x48-byte signed-halfword records here. */
extern s16 gOvl19ActorPairState[];
/* ovl_19 800BA770 walks three 0x40-byte halfword records through 800BAB4C. */
extern s16 gOvl19AuxRecordBase[];
/* ovl_19 800BA770 passes positions at this base, stepping by six halfwords. */
extern s16 gOvl19FixedSpritePositions[];
/* ovl_19 800B80E8 indexes eight original handlers; each overwrites incoming $a0 before use. */
extern void (*gOvl19ModeHandlers[8])(void);
/* ovl_19 800B87C4 indexes seven original handlers; none consumes incoming $a0. */
extern void (*gOvl19StageHandlers[7])(void);
/* ovl_19 800B80E8 loads this signed halfword as the handler index. */
extern s16 gOvl19SceneMode;
/* ovl_19 800B8190 loads this handle with lh and compares it against -1. */
extern s16 gOvl19SoundVoice;
/* ovl_19:7ED8 indexes five handler words; preserve its existing scalar view. */
extern s32 gOvl19PhaseHandlerWords;
/* ovl_19:85F4 indexes this halfword limit table by year/category. */
extern u16 D_800BCFE4[];
/* ovl_19:85F4 reads the signed halfword category used by that limit table. */
extern s16 D_800BF4C8;
/* ovl_19:8F38 increments this word, sounds every 30 ticks and clears on exit. */
extern s32 gOvl19MessageSoundTicks;
/* ovl_19:BADAC clamps an index to 0..19 and reads this signed-word table. */
extern s32 gOvl19ValueLabelOffsets[20];
/* D_800BF4E0 - the s16 field at D_800BF4C0 + 0x20, addressed by ovl_19_func_800B8DE8
 * through its own symbol (the compiler materialises D_800BF4E0 and expresses the
 * neighbouring D_800BF4C0 accesses as negative displacements from it). */
extern s16 D_800BF4E0[];
/* D_800BF5A0 - ovl_19 s16 state record that ovl_19_func_800B9454 anchors on:
 * the compiler materialises D_800BF5A0 (lui + %lo) and derives the D_800BF4C0
 * array and the +0x10/+0x58 command records it passes to the ovl_19 helpers as
 * negative displacements from it. The s16 timer at +0x4 is read signed and
 * unsigned and written back. Absolute-addressed (overlay build is -G0). */
extern s16 D_800BF5A0[];
/* D_800BF630 - ovl_19 s16 state halfword. ovl_19_func_800BA0A0 reads it
 * directly (lui + %lo, then lh) as one of the switch-1 rate sources; the
 * object lives in overlay RAM, so only an extern declaration belongs here. */
extern s16 gOvl19BaseActorStep;
/* D_800BF64E - ovl_19 s16 state halfword. ovl_19_func_800B9E14 loads it
 * directly (lui + %lo, then lh) and compares it against the +0x6 state field
 * of its record argument. Absolute-addressed (overlay build is -G0). */
extern s16 gOvl19StepStartTicks;
/* D_800BF650 - ovl_19 s16 state halfword. ovl_19_func_800B9F40 loads it
 * directly (lui 0x800c + %lo, then lh) and compares it, less 0x1E and 0x3C,
 * against the +0x6 state field of its record argument. Absolute-addressed
 * (overlay build is -G0). */
extern s16 gOvl19DoubleThresholdTicks;
/* ovl_19 800BA834 loads this signed halfword for the case-10 thresholds. */
extern s16 gOvl19State10ThresholdTicks;
extern u16 D_800BCEF0[];
extern s16 D_800BCF0C[];
/* ovl_19 800B8830 indexes this overlay-local table with a signed halfword load. */
extern s16 Ovl19_D_800BCFF0[] __asm__("D_800BCFF0");
extern u16 D_800BCF28[];
/* ovl_19 800B9AB0 indexes byte weights by flag * 16 + direction * 4 + choice. */
extern u8 gOvl19DirectionWeights[];
extern u16 D_800BCEE8[];
extern s16 D_800BCEFC[];

/* EXE 8006E910: 206 sixteen-byte object-program records, loaded by 800E499C.
 * The interpreter branches on handler result == 1; FF advances sequentially.
 * payload[0] is the yield-control byte; operands begin at record+4.
 * The branch-slot service scans this array by cursor equality. */
typedef struct {
    /* 0x00 */ u8 opcode;
    /* 0x01 */ u8 successTarget;
    /* 0x02 */ u8 failureTarget;
    /* 0x03 */ u8 payload[0xD];
} Ovl11ObjectProgramRecord;
extern Ovl11ObjectProgramRecord gOvl11ObjectProgramRecords[];

/* 800E3200 passes this 12-byte animation table to 800D04D4's byte-pointer parameter. */
extern u8 gOvl11SmallRecordHandler4Keyframes[12];

/* 800E4330 indexes consecutive lw offsets by the current object's signed id. */
extern s32 gOvl11ObjectArchiveOffsets[];
/* 800E4330 initializes both cursors to the resident program-record array. */
extern Ovl11ObjectProgramRecord *gOvl11ObjectProgramBase;
extern Ovl11ObjectProgramRecord *gOvl11ObjectProgramCursor;
/* 800E4330 clears this word before initializing the scene. */
extern s32 gOvl11ObjectProgramIndex;
/* 800E40CC dispatches this 113-entry table with lh +4/+6 and lw +8/+C arguments. */
extern s32 (*gOvl11ObjectOpcodeHandlers[0x71])(s16, s16, s32, s32);

/* Ovl19SetActorSpritePauseMode applies flag bit 4 at three 0x40-stride
 * actor sprites and two 0x48-stride auxiliary sprites. These incomplete
 * byte views preserve the embedded sprite bases, not full actor layouts.
 * Overlay -G0: storage remains external, with no tentative definitions. */
extern u8 gOvl19ActorSprites[];
extern u8 gOvl19AuxActorSprites[];
/* ovl_19_func_800BADF8 passes this two-word output to func_8001DFD4. */
extern s32 gOvl19ProjectedSpriteScreen[2];
/* ovl_19_func_800BADF8 fills three halfwords of this eight-byte GTE vector. */
extern s16 gOvl19ProjectedSpriteVector[4];
/* ovl_19_func_800BAFAC passes this two-word output to func_8001DFD4. */
extern s32 gOvl19ProjectedStateScreen[2];
/* ovl_19_func_800BAFAC fills three halfwords of this eight-byte GTE vector. */
extern s16 gOvl19ProjectedStateVector[4];
/* Ovl19LoadUiAndActorSprites reads six word offsets for sprite headers. */
extern s32 gOvl19UiSpriteOffsets[6];
/* The same loader copies this complete blob, retaining its byte alignment. */
typedef struct {
    u8 bytes[0x104C];
} Ovl19SpriteBlob;
extern u8 gOvl19UiSpriteData[0x104C];
/* Ovl19LoadSceneSpriteArchive retains and mirrors 0x13DC bytes here. */
extern u8 gOvl19SceneSpriteData[0x13DC];
/* The scene archive loader binds this header to its retained sprite data. */
extern struct SpriteSourceData gOvl19SceneSpriteSource;
/* UI/actor loading binds this header to sprite-offset table entry 5. */
extern struct SpriteSourceData gOvl19AuxSceneSpriteSource;
/* 800BAC9C stores its set/clear operation here before applying mask 4. */
extern void (*gOvl19ActorSpriteFlagOperation)(u16 *flags, s32 mask);

/* 800D3250's large-bank value anchor and shared scale at +1086.
 * This address view spans intervening state; it is not a record allocation. */
struct Ovl11LargeTaggedValueView {
    u16 value;
    u8 pad_2[0x1086 - 2];
    u16 scale;
};
extern struct Ovl11LargeTaggedValueView gTaggedRecordValueBase;

/* 800D3250's small-bank value anchor at work+7ACA, scale at work+9258. */
struct Ovl11SmallTaggedValueView {
    u16 value;
    u8 pad_2[0x178E - 2];
    u16 scale;
};

/* D_80075AEA - ovl_11 signed halfword state, loaded with lh by
 * ovl_11_func_8010BBA8 (range-tested against 9 and 0xC9). */
extern s16 D_80075AEA;

/* D_80075AD4 - ovl_11 u16 flag, loaded with lhu (absolute access) by
 * ovl_11_func_8010B1C4, which tests it for zero and passes its address
 * to ovl_11_func_8010B778. */
/* 800D0FB4 accesses fields through +40; 8010C1FC uses the sprite at +78,
 * and 8010B778 clears the complete 0xF0-byte record. */
extern u16 gStageMode2RecordStorage[0xF0 / 2] __asm__("gStageMode2Record");
#define gStageMode2Record (gStageMode2RecordStorage[0])

/* 80120358 walks four 0xE-byte rows, with inclusive signed bounds and
 * five category values. Overlay -G0 supplies no GP ownership witness. */
typedef struct {
    /* 0x00 */ s16 min;
    /* 0x02 */ s16 max;
    /* 0x04 */ s16 values[5];
} Ovl11HeldActionRange;
extern Ovl11HeldActionRange gOvl11HeldActionRangeValues[4];

/* 80120414 reads count-prefixed s16 weight/value pairs and four table pointers. */
extern s16 gOvl11HeldActionCategory1Weights[20];
extern s16 gOvl11HeldActionCategory3Weights[8];
extern s16 gOvl11HeldActionCategory2Weights[18];
extern s16 gOvl11HeldActionCategory4Weights[14];
extern s16 *gOvl11HeldActionDefaultWeightTables[4];
/* Six signed fallback values indexed by the action state's category word. */
extern s16 gOvl11HeldActionFallbackValues[6];
/* Ordinary outcomes advance this counter; the twelfth uses a fallback. */
extern s32 gOvl11HeldActionFallbackCounter;
/* 80120414 reads lh +44BA, lhu +44CA and updates the word at +44F8. */
struct Ovl11WeightedSelectionState {
    u8 pad0[0x44BA];
    s16 mode;
    u8 pad44BC[0x44CA - 0x44BC];
    u16 time;
    u8 pad44CC[0x44F8 - 0x44CC];
    s32 flags;
};

/* D_8012862C - ovl_11 u16 table indexed by the s32 field at +8 of the object
 * passed to ovl_11_func_80120358 (lhu load, s16-scale). Absolute-addressed. */
extern u16 gOvl11HeldActionDelayBounds[6];

/* 8011F9EC records the two highest counter categories and a random row.
 * 8011FB80 distinguishes a category's last count from multiple counts. */
typedef struct {
    /* 0x00 */ s16 primaryCategory;
    /* 0x02 */ s16 secondaryCategory;
    /* 0x04 */ s16 randomTextIndex;
} Ovl11CounterChoiceIndices;
typedef struct {
    /* 0x00 */ s16 textOffset;
    /* 0x02 */ s16 multipleMessage;
    /* 0x04 */ s16 singleMessage;
} Ovl11CounterChoiceTextEntry;
extern Ovl11CounterChoiceIndices gOvl11CounterChoiceIndices;
extern Ovl11CounterChoiceTextEntry gOvl11CounterChoiceCategoryText[6];
extern Ovl11CounterChoiceTextEntry gOvl11CounterChoiceRandomText[];

/* ApplyCounterChoice indexes the same storage as halfword arrays. Keep
 * these existing array-backed views: a struct-member address folds the
 * choice-minus-one into the displacement instead of the original index
 * arithmetic. The random index is also the triplet's separately named +4
 * field; these declarations are views, not additional storage or aliases
 * for retired linker symbols. */
extern s16 gOvl11CounterChoiceRandomIndex;
extern s16 gOvl11CounterChoiceIndexWords[] __asm__("gOvl11CounterChoiceIndices");
extern s16 gOvl11CounterChoiceRandomWords[] __asm__("gOvl11CounterChoiceRandomText");

/* D_80071A70 - ovl_11 s16[6] scan table read by ovl_11_func_8011F9EC with lh
 * over six stride-2 entries (-1 sentinel). Absolute-addressed. */
extern s16 gCounterChoiceCategoryCounts[6];

/* D_80070D02 - ovl_11 s16 selector compared against the min/max bounds by
 * ovl_11_func_80120358 (lh load). Absolute-addressed. */
extern s16 D_80070D02;

/* Slot-sprite cache: two nine-s16 ID rows precede two nine-entry descriptor
 * rows, stride 0x1E. D6628 resets exactly 0x24 ID + 0x21C descriptor bytes.
 * Broader existing views below are retained, not additional owned storage.
 * Descriptor offsets: +0/+2 and +4/+6 are variant UVs; +8/+A and +C/+E
 * dimensions; +10..+16 variant offsets; +18/+1A CLUT coordinates. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ char pad[2];
} Ovl11A04B8Entry;
extern Ovl11A04B8Entry gSlotSpriteEntries[][9];

/* 800D6A94 passes this sprite-data buffer to func_80017200. */
extern u8 D_8006CA10[];

/* ovl_11_func_800F0A58 scans the first two nine-s16 rows with lh. */
extern s16 gSlotSpriteIds[288];
/* D688C: header[5] selects a count-prefixed s16 ID list parallel to sprite offsets. */
extern s16 *D_80126254[];
/* ovl_11:800F0A58 prints this "Error\n" string when the slot ID is absent. */
extern char gOvl11MissingSlotSpriteMessage[];
/* 800F9210 reads and writes the two nine-halfword rows at base +0x254A4. */
typedef struct {
    u8 pad_0[0x254A4];
    s16 slots[2][9];
} Ovl11FarSlotTableView;
/* ovl_11_func_800F0A58 copies a 0x1E-byte record at these far-buffer offsets. */
typedef struct {
    u8 pad_0[0x254C8];
    Ovl11A04B8Entry records[32][9];
} Ovl11FarRecordTableView;
typedef struct {
    u8 pad_0[0x445A];
    Ovl11A04B8Entry record;
} Ovl11ActiveRecordView;

/* ovl_11 four-entry state dispatch table. The original rodata entries point
 * to ovl_11_func_800E3DC8/800E40CC/800E4280/800E4330; the dispatcher
 * preserves the selected handler's return value across its final flag clear. */
extern s32 (*gOvl11ObjectLoadHandlers[4])(void);

/* First 20-entry stride-4 queue: D_8006C838 + 0x49E4. The full-queue
 * path in ovl_11_func_800F27B0 shifts 0x4C bytes within this 0x50-byte span. */
extern u8 gOvl11ImmediateEventQueue[0x50];
/* Adjacent second 20-entry queue selected by ovl_11_func_800F2880. */
extern u8 gOvl11DatedEventQueue[0x50];

/* Object pointer and low flag-word cache written by ovl_11_func_800BF450. */
extern u32 *gOvl11CachedGeometryHit;
extern u32 gOvl11CachedGeometryFlags;

/* ovl_11 reset-run halfwords, initialized by 8011F52C and rewritten by
 * 8011F574. Their signed storage views agree with the matched reset leaf. */
extern s16 gOvl11CounterChoicePhase;
extern s16 gOvl11CounterChoiceSavedSelection;
/* 8011F608 reads three word text addresses at +0/+4/+8. */
extern s32 gOvl11CounterChoiceText[3];
/* 8011F608 stores/loads the selected signed text ID and decrements a timer. */
extern s16 gOvl11CounterChoiceMessage;
extern s16 gOvl11CounterChoiceDelay;
/* 8011F608 reads the signed music ID through D_8007AFF0 + 0x2549C. */
struct Ovl11SelectionMusic {
    u8 pad0[0x2549C];
    s16 music;
};

/* 80112FA8 uses signed word loop counters and a four-word position buffer. */
extern s32 gOvl11AreaGridScanColumn;
extern s32 gOvl11AreaGridScanRow;
extern s32 gOvl11AreaGridProbePosition[4];

/* ovl_11 state buffer: 80112D10 clears its +C ten-byte flags, +16
 * 10x10x10 byte grid, initial halfword and word at +400. */
extern u8 gOvl11AreaGridState[];

/* 80113818 passes this data address or its +0x34 byte offset to func_8001AC10. */
extern u8 gOvl11AreaGridTriggerText[];

/* 80112EC4 indexes the +0x16 byte grid with strides 100/10/1 and updates
 * the +0x400 word; 80113818 increments the initial unsigned halfword.
 * 80113310 reads +2/+4/+6, writes +8/+A and indexes ten flags at +C. */
typedef struct {
    u16 plane;
    s16 probability;
    s16 centerX;
    s16 centerY;
    s16 lastX;
    s16 lastY;
    u8 found[10];
    u8 grid[10][10][10];
    u8 pad3FE[2];
    s32 flags;
} Ovl11GridState;

/* ovl_11:80129FF0 — calendar's 24-byte scratch text span. Number and
 * weekday formatters return an end pointer for the FFFF terminator.
 * Preserve the original first-halfword view; callers pass its address. */
extern s16 gOvl11CalendarNumberText;

/* Clock HUD's original 0x20-byte halfword-text scratch span at 8012A008.
 * The leading glyph is also written directly for the five-minute digit. */
typedef struct {
    s16 leadingGlyph;
    u16 field_2;
    s16 field_4;
    s16 text[13];
} Ovl11ClockHudText;
extern Ovl11ClockHudText gOvl11ClockHudText;

/* 800D0DCC: eight-byte text span, two formatted glyphs for 21-u16@+B2
 * followed by FFFF. Preserve the original first-halfword address view. */
extern s16 gOvl11RecordStatusCountText;

/* E2824: twenty enter slots, live at 3/4/9/14; original 800B96BC..800B970C. */
typedef s32 (*Ovl11SmallRecordEnterHandler)(u16 *);
extern Ovl11SmallRecordEnterHandler gOvl11SmallRecordEnterHandlers[20];

/* E276C calls these twenty handler-address words; E2824 tests for presence. */
extern s32 gOvl11SmallRecordStepHandlers[20];

/* E2AAC: seven rows of four unsigned animation/frame pairs at 80124008. */
typedef struct {
    u16 animation;
    u16 frame;
} Ovl11SmallRecordSpriteDescriptor;
extern Ovl11SmallRecordSpriteDescriptor gOvl11SmallRecordSpriteDescriptors[7][4];

/* E2EA0..E3464: packed keyframe arrays, bounded by the next label/sentinel. */
extern u8 gOvl11SmallRecordHandler0Keyframes[8];
extern u8 gOvl11SmallRecordHandler1Code109Keyframes[16];
extern u8 gOvl11SmallRecordHandler1Code10AKeyframes[12];
extern u8 gOvl11SmallRecordHandler2Code109Keyframes[12];
extern u8 gOvl11SmallRecordHandler2Code10AKeyframes[8];
extern u8 gOvl11SmallRecordHandler14Keyframes[8];
extern u8 gOvl11SmallRecordHandler19Keyframes[8];

/* ovl_27_func_800B80B0 copies 0x1800 bytes from this three-pointer table. */
extern void *D_800C4A40[3];
/* ovl_27_func_800B80B0 uses this address as a 0x1800-byte copy destination. */
extern u8 D_800605E8[0x1800];

/* ovl_27:7FE8 saves the pre-mask work+0xC flags; 7F2C resets them. */
extern s32 gOvl27TitleSavedWorkFlags;
/* ovl_27:823C selects and updates this signed halfword setup stage. */
extern s16 gOvl27TitleSetupStage;
/* ovl_27:845C calls this address; the animation chain stores its next callback. */
extern s32 gOvl27TitleAnimationCallback;
/* ovl_27:7F7C initializes the animation coordinates consumed by 8EA0..90DC. */
extern s16 gOvl27TitleBackdropY;
extern s16 gOvl27TitleBackdropExtent;
extern s16 gOvl27TitlePanelX;
/* 800B845C increments this signed halfword and takes its signed remainder. */
extern s16 gOvl27TitleSelection;
/* 800B845C increments and reduces this signed word modulo 108000. */
extern s32 gOvl27TitleAnimationTicks;
/* 800B845C increments and thresholds this signed word at 1800. */
extern s32 gOvl27TitleIdleTicks;
/* 800B845C dispatches on this signed halfword and stores state numbers. */
extern s16 gOvl27TitleMenuStage;
/* 800B845C sets this halfword when either timeout expires. */
extern s16 gOvl27TitleExitVariant;
/* 800B845C passes this address to the halfword-list consumer 80023030. */
extern u16 D_80071A7C[];

/* ovl_27_func_800B823C increments the s32 at D_8006C838 + 0x4488. */
typedef struct {
    u8 pad_0[0x4488];
    s32 counter;
} Ovl27StateCounterView;

/* 800B8064 increments s32@+0x448C; 800B8DA8 sets s32@+0xE624;
 * 800B8C74 reads the text word at +0xE620 and clears completion. */
typedef struct {
    u8 pad_0[0x448C];
    s32 counter;
    u8 pad_4490[0xA190];
    s32 text;
    s32 completed;
} Ovl19CompletionCounterView;

/* D_800C4A2C - ovl_27 halfword state counter at 0x800C4A2C. Read
 * unsigned to increment and signed to threshold-test; absolute-addressed
 * (overlay build is -G0). */
extern s16 gOvl27EffectSlot0Ticks;
/* 800B9A8C/9CB4/9DBC/9EEC advance the other four ovl_27 effect counters. */
extern s16 gOvl27EffectSlot1Ticks;
extern s16 gOvl27EffectSlot3Ticks;
extern s16 gOvl27EffectSlot4Ticks;
extern s16 gOvl27EffectSlot5Ticks;
/* ovl_27_func_800B9B98 increments with lhu/sh and threshold-tests with lh. */
extern s16 gOvl27EffectSlot2Ticks;

/* Ovl27PollBackdropSpritePoolLoad initializes eight stride-0x30 sources
 * at ovl_27:800C4A80..800C4BD0; slots 0..5 pair with the effect counters.
 * Preserve the existing opaque byte views, not another allocation or type. */
extern u8 gOvl27EffectSlot0Source[];
extern u8 gOvl27EffectSlot1Source[0x30];
extern u8 gOvl27EffectSlot2Source[0x30];
extern u8 gOvl27EffectSlot3Source[0x30];
extern u8 gOvl27EffectSlot4Source[0x30];
extern u8 gOvl27EffectSlot5Source[0x30];
extern u8 gOvl27SpritePoolSource6[0x30];
/* The loader copies the 0xD1C-byte shared sprite header to ovl_27:800C4C00. */
extern u8 gOvl27BackdropSpriteData[0xD1C];

/* D_80075FE4 - ovl_11 2-byte table base scanned at 0xB4 stride (first field
 * u16, lhu) by ovl_11_func_800E1F9C and ovl_11_func_800D2160. Undefined in
 * the overlay (undefined_syms_auto.txt, 2B); the -G0 overlay build addresses
 * it absolutely (split lui/%lo). */
extern u16 gSmallRecordTripletBase;

/* ovl_11:80123754 - signed movement-rank threshold, set by 800D1960 and
 * reset to 600 by 800D12B8; compared against both absolute X/Z distances. */
extern s16 gOvl11MovementRankThreshold;

/* ovl_11:80123A18 - nine u16 movement-cell codes indexed by xRank*3+zRank,
 * followed by one padding halfword in the original 0x14-byte extent. */
extern u16 gOvl11MovementCellRanks[];

/* ovl_11:800DD5B0/800DD6F0 install/step this pair. Renderer 800DCFEC
 * forwards progress/duration to 800DD060's vertex interpolation. Preserve
 * u16 storage and the controllers' explicit signed comparison views. */
typedef struct {
    u8 pad[0x2E];
    u16 progress;
    u16 duration;
} Ovl11SceneBlendStateView;
typedef struct {
    u8 pad[0x28];
    u16 flags;
    u8 pad2[2];
    Ovl11SceneBlendStateView *blend;
} Ovl11SceneBlendRecordView;
/* ovl_11:80129188 / 80129190: private two-record index and phase word;
 * absolute addressing, no storage definition in the controls module. */
extern u8 *gOvl11SceneAnimationPair[2];
extern s32 gOvl11SceneAnimationPairState;

/* D_801295D0 - 18-byte status record copied as a whole aggregate by
 * ovl_11_func_800F0474 (into the D_800749F8 / D_8006C838+0x7AB8 record slots)
 * and by ovl_11_func_800E729C (into D_80075AD8 / D_8007AFBC). It is stored as
 * nine halfwords, so the aggregate copy is emitted as lwl/lwr/swl/swr plus one
 * halfword. Absolute-addressed (lui/%lo) in the overlay build. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
} Ovl11StatusTextRecord;

extern Ovl11StatusTextRecord gOvl11StatusTextScratch;

/* Ovl11CopySlotSpritePieces stages its two MoveImage source rectangles here.
 * Original ovl_11:80128D80, matching the PSY-Q RECT layout. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
} Ovl11SlotSpriteCopyRect;

extern Ovl11SlotSpriteCopyRect gOvl11SlotSpriteCopyRect;

/* D_801248B4 - 8-byte RECT (x=320, y=0, w=128, h=255) copied into a local by
 * ovl_11_func_800E351C and offset before LoadImage; w/h are also read from the
 * global to advance the source pointer by the texture byte size. This is the
 * gapless successor of the D_801247E8 delta table (51 s32 entries).
 * Absolute-addressed (lui/%lo). Layout mirrors the PSY-Q RECT. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
} Ovl11SpriteBlockRect;

extern Ovl11SpriteBlockRect gOvl11SpriteBlockBankRect;

/* D_80129420 - 50-entry s32 array filled by ovl_11_func_800E351C with the
 * per-segment word sums produced by ovl_11_func_800E39B8. Absolute-addressed. */
extern s32 gOvl11SpriteBlockChecksums[50];

/* D_801294E8 - s32 destination base stored by ovl_11_func_800E351C (aligned
 * down to a 4-byte boundary) and handed to memcpy. Absolute-addressed. */
extern s32 gOvl11SpriteBlockScratchBase;

/* D_8006F5F0 - 0xE10-byte main-RAM buffer. ovl_11_func_800F0E00 clears and
 * block-copies 0xE10 bytes into it, then scans it as a 0x24-stride array of
 * u16-terminated records. Absolute-addressed (lui + %lo) from the -G0
 * overlay; the overlay only declares it, so an extern scalar belongs here. */
extern u16 D_8006F5F0;

/* 80129618/1C: installed condition count and evaluation state, shared by
 * the installer, pending evaluator and state predicate; original -G0 accesses. */
extern s32 gOvl11ConditionRecordCount;
extern s32 gOvl11ConditionEvaluationState;

/* D_80128C60 - ovl_11 pointer to a u16 record, cleared and re-pointed by
 * ovl_11_func_800CB730. Absolute-addressed (lui %hi + sw/lw %lo) from the -G0
 * overlay; the overlay only declares it, so an extern pointer belongs here. */
extern u16 *gOvl11ProbedObjectCell;

/* D_80127D90 / D_80127DB0 / D_80127DD0 / D_80127E00 / D_80127E20 /
 * D_80127E40 - ovl_11 keyed-position tables searched by
 * ovl_11_func_80110188. Each 0x10-byte entry is three words copied into the
 * caller's vector (arg2 +0/+4/+8) plus a key word at +0xC matched against
 * arg3. Entry counts are 2/2/3/2/2/2 and the search walks with a 0x10
 * stride. Absolute-addressed from the -G0 overlay. */
typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
    /* 0x0C */ s32 key;
} Ovl11ActionPositionRow;
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode0[2];
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode1[2];
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode2[3];
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode3[2];
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode4[2];
extern Ovl11ActionPositionRow gOvl11ActionPositionsMode5[2];

/* D_80070D08 / D_80070D0A - ovl_11 progress halfwords read with lhu by
 * ovl_11_func_80110188 to choose the alternate table half; also used by
 * other matched ovl_11 leaves. Absolute-addressed from the -G0 overlay. */
extern u16 gTaggedRecordPoolExpanded;
extern u16 gB4RecordPoolExpanded;
/* Default area selection uses work +44D4 as the variant80 offset-table index;
 * 800D0FB4 also reads it with lhu. This is not a normalized boolean. */
extern u16 gAreaIndexedVariant;

/* ovl_11:800CE210/800CE37C capture and interpolate these position endpoints.
 * Absolute-addressed overlay state, not small-data definitions. */
typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
} Ovl11CoordTriple;
extern Ovl11CoordTriple gOvl11PositionTransitionTarget;
extern Ovl11CoordTriple gOvl11PositionTransitionStart;
extern s32 gOvl11PositionTransitionStep;
/* ovl_11:800CE2EC selects one of three signed X/Z pairs by script code. */
extern s16 gOvl11PositionTransitionTargets[3][2];

/* ovl_11:800C401C copies held/pressed pad masks (or substituted masks)
 * here; 800C47F0 rotates direction bits. Object actions can replace/clear
 * them afterward (800CD6F4). Preserve s32 word accesses and absolute -G0
 * addressing; these are not hardware input records. */
/* 800C9424 increments this word and compares it unsigned against 8 or 12. */
extern u32 gOvl11FacingStepSoundDelay;
extern s32 gOvl11PrimaryHeldButtons;
extern s32 gOvl11PrimaryPressedButtons;
/* CD6F4 indexes the four u16 d-pad masks at 801232B4 by saved direction +30. */
extern u16 gOvl11ForcedDirectionButtons[4];
/* 800C6C24 loads this word and scales it by ten for the state timer. */
extern s32 gOvl11ClockMinutePeriodTwice;

/* ovl_11:800C401C alone maintains this latch; 800C47F0 uses it when the
 * state mode neither forces nor suppresses direction-button rotation. */
extern s32 gOvl11PrimaryDirectionRemap;

/* D_80071318 - ovl_11 u16 record table read with lhu at 0x16-byte stride by
 * ovl_11_func_80100898, ovl_11_func_80100A8C and ovl_11_func_80100F9C.
 * Absolute-addressed from the -G0 overlay. */
extern u16 gSelectionRecordTable[11];

/* ovl_11:800BC868 sets this flag; 800BC9AC stops/replays the sequence when
 * nonzero, otherwise only adjusts volume. Absolute-addressed overlay state. */
extern s32 gOvl11AreaSoundRestartNeeded;

/* ovl_11:800BC9AC reads asset IDs/flags through work+25388, selector at
 * state+E4C8 and sequence ID at work+2549C. Signedness follows lh/lhu. */
typedef struct {
    s16 primaryId;
    s16 secondaryId;
    u8 reserved4[4];
    u16 flags;
} AreaSoundRecord;
typedef struct {
    char reserved[0xE4C8];
    s16 selector;
} AreaSoundStatus;
typedef struct {
    char reserved0[0x25388];
    AreaSoundRecord *record;
    char reserved1[0x2549C - 0x2538C];
    s16 sequenceId;
} AreaSoundWork;

/* D_80129418 - ovl_11 private s16 state word in the overlay data segment.
 * ovl_11_func_800E3CDC stores the computed frame index here (sh %lo) and
 * ovl_11_func_800E54F8 loads it (lh) to pass to ovl_11_func_800E36CC.
 * Absolute-addressed from -G0 overlay code; the TU only declares it, so a
 * scalar extern keeps the base name. */
extern s16 gOvl11SpriteBankVramXOffset;

/* ovl_11_func_800D3574 selects byte offsets into these sprite-data banks. */
extern s32 gSeasonalObjectSpriteBankBase;
extern s32 gFixedIdSpriteBankBase;
/* ovl_11_func_800D3574 loads word offsets at +4 through +14 from these pointers. */
/* 800D3574 indexes byte-offset arrays selected by calendar season or fixed ID. */
extern s32 *gOvl11ObjectSpriteOffsetsSeason0;
extern s32 *gOvl11ObjectSpriteOffsetsSeason1;
extern s32 *gOvl11ObjectSpriteOffsetsSeason2;
extern s32 *gOvl11ObjectSpriteOffsetsSeason3;
extern s32 *gOvl11FixedIdSpriteOffsets;
/* ovl_11_func_800D3574 reads the engine mode with lw, unlike halfword users. */
extern s32 D_80070CF8_word __asm__("gClockHour");

/* D_8009CBF8: ovl_11_func_800C2884 reads counted words; other users copy bytes. */
extern s32 gAreaSpriteWorkBase;
/* D_8008F7F8 word view: the helper reads u32 counts/items; existing users
 * also address this bank through signed and unsigned scalar views. */
extern u32 D_8008F7F8_words[] __asm__("D_8008F7F8");
/* ovl_11 data contains 50 pointers to counted word-offset lists. */
extern s32 *D_80125DC0[50];
/* ovl_11_func_800BD6A8 reads adjacent ROM boundary words at index*4 and +4. */
extern s32 gOvl11CalendarSpriteBoundaries[];
/* ovl_11_func_800BFD1C loads this pointer and indexes 0xC-byte entries. */
extern s32 *gOvl11AmbientTintTable;
/* BFEA4/C044C select these four hour-class RGB triples (12 Q12 words). */
extern s32 gOvl11DefaultAmbientTintTable[12];
extern s32 gOvl11WeatherAmbientTintTable[12];
/* C044C initializes this 0x30-byte sprite source from the shared data. */
extern struct SpriteSourceData gOvl11WeatherSpriteSource;
/* ovl_11:800C28CC publishes the group and one-based item BEFORE group remap. */
extern s32 gOvl11ScriptSpriteGroup;
extern s32 gOvl11ScriptSpriteItemIndex1;

/* ovl_11_func_800C32C8: lh/lhu at +44C4, lhu +44CE and lh +E778/+E77C. */
struct Ovl11AnimationState {
    char pad_0[0x44C4];
    s16 hour;
    char pad_44C6[8];
    u16 field_44CE;
    char pad_44D0[0xE778 - 0x44D0];
    s16 selected;
    char pad_E77A[2];
    s16 mode;
};

/* ovl_11_func_800C28CC: halfword selectors at the witnessed displacements. */
struct Ovl11SpriteSelectors {
    char pad0[0x44BE];
    u16 day;
    char pad44C0[4];
    u16 hour;
    char pad44C6[0xE776 - 0x44C6];
    s16 season;
    char padE778[4];
    s16 variant;
};
/* Selection-message request: lw/sw phase and latched category, private to
 * selection_message.c; absolute-addressed in the -G0 overlay. */
extern s32 gOvl11SelectionMessagePhase;
extern s32 gOvl11SelectionMessageCategory;

/* 800DBFEC passes these halfword triplets to the matched 800DB00C. */
extern struct Ovl11SceneCoordinates gLoadedAreaClampFirstCorner;
extern struct Ovl11SceneCoordinates gLoadedAreaClampSecondCorner;

/* 800DBFEC: the work pointer at +0x25388 has lh +2 and lw +4. */
struct Ovl11SceneHeader {
    s16 field_0;
    s16 kind;
    s32 id;
    u16 flags;
    s16 music; /* 800DC560 reads/writes +0xA and reads signed +0xC. */
    s16 sound;
};

/* ovl_11_func_800C28CC: lw at D_8007AFF0+0x25388 points to a halfword header. */
struct Ovl11SpriteWork {
    char pad0[0x25388];
    s16 *header;
};

/* ovl_11_func_800BC868: lw pointer at +0x25388, sw words at +0x25498/+0x2549C. */
struct Ovl11SoundWork {
    char pad0[0x25388];
    s16 *header;
    char pad2538C[0x25498 - 0x2538C];
    s32 previous;
    s32 selected;
};

/* ovl_11_func_800BCB5C: lw flags at +0xC/+0x4450, lh +0x44BA/+0x44C0, and lh/lhu +0xE4C8. */
struct Ovl11BCB5CState {
    char pad0[0xC];
    s32 flags0C;
    char pad10[0x4450 - 0x10];
    s32 flags4450;
    char pad4454[0x44BA - 0x4454];
    s16 field44BA;
    char pad44BC[4];
    s16 field44C0;
    char pad44C2[0xE4C8 - 0x44C2];
    s16 fieldE4C8;
};

/* Card service uses the shared u16 retry counter above, with signed limits. */
/* Ovl15BuildCardFilename writes this 0x20-byte object via strcpy. */
extern char Ovl15CardFilename[0x20];

/* 800C907C reads lhu +0x92BE and updates lw/sw flags at +0x92D0. */
struct Ovl11C907CStateView {
    char pad0[0x92BE];
    u16 field_92BE;
    char pad92C0[0x10];
    s32 field_92D0;
};
/* ovl_11_func_800D2764: lh compares these far fields using each global base. */
struct Ovl11D2764StateView {
    char pad_0[0x25476];
    s16 field_25476;
};
struct Ovl11D2764StageView {
    char pad_0[0x91DC];
    s16 field_91DC;
    char pad_91DE[0x92CC - 0x91DE];
    s16 field_92CC;
};
/* Ovl11DispatchStageMode clears these halfword counters; 800CE96C sets 300. */
extern s16 gOvl11Mode0DispatchCounter;
extern s16 gOvl11Mode1DispatchCounter;
extern s16 gOvl11Mode2DispatchCounter;
/* 800DAFD4/800DB00C/800DB054 write/copy/clamp six s16 coordinates, +6 unused. */
extern struct Ovl11SceneCoordinates gOvl11CoordinateClampBounds;

/* 801335F8: Ovl15DrawSlotBanks draws bank-adjusted Giving and Getting (+0xE) bank labels. */
extern u8 gLivestockTransferDirectionTextAnchor[];
/* 80132638: Ovl15CardTransferConfirmSlots draws bank-adjusted Trading livestock / Is that OK. */
extern u8 gCardTransferLivestockConfirmTextAnchor[];
/* 801320A8: Ovl15CardTransferValidateCapacity draws bank-adjusted livestock-transfer instructions. */
extern u8 gLivestockTransferInstructionsTextAnchor[];
/* ovl_15_func_801359FC stores the alternate record-pool byte pointer. */
extern u8 *gOvl15SaveWorkBase;
/* 80135380 restores these word-aligned save blocks (0x4C/0x24/0x34/0x18 bytes). */
extern s32 D_8007AD00[0x13];
/* ovl_11:800BFF74/800C0010 use eight s32 fields; the screen renderer shifts
 * RGB by 12. ovl_15 save/restore copies 0x24 bytes at save offset 0x6308,
 * establishing a trailing word whose purpose is not yet known. */
struct ScreenColorRampState {
    s32 rgbQ12[3];
    s32 stepQ12[3];
    s32 elapsedUpdates;
    s32 durationUpdates;
    u8 unknown_20[4];
};
extern struct ScreenColorRampState gScreenColorRamp;
extern s32 D_8007AFA4[13];
extern s32 D_8007AFD8[6];
/* 80135380 selects this save buffer and reads through byte offset 0x6377. */
extern u8 gOvl15SavePayload[];

/* 80135380's aligned aggregate transfers establish these save-block extents. */
typedef struct { s32 words[0xF0 / 4]; } Ovl15SaveBlockF0;
typedef struct { s32 words[0x30 / 4]; } Ovl15SaveBlock30;
typedef struct { s32 words[0x1F34 / 4]; } Ovl15SaveBlock1F34;
typedef struct { s32 words[0x18 / 4]; } Ovl15SaveBlock18;
typedef struct { s32 words[0x14 / 4]; } Ovl15SaveBlock14;
typedef struct { s32 words[8 / 4]; } Ovl15SaveBlock8;
typedef struct { s32 words[0x94 / 4]; } Ovl15SaveBlock94;
typedef struct { s32 words[0xCFC / 4]; } Ovl15SaveBlockCFC;
typedef struct { s32 words[0x3FC / 4]; } Ovl15SaveBlock3FC;
typedef struct { s32 words[0x2328 / 4]; } Ovl15SaveBlock2328;
typedef struct { s32 words[0x188 / 4]; } Ovl15SaveBlock188;
typedef struct { s32 words[0x40 / 4]; } Ovl15SaveBlock40;
typedef struct { s32 words[0x6C8 / 4]; } Ovl15SaveBlock6C8;
typedef struct { s32 words[0xC / 4]; } Ovl15SaveBlockC;
typedef struct { s32 words[0x3C / 4]; } Ovl15SaveBlock3C;
typedef struct { s32 words[0x108 / 4]; } Ovl15SaveBlock108;
typedef struct { s32 words[0x24 / 4]; } Ovl15SaveBlock24;
typedef struct { s32 words[0x34 / 4]; } Ovl15SaveBlock34;
/* 80134C48 copies 0x74 aligned bytes from D_8007A5C4 into the save buffer. */
typedef struct { s32 words[0x74 / 4]; } Ovl15SaveBlock74;
extern s32 D_8007A5C4[0x74 / 4];
/* 80134C48's scalar word stores in the serialized save buffer. */
typedef struct {
    u8 pad_0[0x2274];
    s32 field_2274;
    s32 field_2278;
    s32 field_227C;
    s32 field_2280;
    u8 pad_2284[0x2320 - 0x2284];
    s32 field_2320;
    s32 field_2324;
    u8 pad_2328[0x2350 - 0x2328];
    s32 field_2350;
    s32 field_2354;
    s32 field_2358;
    s32 field_235C;
    s32 field_2360;
    u8 pad_2364[0x6088 - 0x2364];
    s32 field_6088;
    u8 pad_608C[0x6360 - 0x608C];
    s32 field_6360;
    s32 field_6364;
    s32 field_6368;
} Ovl15SaveBufferWords;
/* 80134C48 clears words in ten 0xB4-byte and twenty 0xB8-byte save records. */
typedef struct {
    s32 state;
    u8 pad_4[0xB4 - 4];
} Ovl15SaveRecordB4;
typedef struct {
    s32 state;
    u8 pad_4[0xB8 - 4];
} Ovl15SaveRecordB8;
typedef struct {
    u8 pad_0[0x8C];
    Ovl15SaveRecordB4 records_B4[10];
    Ovl15SaveRecordB8 records_B8[20];
} Ovl15SaveRecordStates;
/* 80135380 loads the two destination pointers through the D_8006C838 base. */
typedef struct {
    u8 pad_0[0xDDCC];
    Ovl15SaveBlockF0 *block_DDCC;
    u8 pad_DDD0[4];
    Ovl15SaveBlock30 *block_DDD4;
} Ovl15SaveRestoreRuntime;
/* 80135380 copies four unaligned bytes and five u16 fields at stride 14. */
typedef struct { u8 bytes[4]; } Ovl15SaveBytes4;
typedef struct {
    Ovl15SaveBytes4 bytes;
    u16 field_4;
    u16 field_6;
    u16 field_8;
    u16 field_A;
    u16 field_C;
} Ovl15SavedRecord;
typedef struct {
    u8 pad_0[0x1F34];
    Ovl15SavedRecord records[37];
} Ovl15SavedRecordList;
/* 80133808 adds the language offset and reads four halfwords from this text. */
extern u8 D_800535C6[];
/* Address-only text anchor in 80133808; s32 view also witnessed by 800FD194. */
extern s32 D_80053604;
/* 80133808 indexes one unsigned byte with D_8013759E. */
extern u8 D_80137658[];
/* 80133808 and 80134134 pass this overlay buffer to func_80015EE8. */
/* 801456A0 is gOvl15MenuSpriteSources[1], not separate storage. */
/* ovl_15 slot state: sh updates/clears in 80133B28/80134724; signed comparisons use lh. */
extern u16 gOvl15SlotFirstVisible;
extern u16 gOvl15SlotCursor;
extern u16 gOvl15SlotMarkCount;
/* Ovl15ResetSlotSelection clears this 0x1568-byte scratch region. */
extern u8 gOvl15SlotRecordScratch[];
/* 801342A0 fills 15 signed glyph halfwords and formats the row detail text. */
extern s16 gOvl15SlotDetailText[15];
/* ovl_15: mark/unmark at 33F4C/34000 write kind/index; 32C1C stores
 * staging byte addresses at +4 for 331C4. Preserve the signed word ABI. */
typedef struct {
    s16 kind;                 /* -1 empty, 0 small, 1/2 large */
    s16 recordIndex;
    s32 stagedAddress;
} Ovl15Slot;
/* Alternate view preserves the existing local views in other ovl_15 TUs. */
extern Ovl15Slot Ovl15Slots[2][5] __asm__("D_80140F90");
/* 80132C1C packs up to five 0xB4/0xB8-byte copies into this byte buffer. */
extern u8 gOvl15StagedSlotRecords[];
/* 80132C1C clears record words at runtime base +0x7B40/+0x8248. */
typedef struct {
    u8 pad_0[0x7AB4];
    Ovl15SaveRecordStates pool;
} Ovl15TransferRuntimeStates;
/* 80132C1C transfers 0xB8 bytes with runtime alignment checks. */
typedef struct { u8 bytes[0xB8]; } Ovl15TransferBytesB8;
/* Runtime copies in 80132C1C: ten 0xB4-byte records then twenty 0xB8-byte records. */
typedef struct {
    u8 pad_0[0x7AB4];
    u8 records_B4[10][0xB4];
    Ovl15TransferBytesB8 records_B8[20];
} Ovl15TransferRuntimeRecords;

/* ovl_11_func_80107044: 4-byte stride table; original data are +/-0x420. */
extern s32 gOvl11RotationHoldAngles[4]; /* StepSpriteRotation: per-direction hold yaw. */

/* ovl_30_func_80133218 reads and writes signed halfwords at row*10 + column*2. */
extern s16 gOvl11ItemActionTierValues[5][5];
/* ovl_30_func_80133218 passes these original NUL-terminated rodata strings to FntPrint. */
extern char gOvl30FlagCursorMarker[];
extern char gOvl30FlagIdleMarker[];
extern char gOvl30EditorNewlines[];
extern char gOvl30ToolIndexFormat[];
extern char gOvl30ToolParameterFormat[];
/* Ovl30EditItemFields passes these original format strings to FntPrint. */
extern char gOvl30ItemIndexFormat[];
extern char gOvl30ItemBuyFormat[];
extern char gOvl30ItemSellFormat[];
extern char gOvl30ItemLifeFormat[];
extern char gOvl30ItemTiredFormat[];
/* ovl_30_func_80132F18 reads/writes word-sized item and field selectors. */
extern s32 gOvl30ItemIndex;
extern s32 gOvl30ItemFieldIndex;
/* ovl_30_func_80133218 updates these word-sized selectors within 0..4. */
extern s32 gOvl30ToolIndex;
extern s32 gOvl30ToolParameterIndex;
/* ovl_30 data contains five string pointers, indexed by gOvl30ToolIndex. */
extern char *gOvl30ToolNames[5];

/* Selection handlers store callback addresses as words; the existing
 * function-pointer view below addresses the same word for indirect calls. */
extern s32 gOvl11SelectionHandler;
/* 80102268/80103714 read and store this phase with signed-byte operations;
 * the pre-existing unsigned view retains the other controllers' accesses. */
extern s8 gOvl11SelectionModePhase;
/* 80100F9C copies the selected record's unsigned required-flag halfword. */
extern u16 gOvl11SelectionWorkFlags;
/* 80102844 forms byte addresses into the same item-list storage as _slots. */
extern u8 D_80070D42;

/* 80100FFC loads the unlocked-menu bitset with lbu. */
extern u8 gSelectionUnlockedFlags;
/* 80100FFC tests this menu-origin byte with lbu; unsigned view preserves legacy declarations. */
extern u8 D_801273E5_origin __asm__("gOvl11SelectionOriginVariant");
/* 80100FFC clears this halfword before installing the next handler. */
extern s16 D_801273F0;

/* 801014F8 reads the signed menu index with lb and updates it with sb. */
extern s8 D_801273E9_selection __asm__("D_801273E9");
/* 801014F8 stores the state byte to 3 on cancellation. */
extern u8 D_801273E4_state __asm__("gOvl11SelectionModePhase");
/* 801014F8 adds the text-bank byte offset to this base and +0xC/0x16/0x26/0x38. */
extern u8 D_8005399A[];
/* 80101B9C adds D_80054BBC[0] to this text base and its +0x16/+0x28/+0x36 labels. */
extern u8 D_800539E8[];

/* 801000D4/80100A34/80102BCC share this 0x20-stride selection catalog;
 * original row 95 is the 0xE2 sentinel, ending exactly at 8005175C. */
extern struct Ovl11IdAdjustmentObject gSelectionCatalog[96];

/* 80101814 tests lhu flag bits; the unsigned view preserves legacy signed declarations. */
extern u16 D_801273EE_flags __asm__("gOvl11SelectionWorkFlags");
/* 80102A10 walks eight lhu work IDs and passes each nonzero ID to release. */
extern u16 gOvl11SelectionWorkIds[8];
/* 80102F14 walks eight lhu IDs; unsigned view preserves legacy signed declarations. */
extern u16 D_8012CF00_ids[8] __asm__("gOvl11SelectionWorkIds");
/* 801031E4 compares those IDs against one four-entry row selected by 0x181..0x184. */
extern u16 gOvl11RequiredIdClasses[][4];
/* 80101814 walks seven consecutive u16 IDs; 801014A4 also indexes this table with lhu. */
extern u16 gOvl11SelectionFlagSlotIds[];

/* 800F5944 loads signed halfwords from the original four-entry data table. */
/* ovl_11:800F5944 adds these offsets to the selected record's y. */
extern s16 gOvl11RecordSpriteYOffsets[4];
/* 800F5944 indexes that table with this absolute-addressed word. */
/* ovl_11:800F4994 arms index 3; 800F4A58 counts down to zero. */
extern s32 gOvl11RecordSpriteYOffsetIndex;

/* F4994 reads the current record pointer at +DDD4; its signed selector
 * and coordinate view is shared with its callers through game_types.h. */
typedef struct {
    char pad0[0xDDD4];
    struct Ovl11CurrentRecordBoundsEntry *current;
} Ovl11CurrentRecordBoundsWork;

/* F3FF4 reads lh season +44BA, lh mode-1 selector +91DC and lw +DDD4.
 * Record+0 is read as a signed ID/state code here; the publishers' unsigned
 * state store and the general dispatch-record ID view retain their own ABI. */
typedef struct {
    char pad0[0x44BA];
    s16 season;
    char pad44BC[0x91DC - 0x44BC];
    s16 mode1Selector;
    char pad91DE[0xDDD4 - 0x91DE];
    s16 *currentRecordState;
} Ovl11RecordRowGateWork;

/* F4CAC/F4CEC publish a partial record view: state +0, coordinates +8/C/10;
 * records are 0x18 apart, but this original view covers only 0x14 bytes. */
typedef struct {
    u16 state;
    u16 kind;
    s32 word4;
    s32 x;
    s32 y;
    s32 z;
} Ovl11PublishedPoolRecord;
extern Ovl11PublishedPoolRecord *gOvl11PublishedPoolRecord;

/* F4FC8 clears halfword flag bit 0 in the next 0x18-byte record. */
typedef struct {
    char pad0[4];
    u16 flags;
    char pad6[0x18 - 6];
} Ovl11PoolCompletionEntry;
/* F4A58 reads word flags +C and current-record flags +4 through +DDD4. */
typedef struct {
    char pad0[0x0C];
    s32 flags;
    char pad10[0xDDD4 - 0x10];
    Ovl11PoolCompletionEntry *current;
} Ovl11CurrentRecordSpriteWork;
/* F4FC8's shared-work view; keep the independent entry halfword signedness. */
typedef struct {
    char pad0[0x44F8];
    s32 flags;
    char pad44FC[0xDDD4 - 0x44FC];
    Ovl11PoolCompletionEntry *current;
    char padDDD8[0xE650 - 0xDDD8];
    u8 completion;
} Ovl11PoolCompletionWork;
/* ovl_11:800F5888 projection scratch, original 80129620/80129628. */
extern s32 gOvl11RecordSpriteScreen[2];
extern s16 gOvl11RecordSpritePosition[4];

/* 800F5944: header pointer at +0x25388 and lhu selector at +0x25476. */
struct Ovl11DispatchWork {
    char pad0[0x25388];
    s16 *header;
    char pad2538C[0x25476 - 0x2538C];
    u16 selector;
};

/* 800F5160 indexes 6-byte rows and reads three signed halfwords. */
extern u8 gOvl11RecordRowValueTriples[];

/* 800F5740 matches record +2 against these 16-byte coordinate rules.
 * The selector at work+44CE chooses 2 default rules or 10 alternatives. */
struct Ovl11RecordCoordinateRule {
    s32 x;
    s32 y;
    s32 z;
    s32 kind;
};
extern struct Ovl11RecordCoordinateRule gOvl11RecordCoordinateRulesDefault[2];
extern struct Ovl11RecordCoordinateRule gOvl11RecordCoordinateRules1[10];
extern struct Ovl11RecordCoordinateRule gOvl11RecordCoordinateRules2[10];

/* 800F5404 reads status flags, two byte lists and signed counts from this base. */
struct Ovl11FlagRefreshState {
    u8 pad0[0x44CC];
    u8 flags;
    u8 pad44CD[0x44F8 - 0x44CD];
    s32 status;
    u8 pad44FC[0x4AC0 - 0x44FC];
    u8 enabled[22];
    u8 enabled2[22];
    u8 pad4AEC[0x92D0 - 0x4AEC];
    s32 status2;
    u8 pad92D4[0xDD8C - 0x92D4];
    struct Ovl11DispatchRecord *lists[4];
    u8 padDD9C[0xDDD8 - 0xDD9C];
    s16 counts[4];
};
/* 800F5210 starts its cursor at record+2 (kind, then flags), stride 0x18.
 * Keep the signed kind read and shifted partial view, not a second pool. */
typedef struct {
    s16 kind;
    u16 flags;
    u8 pad4[0x18 - 4];
} Ovl11DispatchGateCursor;
/* 800F5210: lh season, lhu variant, lw derived flags and pool slot 10. */
struct Ovl11DispatchGateWork {
    u8 pad0[0x44BA];
    s16 season;
    u8 pad44BC[0x44D4 - 0x44BC];
    u16 indexedVariant;
    u8 pad44D6[0x51C0 - 0x44D6];
    s32 derivedFlags;
    u8 pad51C4[0xDDB4 - 0x51C4];
    Ovl11DispatchGateCursor *records;
};
/* 800F5944: pointer table, two fixed lists and lh counts through D_8006C838. */
struct Ovl11DispatchState {
    char pad0[0xDD8C];
    struct Ovl11DispatchRecord *lists[16];
    struct Ovl11DispatchRecord *list10;
    s32 padDDD0;
    struct Ovl11DispatchRecord *fixed;
    s16 counts[16];
};

/* ovl_25 800BAC9C copies 0x13DC bytes here and uses the base as a sprite header. */
extern u8 gOvl25SharedSpriteData[];
/* ovl_25 800BAC9C initializes this 0x30-byte sprite source from D_800BCDD4. */
extern u8 D_800C021C[];

/* ovl_17:800BB0C4 draws this halfword separator text {0x66, -1} twice. */
extern s16 gOvl17TimerSeparatorText[2];
/* ovl_17 800BB0C4 increments this word and uses unsigned constant division. */
extern u32 gOvl17TimerTicks;

/* 800CB114 indexes signed halfword pairs with 2 * direction + arg2. */
extern s16 Ovl11CollisionOffsets[] __asm__("D_801230B0");
/* 800C9938 indexes signed X/Z pairs by facing; CB114 shares the first table. */
typedef struct {
    s16 x;
    s16 z;
} Ovl11DirectionOffset;
extern Ovl11DirectionOffset gOvl11DirectionOffsetsMode4[];
extern Ovl11DirectionOffset gOvl11DirectionOffsetsFlag100[];
/* 800C498C reads signed halfword pairs indexed by u16 direction * 4. */
extern s16 gOvl11FacingProbeOffsets[][2];
/* 800CB114 uses lw/sw and unsigned comparison for the saturating counter. */
/* CB114 counts qualifying hits to 51; primary_object_update resets on exit. */
extern u32 gOvl11InteractionDispatchCount;

/* 800C580C uses the D_8006C838 base for u16 counters and the +DDD4 record pointer. */
struct Ovl11C580CState {
    u8 pad0[0x44DE];
    u16 count;
    u8 pad44E0[0x51F0 - 0x44E0];
    u16 selected;
    u8 pad51F2[0x5254 - 0x51F2];
    u16 fallback;
    u8 pad5256[0xDDD4 - 0x5256];
    Ovl11CC7CCSpawn *record;
};

/* 800C580C reads the header at +25388 and writes the word at +254A0. */
struct Ovl11C580CWork {
    u8 pad0[0x25388];
    s16 *header;
    u8 pad2538C[0x254A0 - 0x2538C];
    s32 mode;
};

/* 800C580C indexes u16 slots at 8012323C and s32 delays at 80123244. */
extern u16 gOvl11ItemKindSlots[4];
extern s32 gOvl11ActionPhaseDelays[3];

/* ovl_11 CD0F4: ten signed actions at 80123290; +2 retains its scalar anchor. */
extern s16 gOvl11InteractionHitActions[];
extern s16 gOvl11InteractionHitSecondaryAction;
/* 800CD0F4 dispatches an object pointer and tests this absolute flag word. */
extern void *gOvl11InteractionHitRecordView __asm__("gOvl11InteractionHitRecord");
/* 800CB114 publishes an opaque pointer; legacy readers still have private
 * record-pointer declarations, so select this direct view explicitly. */
#ifdef OVL11_COLLISION_OUTPUT_VIEW
extern void *gOvl11InteractionHitRecord;
#endif
/* 800CB114 publishes the interaction probe's record/category result. */
extern s32 gOvl11InteractionHitFlags;
/* 800C79F8 saves the raw address returned by Ovl11FindFlaggedVolume here. */
extern s32 gOvl11LastFlaggedVolumeAddress;
/* Existing publisher spelling preserves its original address-register order. */
extern s32 Ovl11CollisionFlags __asm__("gOvl11InteractionHitFlags");
/* C401C resets; C523C/C541C add 1 << (power/20), retaining u16 wrap. */
extern u16 gOvl11PrimaryFootprintPower;
/* 800C907C saves the dispatched object pointer here and reads its u16 +0x22. */
extern void *Ovl11SavedInteractionObject;
/* 800C498C/800CAADC cache the surface polygon word; action services test its flags. */
extern s32 gOvl11PrimarySurfaceFlags;
/* 800C9C60 clears these three absolute-addressed words with sw. */
extern s32 gOvl11MotionHoldCount;
extern s32 gOvl11ItemActionChargeFrames;
/* C401C counts zero dispatch results through 16; C9C60 resets it. */
extern s32 gOvl11PrimaryNoActionTicks;
/* 800CD0F4 saturating-increments the signed halfword at D_8006C838+0x5240. */
struct Ovl11ActionCounter {
    u8 pad_0[0x5240];
    s16 count;
};

/* 800DBC4C increments 0x80 and 8 byte counters through lbu/sb loops. */
extern u8 gTrackedRecordCounters[0x80];
extern u8 gSecondaryRecordCounters[8];

/* 800DBC4C reads lh at +0x44BA/+0x44BC and lhu at +0xE4C8. */
struct Ovl11TimerState {
    u8 pad_0[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
    u8 pad_44BE[0xE4C8 - 0x44BE];
    u16 field_E4C8;
};

/* ovl_15_func_80136C70 indexes three 4-byte terminated text labels. */
extern u32 gOvl15SaveSlotLabels[3];
/* Ovl15DrawSaveSlotSummary draws bank-adjusted Diary <name> and <month> of <season> <day> Year
 * text bases; the date anchor's +0x1C string is the alternate farm line. */
extern u8 gSaveSlotDiaryTextAnchor[];
extern u8 gSaveSlotDateTextAnchor[];
/* ovl_15_func_80136C70 clears 0x28 bytes and formats halfword text here. */
extern s16 gOvl15MenuTextScratch[20];

/* 800DEB00 reads lh +44BA/+44BC, updates the word at +44F8 and scans 22 bytes at +4AC0. */
typedef struct {
    u8 pad0[0x44BA];
    s16 field_44BA;
    s16 field_44BC;
    u8 pad44BE[0x44F8 - 0x44BE];
    s32 field_44F8;
    u8 pad44FC[0x4AC0 - 0x44FC];
    u8 field_4AC0[22];
} Ovl11Deb00State;

/* 8012F6C0: Ovl15CardWriteConfirmFormat step 1 draws bank-adjusted Want to format. */
extern u8 gCardFormatConfirmTextAnchor[];
extern u8 D_80053114[];
/* 8012F86C, 8012FA00 and 8012FA70 respectively relocate these write-menu prompts. */
extern u8 D_800531B8[];
extern u8 D_8005321A[];
extern u8 D_80053294[];
/* 80130048: Ovl15CardWriteConfirmExit draws bank-adjusted Aborting save / Is that OK. */
extern u8 gCardSaveAbortConfirmTextAnchor[];
extern u8 D_800534DC[];
/* 8012FAE0 selects nine word offsets in three 12-byte rows. */
extern u32 gOvl15CardIconFrameOffsets[3][3];
/* 8012E15C publishes this pointer; 80137300 loads archive 0x10 here and
 * 8012FAE0 reads its card-icon palette and frames. */
extern u8 *gOvl15CardIconResourceBuffer;
/* 8012FAE0 places three 0x80-byte card-icon frames at transfer-buffer
 * offsets 0x80, 0x100 and 0x180; these are not separate allocations. */
/* 8012FAE0 copies a 32-byte under-aligned palette as one aggregate. */
typedef struct { u8 bytes[0x20]; } Ovl15IconPalette;
/* 8012FAE0 writes the SC header, clears +44..+5F and copies a palette at +60. */
typedef struct {
    s16 magic;
    s8 type;
    s8 blocks;
    char title[0x40];
    u8 reserved[0x1C];
    Ovl15IconPalette palette;
} Ovl15CardHeaderBlock;
extern Ovl15CardHeaderBlock Ovl15CardHeader __asm__("D_80140F10");
/* 80134770 concatenates these Shift-JIS title prefix/year/space fragments. */
extern char gOvl15CardTitlePrefix[];
extern char gOvl15CardTitleYearLabel[];
extern char gOvl15CardTitleSpace[];
/* 80134890 copies the Shift-JIS zero glyph and adds to its second byte. */
extern u8 gOvl15CardTitleZeroGlyph[];
/* 80134770 and its two helpers use these non-reentrant string buffers. */
extern char gOvl15CardTitlePart[];
extern u8 gOvl15CardTitleGlyphScratch[];
/* 801349C8 clamps the season to 0..3 before indexing these four strings. */
extern char *gOvl15CardTitleSeasonNames[];
/* 8012FAE0 adds the shared text-bank offset before func_8001AC10. */
extern u8 D_8005346A[];

/* ovl_11_func_8011D35C reads/writes this selection state with lh/sh. */
extern s16 gOvl11WideChoiceSelection;

/* ovl_11:800DF010 dispatches this 20-entry action bank; original table
 * relocations target the DE9C8..DFC44 record-handler unit. */
extern s32 (*gOvl11Code160RequestHandlers[20])(void *);
/* ovl_11 800DEF30 dispatches the 20-word table; matched 800DFC44/800DFCFC take one object and return s32. */
extern s32 (*gOvl11Code160StepHandlers[20])(void *);

/* 8011FFA8 reads/writes the three selection words at +0/+4/+8. */
extern s32 gOvl11HeldActionState[3];

/* 800DC148 scans ten 0xB4-byte records, reading u16 +0/+AE. */
struct Ovl11EventDispatchRecord {
    u16 type;
    u8 pad_2[0xAE - 2];
    u16 active;
    u8 pad_B0[4];
};
/* 800DC148 reads lw +44C0 and updates lw/sw +44F8 through this base. */
struct Ovl11EventDispatchState {
    u8 pad_0[0x44C0];
    s32 mode;
    u8 pad_44C4[0x44F8 - 0x44C4];
    s32 flags;
    u8 pad_44FC[0x7AB4 - 0x44FC];
    struct Ovl11EventDispatchRecord records[10];
};

/* 800D79E8 uses these signed word counters for the 25-by-45 scan. */
extern s32 gOvl11MapGridScanColumn;
extern s32 gOvl11MapGridScanRow;
/* 800D79E8 passes this position to 800BDB88 and steps words +0/+8. */
extern s32 gOvl11MapGridProbePosition[4];

/* 8011DD90 reads signed item IDs; 8011DEBC indexes u16 threshold/price tables. */
typedef struct {
    u8 pad0[0x524C];
    s16 selectedItemId;
    s16 selectedAux;
    u8 pad5250[2];
    s16 materialItemId;
    s16 materialAux;
} Ovl11UpgradeSelectionView;
extern u16 gOvl11UpgradeThresholds[];
extern u16 gOvl11UpgradePrices[];

/* 8011DF4C uses signed word counters for a seven-by-seven scan. */
extern s32 gOvl11Mode6GridScanColumn;
extern s32 gOvl11Mode6GridScanRow;
/* 8011DF4C passes XYZ to Ovl11ResolveSurfaceHeight and steps +0/+8; 16-byte span. */
extern s32 gOvl11Mode6GridProbePosition[4];

/* 800F2400 reads/clears these word flags; 800F24B0/800F2724 set them. */
extern s32 gOvl11EventMessageActive;
extern s32 gOvl11EventMessageLatched;

/* 800CD9A0 publishes BDCE8's record+10 result; 800BBCB8 consumes selectionId. */
typedef struct {
    u16 flags;
    u16 selectionId;
    char pad4[4];
    s16 direction;
    s16 actionCode;
} Ovl11InteractionVolumeSelection;
extern Ovl11InteractionVolumeSelection *gOvl11SelectedInteractionVolume;
/* ovl_11:800BBCB8 saves the descriptor index passed to the BD374 reader. */
extern s32 gOvl11PendingAreaReadIndex;
/* ovl_11:800BBCB8 gates effect 0x4C after the selection's fade completes. */
extern s32 gOvl11AreaSoundEffectPending;
/* ovl_11:800BBCB8 marks/awaits an archive read; 800BCD28 clears the flag. */
extern s32 gOvl11AreaReadStarted;
/* ovl_11:800BBC34 indexes eight callback words at 80121780 by scene phase. */
extern s32 gOvl11ScenePhaseCallbacks[8];

/* 800BBCB8: shared flags, counter, direction and mode through the status base. */
typedef struct {
    char pad0[0xC];
    s32 flags;
    char pad10[0x4488 - 0x10];
    s32 counter;
    char pad448C[0x44BA - 0x448C];
    s16 direction;
    char pad44BC[0x51FE - 0x44BC];
    u16 selectionFlags;
    char pad5200[0x5234 - 0x5200];
    s32 objectFlags;
    char pad5238[0x7A74 - 0x5238];
    s32 mode;
} Ovl11BBCB8Status;

/* 800BBCB8: pointer +25390, signed selection +2547C and state +254A0. */
typedef struct {
    char pad0[0x25390];
    Ovl11InteractionVolumeSelection *object;
    char pad25394[0x2547C - 0x25394];
    s32 selection;
    s32 savedSelection;
    char pad25484[8];
    s32 pendingMode;
    char pad25490[0x10];
    s32 state;
} Ovl11BBCB8Work;
/* C401C publishes the narrowed dispatch result; CD9A0 tests it against 2. */
extern s16 gOvl11PrimaryDispatchResult;

/* 8012FFB4 passes these strings and the 29-entry value menu to the debug UI. */
extern char gOvl30ActionHeading[];
extern char gOvl30ActionStatusFormat[];
extern char gOvl30ActionOffText[];
extern char gOvl30ActionOnText[];
extern struct Ovl30ValueMenuEntry gOvl30ActionMenu;
/* 8012FFB4 loads these words; calls narrow the selected IDs to signed halfwords. */
extern s32 gOvl30DebugActuatorSequence;
extern s32 gOvl30DebugItemId;
extern s32 gOvl30DebugRivalIndex;
extern s32 gOvl30DebugTimeAdvance;
/* 8012FFB4 retains the save-state base while indexing these fields by 0x1D4. */
typedef struct {
    u8 pad0[0x99EA];
    u16 field_99EA;
    u16 field_99EC;
    u8 pad99EE[0x9A06 - 0x99EE];
    u16 field_9A06;
} Ovl30EventRecordView;
/* 8012FFB4 accesses flag words, three event-state halfwords and a random selector. */
typedef struct {
    u8 pad0[0xC];
    u32 flagsC;
    u8 pad10[0x44CE - 0x10];
    u16 state44CE;
    u16 state44D0;
    u16 state44D2;
    u8 pad44D4[0x5234 - 0x44D4];
    u32 flags5234;
    u8 pad5238[0x5492 - 0x5238];
    s16 selected;
    u8 pad5494[0x99E8 - 0x5494];
    struct_80076220 records[37];
    u8 padDD8C[0xE76C - 0xDD8C];
    u16 random;
} Ovl30ActionState;

/* 800DFE74 passes this 12-byte frame sequence to 800D04D4 (69960.data.s). */
extern u8 gOvl11Animation41Sequence[12];
/* Fixed-sequence callbacks DFC44..E0180 each address one animation/hold table;
 * these scalar declarations denote the table bases, not their extents. */
extern u8 gOvl11Animation0Sequence;
extern u8 gOvl11Animation052Sequence;
extern u8 gOvl11Animation50Sequence;
extern u8 gOvl11Animation70Sequence;
extern u8 gOvl11Animation60Sequence;
extern u8 gOvl11Animation3Sequence;
extern u8 gOvl11Animation8Sequence;
/* 800E1A3C passes this 12-byte frame sequence to 800D04D4 (69960.data.s). */

/* 800D31EC stores both halfwords; D3FEC reads the first unsigned for a
 * wrapping debit, and D55F8 copies the second as u16 to object+22. */
extern s16 gOvl11StateValueDebit;
extern s16 gOvl11StateEntryDirection;

/* 8010AE64 selects one of two triples; 69960.data.s contains six halfwords. */
extern u16 gOvl11ActionMotionAnimations[6];

/* Entry callbacks 0/2/1/10 pass these private packed scripts to 800D04D4. */
extern u8 gOvl11ActionEntry0Keyframes;
extern u8 gOvl11ActionEntry2Keyframes;
extern u8 gOvl11ActionEntry1Keyframes;
extern u8 gOvl11ActionEntry10Keyframes;
/* 8010A47C passes this 12-byte sequence from 69960.data.s to 800D04D4. */
extern u8 gOvl11FoundRecordActionKeyframes[12];
/* 8010A47C clears this halfword and passes it as the 800D04D4 frame timer. */
extern u16 gOvl11FoundRecordActionTimer;

/* 8010A654 passes this 12-byte sequence from 69960.data.s to 800D04D4. */
extern u8 gOvl11ProbedRecordActionKeyframes[12];
/* 8010A654 clears this halfword and passes it as the 800D04D4 frame timer. */
extern u16 gOvl11ProbedRecordActionTimer;

/* 8010A850 passes the four-word position +52C8 and reads a pointer +DDD4. */
struct Ovl11A850State {
    u8 pad0[0x52C8];
    Ovl11CC7CCPosition position;
    u8 pad52D8[0xDDD4 - 0x52D8];
    u8 *record;
};

/* 8010FEF8 loads the stride-0x18 record pool through the word at +0xDDCC. */
struct Ovl11FEF8State {
    u8 pad0[0xDDCC];
    u8 *records;
};

/* 8010A850 passes this 20-byte state sequence in 69960.data.s to 800D04D4. */
extern u8 gOvl11RecordHandoffKeyframes[20];
/* 8010A850 clears this u16 timer and passes its address to 800D04D4. */
extern u16 gOvl11RecordHandoffTimer;
/* 8010A850 reads this signed halfword as the previous action state. */
extern s16 gOvl11RecordHandoffPhase;

/* 8010DE68 indexes eight lh entries, and two rows of five signed deltas. */
extern s16 gOvl11ObjectItemResponseMessages[8];
extern s16 gOvl11ObjectLocalItemResponseMessages[8];
extern s16 gOvl11ObjectItemResponseDeltas[2][5];

/* 8010DE68 reads the signed location/item fields and the unsigned event flag. */
typedef struct {
    u8 pad_0[0x44BA];
    s16 area;
    s16 room;
    u8 pad_44BE[0x44CE - 0x44BE];
    u16 event;
    u8 pad_44D0[0x5252 - 0x44D0];
    s16 item;
    s16 alternateItem;
} Ovl11DE68State;

/* 800F1078 stores a record pointer +4454 and reads/passes s16 +44B8/+44C0. */
struct Ovl11ConditionEvaluationWork {
    u8 pad0[0x4454];
    void *current;
    u8 pad4458[0x44B8 - 0x4458];
    s16 date[4];
    s16 time[2];
};

/* 800F1078 loads two pointers +25388/+2538C, then lhu flags at +8. */
struct Ovl11ConditionObject {
    u8 pad0[8];
    u16 flags;
};
struct Ovl11ConditionObjects {
    u8 pad0[0x25388];
    struct Ovl11ConditionObject *first;
    struct Ovl11ConditionObject *second;
};

/* 800F1540 compares a kind against lh +0x2 of the header at work-area +0x2538C. */
typedef struct {
    s16 unk0;
    s16 unk2;
} Ovl11WorkHeader;
typedef struct {
    u8 pad0[0x2538C];
    Ovl11WorkHeader *header;
} Ovl11WorkHeaderView;

/* 800C401C: lh/sh counters, lw/sw flags, two 10-entry position histories,
 * and 10 halfword-pair tags copied with lwl/lwr/swl/swr. */
typedef struct {
    u16 direction;
    s16 phase;
} Ovl11HistoryTag;

typedef struct {
    u8 pad0[0x1C];
    s16 countdown;
    u8 pad1E[0x2E - 0x1E];
    s16 timer;
    u8 pad30[6];
    u16 flags36;
    u16 direction;
    u8 pad3A[0x6C - 0x3A];
    s32 flags;
    u8 pad70[0xFC - 0x70];
    s16 collision[2];
    Ovl11CC7CCPosition position;
    u8 pad110[0x10];
    Ovl11CC7CCPosition historyA[10];
    Ovl11CC7CCPosition historyB[10];
    u16 pad260;
    u16 flags262;
    u8 pad264[0x2CC - 0x264];
    s16 effectTimer[2];
    u8 pad2D0[0x3D4 - 0x2D0];
    Ovl11HistoryTag tags[10];
} Ovl11PlayerUpdate;

/* 800C401C retains this base for the tick at +8 and player at +0x51C8. */
typedef struct {
    s32 pad0[2];
    s32 tick;
    u8 padC[0x51C8 - 0xC];
    Ovl11PlayerUpdate player;
} Ovl11UpdateState;

/* 80104EC8 passes two seven-halfword strings at byte offsets 0 and 0xE. */
extern u16 D_80051F26[];
/* Signed declarations are witnessed by matched 800B85F4 (ovl_19) and 800F19C8 (ovl_11). */
extern s16 gCalendarYear;
extern s16 D_80071A22;
/* 80104EC8, 800CD0C0 and 800EC490 read this unsigned state halfword. */
/* Default area selection's primary variant50 at work +44CE; default-position
 * selection also uses it to index pairs. EC490 exports the raw u16. */
extern u16 gAreaPrimaryVariant;
/* 800CD0C0 indexes three X/Z halfword pairs; original data spans 80125F18..24. */
extern s16 gOvl11DefaultPositionPairs[6];
/* 800CD45C walks four 4-byte records: signed hour +0 and signed byte delta +2. */
typedef struct {
    s16 hour;
    s8 delta;
    char pad3;
} Ovl11HourCounterDelta;
extern Ovl11HourCounterDelta gOvl11HourCounterDeltas[4];
/* 80104EC8 walks five item IDs with lhu and a two-byte stride. */
extern u16 D_801274D0[5];

/* 80104EC8: ten 0xB4-byte slots, id +0, name +4, value +0x16, tile +0x1E, flags +0x34. */
typedef struct {
    u16 id;
    u16 pad2;
    u16 name[7];
    u8 pad12[4];
    s16 value;
    u8 pad18[6];
    s16 tile;
    u8 pad20[0x14];
    s32 flags;
    u8 pad38[0xB4 - 0x38];
} Ovl11ConditionItem;

/* 80104EC8: actor and animal strides and field widths from its indexed reads. */
typedef struct {
    u8 pad0[8];
    u16 kind;
    u8 padA[0x14 - 0xA];
    s16 field14;
    u8 pad16[0x1C - 0x16];
    u16 amount;
    u8 pad1E[0x36 - 0x1E];
    u16 flags;
    u8 pad38[4];
    s16 location;
    s16 tile;
    u8 pad40[6];
    s16 field46;
    u8 pad48[0x1D4 - 0x48];
} Ovl11ConditionActor;
typedef struct {
    u8 pad0[0x16];
    s16 amount;
    u8 pad18[0x30 - 0x18];
    s16 kind;
    u8 pad32[0xB8 - 0x32];
    s16 flags;
    u8 padBA[0xF8 - 0xBA];
} Ovl11ConditionAnimal;

/* 80104EC8: field offsets and load widths witnessed in its original code. */
typedef struct {
    u8 pad_0[0x44b8 - 0x0];
    s16 date[3];
    u16 field_44BE;
    u16 field_44C0;
    u8 pad_44C2[0x44ce - 0x44c2];
    u16 field_44CE;
    u8 pad_44D0[0x44d2 - 0x44d0];
    u16 field_44D2;
    u8 pad_44D4[0x44f8 - 0x44d4];
    s32 field_44F8;
    u8 pad_44FC[0x5224 - 0x44fc];
    s32 field_5224;
    u8 pad_5228[0x5246 - 0x5228];
    s16 field_5246;
    s16 field_5248;
    u8 pad_524A[0x52c6 - 0x524a];
    s16 field_52C6;
    u8 pad_52C8[0x5492 - 0x52c8];
    s16 field_5492;
    u8 pad_5494[0x7AB4 - 0x5494];
    Ovl11ConditionItem items[10];
    u8 pad_81BC[0x91AC - 0x81BC];
    u16 field_91AC;
    u8 pad_91AE[0x91e1 - 0x91ae];
    u8 field_91E1;
    u8 pad_91E2[0x929c - 0x91e2];
    u16 field_929C;
    u8 pad_929E[0x92d1 - 0x929e];
    u8 field_92D1;
    u8 pad_92D2[0x99d0 - 0x92d2];
    Ovl11ConditionActor actors[37];
    u8 pad_DD74[0xde00 - 0xdd74];
    Ovl11ConditionAnimal animals[7];
    u8 pad_E4C8[0xe641 - 0xe4c8];
    u8 field_E641;
    u8 field_E642;
    u8 pad_E643[0xe657 - 0xe643];
    u8 field_E657;
    u8 pad_E658[0xe670 - 0xe658];
    u8 field_E670;
    u8 pad_E671[0xe6ca - 0xe671];
    u8 field_E6CA;
    u8 field_E6CB;
    u8 pad_E6CC[0xe76e - 0xe6cc];
    u16 field_E76E;
    u16 field_E770;
    u16 field_E772;
    u8 pad_E774[0xe77a - 0xe774];
    s16 field_E77A;
    u8 pad_E77C[0xe798 - 0xe77c];
    s16 field_E798;
    s16 field_E79A;
} Ovl11ConditionState;

/* 80104EC8 loads two scene-header pointers through this work-area base. */
typedef struct {
    u8 pad0[0x25388];
    struct Ovl11SceneHeader *header;
    struct Ovl11SceneHeader *otherHeader;
} Ovl11ConditionWork;

/* 800C162C: word flags at +0xC, position at +0x52C8 and reset words. */
struct Ovl11FrameState {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x52C8 - 0x10];
    u16 position[5];
    u8 pad52D2[6];
    s32 field_52D8;
    s32 field_52DC;
    s32 field_52E0;
};

/* 800C162C: halfword resets at +0x253A8/+0x253AA, word mode +0x254A0.
 * 800C14A0 dispatches on the signed word at +0x2548C. */
struct Ovl11FrameWork {
    u8 pad0[0x253A8];
    s16 field_253A8;
    s16 field_253AA;
    u8 pad253AC[0x2548C - 0x253AC];
    s32 dispatch;
    u8 pad25490[0x254A0 - 0x25490];
    s32 mode;
};

/* 800C162C initializes to 2, then caches the mode-0 update result. */
extern s32 gOvl11AreaFrameResult;

/* 800C188C reads flags and a signed selection value for clock-HUD policy.
 * The latter is the same storage exposed by Ovl11SelectionResetState. */
typedef struct {
    char pad0[0xC];
    s32 flags;
    char pad10[0xE4D2 - 0x10];
    s16 selectionValue;
} Ovl11ClockHudPolicyState;

/* 800DD274 indexes signed halfwords at 80123E38 by the +44CE selector. */
extern s16 gOvl11DefaultAreaPrimaryOffsets[];
/* 800DD274 loads this signed halfword for the +44D0 == 0 arm. */
extern s16 gOvl11DefaultAreaVariant60ZeroOffset;
/* 800DD274 loads this signed halfword for the +44D0 != 0 arm. */
extern s16 gOvl11DefaultAreaVariant60NonzeroOffset;
/* 800DD274 loads this signed halfword for the +44D2 == 0 arm. */
extern s16 gOvl11DefaultAreaVariant70ZeroOffset;
/* 800DD274 loads this signed halfword for the +44D2 != 0 arm. */
extern s16 gOvl11DefaultAreaVariant70NonzeroOffset;
/* 800DD274 indexes signed halfwords at 80123E50 by the +44D4 selector. */
extern s16 gOvl11DefaultAreaVariant80Offsets[];

/* 80110CE8/80110DA0 index 16-byte rows and add a column offset to Z/X. */
extern s32 gOvl11SlotPositionsZ600[][4];
extern s32 gOvl11SlotPositionsX400[][4];
/* 80110E34 copies three coordinate words and the +C facing halfword. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u16 facing;
} Ovl11RecordPlacementEntry;
extern Ovl11RecordPlacementEntry gOvl11RecordPlacementTable[];
/* 80110E98 adds the indexed signed halfword to a message-window entry. */
extern s16 gOvl11RecordMessageBases[];

/* Sound-driver cursors are single words/halfwords in the original GP window. */
extern s32 gSoundSpuLoadAddress;
extern s32 gSoundRamLoadCursor;
extern s32 gSoundLastSequenceBytes;
extern s16 gSoundLoadArchiveIndex;

/* ResetSoundDriverSlots initializes six words per slot table and 24 per
 * voice table. Retain scalar-lvalue views for the original &table indexing. */
extern s32 gSoundBankSpuAddressesStorage[6] __asm__("gSoundBankSpuAddresses");
#define gSoundBankSpuAddresses (*gSoundBankSpuAddressesStorage)
extern s32 gSoundLoadIndicesStorage[6] __asm__("gSoundLoadIndices");
#define gSoundLoadIndices (*gSoundLoadIndicesStorage)
extern s32 gSoundLoadIsSequenceStorage[6] __asm__("gSoundLoadIsSequence");
#define gSoundLoadIsSequence (*gSoundLoadIsSequenceStorage)
extern s32 gSoundLoadBankSlotsStorage[6] __asm__("gSoundLoadBankSlots");
#define gSoundLoadBankSlots (*gSoundLoadBankSlotsStorage)
extern s32 gSoundVabIdsStorage[6] __asm__("gSoundVabIds");
#define gSoundVabIds (*gSoundVabIdsStorage)
extern s32 gSoundSequenceIdsStorage[6] __asm__("gSoundSequenceIds");
#define gSoundSequenceIds (*gSoundSequenceIdsStorage)
extern s32 gSoundBankRamStartsStorage[6] __asm__("gSoundBankRamStarts");
#define gSoundBankRamStarts (*gSoundBankRamStartsStorage)
extern s32 gSoundBankRamEndsStorage[6] __asm__("gSoundBankRamEnds");
#define gSoundBankRamEnds (*gSoundBankRamEndsStorage)
extern s32 gSoundSequenceDataStorage[6] __asm__("gSoundSequenceData");
#define gSoundSequenceData (*gSoundSequenceDataStorage)
extern s32 gSoundSequenceByteSizesStorage[6] __asm__("gSoundSequenceByteSizes");
#define gSoundSequenceByteSizes (*gSoundSequenceByteSizesStorage)
extern s32 gSoundSequenceVabIdsStorage[6] __asm__("gSoundSequenceVabIds");
#define gSoundSequenceVabIds (*gSoundSequenceVabIdsStorage)
extern s32 gSoundVoiceGroupsStorage[24] __asm__("gSoundVoiceGroups");
#define gSoundVoiceGroups (*gSoundVoiceGroupsStorage)
extern s32 gSoundVoiceLastUseFramesStorage[24] __asm__("gSoundVoiceLastUseFrames");
#define gSoundVoiceLastUseFrames (*gSoundVoiceLastUseFramesStorage)
/* ClearSoundVoiceKeyOnFrames clears all 24 words; the allocator indexes by SPU voice. */
extern s32 gSoundVoiceKeyOnFrames[24];

/* Sound command 0 tests/resets a GP halfword and uses the setter's GP buffer word. */
extern s16 gSoundCommandResetGuard;
extern s32 gSoundCommandReadBuffer;
/* QueueSoundLoadCommand reads byte pairs; PlaySoundSequenceWithReverb reads
 * {type, depth} halfword pairs. The command indices are unchecked. */
extern u8 gSoundCommandLoadPairs[];
extern s16 gSoundCommandReverbPresets[];
/* Command 0 passes this address as destination RAM; its extent is not established. */
extern s32 gSoundCommandLoadArea;

/* Projected batch drivers test this GP flag; BFEC publishes the batch pointer. */
extern s32 gProjectedRotationScaleEnabled;
extern void *gProjectedPrimitiveBatch;

/* Row placement copies three words from 16-byte rows; D29B0 has special
 * rows 4/5 and D2A88's table ends after two rows at the independent 80123AAC
 * pair table used by 800D3424. Fourth words of placement rows are unused. */
extern s32 gOvl11RowZPlacementTable[][4];
extern s32 gOvl11RowXPlacementTable[][4];

/* ovl_11:800D3424 indexes state*16 + variant*4; D2E6C applies the
 * first halfword as animation and the optional second halfword as frame.
 * The 0x120-byte original table holds 18 rows of four pairs. */
typedef struct {
    u16 animation;
    u16 frame;
} Ovl11StateSpriteFrame;
extern Ovl11StateSpriteFrame gOvl11StateSpriteFrames[][4];
/* ovl_11:800DF3BC and its presentation caller establish the same pair
 * contract. Original 0x90-byte data window: nine rows of four pairs. */
extern Ovl11StateSpriteFrame gOvl11Code160SpriteDescriptors[][4];
/* Overlay-15 twins use separate 2-row X / 6-row Z tables at 801375D8/F8. */
extern s32 gOvl15RowXPlacementTable[][4];
extern s32 gOvl15RowZPlacementTable[][4];

/* Ovl21GetEntityRangeConstant selects these signed halfwords by entity ID. */
extern s16 gOvl21EntityRange480;
extern s16 gOvl21EntityRange420;
extern s16 gOvl21EntityRange360;
/* Ovl21GetEntityObjectRecord maps unmatched IDs to this 0x30-byte sprite. */
extern u8 gOvl21DefaultEntitySprite[];
/* Ovl21GetEntityObjectRecord maps entity 19 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite19[];
/* Ovl21GetEntityObjectRecord maps entity 7 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite7[];
/* Ovl21GetEntityObjectRecord maps entity 6 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite6[];
/* Ovl21GetEntityObjectRecord maps entity 21 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite21[];
/* Ovl21GetEntityObjectRecord maps entity 22 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite22[];
/* Ovl21GetEntityObjectRecord maps entity 34 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite34[];
/* Ovl21GetEntityObjectRecord maps entity 9 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite9[];
/* Ovl21GetEntityObjectRecord maps entity 20 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite20[];
/* Ovl21GetEntityObjectRecord maps entity 25 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite25[];
/* Ovl21GetEntityObjectRecord maps entity 16 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite16[];
/* Ovl21GetEntityObjectRecord maps entity 11 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite11[];
/* Ovl21GetEntityObjectRecord maps entity 5 to this 0x30-byte sprite. */
extern u8 gOvl21EntitySprite5[];

/* 8010D20C maps each state/direction pair to unsigned animation/frame IDs. */
typedef struct {
    u16 animation;
    u16 frame;
} Ovl11ObjectAnimationPair;
extern Ovl11ObjectAnimationPair gOvl11ObjectAnimationPairs[][4];
/* 8010B898: five modes by four directions, unsigned animation/frame pairs. */
extern Ovl11ObjectAnimationPair gOvl11StageMode2AnimationPairs[5][4];
/* 8010D0EC installs the selected halfword as the sprite palette-Y offset. */
extern s16 gOvl11ObjectPaletteYOffsets[8];
/* 8010D250 selects one of four signed modes with Rand(4). */
extern s16 gOvl11ObjectRandomModes[4];
/* 8010DDC8 requests the indexed mode-13 message; eight original halfwords. */
extern s16 gOvl11ObjectMode13Messages[8];
/* CE80/CD80 call these original 14-entry admission and frame-handler banks. */
extern s32 (*gOvl11ObjectModeEntryGuards[14])(void *);
extern s32 (*gOvl11ObjectModeHandlers[14])(void *);

/* E5ADC writes three words at D_80077600 + recordId*0xF8. */
extern u8 D_80077600[];
/* E5B84 clears bit 20 in shared work +5234 when animation status is final. */
typedef struct {
    char pad0[0x5234];
    s32 flags;
} Ovl11AnimationCommandWork;
/* E5D00 reads shared +52C6 and per-record +99EE/+9A16, stride 0x1D4. */
typedef struct {
    char pad0[0x52C6];
    s16 defaultMatchValue;
    char pad52C8[0x99EE - 0x52C8];
    s16 animationFrame;
    char pad99F0[0x9A16 - 0x99F0];
    s16 matchValue;
} Ovl11RecordResetCommandWork;

/* 800B9644 passes the signed entity IDs at scene-work +118/+220 to
 * Ovl21RaiseEntityRangeValue. This is a view, not separate storage. */
typedef struct {
    u8 pad0[0x118];
    s16 firstEntityId;
    u8 pad11A[0x220 - 0x11A];
    s16 secondEntityId;
} Ovl21ResultEntityView;

/* ovl_10 8A5C selects command 0..14 with signed %15 arithmetic. */
extern s32 gOvl10CommandIndex;
/* ovl_10 packed 15..33-byte status copies and FntPrint share this 64-byte buffer. */
extern char gOvl10CommandResultText[0x40];

/* ovl_10 mode/day pointer tables target these original readonly strings. */
extern const char gOvl10LinkModeBoth[6];
extern const char gOvl10LinkModeTransmit[3];
extern const char gOvl10LinkModeReceive[3];
extern const char gOvl10LinkModeDisabled[8];
extern const char gOvl10WeekdayLabels[7][4];
/* 9AA8 copies this complete 30-byte message after a failed card accept. */
extern const char gOvl10DefaultCommandResultText[30];
/* First mode label of the separate current-control pointer table. */
extern const char gOvl10CurrentControlDisabled[4];

/* ovl_10 scene/input state: 889C polls exit; 8A5C latches navigation/execute. */
extern s32 gOvl10MenuExitRequested;
extern char gOvl10InputWorkAnchor[];
extern s32 gOvl10AcceptedCommandIndex;
extern s32 gOvl10CommandNavigationLatch;
extern s32 gOvl10CommandExecuteLatch;
/* 8C14 fills one marker pointer per command; 9060 reads the three columns. */
extern char *gOvl10CommandRowMarkers[];
/* 92D0/94A4/A11C index these resident string-pointer tables. */
extern char *gOvl10LinkModeNamePointers[];
extern char *gOvl10InfoWeekdayNamePointers[];
extern char *gOvl10TimeWeekdayNamePointers[];
extern char *gOvl10ControlModeNamePointers[];
/* SDK result views consumed by the all-info, application/time/serial drawers. */
extern char gOvl10AllInfoBytes[];
extern long gOvl10ApplicationNumber;
extern char gOvl10TimeBytes[];
extern long gOvl10SerialNumber;
/* 9AA8 edits the block number/flag and uses per-bit repeat counters. */
extern s32 gOvl10ExecApplicationNumber;
extern s32 gOvl10ExecFlagEnabled;
extern s32 gOvl10ExecNumberIncrementTicks;
extern s32 gOvl10ExecNumberDecrementTicks;
/* 9D24 edits a signed application selector and four argument bytes. */
extern int gOvl10ApplicationParameterCursor;
extern int gOvl10ApplicationUpperIncrementTicks;
extern int gOvl10ApplicationUpperDecrementTicks;
extern int gOvl10ApplicationLowerIncrementTicks;
extern int gOvl10ApplicationLowerDecrementTicks;
extern int gOvl10ApplicationCursorLatch;
extern short gOvl10ApplicationParameters[];
/* A11C/A2A0 provide McxCurrCtrl/McxFlashAcs's flag/mode arguments. */
extern s32 gOvl10ControlSoundEnabled;
extern s32 gOvl10ControlInfraredMode;
extern s32 gOvl10ControlInfraredButtonLatch;
extern s32 gOvl10FlushWriteSuperior;
/* AB10/ACBC edit McxShowTrans direction/timeout and McxSetLED's state. */
extern s32 gOvl10TransitionDirection;
extern s32 gOvl10TransitionTimeout;
extern s32 gOvl10TransitionTimeoutIncrementTicks;
extern s32 gOvl10TransitionTimeoutDecrementTicks;
extern s32 gOvl10LedEnabled;

#endif /* GLOBALS_OVERRIDE_H */

