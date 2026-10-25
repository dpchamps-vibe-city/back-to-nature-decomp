#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

/* Overlay alias for Rand; signature from its matched definition. */
u32 Rand(s32 arg0);
s32 ovl_19_func_800BA2D4(void);
s32 ovl_19_func_800BA33C(u8 *arg0);

/* Record views from the matched callees ovl_19_func_800BA468/800BA5B4. */
typedef struct {
    char pad_0[0x4];
    s16 unk4;
    char pad_6[0xA];
    s16 unk10;
    char pad_12[0x2];
    s16 unk14;
} ReconA0View;

typedef struct {
    char pad_0[0x10];
    s16 unk10;
    char pad_12[0x2];
    s16 unk14;
} ReconA1View;

/* Record view from the matched callee ovl_19_func_800BA750. */
typedef struct {
    char pad_0[0x2];
    u16 unk2;
    char pad_4[0x2];
    s16 unk6;
} Recon800BA750A0View;

s32 ovl_19_func_800BA468(ReconA0View *arg0, ReconA1View *arg1);
s32 ovl_19_func_800BA5B4(ReconA0View *arg0, ReconA1View *arg1);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);
s32 ovl_19_func_800BA750(Recon800BA750A0View *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);

/* One 0x48-byte actor record in the D_800BF4D0 pair. */
typedef struct {
    s16 unk0;
    s16 command;
    s16 angle;
    s16 timer;
    s16 value;
    s16 unkA;
    s32 flag;
    s16 rest[28];
} Ovl19ActorRecord;

/* Per-frame command driver for the two actors. For each actor, it issues the
 * scripted command 4, resolves a command-4 encounter with the other actor
 * (commands 5-10), or rolls a fresh command and direction (800BA33C). */
void ovl_19_func_800B95D4(void) {
    s32 i;
    s16 angle;
    s16 *actors;
    /* User-authorized binding; see notes/asm-exception-evidence.md. Left to
     * global-alloc, this pointer outranks both angle and the strength-reduced
     * actor pointer, which rotates $s1-$s3. */
    register s16 *opponent asm("$19");
    s16 *state;
    s16 *record;
    s16 *view;
    s32 choice;
    s32 status;
    s32 command;
    s32 one;

    i = 0;
    one = 1;
    actors = D_800BF4C0 + 8;
    view = actors - 8;
    do {
        Ovl19ActorRecord *actor = (Ovl19ActorRecord *)actors + i;

        angle = 0;
        opponent = actors;
        if (i == 0) {
            opponent += 36;
        }
        status = ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)actor);
        record = actors - 8;
        if (status == one && record[i * 32 + 0x51] == 2) {
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)actor, 4, actor->angle, 30);
            record[0x101] = PlaySoundEffect(0x3B);
            record[0x101] = PlaySoundEffect(0x44);
            continue;
        }
        if (actor->command == 4) {
            state = D_800BF4C0;
            if (actor->timer != actor->timer / state[0xBB] * state[0xBB]) {
                continue;
            }
            if (actor->timer > 0) {
                if (opponent[1] == 4 &&
                    ovl_19_func_800BA468((ReconA0View *)opponent, (ReconA1View *)actor) == one &&
                    ovl_19_func_800BA468((ReconA0View *)actor, (ReconA1View *)opponent) == one) {
                    choice = ovl_19_func_800BA2D4();
                    record = (s16 *)(choice * 72 + (s32)actors);
                    opponent = actors;
                    if (choice == 0) {
                        opponent = state + 0x2C;
                    }
                    angle = (s16)(record[2] + ovl_19_func_800BA5B4((ReconA0View *)record, (ReconA1View *)opponent));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)record, 9, (s16)(angle % 4), state[0xC8]);
                    *(s32 *)(record + 6) = one;
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)opponent, 10, opponent[2], state[0xC9]);
                    *(s32 *)(opponent + 6) = 0;
                    continue;
                }
            } else {
                if (ovl_19_func_800BA468((ReconA0View *)actor, (ReconA1View *)opponent) == one) {
                    angle = (s16)(actor->angle + ovl_19_func_800BA5B4((ReconA0View *)actor, (ReconA1View *)opponent));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)actor, 6, (s16)(angle % 4), state[0xC7]);
                    actor->flag = one;
                    command = 7;
                    if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)opponent) == one) {
                        command = 8;
                    }
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)opponent, command, opponent[2], state[0xC7]);
                    state[0x101] = PlaySoundEffect(0x3C);
                } else {
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)actor, 5, actor->angle, state[0xC6]);
                    actor->flag = 0;
                }
                continue;
            }
        }
        if (ovl_19_func_800BA750((Recon800BA750A0View *)actor) == one) {
            command = ovl_19_func_800BA33C((u8 *)actor);
            switch (command) {
                case 0:
                case 1:
                    angle = (s16)Rand(4);
                    break;
                case 3:
                    angle = view[10];
                    break;
                case 2:
                    if ((s32)Rand(101) <= actor->value * 100 / 255) {
                        angle = (s16)(view[10] + ovl_19_func_800BA5B4((ReconA0View *)actor, (ReconA1View *)opponent));
                        angle = (s16)(angle % 4);
                    } else {
                        angle = actor->angle;
                    }
                    break;
            }
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)actor, command, angle, D_800BF4C0[0xBC]);
        }
    } while (view += 36, ++i < 2);
}
