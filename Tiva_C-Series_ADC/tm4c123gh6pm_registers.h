#ifndef __TM4C123GH6PM_REGISTERS_H__
#define __TM4C123GH6PM_REGISTERS_H__

#include <inttypes.h>



/****************************************************************/
const uint32_t  CM4_PERIPHS_BASE_ADDR = (uint32_t)(0xE000E000);
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
const uint32_t  SYS_CONTROL_BASE_ADDR = (uint32_t)(0x400FE000);
const uint32_t  RCC                   = (uint32_t)(0x060); // Run-Mode Clock Configuration
const uint32_t  RCGCTIMER             = (uint32_t)(0x604); // 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control
const uint32_t  RCGCGPIO              = (uint32_t)(0x608); // General-Purpose Input/Output Run Mode Clock Gating Control
const uint32_t  RCGCDMA               = (uint32_t)(0x60C); // Micro Direct Memory Access Run Mode Clock Gating Control
const uint32_t  RCGCUART              = (uint32_t)(0x618); // Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
const uint32_t  RCGCSSI               = (uint32_t)(0x61C); // Synchronous Serial Interface Run Mode Clock Gating Control
const uint32_t  RCGCADC               = (uint32_t)(0x638); // Analog-to-Digital Converter Run Mode Clock Gating Control
const uint32_t  RCGCACMP              = (uint32_t)(0x63C); // Analog Comparator Run Mode Clock Gating Control
const uint32_t  RCGCPWM               = (uint32_t)(0x640); // Pulse Width Modulation Run Mode Clock Gating Control
const uint32_t  PRSSI                 = (uint32_t)(0xA1C); // Synchronous Serial Interface Peripheral Ready
/****************************************************************/



/****************************************************************/
const uint32_t  DMASRCENDP            = (uint32_t)(0x000); // Channel Source End Pointer
const uint32_t  DMADSTENDP            = (uint32_t)(0x004); // Channel Destination Address End Pointer
const uint32_t  DMACHCTL              = (uint32_t)(0x008); // Channel Control Word

const uint32_t  UDIR_MEMACC_BASE_ADDR = (uint32_t)(0x400FF000);
const uint32_t  DMACFG                = (uint32_t)(0x004); // Configuration
const uint32_t  DMACTLBASE            = (uint32_t)(0x008); // Channel Control Base Pointer
const uint32_t  DMAALTBASE            = (uint32_t)(0x00C); // Alternate Channel Control Base Pointer
const uint32_t  DMAWAITSTAT           = (uint32_t)(0x010); // Wait-on Request Status
const uint32_t  DMASWREQ              = (uint32_t)(0x014); // Channel Software Request
const uint32_t  DMAUSEBURSTSET        = (uint32_t)(0x018); // Channel Useburst Set
const uint32_t  DMAUSEBURSTCLR        = (uint32_t)(0x01C); // Channel Useburst Clear
const uint32_t  DMAREQMASKSET         = (uint32_t)(0x020); // Channel Request Mask Set
const uint32_t  DMAREQMASKCLR         = (uint32_t)(0x024); // Channel Request Mask Clear
const uint32_t  DMAENASET             = (uint32_t)(0x028); // Channel Enable Set
const uint32_t  DMAENACLR             = (uint32_t)(0x02C); // Channel Enable Clear
const uint32_t  DMAALTSET             = (uint32_t)(0x030); // Channel Primary Alternate Set
const uint32_t  DMAALTCLR             = (uint32_t)(0x034); // Channel Primary Alternate Clear
const uint32_t  DMAPRIOSET            = (uint32_t)(0x038); // Channel Priority Set
const uint32_t  DMAPRIOCLR            = (uint32_t)(0x03C); // Channel Priority Clear
const uint32_t  DMAERRCLR             = (uint32_t)(0x04C); // Bus Error Clear
const uint32_t  DMACHASGN             = (uint32_t)(0x500); // Channel Assignment
const uint32_t  DMACHIS               = (uint32_t)(0x504); // Interrupt Status
const uint32_t  DMACHMAP0             = (uint32_t)(0x510); // Channel Map Select 0
const uint32_t  DMACHMAP1             = (uint32_t)(0x514); // Channel Map Select 1
const uint32_t  DMACHMAP2             = (uint32_t)(0x518); // Channel Map Select 2
const uint32_t  DMACHMAP3             = (uint32_t)(0x51C); // Channel Map Select 3
const uint32_t  DMAPeriphID4          = (uint32_t)(0xFD0); // Peripheral Identification 4
const uint32_t  DMAPeriphID0          = (uint32_t)(0xFE0); // Peripheral Identification 0
const uint32_t  DMAPeriphID1          = (uint32_t)(0xFE4); // Peripheral Identification 1
const uint32_t  DMAPeriphID2          = (uint32_t)(0xFE8); // Peripheral Identification 2
const uint32_t  DMAPeriphID3          = (uint32_t)(0xFEC); // Peripheral Identification 3
const uint32_t  DMAPCellID0           = (uint32_t)(0xFF0); // PrimeCell Identification 0
const uint32_t  DMAPCellID1           = (uint32_t)(0xFF4); // PrimeCell Identification 1
const uint32_t  DMAPCellID2           = (uint32_t)(0xFF8); // PrimeCell Identification 2
const uint32_t  DMAPCellID3           = (uint32_t)(0xFFC); // PrimeCell Identification 3
/****************************************************************/



/****************************************************************/
const uint32_t  GPIO_PORTAP_BASE_ADDR = (uint32_t)(0x40004000);
const uint32_t  GPIO_PORTAH_BASE_ADDR = (uint32_t)(0x40058000);
const uint32_t  GPIO_PORTBP_BASE_ADDR = (uint32_t)(0x40005000);
const uint32_t  GPIO_PORTBH_BASE_ADDR = (uint32_t)(0x40059000);
const uint32_t  GPIO_PORTCP_BASE_ADDR = (uint32_t)(0x40006000);
const uint32_t  GPIO_PORTCH_BASE_ADDR = (uint32_t)(0x4005A000);
const uint32_t  GPIO_PORTDP_BASE_ADDR = (uint32_t)(0x40007000);
const uint32_t  GPIO_PORTDH_BASE_ADDR = (uint32_t)(0x4005B000);
const uint32_t  GPIO_PORTEP_BASE_ADDR = (uint32_t)(0x40024000);
const uint32_t  GPIO_PORTEH_BASE_ADDR = (uint32_t)(0x4005C000);
const uint32_t  GPIO_PORTFP_BASE_ADDR = (uint32_t)(0x40025000);
const uint32_t  GPIO_PORTFH_BASE_ADDR = (uint32_t)(0x4005D000);
const uint32_t  GPIODIR               = (uint32_t)(0x400); // Direction
const uint32_t  GPIOIS                = (uint32_t)(0x404); // Interrupt Sense
const uint32_t  GPIOIBE               = (uint32_t)(0x408); // Interrupt Both Edges
const uint32_t  GPIOIEV               = (uint32_t)(0x40C); // Interrupt Event
const uint32_t  GPIOIM                = (uint32_t)(0x410); // Interrupt Mask
const uint32_t  GPIORIS               = (uint32_t)(0x414); // Raw Interrupt Status
const uint32_t  GPIOICR               = (uint32_t)(0x41C); // Interrupt Clear
const uint32_t  GPIOAFSEL             = (uint32_t)(0x420); // Alternate Function Select
const uint32_t  GPIODR2R              = (uint32_t)(0x500); // 2-mA Drive Select
const uint32_t  GPIODR4R              = (uint32_t)(0x504); // 4-mA Drive Select
const uint32_t  GPIODR8R              = (uint32_t)(0x508); // 8-mA Drive Select
const uint32_t  GPIOODR               = (uint32_t)(0x50C); // Open Drain Select
const uint32_t  GPIOPUR               = (uint32_t)(0x510); // Pull-Up Select
const uint32_t  GPIOPDR               = (uint32_t)(0x514); // Pull-Down Select
const uint32_t  GPIOSLR               = (uint32_t)(0x518); // Slew Rate Control Select
const uint32_t  GPIODEN               = (uint32_t)(0x51C); // Digital Enable
const uint32_t  GPIOLOCK              = (uint32_t)(0x520); // Lock
const uint32_t  GPIOCR                = (uint32_t)(0x524); // Commit
const uint32_t  GPIOAMSEL             = (uint32_t)(0x528); // Analog Mode Select
const uint32_t  GPIOPCTL              = (uint32_t)(0x52C); // Port Control
const uint32_t  GPIOADCCTL            = (uint32_t)(0x530); // ADC Control
const uint32_t  GPIODMACTL            = (uint32_t)(0x534); // DMA Control
const uint32_t  GPIOPeriphID4         = (uint32_t)(0xFD0); // Peripheral Identification 4
const uint32_t  GPIOPeriphID5         = (uint32_t)(0xFD4); // Peripheral Identification 5
const uint32_t  GPIOPeriphID6         = (uint32_t)(0xFD8); // Peripheral Identification 6
const uint32_t  GPIOPeriphID7         = (uint32_t)(0xFDC); // Peripheral Identification 7
const uint32_t  GPIOPeriphID0         = (uint32_t)(0xFE0); // Peripheral Identification 0
const uint32_t  GPIOPeriphID1         = (uint32_t)(0xFE4); // Peripheral Identification 1
const uint32_t  GPIOPeriphID2         = (uint32_t)(0xFE8); // Peripheral Identification 2
const uint32_t  GPIOPeriphID3         = (uint32_t)(0xFEC); // Peripheral Identification 3
const uint32_t  GPIOPCellID0          = (uint32_t)(0xFF0); // PrimeCell Identification 0
const uint32_t  GPIOPCellID1          = (uint32_t)(0xFF4); // PrimeCell Identification 1
const uint32_t  GPIOPCellID2          = (uint32_t)(0xFF8); // PrimeCell Identification 2
const uint32_t  GPIOPCellID4          = (uint32_t)(0xFFC); // PrimeCell Identification 3
/****************************************************************/



/****************************************************************/
const uint32_t  GPTM_TIMER0_BASE_ADDR = (uint32_t)(0x40030000);
const uint32_t  GPTM_TIMER1_BASE_ADDR = (uint32_t)(0x40031000);
const uint32_t  GPTM_TIMER2_BASE_ADDR = (uint32_t)(0x40032000);
const uint32_t  GPTM_TIMER3_BASE_ADDR = (uint32_t)(0x40033000);
const uint32_t  GPTM_TIMER4_BASE_ADDR = (uint32_t)(0x40034000);
const uint32_t  GPTM_TIMER5_BASE_ADDR = (uint32_t)(0x40035000);
const uint32_t  GPTM_WIDET0_BASE_ADDR = (uint32_t)(0x40036000);
const uint32_t  GPTM_WIDET1_BASE_ADDR = (uint32_t)(0x40037000);
const uint32_t  GPTM_WIDET2_BASE_ADDR = (uint32_t)(0x4004C000);
const uint32_t  GPTM_WIDET3_BASE_ADDR = (uint32_t)(0x4004D000);
const uint32_t  GPTM_WIDET4_BASE_ADDR = (uint32_t)(0x4004E000);
const uint32_t  GPTM_WIDET5_BASE_ADDR = (uint32_t)(0x4004F000);
const uint32_t  GPTMCFG               = (uint32_t)(0x000); // Configuration
const uint32_t  GPTMTAMR              = (uint32_t)(0x004); // Timer A Mode
const uint32_t  GPTMTBMR              = (uint32_t)(0x008); // Timer B Mode
const uint32_t  GPTMCTL               = (uint32_t)(0x00C); // Control
const uint32_t  GPTMSYNC              = (uint32_t)(0x010); // Synchronize
const uint32_t  GPTMIMR               = (uint32_t)(0x018); // Interrupt Mask
const uint32_t  GPTMRIS               = (uint32_t)(0x01C); // Raw Interrupt Status
const uint32_t  GPTMMIS               = (uint32_t)(0x020); // Masked Interrupt Status
const uint32_t  GPTMICR               = (uint32_t)(0x024); // Interrupt Clear
const uint32_t  GPTMTAILR             = (uint32_t)(0x028); // Timer A Interval Load
const uint32_t  GPTMTBILR             = (uint32_t)(0x02C); // Timer B Interval Load
const uint32_t  GPTMTAMATCHR          = (uint32_t)(0x030); // Timer A Match
const uint32_t  GPTMTBMATCHR          = (uint32_t)(0x034); // Timer B Match
const uint32_t  GPTMTAPR              = (uint32_t)(0x038); // Timer A Prescale
const uint32_t  GPTMTBPR              = (uint32_t)(0x03C); // Timer B Prescale
const uint32_t  GPTMTAPMR             = (uint32_t)(0x040); // Timer A Prescale Match
const uint32_t  GPTMTBPMR             = (uint32_t)(0x048); // Timer B Prescale Match
const uint32_t  GPTMTAR               = (uint32_t)(0x048); // Timer A
const uint32_t  GPTMTBR               = (uint32_t)(0x04C); // Timer B
const uint32_t  GPTMTAV               = (uint32_t)(0x050); // Timer A Value
const uint32_t  GPTMTBV               = (uint32_t)(0x054); // Timer B Value
const uint32_t  GPTMRTCPD             = (uint32_t)(0x058); // RTC Predivide
const uint32_t  GPTMTAPS              = (uint32_t)(0x05C); // Timer A Prescale Snapshot
const uint32_t  GPTMTBPS              = (uint32_t)(0x060); // Timer B Prescale Snapshot
const uint32_t  GPTMTAPV              = (uint32_t)(0x064); // Timer A Prescale Value
const uint32_t  GPTMTBPV              = (uint32_t)(0x068); // Timer B Prescale Value
const uint32_t  GPTMPP                = (uint32_t)(0xFC0); // Peripheral Properties
/****************************************************************/



/****************************************************************/
const uint32_t  ANALOG_DIG0_BASE_ADDR = (uint32_t)(0x40038000);
const uint32_t  ANALOG_DIG1_BASE_ADDR = (uint32_t)(0x40039000);
const uint32_t  ADCACTSS              = (uint32_t)(0x000); // Active Sample Sequencer
const uint32_t  ADCRIS                = (uint32_t)(0x004); // Raw Interrupt Status
const uint32_t  ADCIM                 = (uint32_t)(0x008); // Interrupt Mask
const uint32_t  ADCISC                = (uint32_t)(0x00C); // Interrupt Status and Clear
const uint32_t  ADCOSTAT              = (uint32_t)(0x010); // Overflow Status
const uint32_t  ADCEMUX               = (uint32_t)(0x014); // Event Multiplexer Select
const uint32_t  ADCUSTAT              = (uint32_t)(0x018); // Underflow Status
const uint32_t  ADCTSSEL              = (uint32_t)(0x01C); // Trigger Source Select
const uint32_t  ADCSSPRI              = (uint32_t)(0x020); // Sample Sequencer Priority
const uint32_t  ADCSPC                = (uint32_t)(0x024); // Sample Phase Control
const uint32_t  ADCPSSI               = (uint32_t)(0x028); // Processor Sample Sequence Initiate
const uint32_t  ADCSAC                = (uint32_t)(0x030); // Sample Averaging Control
const uint32_t  ADCDCISC              = (uint32_t)(0x034); // Digital Comparator Interrupt Status and Clear
const uint32_t  ADCCTL                = (uint32_t)(0x038); // Control
const uint32_t  ADCSSMUX0             = (uint32_t)(0x040); // Sample Sequence Input Multiplexer Select 0
const uint32_t  ADCSSCTL0             = (uint32_t)(0x044); // Sample Sequence Control 0
const uint32_t  ADCSSFIFO0            = (uint32_t)(0x048); // Sample Sequence Result FIFO 0
const uint32_t  ADCSSFSTAT0           = (uint32_t)(0x04C); // Sample Sequence FIFO 0 Status
const uint32_t  ADCSSOP0              = (uint32_t)(0x050); // Sample Sequence 0 Operation
const uint32_t  ADCSSDC0              = (uint32_t)(0x054); // Sample Sequence 0 Digital Comparator Select
const uint32_t  ADCSSMUX1             = (uint32_t)(0x060); // Sample Sequence Input Multiplexer Select 1
const uint32_t  ADCSSCTL1             = (uint32_t)(0x064); // Sample Sequence Control 1
const uint32_t  ADCSSFIFO1            = (uint32_t)(0x068); // Sample Sequence Result FIFO 1
const uint32_t  ADCSSFSTAT1           = (uint32_t)(0x06C); // Sample Sequence FIFO 1 Status
const uint32_t  ADCSSOP1              = (uint32_t)(0x070); // Sample Sequence 1 Operation
const uint32_t  ADCSSDC1              = (uint32_t)(0x074); // Sample Sequence 1 Digital Comparator Select
const uint32_t  ADCSSMUX2             = (uint32_t)(0x080); // Sample Sequence Input Multiplexer Select 2
const uint32_t  ADCSSCTL2             = (uint32_t)(0x084); // Sample Sequence Control 2
const uint32_t  ADCSSFIFO2            = (uint32_t)(0x088); // Sample Sequence Result FIFO 2
const uint32_t  ADCSSFSTAT2           = (uint32_t)(0x08C); // Sample Sequence FIFO 2 Status
const uint32_t  ADCSSOP2              = (uint32_t)(0x090); // Sample Sequence 2 Operation
const uint32_t  ADCSSDC2              = (uint32_t)(0x094); // Sample Sequence 2 Digital Comparator Select
const uint32_t  ADCSSMUX3             = (uint32_t)(0x0A0); // Sample Sequence Input Multiplexer Select 3
const uint32_t  ADCSSCTL3             = (uint32_t)(0x0A4); // Sample Sequence Control 3
const uint32_t  ADCSSFIFO3            = (uint32_t)(0x0A8); // Sample Sequence Result FIFO 3
const uint32_t  ADCSSFSTAT3           = (uint32_t)(0x0AC); // Sample Sequence FIFO 3 Status
const uint32_t  ADCSSOP3              = (uint32_t)(0x0B0); // Sample Sequence 3 Operation
const uint32_t  ADCSSDC3              = (uint32_t)(0x0B4); // Sample Sequence 3 Digital Comparator Select
const uint32_t  ADCDCRIC              = (uint32_t)(0xD00); // Digital Comparator Reset Initial Conditions
const uint32_t  ADCDCCTL0             = (uint32_t)(0xE00); // Digital Comparator Control 0
const uint32_t  ADCDCCTL1             = (uint32_t)(0xE04); // Digital Comparator Control 1
const uint32_t  ADCDCCTL2             = (uint32_t)(0xE08); // Digital Comparator Control 2
const uint32_t  ADCDCCTL3             = (uint32_t)(0xE0C); // Digital Comparator Control 3
const uint32_t  ADCDCCTL4             = (uint32_t)(0xE10); // Digital Comparator Control 4
const uint32_t  ADCDCCTL5             = (uint32_t)(0xE14); // Digital Comparator Control 5
const uint32_t  ADCDCCTL6             = (uint32_t)(0xE18); // Digital Comparator Control 6
const uint32_t  ADCDCCTL7             = (uint32_t)(0xE1C); // Digital Comparator Control 7
const uint32_t  ADCDCCMP0             = (uint32_t)(0xE40); // Digital Comparator Range 0
const uint32_t  ADCDCCMP1             = (uint32_t)(0xE44); // Digital Comparator Range 1
const uint32_t  ADCDCCMP2             = (uint32_t)(0xE48); // Digital Comparator Range 2
const uint32_t  ADCDCCMP3             = (uint32_t)(0xE4C); // Digital Comparator Range 3
const uint32_t  ADCDCCMP4             = (uint32_t)(0xE50); // Digital Comparator Range 4
const uint32_t  ADCDCCMP5             = (uint32_t)(0xE54); // Digital Comparator Range 5
const uint32_t  ADCDCCMP6             = (uint32_t)(0xE58); // Digital Comparator Range 6
const uint32_t  ADCDCCMP7             = (uint32_t)(0xE5C); // Digital Comparator Range 7
const uint32_t  ADCPP                 = (uint32_t)(0xFC0); // Peripheral Properties
const uint32_t  ADCPC                 = (uint32_t)(0xFC4); // Peripheral Configuration
const uint32_t  ADCCC                 = (uint32_t)(0xFC8); // Clock Configuration
/****************************************************************/



/****************************************************************/
const uint32_t  UART_MODUL0_BASE_ADDR = (uint32_t)(0x4000C000);
const uint32_t  UARTDR                = (uint32_t)(0x000); // Data
const uint32_t  UARTRSRECR            = (uint32_t)(0x004); // Receive Status / Error Clear
const uint32_t  UARTFR                = (uint32_t)(0x018); // Flag
const uint32_t  UARTILPR              = (uint32_t)(0x020); // IrDA Low-Power Register
const uint32_t  UARTIBRD              = (uint32_t)(0x024); // Integer Baud-Rate Divisor
const uint32_t  UARTFBRD              = (uint32_t)(0x028); // Fractional Baud-Rate Divisor
const uint32_t  UARTLCRH              = (uint32_t)(0x02C); // Line Control
const uint32_t  UARTCTL               = (uint32_t)(0x030); // Control
const uint32_t  UARTIFLS              = (uint32_t)(0x034); // Interrupt FIFO Level Select
const uint32_t  UARTIM                = (uint32_t)(0x038); // Interrupt Mask
const uint32_t  UARTRIS               = (uint32_t)(0x03C); // Raw Interrupt Status
const uint32_t  UARTMIS               = (uint32_t)(0x040); // Masked Interrupt Status
const uint32_t  UARTICR               = (uint32_t)(0x044); // Interrupt Clear
const uint32_t  UARTDMACTL            = (uint32_t)(0x048); // DMA Control
const uint32_t  UART9BITADDR          = (uint32_t)(0x0A4); // 9-Bit Self Address
const uint32_t  UART9BITAMASK         = (uint32_t)(0x0A8); // 8-Bit Self Address Mask
const uint32_t  UARTPP                = (uint32_t)(0xFC0); // Peripheral Properties
const uint32_t  UARTCC                = (uint32_t)(0xFC8); // Clock Configuration
const uint32_t  UARTPeriphID4         = (uint32_t)(0xFD0); // Peripheral Identification 4
const uint32_t  UARTPeriphID5         = (uint32_t)(0xFD4); // Peripheral Identification 5
const uint32_t  UARTPeriphID6         = (uint32_t)(0xFD8); // Peripheral Identification 6
const uint32_t  UARTPeriphID7         = (uint32_t)(0xFDC); // Peripheral Identification 7
const uint32_t  UARTPeriphID0         = (uint32_t)(0xFE0); // Peripheral Identification 0
const uint32_t  UARTPeriphID1         = (uint32_t)(0xFE4); // Peripheral Identification 1
const uint32_t  UARTPeriphID2         = (uint32_t)(0xFE8); // Peripheral Identification 2
const uint32_t  UARTPeriphID3         = (uint32_t)(0xFEC); // Peripheral Identification 3
const uint32_t  UARTPCellID0          = (uint32_t)(0xFF0); // PrimeCell Identification 0
const uint32_t  UARTPCellID1          = (uint32_t)(0xFF4); // PrimeCell Identification 1
const uint32_t  UARTPCellID2          = (uint32_t)(0xFF8); // PrimeCell Identification 2
const uint32_t  UARTPCellID3          = (uint32_t)(0xFFC); // PrimeCell Identification 3
/****************************************************************/



/****************************************************************/
const uint32_t  SYNC_SIMOD0_BASE_ADDR = (uint32_t)(0x40008000);
const uint32_t  SYNC_SIMOD1_BASE_ADDR = (uint32_t)(0x40009000);
const uint32_t  SYNC_SIMOD2_BASE_ADDR = (uint32_t)(0x4000A000);
const uint32_t  SYNC_SIMOD3_BASE_ADDR = (uint32_t)(0x4000B000);
const uint32_t  SSICR0                = (uint32_t)(0x000); // SSI Control 0
const uint32_t  SSICR1                = (uint32_t)(0x004); // SSI Control 1
const uint32_t  SSIDR                 = (uint32_t)(0x008); // SSI Data
const uint32_t  SSISR                 = (uint32_t)(0x00C); // SSI Status
const uint32_t  SSICPSR               = (uint32_t)(0x010); // SSI Clock Peripheral
const uint32_t  SSIIM                 = (uint32_t)(0x014); // SSI Interrupt Mask
const uint32_t  SSIRIS                = (uint32_t)(0x018); // SSI Raw Interrupt Status
const uint32_t  SSIMIS                = (uint32_t)(0x01C); // SSI Masked Interrupt Status
const uint32_t  SSIICR                = (uint32_t)(0x020); // SSI Interrupt Clear
const uint32_t  SSIDMACTL             = (uint32_t)(0x024); // SSI DMA Control
const uint32_t  SSICC                 = (uint32_t)(0xFC8); // SSI Clock Configuration
const uint32_t  SSIPeriphID4          = (uint32_t)(0xFD0); // Peripheral Identification 4
const uint32_t  SSIPeriphID5          = (uint32_t)(0xFD4); // Peripheral Identification 5
const uint32_t  SSIPeriphID6          = (uint32_t)(0xFD8); // Peripheral Identification 6
const uint32_t  SSIPeriphID7          = (uint32_t)(0xFDC); // Peripheral Identification 7
const uint32_t  SSIPeriphID0          = (uint32_t)(0xFE0); // Peripheral Identification 0
const uint32_t  SSIPeriphID1          = (uint32_t)(0xFE4); // Peripheral Identification 1
const uint32_t  SSIPeriphID2          = (uint32_t)(0xFE8); // Peripheral Identification 2
const uint32_t  SSIPeriphID3          = (uint32_t)(0xFEC); // Peripheral Identification 3
const uint32_t  SSIPCellID0           = (uint32_t)(0xFF0); // PrimeCell Identification 0
const uint32_t  SSIPCellID1           = (uint32_t)(0xFF4); // PrimeCell Identification 1
const uint32_t  SSIPCellID2           = (uint32_t)(0xFF8); // PrimeCell Identification 2
const uint32_t  SSIPCellID3           = (uint32_t)(0xFFC); // PrimeCell Identification 3
/****************************************************************/



#endif /* __TM4C123GH6PM_REGISTERS_H__ */
