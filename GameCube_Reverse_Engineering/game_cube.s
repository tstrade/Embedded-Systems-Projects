    .data

    .text

    .global sys_init
    .global gamecube_init

# Base addr. 0x400FE000
RCC:        .equ 0x060

# pg. 254
# RCC[26:23]
# Depends on BYPASS = 0
#   if BYPASS = 1, divisor is used
SYSDIV:     .equ 0x0
# 0x09 = 20 MHz
#      > Clk Period = 0.05 us

# RCC[22]
# Enable System Clock Divider
# USESYSDIV:  .equ 0x0040.0000
# 0 -> Sys clock used undivided

# pg. 427
# MINSYSDIV
# Read-Only
# 0x01 = Reserved
# 0x02 = 80 MHz Clk w/ PLL div.  2.5
# 0x03 = 50 MHz Clk w/ PLL div.  4
# 0x04 = 40 MHz Clk w/ PLL div.  5
# 0x07 = 25 MHz Clk w/ PLL div.  8
# 0x09 = 20 MHz Clk w/ PLL div. 10

# pg. 223 Table 5-4
# Bit encodings