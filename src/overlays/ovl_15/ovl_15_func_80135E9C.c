#include "common.h"
#include "ovl15_save_checksums.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libmcrd.h"
#include "psyq/memory.h"
#include "psyq/strings.h"
#include "psyq/libmcx.h"
#include "packet_work.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "beveled_panel.h"
#include "random.h"
#include "ovl15_save_card_title.h"
#include "ovl15_save_menu_ui.h"
#include "ovl15_save_image.h"
#include "ovl15_card_filename.h"


int McxCardType (int);
int McxSync (int, long *, long *);
long MemCardAccept (long chan);
long MemCardExist (long chan);
long MemCardFormat (long chan);
long MemCardGetDirentry (long chan, char *name, struct DIRENTRY *dir, long *files, long ofs, long max);
long MemCardReadFile (long chan, char *file, unsigned long *adrs, long ofs, long bytes);
long MemCardSync (long mode, long *cmds, long *rslt);
long MemCardWriteFile (long chan, char *file, unsigned long *adrs, long ofs, long bytes);
int bcmp (const unsigned char *, const unsigned char *, int);
void *memmove (unsigned char *, const unsigned char *, int);
char *strcpy ();
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);
void ovl_15_func_8013468C (void);
void ovl_15_func_8012EE84 (void);
s32 ovl_15_func_8012F078 (void);
s32 ovl_15_func_8012F0E8 (void);
s32 ovl_15_func_8012F86C (void);
void ovl_15_func_8012F8DC (void);
s32 ovl_15_func_8012F990 (void);
s32 ovl_15_func_8012FA00 (void);
s32 ovl_15_func_8012FA70 (void);
s32 ovl_15_func_8012FFD8 (void);
void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);
s32 ovl_15_func_80136558 (s16 arg0, s16 arg1);
void Ovl15CardReadPromptNoCard (void);
s32 Ovl15CardReadPromptState05 (void);
s32 Ovl15CardReadPromptState06 (void);
s32 Ovl15CardReadPromptState08 (void);
s32 Ovl15CardReadPromptState10 (void);
void Ovl15CardReadBuildFilename (void);
void Ovl15CardReadVerifyChecksums (void);
s32 Ovl15CardReadPromptState14 (void);
s32 Ovl15CardReadPromptState15 (void);
s32 Ovl15CardReadPromptState16 (void);
void Ovl15CardTransferPromptNoCard (void);
s32 Ovl15CardTransferPromptState05 (void);
s32 Ovl15CardTransferPromptState06 (void);
s32 Ovl15CardTransferPromptState08 (void);
s32 Ovl15CardTransferPromptState10 (void);
void Ovl15CardTransferBuildFilename (void);
void Ovl15CardTransferVerifyChecksums (void);
s32 Ovl15CardTransferPromptState14 (void);
s32 Ovl15CardTransferPromptState15 (void);
s32 Ovl15CardTransferPromptState30 (void);
void Ovl15CardTransferCompareHeader (void);
s32 Ovl15CardTransferPromptState17 (void);
void Ovl15CardTransferPromptState21 (void);
void Ovl15CardTransferPromptState22 (void);
void Ovl15CardTransferPromptState23 (void);
void Ovl15CardTransferPromptState24 (void);
void Ovl15CardTransferPromptState25 (void);
s32 Ovl15CardTransferPromptState28 (void);
void ovl_15_func_80134724 (void);
s8 *Ovl15FormatTitleDigits (s16 arg0, s8 *arg1, s16 arg2);
void Ovl15FormatTitleSeason (s16 arg0, char *arg1);
void func_80017A08 (s32 arg0, s32 arg1);
void Ovl15FormatDateNumber (s16 arg0, s16 *arg1, s32 arg2);
u16 *func_8001AA7C (s32 arg0, u16 *arg1);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_8001ABF0 (u16 *dst, u16 *src);
void func_800249C0 (s32 arg0, s16 arg1, s16 arg2);

extern u16 gOvl15CardRetryCount;

s32 ovl_15_func_80135E9C(s16 arg0) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;
    s32 var_s3;
    s32 var_v0;

    var_s3 = 0;
    if (arg0 != 0) {
        var_s3 = 0x10;
        if (arg0 != 1) {
            return 0xC;
        }
    }
    gOvl15CardRetryCount += 1;
    temp_v0 = MemCardSync(1, &sp10, &sp14);
    if (temp_v0 == 0) {
        return 3;
    }
    if (temp_v0 == -1) {
        gOvl15CardRetryCount = 0;
        goto block_19;
    }
    if (sp10 == 1) {
        var_v0 = 5;
        switch (sp14) {
        case 0:
            return 0;
        case 1:
        case 2:
            break;
        case 3:
            return var_v0;
        default:
            break;
        }
block_12:
        if ((s16) gOvl15CardRetryCount >= 0x1F) {
            switch (sp14) {
            case 1:
                return 4;
            case 2:
                return 8;
            }
        }
        MemCardSync(0, &sp10, &sp14);
block_19:
        if (MemCardExist(var_s3) != 1) {
            return 0xC;
        }
        return 3;
    } else {
        return 3;
    }
}
