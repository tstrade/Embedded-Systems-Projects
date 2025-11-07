#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"

static void ssi_init ( void );
static void timers_init ( void );

const uint32_t LATCH = (GPIO_PORTCP_BASE_ADDR + 0x200);
const uint32_t SEVSEG_0 = 0xC0;
const uint32_t SEVSEG_1 = 0xF9;
const uint32_t SEVSEG_2 = 0xA4;
const uint32_t SEVSEG_3 = 0xB0;
const uint32_t SEVSEG_4 = 0x99;
const uint32_t SEVSEG_5 = 0x92;
const uint32_t SEVSEG_6 = 0x82;
const uint32_t SEVSEG_7 = 0xF8;
const uint32_t SEVSEG_8 = 0x80;
const uint32_t SEVSEG_9 = 0x90;


int
main ( void )
{
    ssi_init ();
    return 0;
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
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)       = 0x3;      // Enable SSI operation and loopback-mode
}



static void
timer_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCTIMER)    = 0x1;      // Enable system clock for Timer 0A/B
    asm (" NOP");
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)      = 0x0;      // Disable Timers
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCFG)      = 0x0;      // 32-bit configuration
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAMR)     = 0x2;      // Periodic mode
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTBMR)     = 0x2;

    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAILR)    = 0x2710;   // Timer 0A load interval = 10,000
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTBILR)    = 0x1388;   // Timer 0B load interval = 5,000

    *(uint32_t *)(CM4_PERIOHS_BASE_ADDR + EN0)          = 0x180000; // Allow processor to be interrupted by timers

    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMIMR)      = 0x101;    // Enable interrupts
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)      = 0x1;      // Enable Timers

}



/*
    Initialization and Configuration (TM4C123GH6PM Datasheet, pg 965-967)

    !! Must enable clock and read Rn bit of PRSSI as 0x1 before SSI registers are accessed !!


    Init:
        1. Enable SSI module via RCGCSSI (pg. 346)

        2. Enable clock to GPIO Port B via RCGCGPIO (pg. 340; Table 23-5, pg. 1351)

        3. Set GPIOAFSEL for PB4-PB7 (pg. 672)
        4. Configure PMCn fields via GPIOPCTL to assign SSI signals (pg. 688; Table 10-2, pg. 650)

        5. Set GPIODEN and configure drive strength, drain select, pull-up/pull-down (pg. 682)
            // GPIO Drive select (pg. 673)
            // GPIO drain select?
            // GPIO pull-up/pull-down (pg. 677-680)

        Note: Pull-ups can be used to avoid unnecessary toggles on the SSI pins, which can take the
                slave to a wrong state. In addition, if the SSIClk signal is programmed to steady state
                High through the SPO bit in the SSICR0 register, then software must also configure the
                GPIO port pin corresponding to the SSInClk signal as a pull-up in the GPIO Pull-Up
                Select (GPIOPUR) register.


    Config:
        1. Ensure SSE bit in SSICR1 is cleared (pg. 971)

        2. Select whether SSI is master or slave via SSICR1:
                Master           = 0x0000.0000
                Slave w/ output  = 0x0000.0004
                Slave w/o output = 0x0000.000C

        3. Configure SSI clock source via SSICC (pg. 984)

        4. Configure clock prescale divisor via SSICPSR (pg. 976)

        5. Write SSICR0 with the following: (pg. 969)
                Serial clock rate (SCR)
                Desired clock phase / polarity, if using Freescale SPI (SPH and SPO)
                Protocol mode: Freescale SPI, TI SSF< MICROWIRE (FRF)
                Data size (DSS)

        6. Optionally, SSI module for uDMA use:
                Configure uDMA for SSI use (pg. 585)
                Enable SSI module's TX FIFO or RX FIFO via TXDMAE or RXDMAE in SSIDMACTL

        7. Enable SSI by setting SSE bit in SSICR1 (pg. 971)

*/


/*
    Alice EduBase board incorporates 2 daisy chained HCT595 shift registers
        to multiplex the 7-segment displays.
    1st HCT595 drives anodes, 2nd HCT595 drives cathodes

    (EduBase User Guide, pg. 8)
*/



/*
    Frame Formats (§15.3.4, pg. 956-964)

    *** TI Synchronous Serial ***

    > SSInFss pin is pulsed for one serial clock period starting at its rising edge
        prior to transmission of each frame
        >> The LSB of any given frame is indicated by a pulse for one serial clock period

    > Output data is driven on rising edge of SSInClk and latch data on falling edge





    Freescale SPI


    MICROWIRE

*/


/*
            a
        ---------
        |       |
      f |       | b
        |   g   |
        ---------
        |       |
      e |       | c
        |   d   |
        ---------
            |
    Cathode |


    Segment A, B, C, D, E, F, G, and Decimal Point are driven by
        QA, QB, QC, QD, QE, QF, QG, QH, respectively

    -------------------------------------------------------
    | Number | DP | G | F | E | D | C | B | A | Hex Value |
    -------------------------------------------------------
    |    1   |  0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |    0x06   |
    -------------------------------------------------------
    |    2   |  0 | 1 | 0 | 1 | 1 | 0 | 1 | 1 |    0x5B   |
    -------------------------------------------------------
    |    3   |  0 | 1 | 0 | 0 | 1 | 1 | 1 | 1 |    0x4F   |
    -------------------------------------------------------
    |    4   |  0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 |    0x66   |
    -------------------------------------------------------


    (EduBase User Guide, pg. 9)
*/



/*
    Digit 3, 2, 1, and 0 are driven by QA, QB, QC, and QD of the
        second HCT595, respectively.
    Ex.) Display 1234

        1. Output 0x06 to 1st HCT595, then 0x07 to 2nd HCT595
            to set QA low and QB, QC, and QD high (#1)

        2. Delay 1-5ms

        3. Output 0x5B to 1st HCT595, then 0x0B to 2nd HCT595
            to set QB low and QA, QC, and QD high (#2)

        4. Delay 1-5ms

        5. Output 0x4F to 1st HCT595, then 0x0D to 2nd HCT595
            to set QC low and QA, QB, and QD high (#3)

        6. Delay 1-5ms

        7. Output 0x66 to 1st HCT595, then 0x0E to 2nd HCT595
            to set QD low and QA, QB, and QC high (#4)

        8. Delay 1-5ms

        9. Return to Step 1

    (EduBase User Guide, pg. 10)
*/
