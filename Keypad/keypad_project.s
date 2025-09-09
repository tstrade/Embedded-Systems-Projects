		.data

start_prompt: 	.string 0xC, 0xD, 0xA, 0x9, 0x9, 0x9, 0x9, "Welcome to the Keypad Project!", 0xD, 0xA, 0x9, 0x9, "Press space to start the program. Press 'x' at anytime to quit.", 0xD, 0xA, 0
reply_string: 	.string "You pressed key #", 0
key_string:  	.string "xx", 0
end_prompt:		.string 0xD, 0xA, 0x9, 0x9, 0x9, 0x9, 0x9, "Goodbye!", 0

key:		.byte 0x00
status:		.byte 0x00
scanning:	.byte 0x00

	.text
	.global keypad_init
	.global keypad_project
	.global uart_interrupt_init
    .global gpio_interrupt_init
	.global uart_init
	.global gpio_keypad_init
	.global output_character
	.global read_character
	.global read_string
	.global output_string

	.global string2int
	.global int2string
	.global division
	.global multiplication
	.global newline

	.global UART0_Handler
	.global GPIO_PortD_Handler

ptr_to_start_prompt: 	.word start_prompt
ptr_to_reply_string:	.word reply_string
ptr_to_key_string:		.word key_string
ptr_to_end_prompt:		.word end_prompt

ptr_to_key:				.word key
ptr_to_status:			.word status
ptr_to_scanning:		.word scanning

; GENERAL KEYPAD LAYOUT
;
;   | PA2 | PA3 | PA4 | PA5 |     |
;   -------------------------------
;   |  #0 |  #1 |  #2 |  #3 | PD0 |
;   |  #4 |  #5 |  #6 |  #7 | PD1 |
;   |  #8 |  #9 | #10 | #11 | PD2 |
;   | #12 | #13 | #14 | #15 | PD3 |
;   -------------------------------

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

; GPIO Ports
PORTA_MASK: 	  .equ 0x03C
PA2:			  .equ 0x004
PA3:			  .equ 0x008
PA4:			  .equ 0x010
PA5:			  .equ 0x020
PA2_ADJUSTED:	  .equ 0x000
PA3_ADJUSTED:	  .equ 0x001
PA4_ADJUSTED:	  .equ 0x002
PA5_ADJUSTED:	  .equ 0x003

PORTD_MASK:	  	  .equ 0x00F
PD0:			  .equ 0x001
PD1:			  .equ 0x002
PD2:			  .equ 0x004
PD3:			  .equ 0x008
PD0_ADJUSTED:	  .equ 0x000
PD1_ADJUSTED:	  .equ 0x004
PD2_ADJUSTED:	  .equ 0x008
PD3_ADJUSTED:	  .equ 0x00C

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

; Program-specific Constants
START_KEY:		  .equ 0x020
EXIT_KEY:		  .equ 0x078
STATUS_MENU:	  .equ 0x000
STATUS_ACTIVE:	  .equ 0x001
STATUS_EXIT:	  .equ 0x002

; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START KEYPAD INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
keypad_init:
	PUSH {lr}

	BL uart_init
	BL gpio_keypad_init

	; Menu / Instructions
	LDR r0, ptr_to_start_prompt
	BL output_string

	BL uart_interrupt_init

	POP {lr}
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END KEYPAD INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START KEYPAD PROJECT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
keypad_project:
	PUSH {lr}

    ; Ensure that keypad can't interrupt until user begins
    LDR r0, ptr_to_status
poll_start:
	LDRB r1, [r0]
	CMP r1, #STATUS_ACTIVE
	BLT poll_start

	; Drive Port A pins high to allow handler to do the scanning
	MOV r0, #0x40004000
	LDR r1, [r0, #GPIODATA_A]
	ORR r1, r1, #PORTA_MASK
	STR r1, [r0, #GPIODATA_A]

	BL gpio_interrupt_init

	BL newline
	BL newline

	; Check between each scan if user has opted to end
scan_loop:
	LDR r1, ptr_to_status
	LDRB r2, [r1]
	CMP r2, #STATUS_EXIT
	BEQ end_scan

	B scan_loop


end_scan:
	LDR r0, ptr_to_end_prompt
	BL output_string

	POP {lr}
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END KEYPAD PROJECT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START GPIO PORT D HANDLER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
GPIO_PortD_Handler:
	PUSH {r4, r5, lr}

	; Interrupt generated by Port D
	MOV r0, #0x7000
	MOVT r0, #0x4000

	; Check which row was pressed
	LDR r5, [r0, #GPIORIS]

	; Drop Port A pins to logic low and see which PD# is logic high
	MOV r1, #0x40004000

	; Keypad # = (PA# - 2) + (PD# * 4)

test_pa2:
	; Drop PA3/4/5 to logic low
	MOV r3, #PA2
	STR r3, [r1, #GPIODATA_A]
	nop
	nop
	nop

	LDR r4, [r0, #GPIODATA_D]
	CMP r4, #0x000
	BEQ test_pa3

	MOV r3, #PA2_ADJUSTED
	B calculate_key

test_pa3:
	; Drop PA2/4/5 to logic low
	MOV r3, #PA3
	STR r3, [r1, #GPIODATA_A]
	nop
	nop
	nop

	LDR r4, [r0, #GPIODATA_D]
	CMP r4, #0x000
	BEQ test_pa4

	MOV r3, #PA3_ADJUSTED
	B calculate_key

test_pa4:
	; Drop PA2/3/5 to logic low
	MOV r3, #PA4
	STR r3, [r1, #GPIODATA_A]
	nop
	nop
	nop

	LDR r4, [r0, #GPIODATA_D]
	CMP r4, #0x000
	BEQ test_pa5

	MOV r3, #PA4_ADJUSTED
	B calculate_key

test_pa5:
	; Drop PA2/3/4 to logic low
	MOV r3, #PA5
	STR r3, [r1, #GPIODATA_A]
	nop
	nop
	nop

	LDR r4, [r0, #GPIODATA_D]
	CMP r4, #0x000
	BEQ invalid_input

	MOV r3, #PA5_ADJUSTED
	B calculate_key

invalid_input:
	; Else invalid input
	MOV r3, #0xFFF
	B PortD_Handled

calculate_key:
	; r0 = Port D base address
	; r1 = Port A base address
	; r3 = Port A pin (-2) @ logic high
	; r4 = Port D pin @ logic high
	; r5 = Raw Interrupt Status
	CMP r4, #PD0
	ITT EQ
	ADDEQ r4, r3, #PD0_ADJUSTED
	BEQ PortD_Handled

	CMP r4, #PD1
	ITT EQ
	ADDEQ r4, r3, #PD1_ADJUSTED
	BEQ PortD_Handled

	CMP r4, #PD2
	ITT EQ
	ADDEQ r4, r3, #PD2_ADJUSTED
	BEQ PortD_Handled

	CMP r4, #PD3
	ITT EQ
	ADDEQ r4, r3, #PD3_ADJUSTED
	BEQ PortD_Handled

	; Else invalid input
	MOV r4, #0xFFF

PortD_Handled:
	LDR r0, ptr_to_reply_string
	BL output_string
	LDR r0, ptr_to_key_string
	MOV r1, r4
	BL int2string
	BL output_string
	BL newline

	; Drive Port A pins high and delay to avoid infinite interrupt loop
	MOV r1, #0x40004000
	LDR r3, [r1, #GPIODATA_A]
	ORR r3, r3, #PORTA_MASK
	STR r3, [r1, #GPIODATA_A]
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop


	MOV r0, #0x7000
	MOVT r0, #0x4000
	LDR r1, [r0, #GPIOICR]
	ORR r1, r1, r5
	STR r1, [r0, #GPIOICR]

	POP {r4, r5, lr}
	BX lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END GPIO PORT D HANDLER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START UART0 HANDLER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
UART0_Handler:
	PUSH {lr}

	MOV r0, #0xC000
	MOVT r0, #0x4000

	LDR r1, [r0, #UARTICR]
	ORR r1, r1, #RXIC
	STR r1, [r0, #UARTICR]

	LDR r1, [r0, #UARTDR]		; Load data from UART reg. to see what key was pressed


check_if_start:
	CMP r1, #START_KEY
	BNE check_if_exit

	LDR r0, ptr_to_status
	LDRB r1, [r0]
	CMP r1, #STATUS_ACTIVE
	ITE EQ
	BEQ uart_handled
	MOVNE r1, #STATUS_ACTIVE

	STRB r1, [r0]
	B uart_handled

check_if_exit:
	CMP r1, #EXIT_KEY
	BNE uart_handled

	LDR r0, ptr_to_status
	LDRB r1, [r0]
	CMP r1, #STATUS_ACTIVE
	ITE NE
	BNE uart_handled
	MOVEQ r1, #STATUS_EXIT

	STRB r1, [r0]
	B uart_handled

uart_handled:
	POP {lr}
	BX lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END UART0 HANDLER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;


	.end
