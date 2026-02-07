#include "system.h"
#ifndef SYSTEM_H
#error "system.h not included properly"
#endif
#include "timer.h"
#include "uart.h"
#include <libopencm3/stm32/l0/rcc.h>
#include <libopencm3/stm32/l0/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>

#define BOOTLOADER_SIZE (0x8000U)
#define LED_PORT        (GPIOA)
#define LED_PIN         (GPIO5)

#define UART_PORT       (GPIOA)
#define UART_TX_PIN     (GPIO2)
#define UART_RX_PIN     (GPIO3)


static void vector_setup( void ) {
    SCB_VTOR = BOOTLOADER_SIZE;
}

static void gpio_setup( void ) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_af(LED_PORT, GPIO_AF5, LED_PIN);

    gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_TX_PIN | UART_RX_PIN);
    gpio_set_af(UART_PORT, GPIO_AF4, UART_TX_PIN | UART_RX_PIN);
}


int main ( void ) {
    vector_setup();
    system_setup();
    gpio_setup();
    timer_setup();
    uart_setup();
    uart_write((uint8_t*)"UART OK\r\n", 9);

    uint64_t start_time = system_get_ticks();
    float duty_cycle = 0.0f;
    bool zero_flag = true;
    bool full_flag = false;
    timer_pwm_setup_duty_cycle(duty_cycle);

    while(1) {
        if(system_get_ticks() - start_time >= 10) {
            if (zero_flag == true || full_flag == false) {
                if (duty_cycle == 100.0f) {
                    zero_flag = false;
                    full_flag = true;
                } else {
                    duty_cycle += 1.0f;
                    timer_pwm_setup_duty_cycle(duty_cycle);
                }
            } else if (zero_flag == false || full_flag == true) {
                if (duty_cycle == 0.0f) {
                    zero_flag = true;
                    full_flag = false;
                } else {
                    duty_cycle -= 1.0f;
                    timer_pwm_setup_duty_cycle(duty_cycle);
                }
            }

            start_time = system_get_ticks();
        }

        if(uart_data_available()){
            uint8_t data = uart_read_byte();
            uart_write_byte(data + 1);
        }

        delay_system(1000);
    }

    return 0;
}