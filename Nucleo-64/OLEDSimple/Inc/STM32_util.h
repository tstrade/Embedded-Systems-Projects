#ifndef STM32_UTIL_H_
#define STM32_UTIL_H_

extern volatile long Sys_Ticks;
extern volatile long millisec;
extern volatile long CountDown_ms;
void Init_Sys_Tick(void);
void SysTick_Handler(void);
void Delay_100us(int);
void Delay_1ms(int);

void DigWrite(char port, int pin, int state);
#define HIGH 1
#define LOW  0
#define ON  1
#define OFF 0
#define TRUE  1
#define FALSE 0

void Init_Pin(char port, int pin, int direction);
#define INPUT    0
#define OUTPUT   1
#define ALT      2
#define ANALOG 		3
#define INPUT_PULLUP 4

int DigRead(char port, int pin);

void Init_I2C2(int speed);
char I2C2ReadRegister(unsigned char addr , char );
void I2C2WriteRegister(unsigned char addr, char , char );
void I2C2ReadMultiRegisters(unsigned char addr, char , char , char * );

#endif
