#include "tm4c123gh6pm_registers.h"

static void adc_init ( void );

/**
 * main.c
 */
int main(void)
{
    adc_init ();
    while (1);
	return 0;
}

static void
adc_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCADC)      = 0x3;      // Provide system clock to both modules for now
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     = 0x10;     // Provide system clock to Port E
    asm(" NOP");
    *(uint32_t *)(GPIO_PORTEP_BASE_ADDR + GPIODEN)      = 0x0;      // Disable digital functions
    *(uint32_t *)(GPIO_PORTEP_BASE_ADDR + GPIOAMSEL)    = 0x4;      // Enable analog function of PE2

    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCACTSS)    &= (~0x2);   // Disable Sample Sequencer 1
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCEMUX)     &= (~0xF0);  // Select processor as trigger (via ADCPSSI)
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSSMUX1)    = 0x1;      // Use AIN1 as input sample
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSSCTL1)    = 0x2;      // 1st sample is the last in the sequence
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCACTSS)    |= 0x2;      // Enable Sample Sequencer 1
}

/*
\

    7. Enable the sample sequencer logic by setting the corresponding ASENn bit in the ADCACTSS
        register
 */
