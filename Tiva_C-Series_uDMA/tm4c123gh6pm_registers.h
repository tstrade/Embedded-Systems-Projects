#ifndef __TM4C123GH6PM_REGISTERS_H__
#define __TM4C123GH6PM_REGISTERS_H__

#include <inttypes.h>


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
const uint32_t  GPIODIR               = (uint32_t)(0x400);
const uint32_t  GPIOIS                = (uint32_t)(0x404);
const uint32_t  GPIOIBE               = (uint32_t)(0x408);
const uint32_t  GPIOIEV               = (uint32_t)(0x40C);
const uint32_t  GPIOIM                = (uint32_t)(0x410);
const uint32_t  GPIORIS               = (uint32_t)(0x414);
const uint32_t  GPIOICR               = (uint32_t)(0x41C);
const uint32_t  GPIOAFSEL             = (uint32_t)(0x420);
const uint32_t  GPIOPUR               = (uint32_t)(0x510);
const uint32_t  GPIOPDR               = (uint32_t)(0x514);
const uint32_t  GPIODEN               = (uint32_t)(0x51C);
/****************************************************************/


/****************************************************************/
const uint32_t *GPTM_TIMER0_BASE_ADDR = (uint32_t *)(0x40030000U);
const uint32_t  GPTMCFG               = (uint32_t)(0x000);
const uint32_t  GPTMTAMR              = (uint32_t)(0x004);
const uint32_t  GPTMCTL               = (uint32_t)(0x00C);
const uint32_t  GPTMIMR               = (uint32_t)(0x018);
const uint32_t  GPTMICR               = (uint32_t)(0x024);
const uint32_t  GPTMTAILR             = (uint32_t)(0x028);
const uint32_t  GPTMTAMATCHR          = (uint32_t)(0x030);
const uint32_t  GPTMTAPR              = (uint32_t)(0x038);
const uint32_t  GPTMTAR               = (uint32_t)(0x048);
/****************************************************************/


/****************************************************************/
const uint32_t *SYS_CONTROL_BASE_ADDR = (uint32_t *)(0x400FE000U);
const uint32_t  RCC                   = (uint32_t)(0x060);
const uint32_t  RCGCTIMER             = (uint32_t)(0x604);
const uint32_t  RCGCGPIO              = (uint32_t)(0x608);
const uint32_t  RCGCDMA               = (uint32_t)(0x60C);
/****************************************************************/


/****************************************************************/
const uint32_t *UDIR_MEMACC_BASE_ADDR = (uint32_t *)(0x400FF000U);
const uint32_t  DMACFG                = (uint32_t)(0x004);
const uint32_t  DMAACTLBASE           = (uint32_t)(0x008);
const uint32_t  DMAUSEBURSTSET        = (uint32_t)(0x018);
const uint32_t  DMAUSEBURSTCLR        = (uint32_t)(0x01C);
const uint32_t  DMAREQMASKSET         = (uint32_t)(0x020);
const uint32_t  DMAREQMASKCLR         = (uint32_t)(0x024);
const uint32_t  DMAENASET             = (uint32_t)(0x030);
const uint32_t  DMAALTCLR             = (uint32_t)(0x034);
const uint32_t  DMAPRIOSET            = (uint32_t)(0x038);
const uint32_t  DMAPRIOCLR            = (uint32_t)(0x03C);
const uint32_t  DMACHMAP3             = (uint32_t)(0x51C);
/****************************************************************/


/****************************************************************/
const uint32_t *CM4_PERIPHS_BASE_ADDR = (uint32_t *)(0xE000E000U);
const uint32_t  EN0                   = (uint32_t)(0x100);
/****************************************************************/


#endif /* __TM4C123GH6PM_REGISTERS_H__ */
