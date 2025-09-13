  .text

ref_string:    .string "9999999999999999999999", 0
tmp_string:    .string "0000000000000000000000", 0
sp_string:     .string "x1.0000000000000000000000", 0

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

ASCII_ZERO:        .equ 0x030

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

  POP {pc}

find_decimals:
  PUSH {r4-r5, lr}

  ; r0 is the address of the bit string
  ; r1 is the address of the reference string
  ; r2 is the first byte of the bit string
  ; r3 is the first byte of the reference string

MOV r4, #0x001

decimal_loop:
  MUL r4, r4, #0x005

  ; Skip if 
  LDRB r2, [r0]
  CMP r2, #ASCII_ZERO
  BEQ decimal_loop
  
  LDRB r3, [r1]
  
  ; Can possibly use our division routine 
  ; to take advantage of return values
  ;   (quotient and remainder)

  POP {r4-r5, pc}

  .end
