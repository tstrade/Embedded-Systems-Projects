#include <stdio.h>
#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"

static void dma_init ( void );
static void timer_init ( void );

/**
 * main.c
 */
int main(void)
{
    dma_init ();
	timer_init ();

	return 0;
}


static void
dma_init ( void )
{
    uint32_t CHANNELS = 0x00040020;                             // Channels 5 & 18 correspond to Timer 0A & GPIO Port B

    *(reg)(GPTM_TIMER0_BASE_ADDR + RCGCDMA) = 0x1;              // Enable system clock for DMA
    *(reg)(UDIR_MEMACC_BASE_ADDR + DMACFG) = 0x1;               // Enable uDMA controller
    *(reg)(UDIR_MEMACC_BASE_ADDR + DMAPRIOSET) = CHANNELS;      // Select default priority
    *(reg)(UDIR_MEMACC_BASE_ADDR + DMAALTCLR) = CHANNELS;       // Select for primary control structure
    *(reg)(UDIR_MEMACC_BASE_ADDR + DMAUSEBURSTSET) = CHANNELS;  // Set burst requests only
    *(reg)(UDIR_MEMACC_BASE_ADDR + DMAREQMASKCLR) = CHANNELS;   // Allow uDMA controller to recognize requests
}

static void
timer_init ( void )
{
    *(reg)(SYS_CONTROL_BASE_ADDR + RCGCTIMER) = 0x1;        // Enable system clock for GPTM
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMCTL) = 0x0;          // Disable Timer 0A
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMCFG) = 0x0;          // Select 32-bit configuration
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMTAMR) = 0x2;         // Set Timer 0 for periodic mode
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMTAILR) = 0x7A1200;   // Trigger event every 0.5s (8e6 cycles)
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMIMR) = 0x1;          // Enable Timer 0A time-out interrupts
    *(reg)(GPTM_TIMER0_BASE_ADDR + GPTMCTL) = 0x1;          // Enable Timer 0A
    *(reg)(CM4_PERIPHS_BASE_ADDR + EN0) = 0x80000U;        // Enable interrupts on vector table
}
