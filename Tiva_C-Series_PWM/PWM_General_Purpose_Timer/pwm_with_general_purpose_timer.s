	.data

start_prompt:	.string 0xC, 0xD, 0xA, 0x9, 0x9, 0x9
				.string ">>> ADVANCED RGB LED <<<"
				.string 0xD, 0xA, 0x9, 0x9
				.string "When prompted, select a color by entering its hex value "
				.string "in the form of 0x------"
				.string 0xD, 0xA, 0x9
				.string "Press space to continue, and ESC to quit anytime."
				.string 0xD, 0xA, 0xD, 0xA, 0x9, 0


goodbye:		.string 0xD, 0xA, 0xD, 0xA, 0x9, 0x9
				.string "Goodbye!", 0

input_prompt:	.string 0xD, 0xA, 0x9
				.string "Input Color Hex Value: ", 0

input_buffer:	.string "0x------", 0

rgb_prev_state:		.byte 0x00

rgb_input:			.word 0x000000

red_duty_cycle: 	.byte 0x00
blue_duty_cycle:	.byte 0x00
green_duty_cycle:	.byte 0x00

prog_status:		.byte 0x00

reset_rgb:			.word 0x000000

	.text

	.global Advanced_RGB_LED
	.global illuminate_RGB_LED
	.global rgb_led_init
	.global pwm_with_gpt

	.global uart_interrupt_init
	.global timer_interrupt_init
	.global uart_init
	.global output_character
	.global read_character
	.global read_string
	.global output_string
	.global simple_read_character

	.global string2int
	.global int2string
	.global division
	.global multiplication
	.global newline

ptr_to_start_prompt:	.word start_prompt
ptr_to_goodbye:			.word goodbye
ptr_to_input_prompt:	.word input_prompt
ptr_to_input_buffer:	.word input_buffer
ptr_to_rgb_prev_state:	.word rgb_prev_state
ptr_to_rgb_input:		.word rgb_input
ptr_to_red_dc:			.word red_duty_cycle
ptr_to_blue_dc:			.word blue_duty_cycle
ptr_to_green_dc:		.word green_duty_cycle
ptr_to_reset_rgb:		.word reset_rgb
ptr_to_prog_status:		.word prog_status


; General Purpose Timers
GPTMCFG:          .equ 0x000	; GPTM Configuration
GPTMTAMR:         .equ 0x004	; GPTM Timer A Mode
GPTMCTL:          .equ 0x00C	; GPTM Control
GPTMIMR:          .equ 0x018	; GPTM Interrupt Mask
GPTMICR:	      .equ 0x024	; GPTM Interrupt Clear Register
GPTMTAILR:        .equ 0x028	; GPTM Timer A Interval Load
GPTMTAR:          .equ 0x048	; GPTM Timer A

; General Purpose Input / Output
GPIODATA_A:       .equ 0x3F0	; GPIO Data Register Port A
GPIODATA_D:		  .equ 0x03C	; GPIO Data Register Port D
GPIODIR:          .equ 0x400	; GPIO Direction Register

GPIOIS:		      .equ 0x404	; GPIO Interrupt Sense
GPIOIBE:	      .equ 0x408	; GPIO Interrupt Both Edges
GPIOIEV:	      .equ 0x40C	; GPIO Interrupt Event
GPIOIM:		      .equ 0x410	; GPIO Interrupt Mask
GPIORIS:		  .equ 0x414	; GPIO Raw Interrupt Status
GPIOICR:	      .equ 0x41C	; GPIO Interrupt Clear Register

GPIOAFSEL:		  .equ 0x420	; GPIO Alternate Function Select
GPIOPUR:          .equ 0x510	; GPIO Pull-Up Resistor Register
GPIOPDR:		  .equ 0x514	; GPIO Pull-Down Resistor Register
GPIODEN:          .equ 0x51C	; GPIO Digital Register
GPIOCTL:		  .equ 0x52C	; GPIO Port Control
GPIORCGC:         .equ 0x608   	; GPIO Run Mode Clock Gating Control

; Universal Asynchronous Receiver/Transmitters
UARTDR:			  .equ 0x000	; UART Data Register
U0FR: 			  .equ 0x018	; UART0 Flag Register
UARTIBRD: 		  .equ 0x024	; UART Integer Baud-Rate Divisor
UARTFBRD: 		  .equ 0x028	; UART Fractional Buad-Rate Divisor
UARTLCRH:		  .equ 0x02C	; UART Line Control
UARTCTL:		  .equ 0x030	; UART Control
UARTIM:           .equ 0x038	; UART Interrupt Mask
UARTICR:          .equ 0x044	; UART Interrupt Clear Register
UARTRCGC:		  .equ 0x618	; UART Run Mode Clock Gating Control
UARTCC:			  .equ 0xFC8	; UART Clock Configuration

RXIM:             .equ 0x010	; UART Receive Interrupt Mask
RXIC:             .equ 0x010	; UART Receive Raw Interrupt Status


RCGCTIMER:        .equ 0x604	; 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control

; Cortex M4 Peripherals
ENUART0:          .equ 0x020	; UART0 Enable Bit
ENGPIOD:		  .equ 0x008	; GPIO Port D Enable Bit
EN0:              .equ 0x100	; Interrupt 0-31 Set Enable

; Misc.
NULL:			   .equ 0x000

; Program-specific
GPIO_PORTF_RCGC:  .equ 0x020	; GPIO Port F Enable RCGC Mask
RGB_LED_PINS:  	  .equ 0x00E	; GPIO Port F Pins 1-3 Mask
RED_LED:		  .equ 0x002	; PF1 (Red) Mask
BLUE_LED:		  .equ 0x004	; PF2 (Blue) Mask
GREEN_LED:		  .equ 0x008	; PF3 (Green) Mask

WAIT_START:		  .equ 0x000
WAIT_INPUT:		  .equ 0x001
USER_EXIT:		  .equ 0x002


; ASCII
ASCII_ESC:		   .equ 0x01B
ASCII_SPACE:	   .equ 0x020
ASCII_ZERO:		   .equ 0x030
ASCII_NINE:		   .equ 0x039
ASCII_x:		   .equ 0x078

ASCII_A:		   .equ 0x041
ASCII_B:		   .equ 0x042
ASCII_C:		   .equ 0x043
ASCII_D:		   .equ 0x044
ASCII_E:		   .equ 0x045
ASCII_F:		   .equ 0x046


rgb_led_init:
	PUSH {lr}
	; PF1 = RED
	; PF2 = BLUE
	; PF3 = GREEN

	; System control base address
	MOV r0, #0xE000
	MOVT r0, #0x400F

	; Set GPIORCGC[5] to enable clock
	;	for GPIO Port F
	LDR r1, [r0, #GPIORCGC]
	ORR r1, r1, #GPIO_PORTF_RCGC
	STR r1, [r0, #GPIORCGC]
	nop

	; GPIO Port F base address
	MOV r0, #0x5000
	MOVT r0, #0x4002

	; Set PF1-3 as output
	LDR r1, [r0, #GPIODIR]
	ORR r1, r1, #RGB_LED_PINS
	STR r1, [r0, #GPIODIR]

	; Set PF1-3 Digital Enable
	LDR r1, [r0, #GPIODEN]
	ORR r1, r1, #RGB_LED_PINS
	STR r1, [r0, #GPIODEN]

	POP {pc}



pwm_with_gpt:
	PUSH {lr}

	; Turn off LED if it isn't already
	LDR r0, ptr_to_rgb_input
	MOV r1, #NULL
	STR r1, [r0]

	; Ensure reset value is also "off"
	LDR r0, ptr_to_reset_rgb
	STR r1, [r0]

	; Initialization
	BL uart_init
	BL rgb_led_init

	; Wait for user to start before
	; 	allowing timer interrupts
	LDR r0, ptr_to_start_prompt
	BL output_string

	BL timer_interrupt_init

	; Give user control over continue/exit
wait_on_user_loop:
	BL read_character
	CMP r0, #ASCII_ESC
	BEQ pwm_gpt_end
	CMP r0, #ASCII_SPACE
	BNE wait_on_user_loop

	; Obtain color's hex code from user,
	;	and convert from a string to actual
	;	hex value to set proper duty cycles
	;
get_input:
	LDR r0, ptr_to_input_prompt
	BL output_string
	LDR r0, ptr_to_input_buffer
	BL read_string
	LDR r0, ptr_to_input_buffer
	BL string2hex
	LDR r1, ptr_to_rgb_input
	STR r0, [r1]
	LDR r1, ptr_to_reset_rgb
	STR r0, [r1]
	B wait_on_user_loop


pwm_gpt_end:
	LDR r0, ptr_to_goodbye
	BL output_string

	POP {pc}



Advanced_RGB_LED:
	PUSH {lr}

	; Timer 0 base address
	MOV r0, #0x0000
	MOVT r0, #0x4003

	; Set GPTMICR[0] to clear interrupt
	LDR r1, [r0, #GPTMICR]
	ORR r1, r1, #0x001
	STR r1, [r0, #GPTMICR]

	; r0 = input to illuminate_RGB_LED
	MOV r0, #NULL

	; Each RGB value has a global byte
	;	containing its "duty cycle," i.e.,
	;	some value from 0-255
	LDR r3, ptr_to_rgb_input
	LDR r2, [r3]

	; Decrement each value and once all three hit
	;	0, reset the "duty cycle" bytes to continue
	;	the loop until a new color is input
	TST r2, #0xFF0000
	ITT NE
	SUBNE r2, r2, #0x010000
	ORRNE r0, r0, #RED_LED

	TST r2, #0x00FF00
	ITT NE
	SUBNE r2, r2, #0x000100
	ORRNE r0, r0, #GREEN_LED

	TST r2, #0x0000FF
	ITT NE
	SUBNE r2, r2, #0x000001
	ORRNE r0, r0, #BLUE_LED

	CMP r2, #NULL
	ITTT EQ
	LDREQ r1, ptr_to_reset_rgb
	LDREQ r1, [r1]
	MOVEQ r2, r1

	STR r2, [r3]

	BL illuminate_RGB_LED

	POP {lr}
	BX lr



string2hex:
	PUSH {r4, lr}
	; r0 = Address of string to decode

	; Assume string is properly formatted
	;	i.e., first 2 bytes are "0x"
	MOV r2, #0x002
	MOV r3, #NULL

hex_loop:
	LDRB r1, [r0, r2]
	CMP r1, #NULL
	BEQ end_hex_loop

	ADD r2, r2, #0x001

	; Ensure that the character is either
	;	in the range of '0' through '9' or,
	CMP r1, #ASCII_ZERO
	ITT LT
	MOVLT r0, #NULL
	BLT end_hex_loop

	CMP r1, #ASCII_NINE
	ITE LE
	SUBLE r1, r1, #ASCII_ZERO
	BGT check_abcdef

	B insert_hex_value

check_abcdef:
	; in the range of 'A' through 'F'
	CMP r1, #ASCII_A
	BLT end_hex_loop

	CMP r1, #ASCII_F
	BGT end_hex_loop

	; Subtract 55 to get integer value
	SUB r1, r1, #0x037

insert_hex_value:
	; Insert hex value at appropriate position
	;	based on current byte being read, which
	;	is done from most- to least-significant byte
	;
	LSL r4, r2, #0x002
	RSB r4, r4, #0x020
	LSL r4, r1, r4
	ORR r3, r4, r3

	B hex_loop

end_hex_loop:
	; r0 = Corresponding hex value
	MOV r0, r3
	POP {r4, pc}


	.end
