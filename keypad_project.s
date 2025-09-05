	.text
	.global keypad_init
	.global keypad_project

; Offsets from Table 5-7. System Control Register Map (pg. 232-237)
GPIORCGC:         .equ 0x0608   ; GPIO Clock Register
GPIODIR:          .equ 0x0400	; GPIO Direction Register
GPIODEN:          .equ 0x051C	; GPIO Digital Register
GPIODATA:         .equ 0x03FC	; GPIO Data Register
GPIOPUR:          .equ 0x0510	; GPIO Pull-Up Resistor Register


; GENERAL KEYPAD LAYOUT
;
;   | PA2 | PA3 | PA4 | PA5 |     |
;   -------------------------------
;   |  #0 |  #1 |  #2 |  #3 | PD0 |
;   |  #4 |  #5 |  #6 |  #7 | PD1 | 
;   |  #8 |  #9 | #10 | #11 | PD2 | 
;   | #12 | #13 | #14 | #15 | PD3 |
;   -------------------------------



keypad_init:
	PUSH {lr}	; Store register lr on stack
    
    ; System control register base address in 0x400FE000 (pg. 231)
    MOV r0, #0xE000
    MOVT r0, #0x400F

    ; Set bits 0 and 3 of GPIO Run Mode Clock Gating Control
    ;   to enable the clock for ports A and D
    LDR r1, [r0, #GPIORCGC]
    ORR r1, r1, #0x0009
    STR r1, [r0, #GPIORCGC]

    ; Must be a delay of 3 system clocks after GPIO module clock 
    ;   is enabled (pg. 658-659)
    nop
    nop
    nop

    ; Configure GPIO Port D (base address = 0x40007000)
    MOV r0, #0x7000
    MOVT r0, #0x4000

    ; Configure Pins 0-3 as input 
    LDR r1, [r0, #GPIODIR]
    BIC r1, r1, #0x000F
    STR r1, [r0, #GPIODIR]

    ; Set Digital Enable for Pins 0-3
    LDR r1, [r0, #GPIODEN]
    ORR r1, r1, #0x000F
    STR r1, [r0, #GPIODEN]

    ; Configure GPIO Port A (base address = 0x40007000)
    MOV r0, #0x4000
    MOVT r0, #0x4000

    ; Configure Pins 2-5 as output 
    LDR r1, [r0, #GPIODIR]
    ORR r1, r1, #0x003C
    STR r1, [r0, #GPIODIR]

    ; Set Digital Enable for Pins 2-5
    LDR r1, [r0, #GPIODEN]
    ORR r1, r1, #0x003C
    STR r1, [r0, #GPIODEN]

    

 
	POP {lr}
	MOV pc, lr

keypad_project:
	PUSH {lr}	; Store register lr on stack
    
          ; Your code is placed here
 
	POP {lr}
	MOV pc, lr


	.end

