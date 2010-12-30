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
