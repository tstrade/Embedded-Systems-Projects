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


red_duty_cycle: 	.byte 0x00
blue_duty_cycle:	.byte 0x00
green_duty_cycle:	.byte 0x00

	.text
	.global Advanced_RGB_LED
	.global pwm_init
	.global gptm_init
	.global gpio_init
	.global pwm_with_generator
	.global update_duty_cycles

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
	.global string2hex
	.global division
	.global multiplication
	.global newline

ptr_to_start_prompt:	.word start_prompt
ptr_to_goodbye:			.word goodbye
ptr_to_input_prompt:	.word input_prompt
ptr_to_input_buffer:	.word input_buffer
ptr_to_red_dc:			.word red_duty_cycle
ptr_to_blue_dc:			.word blue_duty_cycle
ptr_to_green_dc:		.word green_duty_cycle


; >>>>>>>>>>>>>>>>>>>>>>>>>>> START MACROS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

; Cortex M4 Peripherals
ENUART0:      		.equ 0x020  ; UART0 Enable Bit
ENGPIOD:	  		.equ 0x008	; GPIO Port D Enable Bit
EN0:          		.equ 0x100	; Interrupt 0-31 Set Enable

; >>> CM4 Peripheral - FPU
CPACR:   		  	.equ 0xD88  ; Coprocessor Access Control (pg. 195)
FPCCR:   		  	.equ 0xF34  ; Floating-Point Context Control (pg. 196)
FPCAR:   		  	.equ 0xF38  ; Floating-Point Context Address (pg. 198)
FPDSCR:  		  	.equ 0xF3C  ; Floating-Point Default Status Control (pg. 199)


; System Control
RCC:          		.equ 0x060  ; Run Mode Clock Configuration
RCGC1:				.equ 0x104	; Run Mode Clock Gating Control Register 1
PPPWM:				.equ 0x340	; Pulse Width Modulator Peripheral Present
RCGCTIMER:    		.equ 0x604	; 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control
RCGCGPIO:     		.equ 0x608  ; GPIO Run Mode Clock Gating Control
RCGCPWM:      		.equ 0x640  ; Pulse Width Modulator Run Mode Clock Gating Control


; General Purpose Input / Output
GPIODATA_A:   		.equ 0x3F0  ; GPIO Data Register Port A
GPIODATA_D:			.equ 0x03C	; GPIO Data Register Port D
GPIODIR:      		.equ 0x400	; GPIO Direction Register
GPIOIS:		    	.equ 0x404	; GPIO Interrupt Sense
GPIOIBE:	    	.equ 0x408	; GPIO Interrupt Both Edges
GPIOIEV:	    	.equ 0x40C	; GPIO Interrupt Event
GPIOIM:		    	.equ 0x410	; GPIO Interrupt Mask
GPIORIS:		  	.equ 0x414	; GPIO Raw Interrupt Status
GPIOICR:	    	.equ 0x41C	; GPIO Interrupt Clear Register
GPIOAFSEL:			.equ 0x420	; GPIO Alternate Function Select
GPIOPUR:      		.equ 0x510	; GPIO Pull-Up Resistor Register
GPIOPDR:		  	.equ 0x514	; GPIO Pull-Down Resistor Register
GPIODEN:      		.equ 0x51C	; GPIO Digital Register
GPIOPCTL:		  	.equ 0x52C	; GPIO Port Control


; General Purpose Timers
GPTMCFG:      		.equ 0x000	; GPTM Configuration
GPTMTAMR:     		.equ 0x004	; GPTM Timer A Mode
GPTMCTL:      		.equ 0x00C	; GPTM Control
GPTMIMR:      		.equ 0x018	; GPTM Interrupt Mask
GPTMICR:	    	.equ 0x024	; GPTM Interrupt Clear Register
GPTMTAILR:    		.equ 0x028	; GPTM Timer A Interval Load
GPTMTAMATCHR:  		.equ 0x030	; GPTM Timer A Match
GPTMTAPR: 	  		.equ 0x038	; GPTM Timer A Prescale
GPTMTAR:      		.equ 0x048	; GPTM Timer A


; Universal Asynchronous Receiver/Transmitters
UARTDR:			 	.equ 0x000	; UART Data Register
RXIM:         	 	.equ 0x010	; UART Receive Interrupt Mask
RXIC:         	 	.equ 0x010	; UART Receive Raw Interrupt Status
U0FR: 			 	.equ 0x018	; UART0 Flag Register
UARTIBRD: 		 	.equ 0x024	; UART Integer Baud-Rate Divisor
UARTFBRD: 		 	.equ 0x028	; UART Fractional Buad-Rate Divisor
UARTLCRH:		 	.equ 0x02C	; UART Line Control
UARTCTL:		 	.equ 0x030	; UART Control
UARTIM:       	 	.equ 0x038	; UART Interrupt Mask
UARTICR:      	 	.equ 0x044	; UART Interrupt Clear Register
UARTRCGC:		 	.equ 0x618	; UART Run Mode Clock Gating Control
UARTCC:			 	.equ 0xFC8	; UART Clock Configuration


; PWM
PWMCTL:     	 	.equ 0x000 	; PWM Master Control
PWMENABLE:  	 	.equ 0x008 	; PWM Output Enable
PWMSTATUS:  	 	.equ 0x020 	; PWM Status
PWMENUPD:   	 	.equ 0x028 	; PWM Enable Update
PWMPP:     		 	.equ 0xFC0 	; PWM Peripheral Properties

; >>> Module Offsets
PWM2CTL:     	 	.equ 0x0C0 	; PWM2 Control
PWM2LOAD:   	 	.equ 0x0D0 	; PWM2 Load
PWM2COUNT:  	 	.equ 0x0D4 	; PWM2 Counter
PWM2CMPA:   	 	.equ 0x0D8 	; PWM2 Compare A
PWM2CMPB:    	 	.equ 0x0DC 	; PWM2 Compare B
PWM2GENA:   	 	.equ 0x0E0 	; PWM2 Generator A Control
PWM2GENB:   	 	.equ 0x0E4 	; PWM2 Generator B Control
PWM2DBCTL:  	 	.equ 0x0E8 	; PWM2 Dead-Band Control
PWM2DBRISE: 	 	.equ 0x0EC 	; PWM2 Dead-Band Rising-Edge Delay
PWM2DBFALL: 	 	.equ 0x0F0 	; PWM2 Dead-Band Falling-Edge Delay

PWM3CTL:     	 	.equ 0x100 	; PWM3 Control
PWM3LOAD:   	 	.equ 0x110 	; PWM3 Load
PWM3COUNT:  	 	.equ 0x114 	; PWM3 Counter
PWM3CMPA:   	 	.equ 0x118 	; PWM3 Compare A
PWM3CMPB:    	 	.equ 0x11C 	; PWM3 Compare B
PWM3GENA:   	 	.equ 0x120	; PWM3 Generator A Control
PWM3GENB:   	 	.equ 0x124 	; PWM3 Generator B Control
PWM3DBCTL:  	 	.equ 0x128 	; PWM3 Dead-Band Control
PWM3DBRISE: 	 	.equ 0x12C  ; PWM3 Dead-Band Rising-Edge Delay
PWM3DBFALL: 	 	.equ 0x130	; PWM3 Dead-Band Falling-Edge Delay


; ASCII
NULL:			 	.equ 0x000
ASCII_ESC:		 	.equ 0x01B
ASCII_SPACE:	 	.equ 0x020
ASCII_ZERO:		 	.equ 0x030
ASCII_NINE:		 	.equ 0x039
ASCII_x:		 	.equ 0x078
ASCII_A:		 	.equ 0x041
ASCII_B:		 	.equ 0x042
ASCII_C:		 	.equ 0x043
ASCII_D:		 	.equ 0x044
ASCII_E:		 	.equ 0x045
ASCII_F:		 	.equ 0x046


; Program-specific
RED_LED:		 	.equ 0x020	; PF1 (Red) Mask
BLUE_LED:		 	.equ 0x040	; PF2 (Blue) Mask
GREEN_LED:		 	.equ 0x080	; PF3 (Green) Mask
RGB_LED_PINS:  	 	.equ 0x00E	; GPIO Port F Pins 1-3 Mask
PWM_GEN_MASK:	 	.equ 0x0E0	; PWM Generator Pins 1-3 Mask
GPIO_PORTF_RCGC: 	.equ 0x020	; GPIO Port F Enable RCGC Mask

CMPA_CONF_MASK:	 	.equ 0x041
CMPB_CONF_MASK:		.equ 0x401


	.sect macros
; Note:
;       The the operations in this macro library do not support
;       conditional options (i.e., either to set NVZC flags to as a part of an IT block)

; ************************** MOVE IMMEDIATES ******************************
; Move Full
;       Takes a 32-bit immediate and loads that value into rd
MOVF 	.macro reg, data
		MOV reg, #(data & 0x0000FFFF)
		MOVT reg, #((data & 0xFFFF0000) >> 16)
		.endm


; Move Full NOT
;       Takes a 32-bit immediate and loads NOT(value) into rd
MOVFN 	.macro rd, immed
		MOVF rd, (~immed)
		.endm


; *********************** LOGICAL OPERATORS ********************************



; Exclusive-Or
;   Takes a source register and a flexible operand,
;   which can be either a register or a 32-bit immediate

XOR 	.macro rd, rs, flexop

		MOVF rd, (src ^^ flexop)
		.endm

; Nor
;   Takes a source register and a flexible operand,
;   which can be either a register or a 32-bit immediate

NOR		.macro rd, rs, flexop
		.asg rs,src_reg
		.asg flexop,flex_reg
		MOVF rd, #(~(src_reg | flex_reg)
		.endm

; XNOR
;   Takes a source register and a flexible operand,
;   which can be either a register or a 32-bit immediate

XNOR 	.macro rd, rs, flexop
		.asg rs,src_reg
		.asg flexop,flex_reg
		MOVF rd, #(~(src_reg ^ flex_reg)
		.endm


; ************************** QUICK CONFIGS *****************************

SRB 	.macro reg,base_addr,offset,value
		LDR reg, [base_addr, #offset]
		.asg reg,tmp
		.eval (tmp | value),tmp
		MOVF reg, tmp
		STR reg, [base_addr, #offset]
		.endm

CLRBIT 	.macro temp,base_addr,offset,value
		LDR temp, [base_addr, #offset]
		BIC temp, temp, value
		STR temp, [base_addr, #offset]
		.endm


	.text

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END MACROS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



pwm_with_generator:
	PUSH {lr}

	BL uart_init
	BL gptm_init
	BL gpio_init
	BL pwm_init


	LDR r0, ptr_to_start_prompt
	BL output_string

wait_on_user_loop:
	BL read_character
	CMP r0, #ASCII_ESC
	BEQ pwm_with_generator_end
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
	BL update_duty_cycles
	B wait_on_user_loop

pwm_with_generator_end:
	LDR r0, ptr_to_goodbye
	BL output_string

	POP {pc}



; General-Purpose Timer Initialization (pg. 725)
; 	TIMERn set in RCGCTIMER (pg. 338) or RCGCWTIMER (pg. 357)
;	Enable GPIO module clock in RCGCGPIO (pg. 340, Table 23-4 pg. 1344)
;	PMCn set in GPIOPCTL to assign signals to appropriate pins (pg. 688, Table 23-5 pg. 1351)
;
gptm_init:
	PUSH {lr}
	; System control base address
    MOVF r0, 0x400FE000

    ; Enable clock on 16/32-bit Timer 0  (pg. 338)
    LDR r1, [r0, #RCGCTIMER]
    ORR r1, r1, #0x001
    STR r1, [r0, #RCGCTIMER]

	; 16/32-bit Timer 0 base address
  	MOVF r0, 0x40030000

	; Ensure timer is disabled by clearing TAEN bit in GPTMCTL (pg. 737)
  	LDR r1, [r0, #GPTMCTL]
  	BIC r1, r1, #0x001
  	STR r1, [r0, #GPTMCTL]

  	; Choose 16-bit mode in GPTMCFG (pg. 727)
  	MOV r1, #0x004
  	STR r1, [r0, #GPTMCFG]

  	; Configure GPTMTAMR -> TAAMS = 1, TACMR = 0, TAMR = 0x002 (pg. 729)
  	LDR r1, [r0, #GPTMTAMR]
  	ORR r1, r1, #0x00A
  	STR r1, [r0, #GPTMTAMR]

  	; Load timer start value of 255 to GPTMTAILR (pg. 756)
  	MOV r1, #0x0FF
  	STR r1, [r0, #GPTMTAILR]

  	; Load match value to GPTMAMATCHR to determine duty cycle (pg. 758)
  	;	Value used along with GPTMTAILR
  	MOV r1, #0x07F
  	STR r1, [r0, #GPTMTAMATCHR]

  	; Enable timer and begin PWM generator by setting TAEN in GPTMCTL
  	LDR r1, [r0, #GPTMCTL]
  	ORR r1, r1, #0x001
  	STR r1, [r0, #GPTMCTL]

	POP {pc}
; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END GPTM INIT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

gpio_init:
	PUSH {lr}
	; System control base address
	MOV r0, #0xE000
	MOVT r0, #0x400F

	; Enable the clock to Port F in RCGCGPIO (pg. 340)
	LDR r1, [r0, #RCGCGPIO]
	ORR r1, r1, #GPIO_PORTF_RCGC
	STR r1, [r0, #RCGCGPIO]
	nop

	; GPIO Port F base address
	MOV r0, #0x5000
	MOVT r0, #0x4002

	; Set direction of GPIO Port F pins 1-3 as output in GPIODIR (pg. 663)
	MOV r1, #RGB_LED_PINS
	STR r1, [r0, #GPIODIR]

	; Configure GPIOAFSEL to program as PWM pins (pg. 671)
	STR r1, [r0, #GPIOAFSEL]

	; Must program PMCx field in GPIOPCTL to select M1PWMx (pg. 688, Table 23-5 pg. 1351)
	MOV r1, #0x5550
	STR r1, [r0, #GPIOPCTL]

	; Enable pins as digital I/Os in GPIODEN (pg. 682)
	MOV r1, #RGB_LED_PINS
	STR r1, [r0, #GPIODEN]

	POP {pc}

; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BEGIN PWM INIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

; Overall - pg. 1239-1240
; Enable clock for PWM module - pg. 354
; Set PWM divisor - pg. 254

pwm_init:
	PUSH {lr}
	; System Control base address
	MOV r0, #0xE000
	MOVT r0, #0x400F

  	; Enable PWM module 1 clock via RCGCPWM in System Control module (pg. 354)
  	;  in System Control module
  	LDR r1, [r0, #RCGCPWM]
  	ORR r1, r1, #0x002
  	STR r1, [r0, #RCGCPWM]

  	; Configure RCC in System Control module to use the
  	;   PWM divide and set to divide by 64 (pg. 254)
  	LDR r1, [r0, #RCC]
  	MOV r2, #0x00F
  	BFI r1, r2, #0x011, #0x004
  	STR r1, [r0, #RCC]


  	; Configure PWM gen for countdown mode with immed. updates
  	; PF1 -> Module 1 PWM 5, controlled by Module 1 Generator 2
  	; PF2 -> Module 1 PWM 6, controlled by Module 1 Generator 3
  	; PF3 -> Module 1 PWM 7, controlled by Module 1 Generator 3

  	; PWM Module 1 base address
  	MOVF r0, 0x40029000

  	; Disable PWM clock via PWMnCTL (pg. 1266)
  	MOV r1, #0x000
  	STR r1, [r0, #PWM2CTL]
  	STR r1, [r0, #PWM3CTL]

  	; When Counter = Comparator A or Zero, invert pwmA (pg. 1282)
  	MOV r1, #CMPA_CONF_MASK
  	STR r1, [r0, #PWM3GENA]

  	; When Counter = Comparator B or Zero, invert pwmB (pg. 1285)
  	MOV r1, #CMPB_CONF_MASK
  	STR r1, [r0, #PWM2GENB]
  	STR r1, [r0, #PWM3GENB]


  	; Set load to countdown from 255 (pg. 1278)
  	MOV r1, #0x0FF
  	STR r1, [r0, #PWM2LOAD]
  	STR r1, [r0, #PWM3LOAD]

  	; Set pulse width for 50% duty cycle (pg. 1280-1281)
  	;	Blue
  	MOV r1, #0x0FF
  	STR r1, [r0, #PWM3CMPA]

	;	Red
  	MOV r1, #0x0FF
  	STR r1, [r0, #PWM2CMPB]

	; 	Green
  	MOV r1, #0x0FF
  	STR r1, [r0, #PWM3CMPB]

  	; Start timers (pg. 1266)
  	MOV r1, #0x001
  	STR r1, [r0, #PWM2CTL]
  	STR r1, [r0, #PWM3CTL]
	nop
	nop

  	; Wait for user before enabling PWM output


  	POP {pc}



update_duty_cycles:
	PUSH {lr}

	; r0 = Hex color code
	; r1 = PWM1 base address
	; r2 = PWMENABLE mask
	MOVF r1, 0x40029000
	MOV r2, #0x000

	; Red
	ANDS r3, r0, #0xFF0000
	ITTT GT
	ORRGT r2, r2, #RED_LED
	LSRGT r3, r3, #0x010
	STRGT r3, [r1, #PWM2CMPB]

	; Green
	ANDS r3, r0, #0x00FF00
	ITTT GT
	ORRGT r2, r2, #GREEN_LED
	LSRGT r3, r3, #0x008
	STRGT r3, [r1, #PWM3CMPB]

	; Blue
	ANDS r3, r0, #0x0000FF
	ITTT GT
	ORRGT r2, r2, #BLUE_LED
	STRGT r3, [r1, #PWM3CMPA]

	STR r2, [r1, #PWMENABLE]

	POP {pc}



  	.end
