#ifndef GRAPHICS_HEAP_H
#define GRAPHICS_HEAP_H

/*
 * Shared layout used by func_8001231C and func_80012598. Each pool contains
 * two buffers. func_8001E160 independently uses the main-OT and primitive
 * offsets; func_800120C8 passes the work-area pointer to GsSetWorkBase.
 * These are layout names, not evidence for an original macro's spelling.
 */
#define GFX_HEAP_BASE             0x801BE1B0
#define GFX_HEAP_CLEAR_SIZE       0x3EE50
#define GFX_HEAP_OT0_OFFSET       0x00000
#define GFX_HEAP_OT1_OFFSET       0x00200
#define GFX_HEAP_OT2_OFFSET       0x00280
#define GFX_HEAP_MAIN_OT_OFFSET   0x00290
#define GFX_HEAP_PRIM_OFFSET      0x04290
#define GFX_HEAP_WORK_OFFSET      0x33090
#define GFX_HEAP_PRIM_SIZE        0x17700
#define GFX_HEAP_WORK_SMALL_SIZE  0x2EE0
#define GFX_HEAP_WORK_LARGE_SIZE  0x5EDC
#define GFX_HEAP_ADDR(offset)     (GFX_HEAP_BASE + (offset))
#define GFX_HEAP_END(workSize)    GFX_HEAP_ADDR(GFX_HEAP_WORK_OFFSET + 2 * (workSize))

/*
 * Existing matching exceptions, shared ONLY by the two heap initializers.
 * MATCH_COPY copies one register value, not memory. Unlike a C assignment,
 * its output is opaque to CSE; volatility retains the explicit instruction.
 * Neither helper pins a register. Callers retain any required bindings and
 * their source-policy allowlist entries. This is encapsulated repair debt,
 * not a claim that an SDK/studio macro or a C impossibility has been proved.
 * See notes/asm-exception-evidence.md.
 */
#define MATCH_COPY(dst, src) \
    __asm__ volatile("addu %0, %1, $zero" : "=r"(dst) : "r"(src))

/*
 * Standalone statements only: deliberately two separately schedulable RTL
 * instructions, with the high-part temporary supplied by the caller.
 * Do not combine into one asm template or add a do/while wrapper without
 * rechecking both functions' bytes and compiler lifetime boundaries.
 */
#define MATCH_LOAD_ADDRESS(dst, high, symbol) \
    __asm__ volatile("lui %0, %%hi(" #symbol ")" : "=r"(high)); \
    __asm__ volatile("addiu %0, %1, %%lo(" #symbol ")" : "=r"(dst) : "r"(high))

#endif
