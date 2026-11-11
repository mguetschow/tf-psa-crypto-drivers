/*
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CC3XX_TEST_UTILS_H__
#define __CC3XX_TEST_UTILS_H__

#include <stdint.h>
#include <stddef.h>

#include "test_framework.h"
#include "tfm_hal_device_header.h"
#include "tfm_utils.h"

#define TEST_ASSERT(cond, msg) \
    if (!(cond)) { \
        TEST_FAIL(msg); \
        return; \
    }

#define TEST_SETUP(x) TEST_ASSERT((x) == 0, "Test setup failed")
#define TEST_TEARDOWN(x) TEST_ASSERT((x) == 0, "Test teardown failed")

#ifdef __cplusplus
extern "C" {
#endif

void cc3xx_add_tests_to_testsuite(struct test_t *test_list, uint32_t test_am,
                                  struct test_suite_t *p_ts, uint32_t ts_size);

static inline uint32_t pmod(int32_t num, int32_t N)
{
    return (((num % N) + N) % N);
}

// inspired by CMSIS/DSP/Testing/FrameworkSource/Timing.cpp
#define SYSTICK_INITIAL_VALUE       0xFFFFFF
static unsigned int startCycles;

static inline void enable_cycle_counter(void)
{
    SysTick->VAL  = 0;
    SysTick->LOAD = SYSTICK_INITIAL_VALUE;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while(SysTick->VAL == 0);
    startCycles = SysTick->VAL;
}

static inline uint32_t get_cycle_count(void)
{
    uint32_t v = SysTick->VAL;
    return (v < startCycles) ?
        startCycles - v :
        SYSTICK_INITIAL_VALUE - (v - startCycles);
}

static inline void reset_cycle_count(void)
{
    SysTick->CTRL = 0;
    enable_cycle_counter();
}

static inline uint32_t cycles_to_ms(uint32_t cycles)
{
    uint64_t elapsed_time_ms;

    elapsed_time_ms = ((uint64_t) cycles * 1e3) / 64000000; // todo: double-check or better find macro

    return elapsed_time_ms;
}

#ifdef __cplusplus
}
#endif

#endif /* __CC3XX_TEST_UTILS_H__ */
