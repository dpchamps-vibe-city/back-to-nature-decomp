#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/*
 * ASSEMBLY EXCEPTION: the historical scheduling-ceiling argument is
 * superseded by a no-asm candidate with exact instruction order and one
 * allocation difference (79/81). See notes/asm-exception-evidence.md for
 * its scope; that result does not prove that clean C is impossible.
 *
 * The active hybrid retains C for sprite/CLUT/branch work and existing asm
 * for the entry and interleaved packet-linking windows. Eight redundant
 * bindings have been removed together, leaving eight explicit bindings.
 * The raw tail still names registers literally: removing a binding does
 * not remove that machine-state requirement. The tail spans multiple SDK
 * operations, so it has not been hidden behind a generic packet macro.
 * Historical research: notes/research/func_80019070-prologue-allocation-and-arg2-truncation.md.
 */

/*
 * Existing exact hybrid with a measured reduction of redundant bindings.
 * This function remains explicitly allowlisted for register-asm and
 * embedded-asm in .pi/autoloop.json; it is not a general policy precedent.
 */
void *func_80019070(s32 *ordering_table, u8 *packet, u32 glyph,
                    s32 x, s16 y, u8 red, u8 green, u8 blue,
                    u32 palette, s32 semitransparent)
{
    SPRT *sprt;
    register s32 *ot __asm__("$11");
    register u32 glyph_reg __asm__("$6");
    register s16 sprite_x __asm__("$13");
    register u32 texture_load __asm__("$14");
    u32 texture_u;
    register u32 palette_load __asm__("$5");
    u32 palette_index;
    u32 header_len;
    register u32 header_code __asm__("$3");
    u32 address_mask;
    register u32 uv_u __asm__("$3");
    u32 uv_v;
    s32 semitrans_reg;
    register s16 y_reg __asm__("$12");
    u32 red_reg;
    u8 green_reg;
    u8 blue_reg;
    u8 code;
    DR_TPAGE *tpage;

    __asm__ volatile(
        "addu %0,$5,$zero\n\t"
        "addiu %1,$zero,4\n\t"
        "addiu %2,$zero,100\n\t"
        "addu %3,$4,$zero\n\t"
        "andi %4,$6,0xffff\n\t"
        "sll $7,$7,16\n\t"
        "sra %5,$7,16\n\t"
        "andi %6,%4,15\n\t"
        "andi %4,%4,240\n\t"
        : "=r"(sprt), "=r"(header_len), "=r"(header_code),
          "=r"(ot), "=r"(glyph_reg), "=r"(sprite_x),
          "=r"(texture_load)
        :
        : "$7", "memory");
    __asm__ volatile("lw %0,32($sp)" : "=r"(palette_load) : : "memory");

    texture_u = texture_load;
    palette_index = palette_load;
    setlen(sprt, header_len);
    setcode(sprt, header_code);
    __asm__ volatile(
        "lw %0,36($sp)\n\t"
        "lh %1,16($sp)\n\t"
        "lbu %2,20($sp)\n\t"
        "lbu %3,24($sp)\n\t"
        "lbu %4,28($sp)"
        : "=r"(semitrans_reg), "=r"(y_reg), "=r"(red_reg),
          "=r"(green_reg), "=r"(blue_reg)
        :
        : "memory");
    glyph_reg >>= 4;

    if (palette_index >= 6) {
        palette_index = 0;
    }

    setClut(sprt, 0x380, D_80049044[palette_index]);
    setRGB0(sprt, red_reg, green_reg, blue_reg);
    __asm__ volatile("" ::: "memory");

    code = 0x64;
    if (semitrans_reg != 0) {
        code = 0x66;
    }

    setcode(sprt, code);
    __asm__ volatile("" ::: "memory");
    __asm__ volatile(
        "lui %0,0xff\n\t"
        "ori %0,%0,0xffff\n\t"
        "lui %1,0xe100"
        : "=r"(address_mask), "=r"(red_reg));
    uv_u = texture_u * 8;
    uv_v = (glyph_reg * 3) << 2;
    __asm__ volatile(
        "sb $3,12($8)\n\t"
        "addiu $3,$zero,8\n\t"
        "sb $2,13($8)\n\t"
        "addiu $2,$zero,12\n\t"
        "lui $6,0xff00\n\t"
        "sh $3,16($8)\n\t"
        "lw $3,0($8)\n\t"
        "and $5,$8,$4\n\t"
        "sh $13,8($8)\n\t"
        "sh $12,10($8)\n\t"
        "sh $2,18($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $3,$3,$6\n\t"
        "and $2,$2,$4\n\t"
        "or $3,$3,$2\n\t"
        "sw $3,0($8)\n\t"
        "addiu $8,$8,20\n\t"
        "lw $2,0($11)\n\t"
        "addiu $3,$zero,1\n\t"
        "and $2,$2,$6\n\t"
        "or $2,$2,$5\n\t"
        "sw $2,0($11)\n\t"
        "sb $3,3($8)\n\t"
        "lw $3,0($8)\n\t"
        "ori $7,$7,0x60e\n\t"
        "sw $7,4($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $3,$3,$6\n\t"
        "and $2,$2,$4\n\t"
        "or $3,$3,$2\n\t"
        "sw $3,0($8)\n\t"
        "lw $2,0($11)\n\t"
        "and $4,$8,$4\n\t"
        "and $2,$2,$6\n\t"
        "or $2,$2,$4\n\t"
        "sw $2,0($11)"
        : "+r"(sprt)
        : "r"(ot), "r"(address_mask), "r"(red_reg),
          "r"(sprite_x), "r"(y_reg), "r"(uv_u), "r"(uv_v)
        : "memory");

    return (u8 *)sprt + sizeof(DR_TPAGE);
}

