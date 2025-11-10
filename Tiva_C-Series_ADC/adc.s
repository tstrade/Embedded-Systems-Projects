	.data
	.text

	.global poll_adc


	.sect "macros"

; Move Full
;       Takes a 32-bit immediate and loads that value into rd
MOVF 	.macro reg, data
		MOV reg, #(data & 0x0000FFFF)
		MOVT reg, #((data & 0xFFFF0000) >> 16)
		.endm

	.text

poll_adc:
	PUSH {lr}

	; Trigger sampling via ADCPSSI
	MOVF r0, 0x40038028
	MOV r1, #0x8
	STR r1, [r0]

	; Poll interrupt status until sample complete
	MOVF r0, 0x40038004
wait_on_adc:
	LDR r2, [r0]
	ANDS r2, r2, r1
	BEQ wait_on_adc

	; Get data from FIFO register (lower 12 bits)
	MOVF r2, 0x400380A8
	LDR r0, [r2]
	BFC r0, #0xC, #0x14

	; Clear interrupt status
	MOVF r2, 0x4003800C
	STR r1, [r2]

	; Data is returned in r0

	POP {pc}

	.end
