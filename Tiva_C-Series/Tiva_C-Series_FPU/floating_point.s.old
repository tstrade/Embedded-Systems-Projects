	.data

start_prompt: 	.string 0xC, 0xD, 0xA, 0x9, 0x9, 0x9, 0x9, "Welcome to the Keypad Project!", 0xD, 0xA, 0x9, 0x9, "Press space to start the program. Press 'x' at anytime to quit.", 0xD, 0xA, 0
reply_string: 	.string "You pressed key #", 0
key_string:  	.string "xx", 0
end_prompt:		.string 0xD, 0xA, 0x9, 0x9, 0x9, 0x9, 0x9, "Goodbye!", 0

ref_string:    	.string "9999999999999999999999", 0
tmp_string:    	.string "0000000000000000000000", 0
sp_string:     	.string "x1.0000000000000000000000", 0

bit_string:	   	.string "00000000000000000000000000000000", 0

frac_string:   .string "0.00000000000000000000000", 0
pow_3:         .string "0.12500000000000000000000", 0
pow_4:         .string "0.06250000000000000000000", 0
pow_8:         .string "0.00390625000000000000000", 0
pow_12:        .string "0.00024414062500000000000", 0
pow_16:        .string "0.00000152587890625000000", 0
pow_20:        .string "0.00000095367431640625000", 0
pow_24:        .string "0.00000005960464477539063", 0

max_frac:      .string "0.99999988079071044921875", 0

sign:      	.byte 0x0
exponent:  	.short 0x0000

key:		.byte 0x00
status:		.byte 0x00

	.text
	.global fpu_init
	.global fpu_project
	.global float2string
	.global string2float
	.global bit2string
	.global string2bit

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

	.global simple_add_ascii
	.global single_precision_add_ascii
	.global single_precision_mul_ascii
	.global decode_fraction

	.global UART0_Handler
	.global GPIO_PortD_Handler

ptr_to_start_prompt: 	.word start_prompt
ptr_to_reply_string:	.word reply_string
ptr_to_key_string:		.word key_string
ptr_to_end_prompt:		.word end_prompt

ptr_to_bit_string:		.word bit_string
ptr_to_ref_string: 		.word ref_string
ptr_to_tmp_string: 		.word tmp_string
ptr_to_sp_string:  		.word sp_string
ptr_to_frac_string:		.word frac_string
ptr_to_pow_3:			.word pow_3
ptr_to_pow_4:			.word pow_4
ptr_to_pow_8:			.word pow_8
ptr_to_pow_12:			.word pow_12
ptr_to_pow_16:			.word pow_16
ptr_to_pow_20:			.word pow_20
ptr_to_pow_24:			.word pow_24
ptr_to_max_frac:		.word max_frac
ptr_to_sign:       		.word sign
ptr_to_exponent:   		.word exponent

ptr_to_key:				.word key
ptr_to_status:			.word status

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


; FPU Registers - Base Addr. = 0xE000E000 (pg. 194)
CPACR:   .equ 0xD88    ; Coprocessor Access Control (pg. 195)
;        Bits 23:22 - CP11 Coproccessor Access Privilege
;        Bits 21:20 - CP10 Coprocessor Access Privilege
;          0x0 - Denied, 0x1 - Privileged, 0x3 - Full Access

FPCCR:   .equ 0xF34    ; Floating-Point Context Control (pg. 196)
;        Bit 31 - ASPEN (Automatic State Preservation Enable)
;                  to preserve FP context on exception entry/exit
;        Bit 30 - LSPEN (Lazy State Preservation Enable)
;        Bit 8 - MONRDY (Monitor Ready) permits setting MON_PEND
;                  when FP stack frame was allocated
;        Bit 6 - BFRDY (Bus Fault Ready)
;        Bit 5 - MMRDY (Memory Management Fault Ready)
;        Bit 4 - HFRDY (Hard Fault Ready)
;        Bit 3 - THREAD (Thread mode)
;        Bit 1 - USER (User Privilege Level)
;        Bit 0 - LSPACT (Lazy State Preservation Active)

FPCAR:   .equ 0xF38    ; Floating-Point Context Address (pg. 198)
;        Bits 31:3 - Address of unpopulated FP register space
;                      allocated on an exception stack frame

FPDSCR:  .equ 0xF3C    ; Floating-Point Default Status Control (pg. 199)
;        Bit 26 - Holds default value for AHP bit in FPSC register
;        Bit 25 - Holds default value for DN bit in FPSC register
;        Bit 24 - Holds default value for FZ bit in FPSC register

; FPSC = Register 22 (pg. 90)
;    Bits[31:28] - NZCV
;    Bits[26:24] - 0b010
;    Bits[23:22] - Rounding mode (nearest, +inf, -inf, to zero)
;    Bits[7,4:0] - Exception bits (input denormal, inexact cumulative
;                                 underflow, overflow, div by 0, invalid op)

; FPU description on pg. 130
; Registers:
;    [D0-D15] - 64-bit doubleword registers
;    [S0-S31] - 32-bit singleword registers
;
;    S<2n> = Least significant half of D<n>
;    S<2n=1> = Most significant half of D<n>

; Enabling FPU (pg. 134):

EXPO_SP_MASK:      .equ 0x0FF ; 8-bit mask
SIGN_SP_SHIFT:     .equ 0x01F ; Shift left 31 bits
EXPO_SP_SHIFT:     .equ 0x017 ; Shift left 23 bits
EXPO_SP_BIAS:      .equ 0x074 ; Bias = 127

ASCII_POINT:       .equ 0x02E
ASCII_ZERO:        .equ 0x030
ASCII_ONE:         .equ 0x031
ASCII_NINE:        .equ 0x039
POW_OFFSET:        .equ 0x018 ; Number of bytes to last possible digit in string

fpu_init:
  PUSH {lr}

  MOV r0, #0xE000E000
  ; Set bits 20-23 to enable CP10 and CP11 coprocessors
  LDR r1, [r0, #CPACR]
  ORR r1, r1, #0xF00000
  STR r1, [r0, #CPACR]

  ; Wait for store to complete
  ;   "Data Synchronization Barrier"
  DSB

  ; Reset pipeline now the FPU is enabled
  ;  "Instruction Synchronization Barrier"
  ISB

  POP {pc}

fpu_project:
	PUSH {lr}

	BL uart_init

	MOV r0, #0x00C
	BL output_character

	; 7.2
	MOV r0, #0x6666
	MOVT r0, #0x40E6

	; 14.86
	MOV r1, #0xC28F
	MOVT r1, #0x416D

	; Transfers single-precision registers to and from ARM core register
	VMOV s0, r0
	VMOV s1, r1

	; Floating-point Add
	VADD.F32 s2, s0, s1

	VMOV r0, s2
	BL float2string

	LDR r0, ptr_to_frac_string
	BL output_string



	POP {pc}


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

; r0 = floating point number
; r1 = address of string
float2string:
  	PUSH {r3-r5, lr}

  	; Store in case needed later
  	MOV r4, r0
  	MOV r5, r1

  	; Store sign of floating point
  	LDR r2, ptr_to_sign
  	LSR r3, r0, #SIGN_SP_SHIFT
  	STRB r3, [r2]

  	; Store exponent of floating point
  	LDR r2, ptr_to_exponent
  	LSR r3, r0, #EXPO_SP_SHIFT
  	AND r3, r3, #EXPO_SP_MASK
  	SUB r3, r3, #EXPO_SP_BIAS
  	STR r3, [r2]

  	; Isolate fraction
  	BFC r0, #0x017, #0x009
	BL decode_fraction

  	POP {r3-r5,pc}

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


decode_fraction:
	PUSH {r4-r5, lr}
	; r0 = floating point fraction to decode

	MOV r4, r0
	MOV r5, #0x006

decode_loop:
	SUBS r5, r5, #0x001
	BLT decode_end

	ANDS r2, r4, #0x00F
	LSR r4, r4, #0x004
	BEQ decode_loop

case_pow_24:
	CMP r5, #0x005
	BNE case_pow_20

	LDR r1, ptr_to_pow_24
	B add_frac

case_pow_20:
	CMP r5, #0x004
	BNE case_pow_16

	LDR r1, ptr_to_pow_20
	B add_frac

case_pow_16:
	CMP r5, #0x003
	BNE case_pow_12

	LDR r1, ptr_to_pow_16
	B add_frac

case_pow_12:
	CMP r5, #0x002
	BNE case_pow_8

	LDR r1, ptr_to_pow_12
	B add_frac

case_pow_8:
	CMP r5, #0x001
	BNE case_pow_4

	LDR r1, ptr_to_pow_8
	B add_frac

case_pow_4:
	LDR r1, ptr_to_pow_4

add_frac:
	LDR r0, ptr_to_frac_string
	BL single_precision_mul_ascii

	B decode_loop

decode_end:
	POP {r4-r5, pc}



	.end
