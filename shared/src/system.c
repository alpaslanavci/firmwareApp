#include "system.h"
#include <libopencm3/stm32/l0/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>

static volatile uint64_t ticks = 0;

void sys_tick_handler( void ) {
    ticks++;
}

static void rcc_setup( void ) {
    struct rcc_clock_scale pll_setup = {
        .pll_mul = RCC_CFGR_PLLMUL_MUL4,
        .pll_div = RCC_CFGR_PLLDIV_DIV4,
        .pll_source = RCC_CFGR_PLLSRC_HSI16_CLK,
        .flash_waitstates = 0,
        .voltage_scale = PWR_SCALE1,
        .hpre = RCC_CFGR_HPRE_NODIV,
        .ppre1 = RCC_CFGR_PPRE1_NODIV,
        .ppre2 = RCC_CFGR_PPRE2_NODIV,
        .ahb_frequency = AHB_FREQ,
        .apb1_frequency = APB1_FREQ,
        .apb2_frequency = APB2_FREQ,
    };

    rcc_clock_setup_pll(&pll_setup);
}

static void systick_setup( void ) {
    systick_set_frequency(SYSTICK_FREQ, AHB_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

uint64_t system_get_ticks( void ) {
    return ticks;
}

void system_setup( void ) {
    rcc_setup();
    systick_setup();
}

void delay_system( uint64_t milleseconds ) {
    uint64_t end_time = system_get_ticks() + milleseconds;
    while (system_get_ticks() < end_time) {
        // do nothing
    }
}