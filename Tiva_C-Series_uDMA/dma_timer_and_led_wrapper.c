#include <stdio.h>
#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"

static void dma_init ( void );
static void timer_init ( void );
static void gpio_init ( void );
static void uart_init ( void );

extern uint32_t ptr_to_channel_control;
extern uint32_t ptr_to_channel_src;

#define ASCII_ESC (0x01B)
/**
 * main.c
 */
int main(void)
{
    gpio_init ();
    uart_init ();
    dma_init ();
    timer_init ();


    while (1);

    return 0;
}




static void
timer_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCTIMER)  |= 0x1;      // Enable system clock for GPTM
    asm (" NOP");
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)     = 0x0;      // Disable Timer 0A
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCFG)     = 0x0;      // Select 32-bit configuration
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAMR)   |= 0x2;      // Set Timer 0 for periodic mode
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMTAILR)   = 0xF42400; // Trigger event every 0.5s (8e6 cycles)
    *(uint32_t *)(CM4_PERIPHS_BASE_ADDR + EN0)        |= 0x80000U; // Enable interrupts on vector table
    *(uint32_t *)(GPTM_TIMER0_BASE_ADDR + GPTMCTL)    |= 0x1;      // Enable Timer 0A
}

static void
gpio_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     |= 0x2;     // Enable system clock for GPIO Port B
    asm (" NOP");
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODIR)      |= 0x0F;    // Set PB0-3 as output
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOAFSEL)    &= (~0x0F); // Use as GPIO pins
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOPDR)      |= 0x0F;    // Use pull-down resistor
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODEN)      |= 0x0F;    // Set digital enable
}

static void
uart_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCUART)     |= 0x1;   // Enable system clock for UART0
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     |= 0x1;   // Enable system clock for GPIO Port A
    asm (" NOP");
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCTL)       = 0x0;   // Disable UART0 Control
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTIBRD)      = 0x8;   // Set for 115200 baud
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTFBRD)      = 0x2C;  // Set for 115200 baud
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCC)        = 0x0;   // Use system clock

    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIODEN)      |= 0x3;   // Make PA0-1 digital ports
    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIOAFSEL)    |= 0x3;   // Set PA0-1 as alt. function
    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIOPCTL)     |= 0x11;  // Configure PA0-1 for UART
}

static void
dma_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCDMA)         = 0x1;      // Enable system clock for DMA
    asm (" NOP");
    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMACFG)          = 0x1;      // Enable uDMA controller
    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMACTLBASE)      = ptr_to_channel_control;
    *(uint32_t *)(ptr_to_channel_control + 0x120)          = ptr_to_channel_src + 0x0F;
    *(uint32_t *)(ptr_to_channel_control + 0x124)          = 0x4000503C;
    *(uint32_t *)(ptr_to_channel_control + 0x128)          = 0xC00000F0;

    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMAALTCLR)       = 0x40000; // Select for primary control structure
    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMAREQMASKCLR)   = 0x40000; // Allow uDMA controller to recognize requests
    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMAUSEBURSTSET)  = 0x40000; // Set burst requests only
    *(uint32_t *)(CM4_PERIPHS_BASE_ADDR + EN1)             = 0x4000;  // Allow DMA to interrupt on completion
    *(uint32_t *)(UDIR_MEMACC_BASE_ADDR + DMAENASET)       = 0x40000; // Enable Channel 18
    *(uint32_t *)(ptr_to_channel_control + 0x128)         |= 0x1;
}

/*
    ; LEDs on Port B, Pins 0,1,2,3

    ; GPTM 16/32-bit Timer 0 base: 0x40030000
    ;   Dedicated channel for each timer
    ;   Burst request generated on timer interrupt (pg. 705)
    ;   Use GPIODATA offset 0x03C to control PB0-PB3 (pg. 654)

    ; GPTimer 0A is burst only, Channel 18, End. 0 (pg. 587)

    ;   "When using uDMA to transfer data to and from a peripheral,
    ;    the peripheral must disable all interrupts to the NVIC (pg. 599)
    ;
    ;    When transfer is complete, uDMA controller generates an interrupt (pg. 600)"


    ; Initialization and Configuration (pg. 600-601)
    ;   1.) Enable uDMA clock using RCGCDMA register (pg. 342)


    ;   2.) Enable uDMA controller by setting the MASTEREN bit
    ;        of the DMA Configuration (DMACFG) register


    ;   3.) Program location of channel control table by writing
    ;        the base address of the table to the DMA Channel Control
    ;        Base Pointer (DMACTLBASE) register. The base address
    ;        must be aligned on a 1024-byte boundary (pg. 619)


    ; Configure the channel attributes (Table 9-3, pg. 590)
    ;   1.) Program Bit 5 and 18 DMA Channel Priority Set (DMAPRIOSET) or
    ;        DMA Channel Priority Clear (DMAPRIOCLR) registers to
    ;        set the channel either to high or default priority (pg.632)

    ;   2.) Set Bit 5 and 18 of the DMA Channel Primary Alternate Clear
    ;        (DMAALTCLR) register to select the primary channel
    ;        control structure for this transfer (pg. 630)

    ;   3.) Set Bit 5 and 18 of the DMA Channel Useburst SET (DMAUSEBURSTSET)
    ;        register to allow the uDMA controller to respond (pg. 623)
    ;        to burst requests only

    ;   4.) Set Bit 5 of the DMA Channel Request Mask Clear
    ;        (DMAREQMASKCLR) register to allow the uDMA controller
    ;        to recognize requests for this channel (pg. 626)


    ; Control Structure Memory Map (Table 9-3, pg. 590)
    ;
    ;   Must allocate first have of control table (0x000 - 0x200)
    ;   Offsets = (Channel) # << 4

    ; Channel Control Structure (Table 9-4, pg. 590)
    ;
    ;   Offset - Description
    ;   0x000  - Source End Pointer
    ;               Non-incrementing, point to transfer addr.
    ;   0x004  - Destination End Pointer
    ;               Non-incrementing, point to destination addr.
    ;   0x008  - Control Word
    ;   0x00C  - Unused


    ; Channel Control Word (pg. 611-614)
    ;
    ;   DSTINC      (31:30) = 0b11 -> No dest. addr increment
    ;   DSTSIZE     (28:28) = 0b00 -> 8-bit dest. data size
    ;   SRCINC      (27:26) = 0b00 -> 8-bit src. addr increment
    ;   SRCSIZE     (25:24) = 0b00 -> 8-bit src. data size
    ;   Reserved    (23:18)
    ;   ARBSIZE     (17:14) = 0x00 -> Arbitrates after 1 transfer
    ;   XFERSIZE    (13:4)  = 0x10 -> Transfer 16 items
    ;   NXTUSEBURST (3)     = 0b00 -> N/A
    ;   XFERMODE    (2:0)   = 0b01 -> Use basic transfer mode
    ;
    ;   >>> Control Word = 0xC0000101
    ;
    ;       ! Must be reconfigured between each transfer !
    ;       ! (src/dest pointers will not change)        !
    ;
    ;       ! Channel is automatically disabled after a  !
    ;       ! transfer, must be enabled each time        !
*/
