#include "tm4c123gh6pm_registers.h"
#include <stdio.h>

static void adc_init ( void );

/**
 * main.c
 */
int main(void)
{
    adc_init ();

    uint32_t value;

    while (1)
    {
        *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCPSSI) = 0x8;
        while ( (*(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCRIS) & 0x8) == 0x0);
        value = *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSSFIFO3) & 0x0FFF;
        *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCISC) = 0x8;
    }
	return 0;
}

static void
adc_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCADC)      = 0x1;      // Provide system clock to ADC0
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     = 0x10;     // Provide system clock to Port E
    asm(" NOP");
    *(uint32_t *)(GPIO_PORTEP_BASE_ADDR + GPIODEN)      = 0x0;      // Disable digital functions
    *(uint32_t *)(GPIO_PORTEP_BASE_ADDR + GPIOAFSEL)    = 0x4;      // Select alternate function for PE2
    *(uint32_t *)(GPIO_PORTEP_BASE_ADDR + GPIOAMSEL)    = 0x4;      // Enable analog function of PE2

    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCACTSS)     = 0x0;      // Disable Sample Sequencer 3
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCEMUX)      = 0x0;      // Select processor as trigger (via ADCPSSI)
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSSMUX3)    = 0x1;      // Use AIN1 as input sample
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSSCTL3)    = 0x6;      // 1st sample is the last in the sequence
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCACTSS)     = 0x8;      // Enable Sample Sequencer 3
}
