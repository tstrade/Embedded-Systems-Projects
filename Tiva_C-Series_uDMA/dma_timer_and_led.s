    .data

src:    .word 0x03020100          ; Data buffer
		.word 0x07060504
		.word 0x0B0A0908
		.word 0x0F0E0D0C



; Allocate space for control table
channel_control:
	.align 1024
    .space 1024


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

    .global ptr_to_channel_control
    .global ptr_to_channel_src


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

	; Clear DMA channel interrupt
	MOVF r0, 0x400FF504
	MOV r1, #0x40000;
	STR r1, [r0]

	LDR r0, ptr_to_channel_control
	ADD r0, r0, #0x120
	LDR r1, ptr_to_channel_src
	ADD r1, r1, #0x0F
	STR r1, [r0]

	MOVF r2, 0x400FF028
	MOV r3, #0x40000
	STR r3, [r2]

	; Reset control word
	MOVF r1, 0xC00000F1
	STR r1, [r0, #0x08]


timer_interrupt_handled:


	POP {lr}
	BX lr



    .end
