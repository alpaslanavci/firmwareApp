#ifndef SYSTEM_H
#define SYSTEM_H

#include "common-defines.h"


/* System Defines */
#define CLOCK_FREQ      (16000000)
#define AHB_FREQ        (16000000)
#define APB1_FREQ       (16000000)
#define APB2_FREQ       (16000000)
#define SYSTICK_FREQ    (1000)

/* Function Declarations */
void system_setup( void );
uint64_t system_get_ticks( void );
void delay_system( uint64_t milleseconds );

#endif