/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DELAY_H
#define _LINUX_DELAY_H

/*
 * Copyright (C) 1993 Linus Torvalds
 *
 * Delay routines, using a pre-computed "loops_per_jiffy" value.
 *
 * Please note that ndelay(), udelay() and mdelay() may return early for
 * several reasons:
 *  1. computed loops_per_jiffy too low (due to the time taken to
 *     execute the timer interrupt.)
 *  2. cache behaviour affecting the time it takes to execute the
 *     loop function.
 *  3. CPU clock rate changes.
 *
 * Please see this thread:
 *   http://lists.openwall.net/linux-kernel/2011/01/09/56
 */

#include <linux/kernel.h>

extern unsigned long loops_per_jiffy;

#include <asm/delay.h>

/*
 * Using udelay() for intervals greater than a few milliseconds can
 * risk overflow for high loops_per_jiffy (high bogomips) machines. The
 * mdelay() provides a wrapper to prevent this.  For delays greater
 * than MAX_UDELAY_MS milliseconds, the wrapper is used.  Architecture
 * specific values can be defined in asm-???/delay.h as an override.
 * The 2nd mdelay() definition ensures GCC will optimize away the 
 * while loop for the common cases where n <= MAX_UDELAY_MS  --  Paul G.
 */

#ifndef MAX_UDELAY_MS
#define MAX_UDELAY_MS	5
#endif

#define MAX_MDELAY	1000

#ifndef CONFIG_SAMSUNG_PRODUCT_SHIP
extern unsigned long sec_delay_check;

#define BUG_MDELAY_LOG(irq, atomic)  if (unlikely(irq) || unlikely(atomic)) { \
        if (irq) \
            pr_err("BUG: mdelay called @ IRQ disabled context\n"); \
        if (atomic) \
            pr_err("BUG: mdelay called @ non-Atomic context\n"); \
    }
#endif

#ifndef mdelay
#ifdef CONFIG_SAMSUNG_PRODUCT_SHIP
#define mdelay(n) (\
	(__builtin_constant_p(n) && (n)<=MAX_UDELAY_MS) ? udelay((n)*1000) : \
	({unsigned long __ms=(n); while (__ms--) udelay(1000);}))
#else
#define mdelay(n) ({\
    BUILD_BUG_ON(__builtin_constant_p(n) && (n) > MAX_MDELAY); \
    if (__builtin_constant_p(n) && (n) <= MAX_UDELAY_MS) \
        udelay((n) * 1000); \
    else {\
        unsigned long __ms = (n); \
        bool irq_cond = irqs_disabled();\
        bool atomic_cond = !in_atomic();\
        BUG_MDELAY_LOG(irq_cond, atomic_cond);\
        BUG_ON((irq_cond || atomic_cond) && sec_delay_check);\
        while (__ms--) \
            udelay(1000); \
        } \
    })
#endif
#endif

#ifndef ndelay
static inline void ndelay(unsigned long x)
{
	udelay(DIV_ROUND_UP(x, 1000));
}
#define ndelay(x) ndelay(x)
#endif

extern unsigned long lpj_fine;
void calibrate_delay(void);
void msleep(unsigned int msecs);
unsigned long msleep_interruptible(unsigned int msecs);
void usleep_range(unsigned long min, unsigned long max);

static inline void ssleep(unsigned int seconds)
{
	msleep(seconds * 1000);
}

#endif /* defined(_LINUX_DELAY_H) */
