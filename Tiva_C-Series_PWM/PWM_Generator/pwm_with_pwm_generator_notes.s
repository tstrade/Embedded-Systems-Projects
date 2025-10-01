PWMCTL:     .equ 0x000    ; PWM Master Control
PWMENABLE:  .equ 0x004    ; PWM Output Enable
PWMSTATUS:  .equ 0x020    ; PWM Status
PWMENUPD:   .equ 0x028    ; PWM Enable Update

PWM0CTL:    .equ 0x040    ; PWM0 Control
PWM0LOAD:   .equ 0x050    ; PWM0 Load
PWM0COUNT:  .equ 0x054    ; PWM0 Counter
PWM0CMPA:   .equ 0x058    ; PWM0 Compare A
PWM0CMPB:   .equ 0x05C    ; PWM0 Compare B
PWM0GENA:   .equ 0x060    ; PWM0 Generator A Control
PWM0GENB:   .equ 0x064    ; PWM0 Generator B Control
PWM0DBCTL:  .equ 0x068    ; PWM0 Dead-Band Control
PWM0DBRISE: .equ 0x06C    ; PWM0 Dead-Band Rising-Edge Delay
PWM0DBFALL: .equ 0x070    ; PWM0 Dead-Band Falling-Edge Delay

; Subsequent PWM modules inc. base address by 0x040

PWMPP:      .equ 0xFC0    ; PWM Peripheral Properties


pwm_init:
  PUSH {lr}
  ; Initialization and Config from pg. 1239
  
  ; Enable PWM clock in via RCGC0 System Control module
  MOV r0, #0xE000
  MOVT r0, #0x400F
  LDR r1, [r0, #RCGC0]
  ORR r1, r1, #0x00100000
  STR r1, [r0, #RCGC0]

  ; Must be a delay of 3 system clocks before PWM registers
  ;  can be programmed

  ; Enable clock to appropriate GPIO module via RCGC2
  ;  in System Control module


  ; Enable appropriate pins via GPIOAFSEL
  ;  Table 23-4 on pg. 1344


  ; Configure RCC in System Control module to use PWM divide
  ;  and set divider


  ; Configure PWM gen for countdown mode with immed. updates
  MOV r0, #0x8000
  MOVT r0, #0x4002

  LDR r1, [r0, #PWM0CTL]
  BIC r1, r1, #0x001
  STR r1, [r0, #PWM0CTL]

  LDR r1, [r0, #PWM0GENA]
  ORR r1, r1, #0x08C
  STR r1, [r0, #PWM0GENA]

  LDR r1, [r0, #PWM0GENB]
  ORR r1, r1, #0x80C
  STR r1, [r0, #PWM0GENB]


  ; Set period (PWM Clk Src is 10 MHz, Sys Clk is divided by 2 -> 400 clock ticks per period)
  ;  Use this value to set PWM0LOAD reg.
  ;  In count-down mode, set LOAD field to requested period minus one
  MOV r1, #0x18F
  STR r1, [r0, #PWM0LOAD]

  ; Set pulse width of MnPWM0 pin for 25% duty cycle
  MOV r1, #0x12B
  STR r1, [r0, #PWM0CMPA]

  ; Set pulse width of MnPWM1 pin for 75% duty cycle
  MOV r1, #0x63
  STR r1, [r0, #PWM0CMPB]

  ; Start timers in PWM generator 0
  MOV r1, #0x001
  STR r1, [r0, #PWM0CTL]

  ; Enable PWM outputs
  LDR r1, [r0, #PWM0CTL]
  ORR r1, r1, #0x003
  STR r1, [r0, #PWM0CTL]

  
