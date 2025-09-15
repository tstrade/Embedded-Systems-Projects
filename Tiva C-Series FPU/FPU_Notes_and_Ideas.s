  .text

empty_string:  .string "0.00000000000000000000000", 0
pow_3:         .string "0.12500000000000000000000", 0
pow_4:         .string "0.06250000000000000000000", 0
pow_8:         .string "0.00390625000000000000000", 0
pow_12:        .string "0.00024414062500000000000", 0
pow_16:        .string "0.00000152587890625000000", 0
pow_20:        .string "0.00000095367431640625000", 0
pow_24:        .string "0.00000005960464477539063", 0

max:           .string "0.99999988079071044921875", 0


sign:      .byte 0x0
exponent:  .hword 0x0000

  .data
  .global float2string
  .global int2string
  .global string2int

; SINGLE-PRECISION FORMAT
; 
; | 1-Bit Sign | 8-Bit Exponent | 23-Bit Mantissa |

ptr_to_ref_string: .word ref_string
ptr_to_tmp_string: .word tmp_string
ptr_to_sp_string:  .word sp_string
ptr_to_sign:       .word sign
ptr_to_exponent:   .word exponent

EXPO_SP_MASK:      .equ 0x0FF ; 8-bit mask
SIGN_SP_SHIFT:     .equ 0x01F ; Shift left 31 bits
EXPO_SP_SHIFT:     .equ 0x017 ; Shift left 23 bits
EXPO_SP_BIAS:      .equ 0x074 ; Bias = 127

ASCII_POINT:       .equ 0x02E
ASCII_ZERO:        .equ 0x030
ASCII_ONE:         .equ 0x031
ASCII_NINE:        .equ 0x039
POW_OFFSET:        .equ 0x018 ; Number of bytes to last possible digit in string

simple_add_ascii:
  PUSH {lr}
  
  ; r0 = first ascii value
  ; r1 = second ascii value
  ; r2 = carry-in value
  ADD r0, r0, r1
  SUB r0, r0, #ASCII_ZERO

  CMP r0, #ASCII_NINE
  ITTE GT
  SUBGT r0, r0, #0x00A
  MOVGT r1, #ASCII_ONE
  MOVLE r1, #ASCII_ZERO

  ADD r0, r0, r2
  SUB r0, r0, #ASCII_ZERO
  
  ; Returns sum and carry-out, if applicable
  POP {pc}


single_precision_add_ascii:
  PUSH {r4-r5, lr}
  ; r0 = base address of first string
  ; r1 = base address of second string
  ADD r4, r0, #POW_OFFSET  ; These might need to be sub I forgot 
  ADD r5, r1, #POW_OFFSET  ; what endianness tiva uses

  MOV r2, #ASCII_ZERO      ; First carry-in value should be zero

  ; Load least significant byte of each string 
  ;  until the decimal point is reached
sp_add_ascii_loop:
  LDRB r0, [r4]
  CMP r0, #ASCII_POINT
  BEQ sp_add_ascii_end
  
  LDRB r1, [r5], #-0x001

  ; Add and store sum / carry-out value
  BL simple_add_ascii

  MOV r2, r1 
  STRB r0, [r4], #-0x001

  B sp_add_ascii_loop  

sp_add_ascii_end:
  POP {r4-r5, pc}




single_precision_mul_ascii:
  PUSH {r4-r5, lr}
  ; r0 = base address of string to multiply
  ; r1 = multiplier
  MOV r4, r0
  MOV r5, r1

sp_mul_loop:
  SUBS r5, r5, #0x001
  BLT sp_mul_end

  MOV r0, r4
  MOV r1, r4
  BL single_precision_add_ascii

  B sp_mul_loop
  
sp_mul_end:
  POP {r4-r5, pc}




; r0 = floating point number
; r1 = address of string
float2string:
  PUSH {r3-r5, lr}

  ; Store in case needed later
  MOV r4, r0
  MOV r5, r1 

  ; Store sign of floating point
  LDR r2, ptr_to_sign
  SLR r3, r0, #SIGN_SP_SHIFT
  STRB r3, [r2]

; Should use the exponent on the mantissa before isolating 
  ; Store exponent of floating point
  LDR r2, ptr_to_exponent
  SLR r3, r0, #EXPO_SP_SHIFT
  AND r3, r3, #EXPO_SP_MASK
  SUB r3, r3, #EXPO_SP_BIAS
  STR r3, [r2]  

  ; Isolate fraction
  BFC r0, #0x017, #0x009
  LDR r1, ptr_to_tmp_string
  BL bit2string

; We essentially want:
:   First "byte" <= "8" -> add pow_4 "byte" times
;        else add pow_4 8 times and pow_3 8 - "byte" times
;
;  After that each byte can be added in a more normal fashion
;
;    e.g., Second "byte" -> add pow_8 "byte" times
;          Third "byte" -> add pow_12 "byte" times
;          etc.

  ; Start decoding bit string
decode_bit_string:
  LDRB r3, [r1]
  CMP r3, #ASCII_ONE
  

  POP {r3-r5, pc}
