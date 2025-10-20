    .data

src:    .word 0x00010203          ; Data buffer
		.word 0x04050607
		.word 0x08090A0B
		.word 0x0C0D0E0F


    .align 1024

channel_control:
    .space 0x120		; Channels 0-17 unused

channel18:   .word src
    		 .word 0x4000503C
    		 .word 0xC00000F1
    		 .word 0x00000000

    .space 0xD0			; Rest of table allocation


    .text

    .global dma_project
    .global TimerHandler

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

ptr_to_channel_src:   			.word src
ptr_to_channel_control:			.word channel_control
ptr_to_channel18:				.word channel18

    .global ptr_to_channel_control


	.sect "macros"

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


dma_project:
    PUSH {lr}


    POP {pc}

TimerHandler:
	PUSH {lr}

	MOVF r0, 0x40030024
	LDR r1, [r0]
	ORR r1, r1, #0x01
	STR r1, [r0]

	; Check if DMA channel generated interrupt
	MOVF r0, 0x400FF504
	LDR r1, [r0]

	; Clear DMA channel interrupt
	STR r1, [r0]

	LDR r0, ptr_to_channel_src
	LDR r1, [r0]  ; Src. address
	; If the destination addr. = src + 16, reset to src
	;	else, allow it to increment
	ADD r2, r1, #0x10
	CMP r2, r1
	IT EQ
	LDREQ r1, ptr_to_channel_src
	LDR r0, ptr_to_channel18

	; Set updated src addr.
	STR r1, [r0]

	; Reset control word
	MOVF r1, 0xC00000F1
	STR r1, [r0, #0x08]



timer_interrupt_handled:

	POP {lr}
	BX lr




    .end
