	.data

	.text

	.global Timer0A_Handler
	.global Timer0B_Handler

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

	MOVF r0, 0x40030024
	LDR r1, [r0]
	ORR r1, r1, #0x1
	STR r1, [r0]

	POP {lr}
	BX lr


Timer0B_Handler:
	PUSH {lr}

	MOVF r0, 0x40030024
	LDR r1, [r0]
	ORR r1, r1, #0x100
	STR r1, [r0]

	POP {lr}
	BX lr
