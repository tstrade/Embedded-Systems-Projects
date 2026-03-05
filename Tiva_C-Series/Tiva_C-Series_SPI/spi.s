	.data

buffer:		.word 0x00000000
			.word 0x00000000

display:	.word 0x00000000

	.text

	.global Timer0A_Handler
	.global ptr_to_buffer
	.global ptr_to_display

ptr_to_buffer:		.word buffer
ptr_to_display: 	.word display

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

Timer0A_Handler:
	PUSH {lr}

	; Clear interrupt status
	MOVF r0, 0x40030024
	LDR r1, [r0]
	ORR r1, r1, #0x1
	STR r1, [r0]

	; Unlatch
	MOVF r0, 0x400063FC
	MOV r1, #0x0
	STR r1, [r0]

	; Check if transmitting
	MOVF r0, 0x4000A00C
wait_on_prev:
	LDR r1, [r0]
	AND r1, r1, #0x10
	CMP r1, #0x10
	BEQ wait_on_prev


	; Write data once free
	MOVF r0, 0x4000A008

	LDR r1, ptr_to_display
	LDR r2, [r1]
	LDRH r3, [r2], #0x2
	STRH r3, [r0]

	LDR r3, ptr_to_buffer
	ADD r3, r3, #0x8

	CMP r2, r3
	ITTE EQ
	SUBEQ r3, r3, #0x8
	STREQ r3, [r1]
	STRNE r2, [r1]

	; Check if transmitting
	MOVF r0, 0x4000A00C
wait_on_curr:
	LDR r1, [r0]
	AND r1, r1, #0x10
	CMP r1, #0x10
	BEQ wait_on_curr

	; Latch
	MOVF r0, 0x400063FC
	MOV r1, #0x80
	STR r1, [r0]

	POP {lr}
	BX lr
