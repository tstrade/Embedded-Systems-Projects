#include <inttypes.h>
#include <stdio.h>
#include "tm4c123gh6pm_registers.h"

static void adc_init ( void );
extern int poll_adc ( void );

static void ssi_init ( void );
static void timer_init ( void );
static void SPI_7_Seg ( uint32_t, uint32_t, uint32_t, uint32_t );

const uint32_t SEVSEG[16] =
        {
         0xC0,
         0xF9,
         0xA4,
         0xB0,
         0x99,
         0x92,
         0x82,
         0xF8,
         0x80,
         0x90,
         0x88,
         0x83,
         0xC6,
         0xA1,
         0x86,
         0x8E
        };

extern uint32_t ptr_to_buffer;
extern uint32_t ptr_to_display;
extern uint32_t ptr_to_i;

/**
 * main.c
 */
int main(void)
{
    adc_init ();
    *(uint32_t *)ptr_to_i = 0xFF;
    SPI_7_Seg ( 0, 0, 0, 0 );
    ssi_init ();
    timer_init ();

    uint32_t data;

    while (1)
    {
        while (*(uint32_t *)ptr_to_i);
        data = poll_adc ();
        *(uint32_t *)(CM4_PERIPHS_BASE_ADDR + EN0)         &= (~0x80000);
        SPI_7_Seg ( 0, ((data & 0x0F00) >> 8), ((data & 0x00F0) >> 4), (data & 0x000F) );
        *(uint32_t *)ptr_to_i = 0xFF;
        *(uint32_t *)(CM4_PERIPHS_BASE_ADDR + EN0)          = 0x80000;
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
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCSAC)       = 0x6; // Averaging?
    *(uint32_t *)(ANALOG_DIG0_BASE_ADDR + ADCACTSS)     = 0x8;      // Enable Sample Sequencer 3
}


static void
ssi_init ( void )
{
    // Init
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCSSI)     |= 0x4;          // Enable system clock for SSI Module 2
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)    |= 0x6;          // Enable system clock for GPIO Port B and C
    asm (" NOP");
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOAFSEL)    = 0x090;        // Set PB4/7 as alternate function
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOPCTL)     = 0x20020000;   // Configure PB4/7 for SSI functionality
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODEN)      = 0x090;        // Enable PB4/7 digital functions
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODIR)      = 0x090;        // Set PB4/7 as output
    *(uint32_t *)(GPIO_PORTCP_BASE_ADDR + GPIODEN)      = 0x080;        // Enable PC7 digital functions
    *(uint32_t *)(GPIO_PORTCP_BASE_ADDR + GPIODIR)      = 0x080;        // Set PC7 as output

    while ( !(*(uint32_t *)(SYS_CONTROL_BASE_ADDR + PRSSI) & 0x4) );

    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)       = 0x0;      // Configure SSI as master
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICC)        = 0x0;      // Use system clock as baud source
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR0)       = 0x0F;     // Data captured on first clk edge, steady state low, 16-bit data
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICPSR)      = 0x4;      // Set prescale divisor to 4
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)       = 0x2;      // Enable SSI operation
}


static void
timer_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCTIMER)    = 0x1;      // Enable system clock for Timer 0A/B
    asm (" NOP");
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)      = 0x0;      // Disable Timers
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCFG)      = 0x0;      // 32-bit configuration
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAMR)     = 0x2;      // Periodic mode

    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAILR)    = 0x2710;   // Timer 0A load interval = 10,000

    *(uint32_t *)(CM4_PERIPHS_BASE_ADDR + EN0)          = 0x80000;  // Allow processor to be interrupted by timers

    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMIMR)      = 0x1;      // Enable interrupts
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)      = 0x1;      // Enable Timers
}



static void
SPI_7_Seg ( uint32_t DSP1, uint32_t DSP2, uint32_t DSP3, uint32_t DSP4 )
{
    *(uint32_t *)(ptr_to_display)       = ptr_to_buffer;

    *(uint32_t *)(ptr_to_buffer)        = ((SEVSEG[DSP1] << 24) | 0x00080000);
    *(uint32_t *)(ptr_to_buffer)       |= ((SEVSEG[DSP2] <<  8) | 0x00000004);
    *(uint32_t *)(ptr_to_buffer + 4)    = ((SEVSEG[DSP3] << 24) | 0x00020000);
    *(uint32_t *)(ptr_to_buffer + 4)   |= ((SEVSEG[DSP4] <<  8) | 0x00000001);
}

