#include "common.h"
#include "ovl15_save_checksums.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libmcrd.h"
#include "psyq/memory.h"
#include "psyq/strings.h"
#include "psyq/libmcx.h"
#include "psyq/libgpu.h"
#include "packet_work.h"

typedef void (*M2C_a048f66bc407_SpriteCallback)(s32, SpriteSourceData *, s16, s32, s32);

int McxCardType (int);
int McxSync (int, long *, long *);
long MemCardAccept (long chan);
long MemCardExist (long chan);
long MemCardFormat (long chan);
long MemCardGetDirentry (long chan, char *name, struct DIRENTRY *dir, long *files, long ofs, long max);
long MemCardReadFile (long chan, char *file, unsigned long *adrs, long ofs, long bytes);
long MemCardSync (long mode, long *cmds, long *rslt);
void SsSeqSetVol (short, short, short);
void SsUtAllKeyOff (short);
void *memmove (unsigned char *, const unsigned char *, int);
char *strcpy ();
void ovl_15_func_801372F0 (void);
void ovl_15_func_801372F8 (void);
s32 ovl_15_func_80137300 (void);
void ovl_15_func_80137398 (void);
void ovl_15_func_801373A0 (void);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_800212A8 (s32 soundId, s32 lo, s32 hi);
void func_800223D4 (s32 arg0, M2C_a048f66bc407_SpriteCallback callback);
void func_800136D4 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
s32 func_80022DF8 (void);
void SetVal8005E2BC (s32 arg0);
void SetVal8005E334 (s32 arg0);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);
void ovl_15_func_8013468C (void);
s32 func_8002029C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void ClearVal8005E2CC (void);
void func_800226F0 (void);
void ovl_15_func_8012E15C (void);
void func_800132B8 (s32 arg0, s32 arg1, s32 arg2);
void func_8001FE00 (s32 arg0);
void func_8002495C (s16 arg0);
void func_8002238C (s32 arg0);
s32 func_80013394 (void);
void func_80017A08 (s32 arg0, s32 arg1);
void func_80017A38 (s16 arg0, s16 arg1);
void func_800132F0 (s32 arg0, s32 arg1, s32 arg2);
void func_8001FE34 (s32 arg0);
void func_80011EF0 (s32 arg0);
void ovl_15_func_8012ED18 (void);
void ovl_15_func_8012EE84 (void);
void ovl_15_func_8012EEEC (void);
s32 ovl_15_func_8012F078 (void);
s32 ovl_15_func_8012F0E8 (void);
void ovl_15_func_8012F6C0 (void);
s32 ovl_15_func_8012F86C (void);
void ovl_15_func_8012F8DC (void);
s32 ovl_15_func_8012F990 (void);
s32 ovl_15_func_8012FA00 (void);
s32 ovl_15_func_8012FA70 (void);
void ovl_15_func_8012FF28 (void);
s32 ovl_15_func_8012FFD8 (void);
void ovl_15_func_80130048 (void);
void ovl_15_func_801300E4 (void);
void ovl_15_func_8013010C (void);
s32 ovl_15_func_80135E9C (s16 arg0);
void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80137228 (s16 arg0, s16 arg1);
s8 ovl_15_func_80136990 (void);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);
s8 ovl_15_func_801370B4 (void);
s32 func_8001FABC (s16 arg0);
s32 ovl_15_func_80136558 (s16 arg0, s16 arg1);
void ovl_15_func_801359FC (void);
void ovl_15_func_80137544 (char *arg0, s32 arg1);
s8 *ovl_15_func_80134770 (s16 arg0);
s32 ovl_15_func_80136660 (s16 arg0, s16 arg1, u32 *arg2);
void ovl_15_func_80130468 (void);
void ovl_15_func_801305D4 (void);
void ovl_15_func_8013063C (void);
s32 ovl_15_func_801307D8 (void);
s32 ovl_15_func_80130848 (void);
s32 ovl_15_func_80130A94 (void);
s32 ovl_15_func_80130BBC (void);
void ovl_15_func_80130C2C (void);
void ovl_15_func_80130C8C (void);
void ovl_15_func_80130D3C (void);
s32 ovl_15_func_80130DDC (void);
s32 ovl_15_func_80130E4C (void);
s32 ovl_15_func_80130EBC (void);
void ovl_15_func_80130F2C (void);
void ovl_15_func_80130FC8 (void);
s32 ovl_15_func_801367F8 (s16 arg0, s16 arg1, u32 *arg2);
void ovl_15_func_80135A78 (void);
void ovl_15_func_8013143C (void);
void ovl_15_func_801315B0 (void);
void ovl_15_func_80131618 (void);
s32 ovl_15_func_80131778 (void);
s32 ovl_15_func_801317E8 (void);
s32 ovl_15_func_80131A34 (void);
s32 ovl_15_func_80131B5C (void);
void ovl_15_func_80131BCC (void);
void ovl_15_func_80131C2C (void);
void ovl_15_func_80131CDC (void);
s32 ovl_15_func_80131D88 (void);
s32 ovl_15_func_80131DF8 (void);
s32 ovl_15_func_80131E68 (void);
void ovl_15_func_80131ED8 (void);
s32 ovl_15_func_80131F5C (void);
void ovl_15_func_8013237C (void);
void ovl_15_func_80132408 (void);
void ovl_15_func_80132494 (void);
void ovl_15_func_80132520 (void);
void ovl_15_func_801325AC (void);
s32 ovl_15_func_801328C4 (void);
void ovl_15_func_80132934 (void);
void ovl_15_func_801329D0 (void);
void ovl_15_func_801329F8 (void);
s8 *ovl_15_func_80134890 (s16 arg0, s8 *arg1, s16 arg2);
void ovl_15_func_801349C8 (s16 arg0, char *arg1);
u32 Rand (s32 arg0);
void ovl_15_func_8013703C (s16 arg0, s16 *arg1, s32 arg2);
u16 *func_8001AA7C (s32 arg0, u16 *arg1);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_8001ABF0 (u16 *dst, u16 *src);
void func_800249C0 (s32 arg0, s16 arg1, s16 arg2);
void func_8001719C (u8 *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);

void ovl_15_func_801372A0(s16 arg0) {
    ovl_15_func_801372F0();
    ovl_15_func_801372F8();
    ovl_15_func_80137300();
    ovl_15_func_80137398();
    if (arg0 != 0) {
        ovl_15_func_801373A0();
    }
}
