void DigWrite(char port, int pin, int state); //Gen.Purpose input
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

int DigRead(char port, int pin); //Gen.Purpose input

