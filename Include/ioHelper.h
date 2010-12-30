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

#ifndef IOHELPER
#define IOHELPER

#include <avr/io.h>
#define outputLow(port,pin) port &= ~(1<<pin)
#define outputHigh(port,pin) port |= (1<<pin)
#define toggleOutput(port,pin) port ^= (1<<pin)
#define setInput(portdir,pin) portdir &= ~(1<<pin)
#define setOutput(portdir,pin) portdir |= (1<<pin)
#define digitalRead(pinPort,pin) ((pinPort>>pin)& 1)//Eg PINB,PB4

void blink(uint16_t LED, void (*delay_ms)(uint16_t));

#endif



/*
 * example code for reading writing a pin
 *#define LED PB4
  #define INPUT PB3
  setOutput(DDRB,LED);
  setInput(DDRB,INPUT);
  while(1){ 
	//uint8_t i = PINB & (1<<INPUT);//works
	//((PINB & (1<<INPUT))>>INPUT);//also works
	//uint8_t i = PINB4;
	if (digitalRead(PINB,INPUT)){
	outputHigh(PORTB,LED);
	}else{
	outputLow(PORTB,LED);
	}
*/
