/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_KASAN_H
#define __ASM_KASAN_H

#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/memory.h>
#include <asm/pgtable-types.h>

/*
 * With HAKC (CONFIG_PAC_MTE_COMPART), __tag_get returns the real top byte of
 * an address, but KASAN_GENERIC expects arch_kasan_get_tag to return 0 (no
 * tagging). Force the KASAN tag helpers to be no-ops when GENERIC is on, so
 * kasan_unpoison_shadow does not write the kernel-VA top byte (0xff) into
 * shadow and falsely mark every fresh allocation as freed.
 */
#if defined(CONFIG_KASAN_GENERIC) && !defined(CONFIG_KASAN_SW_TAGS)
#define arch_kasan_set_tag(addr, tag)	((void *)(addr))
#define arch_kasan_reset_tag(addr)	((void *)(addr))
#define arch_kasan_get_tag(addr)	0
#else
#define arch_kasan_set_tag(addr, tag)	__tag_set(addr, tag)
#define arch_kasan_reset_tag(addr)	__tag_reset(addr)
#define arch_kasan_get_tag(addr)	__tag_get(addr)
#endif

#ifdef CONFIG_KASAN

/*
 * KASAN_SHADOW_START: beginning of the kernel virtual addresses.
 * KASAN_SHADOW_END: KASAN_SHADOW_START + 1/N of kernel virtual addresses,
 * where N = (1 << KASAN_SHADOW_SCALE_SHIFT).
 *
 * KASAN_SHADOW_OFFSET:
 * This value is used to map an address to the corresponding shadow
 * address by the following formula:
 *     shadow_addr = (address >> KASAN_SHADOW_SCALE_SHIFT) + KASAN_SHADOW_OFFSET
 *
 * (1 << (64 - KASAN_SHADOW_SCALE_SHIFT)) shadow addresses that lie in range
 * [KASAN_SHADOW_OFFSET, KASAN_SHADOW_END) cover all 64-bits of virtual
 * addresses. So KASAN_SHADOW_OFFSET should satisfy the following equation:
 *      KASAN_SHADOW_OFFSET = KASAN_SHADOW_END -
 *				(1ULL << (64 - KASAN_SHADOW_SCALE_SHIFT))
 */
#define _KASAN_SHADOW_START(va)	(KASAN_SHADOW_END - (1UL << ((va) - KASAN_SHADOW_SCALE_SHIFT)))
#define KASAN_SHADOW_START      _KASAN_SHADOW_START(vabits_actual)

void kasan_init(void);
void kasan_copy_shadow(pgd_t *pgdir);
asmlinkage void kasan_early_init(void);

#else
static inline void kasan_init(void) { }
static inline void kasan_copy_shadow(pgd_t *pgdir) { }
#endif

#endif
#endif
