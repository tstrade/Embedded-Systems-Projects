#ifndef __TM4C123GH6PM_REGISTERS_H__
#define __TM4C123GH6PM_REGISTERS_H__

#include <inttypes.h>



/****************************************************************/
const uint32_t *CM4_PERIPHS_BASE_ADDR = (uint32_t *)(0xE000E000U);
const uint32_t  EN0                   = (uint32_t)(0x100); // Interrupt 0-31 Set Enable
const uint32_t  EN1                   = (uint32_t)(0x104); // Interrupt 32-63 Set Enable
const uint32_t  EN2                   = (uint32_t)(0x108); // Interrupt 64-95 Set Enable
const uint32_t  EN3                   = (uint32_t)(0x10C); // Interrupt 96-127 Set Enable
const uint32_t  EN4                   = (uint32_t)(0x110); // Interrupt 128-138 Set Enable
const uint32_t  DIS0                  = (uint32_t)(0x180); // Interrupt 0-31 Clear Enable
const uint32_t  DIS1                  = (uint32_t)(0x184); // Interrupt 32-63 Clear Enable
const uint32_t  DIS2                  = (uint32_t)(0x188); // Interrupt 64-95 Clear Enable
const uint32_t  DIS3                  = (uint32_t)(0x18C); // Interrupt 96-127 Clear Enable
const uint32_t  DIS4                  = (uint32_t)(0x190); // Interrupt 128-138 Clear Enable
const uint32_t  CPAC                  = (uint32_t)(0xD88); // Coprocessor Access Control
const uint32_t  FPCC                  = (uint32_t)(0xF34); // Floating-Point Context Control
const uint32_t  FPCA                  = (uint32_t)(0xF38); // Floating-Point Context Address
const uint32_t  FPDSC                 = (uint32_t)(0xF3C); // Floating-Point Default Status Control
/****************************************************************/


/****************************************************************/
const uint32_t *SYS_CONTROL_BASE_ADDR = (uint32_t *)(0x400FE000U);
const uint32_t  RCC                   = (uint32_t)(0x060); // Run-Mode Clock Configuration
const uint32_t  RCGCTIMER             = (uint32_t)(0x604); // 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control
const uint32_t  RCGCGPIO              = (uint32_t)(0x608); // General-Purpose Input/Output Run Mode Clock Gating Control
const uint32_t  RCGCDMA               = (uint32_t)(0x60C); // Micro Direct Memory Access Run Mode Clock Gating Control
const uint32_t  RCGCUART              = (uint32_t)(0x618); // Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
const uint32_t  RCGCPWM               = (uint32_t)(0x640); // Pulse Width Modulation Run Mode Clock Gating Control
/****************************************************************/


/****************************************************************/
const uint32_t  DMASRCENDP            = (uint32_t)(0x000); // Channel Source End Pointer
const uint32_t  DMADSTENDP            = (uint32_t)(0x004); // Channel Destination Address End Pointer
const uint32_t  DMACHCTL              = (uint32_t)(0x008); // Channel Control Word

const uint32_t *UDIR_MEMACC_BASE_ADDR = (uint32_t *)(0x400FF000U);
const uint32_t  DMACFG                = (uint32_t)(0x004); // Configuration
const uint32_t  DMAACTLBASE           = (uint32_t)(0x008); // Channel Control Base Pointer
const uint32_t  DMAUSEBURSTSET        = (uint32_t)(0x018); // Channel Useburst Set
const uint32_t  DMAUSEBURSTCLR        = (uint32_t)(0x01C); // Channel Useburst Clear
const uint32_t  DMAREQMASKSET         = (uint32_t)(0x020); // Channel Request Mask Set
const uint32_t  DMAREQMASKCLR         = (uint32_t)(0x024); // Channel Request Mask Clear
const uint32_t  DMAENASET             = (uint32_t)(0x030); // Channel Enable Set
const uint32_t  DMAALTCLR             = (uint32_t)(0x034); // Channel Primary Alternate Clear
const uint32_t  DMAPRIOSET            = (uint32_t)(0x038); // Channel Priority Set
const uint32_t  DMAPRIOCLR            = (uint32_t)(0x03C); // Channel Priority Clear
const uint32_t  DMACHMAP0             = (uint32_t)(0x510); // Channel Map Select 0
const uint32_t  DMACHMAP1             = (uint32_t)(0x514); // Channel Map Select 1
const uint32_t  DMACHMAP2             = (uint32_t)(0x518); // Channel Map Select 2
const uint32_t  DMACHMAP3             = (uint32_t)(0x51C); // Channel Map Select 3
/****************************************************************/


/****************************************************************/
const uint32_t *GPIO_PORTAP_BASE_ADDR = (uint32_t *)(0x40004000U); 
const uint32_t *GPIO_PORTAH_BASE_ADDR = (uint32_t *)(0x40058000U);
const uint32_t *GPIO_PORTBP_BASE_ADDR = (uint32_t *)(0x40005000U);
const uint32_t *GPIO_PORTBH_BASE_ADDR = (uint32_t *)(0x40059000U);
const uint32_t *GPIO_PORTCP_BASE_ADDR = (uint32_t *)(0x40006000U);
const uint32_t *GPIO_PORTCH_BASE_ADDR = (uint32_t *)(0x4005A000U);
const uint32_t *GPIO_PORTDP_BASE_ADDR = (uint32_t *)(0x40007000U);
const uint32_t *GPIO_PORTDH_BASE_ADDR = (uint32_t *)(0x4005B000U);
const uint32_t *GPIO_PORTEP_BASE_ADDR = (uint32_t *)(0x40024000U);
const uint32_t *GPIO_PORTEH_BASE_ADDR = (uint32_t *)(0x4005C000U);
const uint32_t *GPIO_PORTFP_BASE_ADDR = (uint32_t *)(0x40025000U);
const uint32_t *GPIO_PORTFH_BASE_ADDR = (uint32_t *)(0x4005D000U);
const uint32_t  GPIODIR               = (uint32_t)(0x400); // Direction
const uint32_t  GPIOIS                = (uint32_t)(0x404); // Interrupt Sense
const uint32_t  GPIOIBE               = (uint32_t)(0x408); // Interrupt Both Edges
const uint32_t  GPIOIEV               = (uint32_t)(0x40C); // Interrupt Event
const uint32_t  GPIOIM                = (uint32_t)(0x410); // Interrupt Mask
const uint32_t  GPIORIS               = (uint32_t)(0x414); // Raw Interrupt Status
const uint32_t  GPIOICR               = (uint32_t)(0x41C); // Interrupt Clear
const uint32_t  GPIOAFSEL             = (uint32_t)(0x420); // Alternate Function Select
const uint32_t  GPIOPUR               = (uint32_t)(0x510); // Pull-Up Select
const uint32_t  GPIOPDR               = (uint32_t)(0x514); // Pull-Down Select
const uint32_t  GPIODEN               = (uint32_t)(0x51C); // Digital Enable
const uint32_t  GPIOPCTL              = (uint32_t)(0x52C); // Port Control
const uint32_t  GPIODMACTL            = (uint32_t)(0x534); // DMA Control
/****************************************************************/


/****************************************************************/
const uint32_t *GPTM_TIMER0_BASE_ADDR = (uint32_t *)(0x40030000U);
const uint32_t *GPTM_TIMER1_BASE_ADDR = (uint32_t *)(0x40031000U);
const uint32_t *GPTM_TIMER2_BASE_ADDR = (uint32_t *)(0x40032000U);
const uint32_t *GPTM_TIMER3_BASE_ADDR = (uint32_t *)(0x40033000U);
const uint32_t *GPTM_TIMER4_BASE_ADDR = (uint32_t *)(0x40034000U);
const uint32_t *GPTM_TIMER5_BASE_ADDR = (uint32_t *)(0x40035000U);
const uint32_t *GPTM_WIDET0_BASE_ADDR = (uint32_t *)(0x40036000U);
const uint32_t *GPTM_WIDET1_BASE_ADDR = (uint32_t *)(0x40037000U);
const uint32_t *GPTM_WIDET2_BASE_ADDR = (uint32_t *)(0x4004C000U);
const uint32_t *GPTM_WIDET3_BASE_ADDR = (uint32_t *)(0x4004D000U);
const uint32_t *GPTM_WIDET4_BASE_ADDR = (uint32_t *)(0x4004E000U);
const uint32_t *GPTM_WIDET5_BASE_ADDR = (uint32_t *)(0x4004F000U);
const uint32_t  GPTMCFG               = (uint32_t)(0x000); // Configuration
const uint32_t  GPTMTAMR              = (uint32_t)(0x004); // Timer A Mode
const uint32_t  GPTMTBMR              = (uint32_t)(0x008); // Timer B Mode
const uint32_t  GPTMCTL               = (uint32_t)(0x00C); // Control
const uint32_t  GPTMIMR               = (uint32_t)(0x018); // Interrump Mask
const uint32_t  GPTMICR               = (uint32_t)(0x024); // Interrupt Clear
const uint32_t  GPTMTAILR             = (uint32_t)(0x028); // Timer A Interval Load
const uint32_t  GPTMTBILR             = (uint32_t)(0x02C); // Timer B Interval Load
const uint32_t  GPTMTAMATCHR          = (uint32_t)(0x030); // Timer A Match
const uint32_t  GPTMTBMATCHR          = (uint32_t)(0x034); // Timer B Match
const uint32_t  GPTMTAPR              = (uint32_t)(0x038); // Timer A Prescale
const uint32_t  GPTMTBPR              = (uint32_t)(0x03C); // Timer B Prescale
const uint32_t  GPTMTAR               = (uint32_t)(0x048); // Timer A
const uint32_t  GPTMTBR               = (uint32_t)(0x04C); // Timer B
/****************************************************************/


#endif /* __TM4C123GH6PM_REGISTERS_H__ */
