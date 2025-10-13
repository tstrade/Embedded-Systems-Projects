    .data
    .text

    .global dma_project
    .global dma_init_asm

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
RCGCDMA:            .equ 0x60C  ; Micro Direct Memory Access Run Mode Clock Gating Control
RCGCPWM:      		.equ 0x640  ; Pulse Width Modulator Run Mode Clock Gating Control


; Direct Memory Management

; >>> Offset from Channel Control Table Base
DMASRCENDP:         .equ 0x000  ; DMA Channel Source Address End Pointer
DMADSTENDP:         .equ 0x004  ; DMA channel Destination Address End Pointer
DMACHCTL:           .equ 0x008  ; DMA Channel Control word

; >>> Offset from uDMA Base Address
DMACFG:             .equ 0x004  ; DMA Configuration
DMACTLBASE:         .equ 0x008  ; DMA Channel Control Base Pointer
DMAUSEBURSTSET:     .equ 0x018  ; DMA Channel Useburst Set
DMAUSEBURSTCLR:     .equ 0x01C  ; DMA Channel Useburst Clear
DMAREQMASKSET:      .equ 0x020  ; DMA Channel Request Mask Set
DMAREQMASKCLR:      .equ 0x024  ; DMA Channel Request Mask Clear
DMAENASET:          .equ 0x030  ; DMA Channel Enable Set
DMAALTCLR:			.equ 0x034	; DMA Channel Primary Alternate Clear
DMAPRIOSET:         .equ 0x038  ; DMA Channel Priority Set
DMAPRIOCLR:         .equ 0x03C  ; DMA Channel Priority Clear
DMACHMAP3:          .equ 0x51C  ; DMA Channel Map Select 3




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

	.text

; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END MACROS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

dma_project:
    PUSH {lr}


    POP {pc}



    .end
