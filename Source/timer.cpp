#include "timer.h"
timer0::timer0(){
// To set clock:
// 1MHZ is 1,000,000 ticks per secomnd
// 1000 milli in 1 second
// xMHZ = 1000millis
// so MHZ/millis gives # HZ per millis
// (HZ/millis)/prescaler= Top counter number

// EG:for 8MHZ clock
// 8000000/1000
// 8000.0000000000
// 8000/256
// 31.2500000000 TOP counter

	//set CTC (clear timer on compare match mode)
	TCCR0A = (1<<WGM01); 
	//sets prescaler clkIO/256  ***THIS MIGHT CAUSE ISSUES SETS FOR ALL CLOCKS**!!!!
	TCCR0B = (1<<CS02);
	//sets interupt enable for OCF0A (TIMER_COMPA_vect)
	TIMSK0 = (1<<OCIE0A);
	//sets TOP counter match A at 31 
	OCR0A = 31;
}

uint32_t timer0::millis()
{
	uint32_t mill;
	uint8_t oldSREG = SREG; //remember last value of interupts
	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	mill = millis_count;
	SREG = oldSREG; // rewrite reg value to turn back on interupts
	return mill;
}

bool timer0::delay(uint16_t delayMils){
	static uint64_t oldTime;
	uint64_t nowTime = millis();
	if ( (nowTime - oldTime) > delayMils ){
		oldTime=nowTime;
		return true;
	}return false;
}

//interupt declaration
 ISR(TIMER0_COMPA_vect)
{
	++Timer0.millis_count;
	//OCR0A = 10; //sets upper breakpoint A
}

//instaintiate class
timer0 Timer0;
