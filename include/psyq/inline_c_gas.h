/* Generated diagnostic GAS substitutions; include after inline_c.h. */
#undef gte_rtps
#define gte_rtps() __asm__ volatile("nop;nop;rtps")

#undef gte_avsz4
#define gte_avsz4() __asm__ volatile("nop;nop;avsz4")

#undef gte_rtpt_b
#define gte_rtpt_b() __asm__ volatile("rtpt")

#undef gte_nclip_b
#define gte_nclip_b() __asm__ volatile("nclip")

#undef gte_avsz3_b
#define gte_avsz3_b() __asm__ volatile("avsz3")

#undef gte_avsz4_b
#define gte_avsz4_b() __asm__ volatile("avsz4")

#undef gte_rtir
#define gte_rtir() __asm__ volatile("nop;nop;rtir")

#undef gte_rtv0tr
#define gte_rtv0tr() __asm__ volatile("nop;nop;rtv0tr")
