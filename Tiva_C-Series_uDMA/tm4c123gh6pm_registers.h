#ifndef __TM4C123GH6PM_REGISTERS_H__
#define __TM4C123GH6PM_REGISTERS_H__

#include <inttypes.h>

typedef uint32_t* reg;
typedef uint32_t offset;

const reg GPTM_TIMER0_BASE_ADDR = (reg)(0x40030000U);
const offset GPTMCFG               = (offset)(0x000);
const offset GPTMTAMR              = (offset)(0x004);
const offset GPTMCTL               = (offset)(0x00C);
const offset GPTMIMR               = (offset)(0x018);
const offset GPTMICR               = (offset)(0x024);
const offset GPTMTAILR             = (offset)(0x028);
const offset GPTMTAMATCHR          = (offset)(0x030);
const offset GPTMTAPR              = (offset)(0x038);
const offset GPTMTAR               = (offset)(0x048);


const reg SYS_CONTROL_BASE_ADDR = (reg)(0x400FE000U);
const offset RCC                   = (offset)(0x060);
const offset RCGCTIMER             = (offset)(0x604);
const offset RCGCGPIO              = (offset)(0x608);
const offset RCGCDMA               = (offset)(0x60C);


const reg UDIR_MEMACC_BASE_ADDR = (reg)(0x400FF000U);
const offset DMACFG                = (offset)(0x004);
const offset DMAACTLBASE           = (offset)(0x008);
const offset DMAUSEBURSTSET        = (offset)(0x018);
const offset DMAUSEBURSTCLR        = (offset)(0x01C);
const offset DMAREQMASKSET         = (offset)(0x020);
const offset DMAREQMASKCLR         = (offset)(0x024);
const offset DMAENASET             = (offset)(0x030);
const offset DMAALTCLR             = (offset)(0x034);
const offset DMAPRIOSET            = (offset)(0x038);
const offset DMAPRIOCLR            = (offset)(0x03C);
const offset DMACHMAP3             = (offset)(0x51C);


const reg CM4_PERIPHS_BASE_ADDR = (reg)(0xE000E000U);
const offset EN0                   = (offset)(0x100);

#endif /* __TM4C123GH6PM_REGISTERS_H__ */
