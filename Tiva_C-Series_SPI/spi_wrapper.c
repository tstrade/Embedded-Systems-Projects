#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"

static void ssi_init ( void );

const uint32_t SSI2Clk  = (uint32_t)(0x010);
const uint32_t SSI2Fss  = (uint32_t)(0x020);
const uint32_t SSI2Rx   = (uint32_t)(0x040);
const uint32_t SSI2Tx   = (uint32_t)(0x080);
const uint32_t PB_PINS  = (uint32_t)(0x0F0);
const uint32_t SSI_MOD  = (uint32_t)(0x040);
const uint32_t PB_CTL   = (uint32_t)(0x22220000);


int 
main ( void )
{
    return 0;
}

static void
ssi_init ( void )
{
    // Init
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCSSI)     |= 0x4;      // Enable system clock for SSI Module 2
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)    |= 0x2;      // Enable system clock for GPIO Port B
    asm (" NOP");
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOAFSEL)    = PB_PINS;  // Set PB4-7 as alternate function
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOPCTL)     = PB_CTL;   // Configure PB4-7 for SSI functionality
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODEN)      = PB_PINS;  // Enable PB4-7 digital functions
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODIR)      = 0x0B0;    // Set P4,P5,P7 as output and P6 as input
    // PB4 = I/O (clock)
    // PB5 = I/O (frame signal)
    // PB6 = I   (receive)
    // PB7 = O   (transmit)
    while ( !(*(uint32_t *)(SYS_CONTROL_BASE_ADDR + PRSSI) & 0x040) );

    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)      &= ~(0x020); // Ensure SSI is disabled
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)       = 0x0;      // Configure SSI as master
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICC)        = 0x0       // Use system clock os baud source
    // SSInClk = SysClk / (CPSDVSR * (1 + SCR))
    // SRC is programmed in SSICR0
    // Must be an even number
    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICPSR)      = 0x2;      // Set prescale divisor to 2

    // Bit Rate = SysClk / (CPSDVSR * (1 + SCR))
    //*(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR0)       = 
    // SPH = 0
    // SPO = 0
    // FRF = ?
    // DSS = 3 (value + 1 = 4-bit data size)

    *(uint32_t *)(SYNC_SIMOD2_BASE_ADDR + SSICR1)       = 0x020;    // Enable SSI operation
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