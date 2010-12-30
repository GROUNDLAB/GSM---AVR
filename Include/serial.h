          /*AVR SOURCE FILES FOR GSM,SERIAL FUNCTIONALITY*\
*                                                                     1%*
* Copyright (C) 2010  Justin Downs of GRounND LAB                       *
* www.GroundLab.cc                                                      *
*                                                                       *
* This program is free software: you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation, either version 3 of the License, or     *
* at your option) any later version.                                    *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* with this program.  If not, see <http://www.gnu.org/licenses/>.       *
***************************************************/

#ifndef SERIALTEST
#define SERIALTEST
#include <inttypes.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

//DEFINE TO USE
//#define SERIAL0
//#define SERIAL1
#define SERIAL2
#define SERIAL3


struct ring_buffer;

class SerialPort{

private:
	ring_buffer* rx_buffer;
	//Registers
	volatile uint8_t* ubrrh;
	volatile uint8_t* ubrrl;
	volatile uint8_t* ucsra;
	volatile uint8_t* ucsrb;
	volatile uint8_t* ucsrc;
	volatile uint8_t* udr;

	//bit offsets
	uint8_t rxen;
	uint8_t txen;
	uint8_t rxcie;
	uint8_t txcie;
	uint8_t udre;
	uint8_t usbs;
	uint8_t ucsz;
	uint8_t u2x;
public:
	SerialPort (
	ring_buffer *rx_buffer,
	//Registers
	volatile uint8_t* ubrrh, volatile uint8_t* ubrrl,
	volatile uint8_t* ucsra, volatile uint8_t* ucsrb,
	volatile uint8_t* ucsrc, volatile uint8_t* udr, 
	//last are offset numbers for bits
	uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t txcie,
	uint8_t udre, uint8_t usbs, uint8_t ucsz, uint8_t u2x);

	SerialPort(){}
	void USART_Transmit(char data);
	void write();

	template<typename T>
	void write(T);
	
	template <typename T>
	void write(T*);
	
	void store_char(unsigned char c, ring_buffer *rx_buffer);
	char read();
	uint8_t available();
	void flush();
	void USART_Init( uint32_t baud );

};

template <typename T>
void SerialPort::write(T ch){
USART_Transmit(ch);
}

template <typename T>
void SerialPort::write(T* str)
{
while (*str) write(*str++);
}
 
#ifdef SERIAL0
extern SerialPort Serial;
#endif

#ifdef SERIAL1
extern SerialPort Serial1;
#endif

#ifdef SERIAL2
extern SerialPort Serial2;
#endif

#ifdef SERIAL3
extern SerialPort Serial3;
#endif

#endif
