#ifndef TIMER_0
#define TIMER_0
#include <avr/interrupt.h>
#include <inttypes.h>

class timer0{
public:
volatile uint64_t millis_count;  //init to 0
timer0();
//static void init();
uint32_t millis();
bool delay(uint16_t delayMils);
};

extern timer0 Timer0;
#endif
