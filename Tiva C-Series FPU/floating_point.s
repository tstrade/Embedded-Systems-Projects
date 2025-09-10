; FPU Registers - Base Addr. = 0xE000E000 (pg. 194)
CPAC:    .equ 0xD88    ; Coprocessor Access Control (pg. 195)
;        Bits 23:22 - CP11 Coproccessor Access Privilege 
;        Bits 21:20 - CP10 Coprocessor Access Privilege
;          0x0 - Denied, 0x1 - Privileged, 0x3 - Full Access

FPCC:    .equ 0xF34    ; Floating-Point Context Control (pg. 196)
;        Bit 31 - ASPEN (Automatic State Preservation Enable)
;                  to preserve FP context on exception entry/exit
;        Bit 30 - LSPEN (Lazy State Preservation Enable)
;        Bit 8 - MONRDY (Monitor Ready) permits setting MON_PEND
;                  when FP stack frame was allocated
;        Bit 6 - BFRDY (Bus Fault Ready)
;        Bit 5 - MMRDY (Memory Management Fault Ready)
;        Bit 4 - HFRDY (Hard Fault Ready)
;        Bit 3 - THREAD (Thread mode)
;        Bit 1 - USER (User Privilege Level)
;        Bit 0 - LSPACT (Lazy State Preservation Active)

FPCA:    .equ 0xF38    ; Floating-Point Context Address (pg. 198)
;        Bits 31:3 - Address of unpopulated FP register space
;                      allocated on an exception stack frame

FPDSC:   .equ 0xF3C    ; Floating-Point Default Status Control (pg. 199)
;        Bit 26 - Holds default value for AHP bit in FPSC register
;        Bit 25 - Holds default value for DN bit in FPSC register
;        Bit 24 - Holds default value for FZ bit in FPSC register

; FPSC = Register 22 (pg. 90)
;    Bits[31:28] - NZCV
;    Bits[26:24] - 0b010
;    Bits[23:22] - Rounding mode (nearest, +inf, -inf, to zero)
;    Bits[7,4:0] - Exception bits (input denormal, inexact cumulative
;                                 underflow, overflow, div by 0, invalid op)

; FPU description on pg. 130
; Registers:
;    [D0-D15] - 64-bit doubleword registers
;    [S0-S31] - 32-bit singleword registers
;
;    S<2n> = Least significant half of D<n>
;    S<2n=1> = Most significant half of D<n>

; Enabling FPU (pg. 134):

fpu_init:
  PUSH {lr}

  MOV r0, #0xE000E000
  ; Set bits 20-23 to enable CP10 and CP11 coprocessors
  LDR r1, [r0, #CPACR]
  ORR r1, r1, #0xF00000
  STR r1, [r0, #CPACR]

  ; Wait for store to complete
  ;   "Data Synchronization Barrier"
  DSB

  ; Reset pipeline now the FPU is enabled
  ;  "Instruction Synchronization Barrier"
  ISB
  
  POP {lr}
  MOV pc, lr


  
