/* Scene selector/tuning defaults and the two initial-record banks. The
 * initializer uses the second table banks and sets record flag halfword 1;
 * the reset twin uses the first banks and clears it, preserving two fields
 * the initializer clears. These are intentionally distinct operations.
 * Public contract: ovl25_scene_record_seeders.h; unit evidence: file-groupings.
 */
#include "common.h"
#include "ovl25_scene_record_seeders.h"

void Ovl25ResetSceneState(void) {
    char *work;

    work = (char *)&gOvl25SceneWorkWords;
    *(s16 *)(work + 0x0) = 0;
    *(s16 *)(work + 0x2) = 0;
    Ovl25InitSceneRecordState();
    *(s16 *)(work + 0x4D4) = 5;
    *(s16 *)(work + 0x4D6) = 2;
    *(s16 *)(work + 0x4D8) = 5;
    *(s16 *)(work + 0x4DA) = 1;
    *(s16 *)(work + 0x4DC) = 3;
    *(s16 *)(work + 0x4D2) = 0xFA0;
    *(s16 *)(work + 0x4D0) = 0x82;
    *(s16 *)(work + 0x4E4) = 0x46;
    *(s16 *)(work + 0x4E2) = 0x140;
    *(s16 *)(work + 0x4DE) = 0x2AD;
    *(s16 *)(work + 0x4E0) = 0xA;
    *(s16 *)(work + 0x4CE) = 0xC8;
}

void Ovl25SeedInitialSceneRecords(void);

void Ovl25InitSceneRecordState(void) {
    char *work;
    s16 *anchor;

    work = (char *)&gOvl25SceneWorkWords;
    anchor = (s16 *)(work + 0x3D0);
    *(s16 *)(work + 0x4C8) = -1;
    *(s16 *)(work + 0x3D0) = 0x1320;
    *(s32 *)(work + 4) = 0;
    *(s32 *)(work + 8) = 0;
    anchor[1] = -0xA;
    anchor[2] = 0xA4;
    *(s16 *)(work + 0xE) = 0;
    *(s16 *)(work + 0x4CA) = -1;
    *(s16 *)(work + 0x4CC) = 0;
    *(s16 *)(work + 0xC) = 0;
    Ovl25SeedInitialSceneRecords();
}

void Ovl25SeedInitialSceneRecords(void) {
    s32 recordIndex;
    s32 initialFixedValue;
    s32 enabled;
    s16 *record;
    Ovl25SeedRecord *sceneSource;
    Ovl25SeedRecord *auxSource;
    u8 *sceneTable;
    u8 *auxTable;

    recordIndex = 0;
    enabled = 1;
    initialFixedValue = 0x32000;
    sceneTable = (u8 *)gOvl25InitialSceneRecords;
    sceneSource = (Ovl25SeedRecord *)(sceneTable + 0x30);
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[8] = recordIndex;
        record[9] = enabled;
        record[10] = 0;
        record[11] = 0;
        record[12] = 0;
        *(s32 *)(record + 14) = initialFixedValue;
        *(Ovl25SeedRecord *)(record + 16) = *sceneSource;
        sceneSource++;
        recordIndex++;
    } while (recordIndex < 6);

    recordIndex = 0;
    auxTable = (u8 *)gOvl25InitialAuxRecords;
    auxSource = (Ovl25SeedRecord *)(auxTable + 0x10);
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[0x170] = recordIndex;
        *(Ovl25SeedRecord *)(record + 0x178) = *auxSource;
        auxSource++;
        recordIndex++;
    } while (recordIndex < 2);

    record = gOvl25SceneWorkWords;
    record[0x171] = 0xA;
    record[0x1AD] = 0xB;
}

void Ovl25ResetSceneRecords(void) {
    s32 recordIndex;
    s32 initialFixedValue;
    s16 *record;
    Ovl25SeedRecord *sceneSource;
    Ovl25SeedRecord *auxSource;

    recordIndex = 0;
    initialFixedValue = 0x32000;
    sceneSource = (Ovl25SeedRecord *)gOvl25InitialSceneRecords;
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[8] = recordIndex;
        record[9] = 0;
        record[10] = 0;
        *(s32 *)(record + 14) = initialFixedValue;
        *(Ovl25SeedRecord *)(record + 16) = *sceneSource;
        sceneSource++;
        recordIndex++;
    } while (recordIndex < 6);

    recordIndex = 0;
    auxSource = (Ovl25SeedRecord *)gOvl25InitialAuxRecords;
    do {
        record = gOvl25SceneWorkWords + recordIndex * 0x3C;
        record[0x170] = recordIndex;
        *(Ovl25SeedRecord *)(record + 0x178) = *auxSource;
        auxSource++;
        recordIndex++;
    } while (recordIndex < 2);

    record = gOvl25SceneWorkWords;
    record[0x171] = 0xA;
    record[0x1AD] = 0xB;
}
