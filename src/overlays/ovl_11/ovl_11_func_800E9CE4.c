#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"
#include "ovl11_object_archive_pipeline.h"

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
} Ovl11Entry;

s32 ovl_11_func_800E9CE4(s16 arg0) {
    Ovl11Entry *p;

    p = (Ovl11Entry *)Ovl11GetObjectProgramCursor();
    p->unk1 = ((Ovl11Entry *)gOvl11ValueSlots)[arg0].unk0;
    return 1;
}
