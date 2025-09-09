	.text
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


; General Purpose Timers
GPTMCFG:          .equ 0x000	; GPTM Configuration
GPTMTAMR:         .equ 0x004	; GPTM Timer A Mode
GPTMCTL:          .equ 0x00C	; GPTM Control
GPTMIMR:          .equ 0x018	; GPTM Interrupt Mask
GPTMICR:	      .equ 0x024	; GPTM Interrupt Clear Register
GPTMTAILR:        .equ 0x028	; GPTM Timer A Interval Load
GPTMTAR:          .equ 0x048	; GPTM Timer A

; General Purpose Input / Output
GPIODATA:         .equ 0x3FC	; GPIO Data Register
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

PORTD_MASK:	  	  .equ 0x00F
PD0:			  .equ 0x001
PD1:			  .equ 0x002
PD2:			  .equ 0x004
PD3:			  .equ 0x008

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

; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START UART INTERRUPT INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
uart_interrupt_init:

	PUSH {lr}

    MOV r0, #0xC000                 ;
    MOVT r0, #0x4000                ; UART0 Base Address
    LDR r1, [r0, #UARTIM]           ; Load UART Interrupt Mask Register
    ORR r1, r1, #RXIM               ; Set Receive Interrupt Mask (Bit 4)
    STR r1, [r0, #UARTIM]           ; Store back to UART Interrupt Mask Register

    MOV r0, #0xE000E000             ; EN0 Base Address
    LDR r1, [r0, #EN0]              ; Load EN0 Register
    ORR r1, r1, #ENUART0            ; Set UART0 Enable (Bit 4)
    STR r1, [r0, #EN0]              ; Store back to EN0 Register

	POP {lr}
    MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END UART INTERRUPT INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START GPIO INTERRUPT INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
gpio_interrupt_init:

	PUSH {lr}

	; Enable interrupts for Port D
    MOV r0, #0x7000
    MOVT r0, #0x4000		        ; Port D Base Address

	; Mask interrupt
	LDR r1, [r0, #GPIOIM]
	BIC r1, r1, #PORTD_MASK			; Clear bits 0:3 to prevent interrupt during config
	STR r1, [r0, #GPIOIM]

    ; Edge sensitive
    LDR r1, [r0, #GPIOIS]
    BIC r1, r1, #PORTD_MASK		    ; Clear bits 0:3 to set edge-sensitive
	STR r1, [r0, #GPIOIS]

	; Single edge triggering
	LDR r1, [r0, #GPIOIBE]
	BIC r1, r1, #PORTD_MASK		    ; Clear bits 0:3 to allow GPIOIEV to control
	STR r1, [r0, #GPIOIBE]

    ; Rising edge
    LDR r1, [r0, #GPIOIEV]
    ORR r1, r1, #PORTD_MASK			; Set bits 0:3 for rising-edge interrupts
	STR r1, [r0, #GPIOIEV]

    ; Enabling interrupt (GPIO)
    LDR r1, [r0, #GPIOIM]
	ORR r1, r1, #PORTD_MASK		    ; Set bits 0:3 to re-enable interrupts
	STR r1, [r0, #GPIOIM]

    ; Enabling interrupt (Proccessor)
    MOV r0, #0xE000E000		        ; EN0 Base Address

    LDR r1, [r0, #EN0]              ; (pg. 104 - Vector table; pg. 142 - Reg. Description)
    ORR r1, r1, #ENGPIOD            ; Enable Processor Interrupts for Port D (Bit 3)
    STR r1, [r0, #EN0]

	POP {lr}
    MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END GPIO INTERRUPT INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START GPIO KEYPAD INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
gpio_keypad_init:

	PUSH {lr}

	; System control register base address in 0x400FE000 (pg. 231)
    MOV r0, #0xE000
    MOVT r0, #0x400F

    ; Set bits 0 and 3 of GPIO Run Mode Clock Gating Control
    ;   to enable the clock for ports A and D (pg. 341)
    LDR r1, [r0, #GPIORCGC]
    ORR r1, r1, #0x009
    STR r1, [r0, #GPIORCGC]

    ; Must be a delay of 3 system clocks after GPIO module clock
    ;   is enabled (pg. 658-659)
    nop

    ; Configure GPIO Port D (base address = 0x40007000)
    MOV r0, #0x7000
    MOVT r0, #0x4000

    ; Configure Pins 0-3 as input (pg. 663)
    LDR r1, [r0, #GPIODIR]
    BIC r1, r1, #PORTD_MASK
    STR r1, [r0, #GPIODIR]

    ; Enable Pull-Down Resistor to ensure default state is logic low
    LDR r1, [r0, #GPIOPDR]
    ORR r1, r1, #PORTD_MASK
    STR r1, [r0, #GPIOPDR]

    ; Set Digital Enable for Pins 0-3 (pg. 682)
    LDR r1, [r0, #GPIODEN]
    ORR r1, r1, #PORTD_MASK
    STR r1, [r0, #GPIODEN]

    ; Configure GPIO Port A (base address = 0x40004000)
    MOV r0, #0x40004000

    ; Configure Pins 2-5 as output
    LDR r1, [r0, #GPIODIR]
    ORR r1, r1, #PORTA_MASK
    STR r1, [r0, #GPIODIR]

    ; Set Pins 2-5 to use alternate function
    ; 0 = GPIO; 1 = Peripheral (pg. 672)
    LDR r1, [r0, #GPIOAFSEL]
    BIC r1, r1, #PORTA_MASK
    STR r1, [r0, #GPIOAFSEL]

    ; Change Pins 2-5 to GPIO
    LDR r1, [r0, #GPIOCTL]
    BFC r1, #0x008, #0x010
    STR r1, [r0, #GPIOCTL]

    ; Change is available on second clock cycle (pg. 680)
    nop

    ; Set Digital Enable for Pins 2-5
    LDR r1, [r0, #GPIODEN]
    ORR r1, r1, #PORTA_MASK
    STR r1, [r0, #GPIODEN]

	POP {lr}
    MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END GPIO KEYPAD INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;


; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START UART_INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
uart_init:
	PUSH {lr}

	; System control register base address in 0x400FE000 (pg. 231)
	MOV r0, #0xE000
    MOVT r0, #0x400F

	; Enable clock for UART0
	LDR r1, [r0, #UARTRCGC]
	ORR r1, r1, #0x001
    STR r1, [r0, #UARTRCGC]

	; Enable clock for GPIO Port A
    LDR r1, [r0, #GPIORCGC]
    ORR r1, r1, #0x001
    STR r1, [r0, #GPIORCGC]
    nop
    nop

	MOV r0, #0xC000
    MOVT r0, #0x4000

    ; Disable UART0 Control
    MOV r1, #0x000
    STR r1, [r0, #UARTCTL]

	; Set UART0_IBRD_R for 115200 baud
    MOV r1, #0x008
    STR r1, [r0, #UARTIBRD]

	; Set UART0_FBRD_R for 115200 baud
    MOV r1, #0x02C
    STR r1, [r0, #UARTFBRD]

    ; Use system clock
    MOV r1, #0x000
    STR r1, [r0, #UARTCC]

	; Use 8-bit word length, 1 stop bit, no parity
    MOV r1, #0x060
    STR r1, [r0, #UARTLCRH]

	; Enable UART0 Control
	MOV r1, #0x301
    STR r1, [r0, #UARTCTL]

	; Port A base address
	MOV r0, #0x40004000

    ; Make PA0 and PA1 as Digital Ports
    LDR r1, [r0, #GPIODEN]
    ORR r1, r1, #0x003
    STR r1, [r0, #GPIODEN]

	; Change PA0 and PA1 to use an alternate function
    LDR r1, [r0, #GPIOAFSEL]
    ORR r1, r1, #0x003
    STR r1, [r0, #GPIOAFSEL]

    ; Configure PA0 and PA1 for UART
    LDR r1, [r0, #GPIOCTL]
    ORR r1, r1, #0x011
    STR r1, [r0, #GPIOCTL]

	POP {lr}
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END UART_INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START OUTPUT_CHARACTER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
output_character:
	PUSH {lr}

	MOV r1, #0xC000		;Load 0x4000C000 into r1
	MOVT r1, #0x4000

poll_txff:
	LDRB r3, [r1, #U0FR];Get data from address at r1 + offset
	AND r3, r3, #0x20	;Check TxFF
	CMP r3, #0x20
	BEQ poll_txff		;Branch if TxFF is 1

	STRB r0, [r1]		;Store r0 value at address in r2

	POP {lr}
	MOV pc, lr

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END OUTPUT_CHARACTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START READ_CHARACTER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
read_character:
	PUSH {r4-r6,lr}    ; Save registers to stack.

	; Your code to receive a character obtained from the keyboard
	; in PuTTy is placed here.  The character is returned in r0.
	MOV r4, #0xC000		;Initialize base address 0x4000C000 in r4
	MOVT r4, #0x4000

RxFETest:
	LDRB r5, [r4, #U0FR];Load address + offset byte into r5
	AND r6, r5, #0x10	;AND r2 with 00010000 and store in r3 to test RxFE
	CMP r6, #16			;Compare r3 to 16 (RxFE = 1)
	BEQ	RxFETest		;RxFE is empty, loop

	LDRB r0, [r4]		;Load data into r0

	POP {r4-r6, lr}	; Restore registers
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END READ_CHARACTER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START READ_STRING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
read_string:
	PUSH {r4-r12, lr} 	; Save registers to stack

	; Your code for your read_string routine is placed here
	MOV r4, r0			; Store address from r0 in r4
	MOV r5, #0x00		; Store NULL character in r5

readLoop:
	BL read_character	;Branch and link to read_character
	BL output_character ;Output character
	CMP r0, #0x0D		;Is the character the enter key?
	BEQ Enter			;Go to Enter

	STRB r0, [r4]		;Store the read character at the address of r4
	ADD r4, r4, #0x01	;Move the address over a byte
	B readLoop

Enter:
	STRB r5, [r4]		;Store NULL in address of r4

	POP {r4-r12,lr}   	; Restore registers
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END READ_STRING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START OUTPUT_STRING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
output_string:
	PUSH {r4-r12,lr} 	; Save registers on stack

	; Your code for your output_string routine is placed here
	MOV r4, r0

poll_string:
	LDRB r0, [r4]
	CMP r0, #0x00
	BEQ output_string_end

	BL output_character

	ADD r4, r4, #0x01
	B poll_string

output_string_end:

	POP {r4-r12,lr}   	; Restore registers
	MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END OUTPUT_STRING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START INT2STRING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;

int2string:
	PUSH {r4-r12,lr} 	; Store any registers in the range of r4 through r12
						; that are used in your routine.  Include lr if this
						; routine calls another routine.

	; Your code for your int2string routine is placed here
	MOV r4, r0			;Copy base address into r4
	MOV r5, #0			;Initialize a counter to 0
	MOV r6, #10			;Initialize a register to 10

getChar:
	MOV r2, r1
	UDIV r1, r1, r6		;Divide r1 by 10 (r6)
	MLS r7, r1, r6, r2	;Get remainder and put it in r7
	ADD r8, r7, #0x30	;Store remainder + 0x00
	PUSH {r8}			;Push value onto stack
	ADD r5, r5, #1		;Increment counter

	CMP r1, #0			;Compare quotient to zero
	BNE getChar			;Branch to getChar if quotient is not zero

unstack:
	POP {r8}			;Get value pushed onto stack
	STRB r8, [r4]		;Store value at address at r4
	ADD r4, r4, #0x01	;Move address by 1 byte
	SUB r5, r5, #1		;Decrement counter
	CMP r5, #0			;Compare counter to 0
	BNE	unstack			;Branch to unstack

	MOV r5, #0x00
	STRB r5, [r4]		;Store NULL at last byte

	POP {r4-r12,lr}   	; Restore registers all registers preserved in the
						; PUSH at the top of this routine from the stack.
	MOV pc, lr

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END INT2STRING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START STRING2INT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;

string2int:
	PUSH {r4-r12,lr} 	; Store any registers in the range of r4 through r12
						; that are used in your routine.  Include lr if this
						; routine calls another routine.

	; Your code for your string2int routine is placed here
	MOV r3, #0x30		;Make r3 0x30 (ASCII 0)
	MOV r4, #0x39		;Make r4, 0x39 (ASCII 9)
	MOV r5, #0			;Make r5, 0
	SUB r6, r0, #0x01	;Hold base address -1 byte
	MOV r7, #10

getDigit:
	ADD r6, r6, #0x01	;Increment address by 1
	LDRB r0, [r6]		;Get first digit
	CMP r0, #0			;Check if NULL
	BEQ done			;If NULL we are done

	SUB r0, r0, r3		;Get integer version
	CMP r0, #0			;Check if ASCII is really a number
	BLT getDigit		;Not a number, branch

	CMP r0, #9			;Check if ASCII is really a number
	BGT getDigit		;Not a number, branch

	MUL r5, r5, r7		;Multiply r5 by 10
	ADD r5, r5, r0		;r5 + r0
	B getDigit			;Branch getDigit

done:
	MOV r0, r5			;Store final value in r0


	POP {r4-r12,lr}   		; Restore registers all registers preserved in the
						; PUSH at the top of this routine from the stack.
	mov pc, lr

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END STRING2INT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;

; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START DIVISION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
division:
        PUSH {r4-r12,lr}	; Save registers to stack

        CMP r1, #0x0        ; Divide-by-zero?
        BEQ div_end         ; If divisor = 0, don't divide
        MOV r4, #0xF        ; Init counter (r4) to 15
        MOV r5, #0x0        ; Init quotient (r5) to 0
        LSL r1, r1, #0xF    ; Logical left shift divisor (r1) 15 places
        MOV r6, r0          ; Init remainder (r6) to dividend (r0)

remainder:
        SUB r6, r6, r1      ; Remainder := remainder - divisor
        CMP r6, #0x0        ; Is remainder < 0?
        BLT remainder_lt    ; If remainder < 0, restore divisor to remainder
        LSL r5, r5, #0x1    ; If remainder >= 0, logical left shift quotient
        ORR r5, r5, #0x1    ; Set quotient LSB to 1 (could also just do Q + 1)
        B counter           ; Skip past remainder-restoration code

remainder_lt:
        ADD r6, r6, r1      ; Remainder := remainder + divisor
        LSL r5, r5, #0x1    ; Logical left shift quotient

counter:
        LSR r1, r1, #0x1    ; Logical right shift divisor
        CMP r4, #0x0        ; Is counter > 0?
        BLE div_end         ; If counter <= 0, division is complete
        SUB r4, r4, #0x1    ; If counter > 0, decrement counter
        B remainder         ; Repeat remainder - divisor process

div_end:

        MOV r0, r5
        POP {r4-r12,lr}		; Restore registers
        MOV pc, lr
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END DIVISION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ;



; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> START MULTIPLICATION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;
multiplication:
        PUSH {r4-r12,lr}	; Save registers to stack

        ; Are we multiplying by zero?
        AND r4, r0, r1      ; Are either the multiplier (r0) or
        CMP r4, #0x0        ; the multiplicand (r1) = 0?
        BEQ mult_zero

        MOV r4, #0xF        ; Initialize counter (r4) to 15
        MOV r5, #0x0        ; Initialize product (r5) to 0
        MOV r6, #0x1        ; Initialize LSB mask (r6) to 1
product:
        AND r7, r6, r0      ; Is multiplier LSB = 0?
        CMP r7, #0x0
        BEQ shifter
        ADD r5, r5, r1      ; Product := product + multiplicand

shifter:
        LSL r1, r1, #0x1    ; Logical shift left multiplicand 1 bit
        LSR r0, r0, #0x1    ; Logical shift right multiplier 1 bit
        CMP r4, #0x0        ; Is counter > 0?
        BLE mult_end        ; If counter <= 0, multiplication is complete
        SUB r4, r4, #0x1    ; If counter > 0, decrement counter
        B product

mult_zero:
        MOV r5, #0x0

mult_end:

        MOV r0, r5          ; Place result into r0
        POP {r4-r12,lr}		; Restore registers

        MOV pc, lr
; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END MULTIPLICATION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ;

newline:
	PUSH {r4-r12,lr}
	MOV r0, #0x0A
	BL output_character

	MOV r0, #0x0D
	BL output_character

	POP {r4-r12,lr}
	MOV pc,lr


	.end
