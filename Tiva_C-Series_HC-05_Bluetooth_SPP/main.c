#include <inttypes.h>
#include "tm4c123gh6pm_registers.h"
#include "HC05_Commands.h"

extern int strcmp ( char *, char * );
extern char *strcat ( char *, char * );
extern void output_string ( char * );
extern void read_string ( char * );
static void uart_init ( void );
static void hc05_init ( void );

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


static void
hc05_init ( void )
{
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + 0x30)         = 0x4; // Set HC-05's PIO11 high
    asm (" NOP");
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + 0x30)        |= 0x8; // Provide power to HC-05

    send_command (set_module_mode, "1");
    // Wait for response
    send_command (set_serial_parameter, "115200", "1", "0");
    // Wait for response
    // Drop PO11 low then unpower and repower? Or is it unpower, PO11 low, then repower?
}


static void
uart_init ( void )
{
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCUART)     = 0x003; // Provide system clock to UART0, UART1
    *(uint32_t *)(SYS_CONTROL_BASE_ADDR + RCGCGPIO)     = 0x003; // Provide system clock to GPIO Port A, GPIO Port B
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

    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTCTL)      = 0x000; // Disable UART1 control
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTIBRD)     = 0x008; // Set for 115200 baud (for now)
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTFBRD)     = 0x02C; // Set for 115200 baud (for now)
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTCC)       = 0x000; // Use system clock
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTLCRH)     = 0x060; // 8-bit data, stop bit, no parity
    *(uint32_t *)(UART_MODUL1_BASE_ADDR + UARTCTL)      = 0x301; // Enable UART1 Control

    /* PB2, PB3 will be used to control the HC-05's VCC & EN pins */
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIODEN)      = 0x00F; // Enable PB0, PB1, PB3, PB4 digital functions
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOAFSEL)    = 0x003; // Change PB0, PB1, to alternate function
    *(uint32_t *)(GPIO_PORTBP_BASE_ADDR + GPIOPCTL)     = 0x011; // Configure PB0, PB1 for UART
}
