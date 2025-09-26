; 2 total PWM modules
;  > 4 PWM generator blocks and a control block
;   >> 16 PWM outputs
;   >> control block determines polarity
;   >> each generator block produces 2 PWM signals
;      w/ same timer / frequency (can be independent
;      or single pair of complementary signals w/ 
;      dead-band delays inserted)

; PWM Generator Block Features:
;  > 1 fault-condition handling inputs for low-latency shutdown
;      and to prevent damage to motor (total 2 inputs)
;
;  > 16-bit counter 
;    >> Down or Up/Down mode
;    >> Output freq. controlled by 16-bit load value
;    >> Load value updates can be synchronized
;    >> Produces output signals at zero and load value
;
;  > 2 PWM comparators
;    >> Value updates can be synchronized
;    >> Produces output signals on match
;
;  > PWM signal generator
;    >> Output PWM signal based on actions taken as result
;        of counter and comparator output signals
;
;  > Dead-band generator
;    >> Produces 2 PWM signals w/ programmable dead-band delays
;        suitable for driving a half-H bridge
;    >> Can be bypassed, leaving input PWM signals unmodified
;
;  > Initiate ADC sample sequence
;  > PWM output enable of each PWM signal
;  > Option output inversion (polarity control)
;  > Optional fault handling
;  > Synchronization of timers in PWM generator blocks
;  > Synchronization of timer/comparator updates
;  > Extended PWM synchronization of timer/comparator updates
;  > Interrupt status summary
;  > Extended PWM fault handling, w/ multiple fault signals, 
;      programmable polarities, and filtering
;  > Independent or synchronized operation



  .text

  .data

RCGC0:      .equ ;offset
RCGC2:      .equ ;offset
GPIOAFSEL:  .equ ;offset
GPIOPCTL:   .equ ;offset
RCC:        .equ ;offset

PWMENABLE:  .equ 0x008
PWM0CTL:    .equ 0x040
PWM0LOAD:   .equ 0x050
PWM0CMPA:   .equ 0x058
PWM0CMPB:   .equ 0x05C
PWM0GENA:   .equ 0x060
PWM0GENB:   .equ 0x064

pwm_init:
  PUSH {lr}

  ; Enable PWM clock
  MOV r0, #0xE000E000
  MOV r1, #0x00100000
  STR r1, [r0, #RCGC0]

  ; Enable clock for appropriate GPIO module
  MOV r1, #GPIO_MODULE_BIT
  STR r1, [r0, #RCGC2]

  ; Enable alt. function for appropriate pins
  MOV r0, #GPIO_PORT_BASE_ADDR
  LDR r1, [r0, #GPIOAFSEL]
  ORR r1, r1, #GPIO_PIN_MASK
  STR r1, [r0, #GPIOAFSEL]

  ; Configure PMCn fields to assign PWM signals to pins
  LDR r1, [r0, #GPIOCTL]
  ; Config
  STR r1, [r0, #GPIOCTL]

  ; Configure RCC to use PWM divide (USEPWMDIV) and 
  ;  set divider (PWMDIV) to divide by 2 (000)
  MOV r0, #0xE000E000
  LDR r1, [r0, #RCC]
  ; Config
  STR r1, [r0, #RCC]

  ; Configure PWM for countdown mode w/ immediate
  ;  updates to parameters
  MOV r0, #0x40028000
  MOV r1, #0x000
  STR r1, [r0, #PWM0CTL]
  MOV r1, #0x08C
  STR r1, [r0, #PWM0GENA]
  MOV r1, #0x080C
  STR r1, [r0, #PWM0GENB]

  ; Set period (e.g., 25 kHz -> 1/25000 = 40 us)
  ;  PWM clock source is 10 MHz, sys clock is 5 MHz
  ;  -> 400 clock ticks per period
  ; In count-down mode, set LOAD field in PWM0LOAD
  ;  to requested period minus one
  MOV r1, #REQ_PERIOD
  STR r1, [r0, #PWM0LOAD]

  ; Set pulse width of MnPWM0 pin for 25% duty cycle
  MOV r1, #0x012B
  STR r1, [r0, #PWM0CMPA]

  ; Set pulse width of MnPWM1 for 75% duty cycle
  MOV r1, #0x063
  STR r1, [r0, #PWM0CMPB]

  ; Start timers in PWM0
  MOV r1, #0x001
  STR r1, [r0, #PWM0CTL]

  ; Enable PWM outputs
  MOV r1, #0x003
  STR r1, [r0, #PWMENABLE]

  POP {pc}
  





