	.data

temp_int_string:	.string "000000000000000000", 0
temp_frac_string:	.string "000000000000000000", 0
temp_fp_string:		.string "000000000000000000000000000000000000", 0

sign:      		.byte 0x00
exponent:  		.byte 0x03

; 0 = Waiting to Begin
; 1 = 1st Operand
; 2 = Operation
; 3 = 2nd Operand (if applicable)
; 4 = Ready to Calculate
; 5 = Exit

calc_state:		.byte 0x00

	.text
	.global fpu_init
	.global fpu_project
	.global float2string
	.global string2float
	.global append_string

	.global uart_interrupt_init
	.global uart_init
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

ptr_to_temp_int_string:		.word temp_int_string
ptr_to_temp_frac_string:	.word temp_frac_string
ptr_to_temp_fp_string:		.word temp_fp_string

ptr_to_sign:       			.word sign
ptr_to_exponent:   			.word exponent
ptr_to_calc_state:			.word calc_state


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

ASCII_ESC:		   .equ 0x01B
ASCII_SPACE:	   .equ 0x020
ASCII_TIMES:	   .equ 0x02A
ASCII_PLUS:		   .equ 0x02B
ASCII_POINT:       .equ 0x02E
ASCII_MINUS:	   .equ 0x02D
ASCII_DIVIDE:	   .equ 0x02F
ASCII_ZERO:        .equ 0x030
ASCII_NINE:        .equ 0x039
ASCII_EQUAL:	   .equ 0x03D
ASCII_SQUARE:	   .equ 0x05E
ASCII_SQRT:		   .equ 0x072
ASCII_YES:		   .equ 0x079

NULL:			   .equ 0x000

STATE_WAIT:		   .equ 0x000
STATE_FIRSTOP:	   .equ 0x001
STATE_OPERATION:   .equ 0x002
STATE_SECONDOP:	   .equ 0x003
STATE_READY:	   .equ 0x004
STATE_EXIT:		   .equ 0x005


; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START FPU INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
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

  ; Set to round to nearest
  LDR r1, [r0, #FPDSCR]
  BIC r1, r1, #0x00C00000
  STR r1, [r0, #FPDSCR]

  POP {pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END FPU INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START FPU PROJECT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
fpu_project:
	PUSH {lr}

	BL uart_init

	MOV r0, #0x00C
	BL output_character

	nop
	nop



	POP {pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END FPU PROJECT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;






; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START UART0 HANDLER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
UART0_Handler:
	PUSH {lr}

	MOV r0, #0xC000
	MOVT r0, #0x4000

	LDR r1, [r0, #UARTICR]
	ORR r1, r1, #RXIC
	STR r1, [r0, #UARTICR]

	LDR r1, [r0, #UARTDR]

	LDR r2, ptr_to_calc_state
	LDRB r3, [r2]

	; Ignore input if user elected to exit completely
	CMP r3, #STATE_EXIT
	BEQ uart_handled

	; User can exit anytime by pressing escape
	CMP r1, #ASCII_ESC
	ITT EQ
	MOVEQ r3, #STATE_EXIT
	STRBEQ r3, [r2]
	BEQ uart_handled

	; If user has not started yet, check if 'y'es
check_wait:
	CMP r3, #STATE_WAIT
	BNE check_firstop

	; 	Ignore if user presses anything except for 'y'es
	CMP r1, #ASCII_YES
	BNE uart_handled

	MOV r3, #STATE_FIRSTOP
	STRB r3, [r2]
	BL newline
	B uart_handled

; If inputting either fp operand, will
;	need to use a buffer to store value
;	then convert, calculate, convert, output
check_space:
	CMP r1, #ASCII_SPACE
	ADD r3, r3, #0x001
	CMP r3, #STATE_EXIT
	IT EQ
	MOVEQ r3, #STATE_FIRSTOP

check_firstop:
	CMP r3, #STATE_FIRSTOP
	BNE check_operation



check_operation:
	CMP r3, #STATE_OPERATION
	BNE check_secondop

check_secondop:
	CMP r3, #STATE_SECONDOP
	BNE check_ready

check_ready:
	CMP r3, #STATE_READY
	BNE uart_handled




	; Operation entry
	;
	;	+
	;
	;	-
	;
	;	x
	;
	;   /
	;
	;	^ (square)
	;
	;   r (square root)
	;
	;	=

uart_handled:
	POP {lr}
	BX lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END UART0 HANDLER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START STRING2FLOAT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
string2float:
	PUSH {r4-r5, lr}

	; r0 = address of fp string

	; Once the radix point is found, we will
	; 	insert a null byte to split the
	;	integer part from the fractional part
	MOV r4, r0
	MOV r2, #NULL

find_radix:
	LDRB r1, [r4], #0x001
	CMP r1, #ASCII_POINT
	ITE NE
	BNE find_radix
	STRBEQ r2, [r4, #-0x001]


	; Obtain the integer part of the fp string
	;	(w/ rounding to 0)
	BL string2int
	VMOV s0, r0
	VCVT.F32.S32 s0, s0
	VPUSH {s0}

	; Obtain the fractional part of the fp string
	;	(w/ rounding to nearest)
	MOV r0, r4
	BL string2int
	VMOV s1, r0
	VCVT.F32.S32 s1, s1

	VMOV.F32 s2, #10.0
	; Find how many fractional places are used
find_frac_size:
	LDRB r0, [r4], #0x001
	CMP r0, #NULL
	ITT NE
	VDIVNE.F32 s1, s1, s2
	BNE find_frac_size

	VPOP {s0}
	VADD.F32 s0, s0, s1
	VMOV r0, s0

	; r0 = fp value

	POP {r4-r5, pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END STRING2FLOAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START FLOAT2STRING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
float2string:
	PUSH {r4-r6, lr}

	; r0 = fp to convert
	; r1 = address of fp string
	MOV r4, r0
	MOV r5, r1

  	; Store sign of floating point
  	ANDS r2, r0, #0x8000000
	ITT NE
	MOVNE r2, #ASCII_MINUS
  	STRBNE r2, [r5]

	; Obtain integer portion of fp number
	VMOV s0, r0
	VCVT.S32.F32 s1, s0
	VCVT.F32.S32 s1, s1

	; Remove integer portion to only handle
	; 	the fractional portion
	VSUB.F32 s0, s0, s1


	; Load how many decimal places the
	;	user expects to see
	LDR r2, ptr_to_exponent
	LDRB r3, [r2]
	VMOV.F32 s3, #10.

	; Multiply fraction by specified
	;	powers of 10 to obtain integer
	;	representation (+ round down)
fp_pow_loop:
	VMUL.F32 s0, s0, s3
	SUBS r3, r3, #0x001
	BGT fp_pow_loop

	; CHECKPOINT:
	;	r0 = fp to convert
	;	r1 = address of fp string
	;	s0 = fractional portion
	;	s1 = integer portion
	;VPUSH {s0}

	LDR r0, ptr_to_temp_int_string
	VCVT.S32.F32 s1, s1
	VMOV r1, s1
	BL int2string

	;VPOP {s0}
	LDR r0, ptr_to_temp_frac_string
	VCVT.S32.F32 s0, s0
	VMOV r1, s0
	BL int2string

	; Combine strings into full representation
	MOV r0, r5
	LDRB r2, [r0]
	CMP r2, #ASCII_MINUS
	IT EQ
	ADDEQ r0, #0x001

	LDR r1, ptr_to_temp_int_string
	BL append_string

	MOV r1, #ASCII_POINT
	LDRB r1, [r0], #0x001
	LDR r1, ptr_to_temp_frac_string
	BL append_string

	; r0 = address of fp string
	MOV r0, r5
	POP {r4-r6, pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END FLOAT2STRING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START APPEND STRING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
append_string:
	PUSH {lr}
	; r0 = address* of string to append to
	; r1 = base address of string to append
	;
	;	* Appending will start AT this address,
	;	  i.e., will overwrite the value at r0
append_loop:
	LDRB r2, [r0]
	LDRB r3, [r1], #0x001

	; If the appendee is at its end,
	;	stop from overwriting external data
	CMP r2, #NULL
	BEQ end_append

	; If the appender is at its end,
	;	stop from reading external data
	CMP r3, #NULL
	BEQ end_append

	; Store character and inc.
	STRB r3, [r0], #0x001

	B append_loop

end_append:
	; r0 will return as the address
	;	of the next character
	POP {pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END APPEND STRING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;





	.end
