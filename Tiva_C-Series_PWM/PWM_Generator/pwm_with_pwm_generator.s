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
	.global illuminate_RGB_LED
	.global rgb_led_init
	.global pwm_init
	.global pwm_project

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
ptr_to_rgb_input:		.word rgb_input
ptr_to_red_dc:			.word red_duty_cycle
ptr_to_blue_dc:			.word blue_duty_cycle
ptr_to_green_dc:		.word green_duty_cycle


; Cortex M4 Peripherals
ENUART0:          .equ 0x020	; UART0 Enable Bit
ENGPIOD:		  .equ 0x008	; GPIO Port D Enable Bit
EN0:              .equ 0x100	; Interrupt 0-31 Set Enable

; >>> CM4 Peripheral - FPU
CPACR:   		  .equ 0xD88    ; Coprocessor Access Control (pg. 195)
FPCCR:   		  .equ 0xF34    ; Floating-Point Context Control (pg. 196)
FPCAR:   		  .equ 0xF38    ; Floating-Point Context Address (pg. 198)
FPDSCR:  		  .equ 0xF3C    ; Floating-Point Default Status Control (pg. 199)


; System Control
RCGC2:			  .equ 0x108 	; Run Mode Clock Gating Control Register 2
RCGCTIMER:        .equ 0x604	; 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control
RCGCGPIO:         .equ 0x608   	; GPIO Run Mode Clock Gating Control


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


; General Purpose Timers
GPTMCFG:          .equ 0x000	; GPTM Configuration
GPTMTAMR:         .equ 0x004	; GPTM Timer A Mode
GPTMCTL:          .equ 0x00C	; GPTM Control
GPTMIMR:          .equ 0x018	; GPTM Interrupt Mask
GPTMICR:	      .equ 0x024	; GPTM Interrupt Clear Register
GPTMTAILR:        .equ 0x028	; GPTM Timer A Interval Load
GPTMTAR:          .equ 0x048	; GPTM Timer A


; Universal Asynchronous Receiver/Transmitters
UARTDR:			  .equ 0x000	; UART Data Register
RXIM:             .equ 0x010	; UART Receive Interrupt Mask
RXIC:             .equ 0x010	; UART Receive Raw Interrupt Status
U0FR: 			  .equ 0x018	; UART0 Flag Register
UARTIBRD: 		  .equ 0x024	; UART Integer Baud-Rate Divisor
UARTFBRD: 		  .equ 0x028	; UART Fractional Buad-Rate Divisor
UARTLCRH:		  .equ 0x02C	; UART Line Control
UARTCTL:		  .equ 0x030	; UART Control
UARTIM:           .equ 0x038	; UART Interrupt Mask
UARTICR:          .equ 0x044	; UART Interrupt Clear Register
UARTRCGC:		  .equ 0x618	; UART Run Mode Clock Gating Control
UARTCC:			  .equ 0xFC8	; UART Clock Configuration


; PWM
PWMCTL:     	  .equ 0x000    ; PWM Master Control
PWMENABLE:  	  .equ 0x004    ; PWM Output Enable
PWMSTATUS:  	  .equ 0x020    ; PWM Status
PWMENUPD:   	  .equ 0x028    ; PWM Enable Update
PWMPP:     		  .equ 0xFC0    ; PWM Peripheral Properties

; >>> Module Offset = (Module #) * 0x40
PWM0CTL:     	  .equ 0x040    ; PWM0 Control
PWM0LOAD:   	  .equ 0x050    ; PWM0 Load
PWM0COUNT:  	  .equ 0x054    ; PWM0 Counter
PWM0CMPA:   	  .equ 0x058    ; PWM0 Compare A
PWM0CMPB:    	  .equ 0x05C    ; PWM0 Compare B
PWM0GENA:   	  .equ 0x060    ; PWM0 Generator A Control
PWM0GENB:   	  .equ 0x064    ; PWM0 Generator B Control
PWM0DBCTL:  	  .equ 0x068    ; PWM0 Dead-Band Control
PWM0DBRISE: 	  .equ 0x06C    ; PWM0 Dead-Band Rising-Edge Delay
PWM0DBFALL: 	  .equ 0x070    ; PWM0 Dead-Band Falling-Edge Delay


; ASCII
NULL:			  .equ 0x000
ASCII_ESC:		  .equ 0x01B
ASCII_SPACE:	  .equ 0x020
ASCII_ZERO:		  .equ 0x030
ASCII_NINE:		  .equ 0x039
ASCII_x:		  .equ 0x078
ASCII_A:		  .equ 0x041
ASCII_B:		  .equ 0x042
ASCII_C:		  .equ 0x043
ASCII_D:		  .equ 0x044
ASCII_E:		  .equ 0x045
ASCII_F:		  .equ 0x046


; Program-specific
GPIO_PORTF_RCGC:  .equ 0x020	; GPIO Port F Enable RCGC Mask
RGB_LED_PINS:  	  .equ 0x00E	; GPIO Port F Pins 1-3 Mask
RED_LED:		  .equ 0x002	; PF1 (Red) Mask
BLUE_LED:		  .equ 0x004	; PF2 (Blue) Mask
GREEN_LED:		  .equ 0x008	; PF3 (Green) Mask



; Overall - pg. 1239-1240
; Enable clock GPIO pins - pg. 341
; Setup GPIO pints Alt Func mode - pg. 671
; Select Alt Func to be PWM - pg. 688 & pg. 1351
; Enable clock for PWM module - pg. 354
; Set PWM divisor - pg. 254

; Red - Module 1, PWM output 5
pwm_init:
  PUSH {lr}
  ; Initialization and Config from pg. 1239

  ; Enable PWM clock in via RCGC0 System Control module
  MOV r0, #0xE000
  MOVT r0, #0x400F
  LDR r1, [r0, #RCGC0]
  ORR r1, r1, #0x00100000
  STR r1, [r0, #RCGC0]

  ; Must be a delay of 3 system clocks before PWM registers
  ;  can be programmed

  ; Enable clock to appropriate GPIO module via RCGC2
  ;  in System Control module


  ; Enable appropriate pins via GPIOAFSEL
  ;  Table 23-4 on pg. 1344


  ; Configure RCC in System Control module to use PWM divide
  ;  and set divider


  ; Configure PWM gen for countdown mode with immed. updates
  MOV r0, #0x8000
  MOVT r0, #0x4002

  LDR r1, [r0, #PWM0CTL]
  BIC r1, r1, #0x001
  STR r1, [r0, #PWM0CTL]

  LDR r1, [r0, #PWM0GENA]
  ORR r1, r1, #0x08C
  STR r1, [r0, #PWM0GENA]

  LDR r1, [r0, #PWM0GENB]
  ORR r1, r1, #0x80C
  STR r1, [r0, #PWM0GENB]


  ; Set period (PWM Clk Src is 10 MHz, Sys Clk is divided by 2 -> 400 clock ticks per period)
  ;  Use this value to set PWM0LOAD reg.
  ;  In count-down mode, set LOAD field to requested period minus one
  MOV r1, #0x18F
  STR r1, [r0, #PWM0LOAD]

  ; Set pulse width of MnPWM0 pin for 25% duty cycle
  MOV r1, #0x12B
  STR r1, [r0, #PWM0CMPA]

  ; Set pulse width of MnPWM1 pin for 75% duty cycle
  MOV r1, #0x63
  STR r1, [r0, #PWM0CMPB]

  ; Start timers in PWM generator 0
  MOV r1, #0x001
  STR r1, [r0, #PWM0CTL]

  ; Enable PWM outputs
  LDR r1, [r0, #PWM0CTL]
  ORR r1, r1, #0x003
  STR r1, [r0, #PWM0CTL]
