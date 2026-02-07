#include "timer.h"
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

#define PRESCALER   (16)
#define ARR         (1000)


void timer_setup( void ) {
    /* Setup the peripheral clock */
    rcc_periph_clock_enable(RCC_TIM2);

    /* We want 1000Hz with 1000 as the ARR */
    timer_set_prescaler(TIM2, (PRESCALER - 1));
    timer_set_period(TIM2, (ARR - 1));

    /* High level setup of the PWM timer */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);

    /* Enable the timer */
    timer_enable_counter(TIM2);
    timer_enable_oc_output(TIM2, TIM_OC1);

}

void timer_pwm_setup_duty_cycle( float duty_cycle ) {
    const float raw_value = (float)ARR * (duty_cycle / 100.0f);
    timer_set_oc_value(TIM2, TIM_OC1, (uint32_t)raw_value);
}
