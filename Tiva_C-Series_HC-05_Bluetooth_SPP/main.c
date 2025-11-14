#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"
#include "HC-05_Commands.h"

static void uart_init ( void );

int
main ( )
{
    return 0;
}

static void
uart_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCUART)     = 0x3; // Provide system clock to UART0/1
    // Enable clock for appropriate GPIO modules
    asm (" NOP");
    // Set GPIOAFSEL appropriately
    // Configure GPIOPCTL[PMCn]
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCTL)      = 0x0; // Disable UART0 control
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTCTL)      = 0x0; // Disable UART1 control
}