	.data

command_buffer:
	.space 128

	.text

	.global strcmp
	.global strcat
	.global strlen
	.global output_character
	.global read_character
	.global output_string
	.global read_string
	.global clear_cmd_buffer

cmd_buff_ptr:	.word command_buffer

	.global cmd_buff_ptr

NULL_BYTE:	.equ 0x00
U0FR:		.equ 0x18
ASCII_CR:	.equ 0x0D
ASCII_NL:	.equ 0x0A


	.sect "macros"

; Move Full
;       Takes a 32-bit immediate and loads that value into rd
MOVF 	.macro reg, data
		MOV reg, #(data & 0x0000FFFF)
		MOVT reg, #((data & 0xFFFF0000) >> 16)
		.endm

	.text

; *******************************************************************************************************

; r0 = str1 addr; r1 = str2 addr
strcmp:
	PUSH {r4, lr}

	; r4 = status (0 = equal, 1 = not equal)
	MOV r4, #0x00

cmp_loop:
	LDRB r2, [r0], #0x01
	LDRB r3, [r1], #0x01
	; Check if bytes are equal
	CMP r2, r3
	ITT NE
	MOVNE r4, #0x01
	BNE cmp_end

	; If bytes are equal, we only have to
	; 	compare one of the bytes to NULL_BYTE
	CMP r2, #NULL_BYTE
	BEQ cmp_end

	B cmp_loop

cmp_end:
	MOV r0, r4
	POP {r4, pc}

; *******************************************************************************************************

; r0 = str1 addr; r1 = str2 addr
strcat:
	PUSH {r4, lr}

	; Concatenation will be stored in a buffer to ensure
	; 	space for all commands (variable # of params)
	LDR r2, cmd_buff_ptr

	; Limit concatenation size to 127 characters (arbitrary)
	;	128th character MUST be a NULL byte to ensure proper string
	MOV r3, #0x10000

concat_first:
	SUBS r3, r3, #0x01
	BLT concat_end
	LDRB r4, [r0], #0x01
	CMP r4, #NULL_BYTE
	ITT NE
	STRBNE r4, [r2], #0x01
	BNE concat_first

concat_second:
	SUBS r3, r3, #0x01
	BLT concat_end
	LDRB r4, [r1], #0x01
	CMP r4, #NULL_BYTE
	ITT NE
	STRBNE r4, [r2], #0x01
	BNE concat_second

concat_end:
	LDR r0, cmd_buff_ptr
	POP {r4, pc}

; *******************************************************************************************************

; r0 = string address
strlen:
	PUSH {lr}

	MOV r1, #0x00

len_loop:
	LDRB r2, [r0], #0x01
	CMP r2, #NULL_BYTE
	ITT NE
	ADDNE r1, r1, #0x01
	BNE len_loop

	MOV r0, r1
	POP {pc}

; *******************************************************************************************************

output_character:
	PUSH {lr}

	MOVF r1, 0x4000C000		; UART0 base addr

poll_txff:
	LDRB r2, [r1, #U0FR]	; Get data UART flag register
	TST r2, #0x20		; Check TxFF
	BNE poll_txff			; Branch if TxFF is 1

	STRB r0, [r1]			; Store byte to UART data register

	POP {pc}

; *******************************************************************************************************

read_character:
	PUSH {lr}

	MOVF r1, 0x4000C000		; UART0 base addr

RxFETest:
	LDRB r2, [r1, #U0FR]	; Load address + offset byte into r5
	TST r2, #0x10			; Check RxFE
	BNE	RxFETest			; RxFE is empty, loop

	LDRB r0, [r1]			; Load data into r0

	POP {pc}

; *******************************************************************************************************

read_string:
	PUSH {r4, lr}

	MOV r4, r0				; Save input address

read_loop:
	BL read_character
	BL output_character
	CMP r0, #0x0D			; Check if "enter" was pressed
	BEQ enter

	STRB r0, [r4], #0x01	; Store the read character at input addr
	B read_loop

enter:
	MOV r1, #NULL_BYTE
	STRB r1, [r4]

	POP {r4, pc}

; *******************************************************************************************************

output_string:
	PUSH {r4, lr}

	MOV r4, r0				; Save input address

poll_string:
	LDRB r0, [r4], #0x01

	CMP r0, #NULL_BYTE		; Poll until NULL byte is reached
	BEQ output_string_end

	BL output_character
	B poll_string

output_string_end:

	POP {r4, pc}

; *******************************************************************************************************

clear_cmd_buffer:
	PUSH {lr}

	LDR r0, cmd_buff_ptr
	MOV r1, #0x10
	MOV r2, #0x000
clear_loop:
	SUBS r1, r1, #0x01
	BEQ end_clear
	STRD r2, r2, [r0], #0x8
	B clear_loop

end_clear:
	POP {pc}

	.end
