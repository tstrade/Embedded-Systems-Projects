    .data

src:    .byte 0x03          ; Data buffer
		.byte 0x02
		.byte 0x01
		.byte 0x00
		.byte 0x07
		.byte 0x06
		.byte 0x05
		.byte 0x04
		.byte 0x0B
		.byte 0x0A
		.byte 0x09
		.byte 0x08
		.byte 0x0F
		.byte 0x0E
		.byte 0x0D
		.byte 0x0C


    .sect "control_channel_table"
    .align 1024

    .space 0x50			; Channels 0-4 unused
    ; Channel 5
    .word src           ; Source End Pointer
    .word 0x4000503C    ; Destination End Pointer
    .word 0xCC000009    ; Control Word
    .word 0x00000000    ; Unused

    .space 0xC0			; Channels 6-17 unused
    ; Channel 18
    .word src
    .word 0x4000503C
    .word 0xCC000009
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

ptr_to_channel_control_table:   .word src

    .global ptr_to_channel_control_table


	.sect "macros"
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


dma_project:
    PUSH {lr}


    POP {pc}

TimerHandler:
	PUSH {lr}

	MOVF r0, 0x40030024
	LDR r1, [r0]
	ORR r1, r1, #0x01
	STR r1, [r0]

	POP {lr}
	BX lr

    .end
