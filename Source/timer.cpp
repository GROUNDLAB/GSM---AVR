/*AVR SOURCE FILES FOR GSM,SERIAL FUNCTIONALITY                         *
*                     Copyright (C) 2010  Justin Downs of GRounND LAB	*
*                     www.GroundLab.cc     1%                          	*
*                                                                 	*
* This program is free software: you can redistribute it and/or modify 	*
* it under the terms of the GNU General Public License as published by 	*
* the Free Software Foundation, either version 3 of the License, or    	*
* at your option) any later version.                                   	*
*                                                                      	*
* This program is distributed in the hope that it will be useful,      	*
* but WITHOUT ANY WARRANTY; without even the implied warranty of       	*
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        	*
* GNU General Public License for more details.                         	*
*                                                                      	*
* You should have received a copy of the GNU General Public License    	*
* with this program.  If not, see <http://www.gnu.org/licenses/>.      	* 
************************************************************************/




#include "timer.h"
timer0::timer0(){
// To set clock:
// 1MHZ is 1,000,000 ticks per second
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
	//sets interrupt enable for OCF0A (TIMER_COMPA_vect)
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
	SREG = oldSREG; // rewrite reg value to turn back on interrupts
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

//interrupt declaration
 ISR(TIMER0_COMPA_vect)
{
	++Timer0.millis_count;
	//OCR0A = 10; //sets upper breakpoint A
}

//instantiate class
timer0 Timer0;
