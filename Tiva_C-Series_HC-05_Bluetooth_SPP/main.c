#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"
#include "HC05_Commands.h"

extern int strcmp ( char *, char * );
extern char *strcat ( char *, char * );
extern void output_string ( char * );
extern void read_string ( char * );
static void uart_init ( void );

/**
 * main.c
 */
int main(void)
{
	uart_init ();

	output_string ("\f");
	send_command (set_module_name, "AKATOSH");

	output_string ("Done!\r\n");

	return 0;
}

/*
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
*/

static void
uart_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCUART)     = 0x001; // Provide system clock to UART0
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     = 0x001; // Provide system clock to GPIO Port A
    asm (" NOP");
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCTL)      = 0x000; // Disable UART0 Control
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTIBRD)     = 0x008; // Set for 115200 baud
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTFBRD)     = 0x02C; // Set for 115200 baud
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCC)       = 0x000; // Use system clock
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTLCRH)     = 0x060; // 8-bit data, 1 stop bit, no parity
    *(uint32_t *)(UART_MODUL0_BASE_ADDR + UARTCTL)      = 0x301; // Enable UART0 Control

    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIODEN)      = 0x003; // Enable PA0, PA1 digital functions
    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIOAFSEL)    = 0x003; // Change PA0, PA1 to alternate function
    *(uint32_t *)(GPIO_PORTAP_BASE_ADDR + GPIOPCTL)     = 0x011; // Configure PA0, PA1 for UART
}
