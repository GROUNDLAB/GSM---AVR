/*************AVR SOURCE FILES FOR GSM,SERIAL FUNCTIONALITY*************\
* 1%  									*
*  			Copyright (C) 2010  Justin Downs of GRounND LAB	*
*       								*
*   			www.GroundLab.cc				*
*     									*
* This program is free software: you can redistribute it and/or modify	*
* it under the terms of the GNU General Public License as published by	*
* the Free Software Foundation, either version 3 of the License, or	*
* at your option) any later version.					*
*									*
* This program is distributed in the hope that it will be useful,	*
* but WITHOUT ANY WARRANTY; without even the implied warranty of	*
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
* GNU General Public License for more details.				*
*									*
* You should have received a copy of the GNU General Public License	*
* with this program.  If not, see <http://www.gnu.org/licenses/>.	*
\***********************************************************************/



#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "ioHelper.h"
#include "timer.h"
#include "gsmbase.h"
#include "gsmSMS.h"
#include "gsmGPRS.h"
#include "gsmMaster.h"
//****************************************8
//Basic debug helpers
void talk();	//lets you type and get reply to Telit through Atmel
void blink();	//a hello world blink test for the pins

//****************************************8
//base class tester functions
void talkReply();   	// you type commands to ATMEL serial and telit responce is catched by GSMbase function.
void sendCommand(); 	// shows a typical send command using GSMbase class
void GSMbaseTester();	// tests GSMbase member functions
void turnOnTester();	// cycles a on off sequence
void talkReplyCatchTest();	
//*****************************************8
//SMS derived class tester
void gsmSMSTester();	// test gsmSMS member functions

//*****************************************8
//GPRS derived class tester
void gsmGPRSTester();
void ftp();
//*****************************************8
//GPRS derived class tester
void masterTester();
//*****************************************8

uint32_t millisWrapper();
uint32_t (*milP)() = &millisWrapper;	//used, so with arduino you can pass &millis() straight in.
uint32_t millisWrapper(){return Timer0.millis();}

//*****************************************8

GSMbase GSMb(Serial3,milP,&Serial2);	//GSMbase TELIT BASE FUNCTIONALITY
gsmSMS  GsmSMS(Serial3,milP,&Serial2);	//gsmSMS TELIT SMS
gsmGPRS GsmGPRS(Serial3,milP,&Serial2); //gsmGPRS TELIT GPRS
gsmMASTER GsmMASTER(Serial3,milP,&Serial2);//combine base SMS and GPRS

int main(void){
	sei(); 				//Enable interupts
	//only these are declared, to turn on other serial ports do so in serial.h
	Serial3.USART_Init(9600); 	//Telit serial
	Serial2.USART_Init(9600); 	//debug serial
	
	//GSMb.init(3);	// init Telit ***ALWAYS INIT AFTER SERIAL SETUP***
	//GsmSMS.init(3); // init Telit derived class calls base init() 
	//GsmGPRS.init(3); // init Telit derived class calls base init() 	
	if (GsmMASTER.init(3)) Serial2.write("good init");
	else Serial2.write("bad init");

	while(1){
		talk();
	//	talkReply();
	//	talkReplyCatchTest();
	//	sendCommand();
	//	GSMbaseTester();
	//	gsmSMSTester();
	//	gsmGPRSTester();
	//	turnOnTester();
	//	masterTester();	//master blaster
	}
}


//////////////////////////////////////////////////////////////////////////TALKING
void talk(){

	while (Serial3.available()>0){
		Serial2.write(Serial3.read());
	}
	while (Serial2.available()>0){
		Serial3.write(Serial2.read());
	}
}



////**PLEASE CHANGE INFO BEFORE RUNNING SO I DON"T GET HIT!!!**||||||||
///////////////////////////////////////////////////////////////////////////MASTER CLASS TESTERS
void masterTester(){
	uint32_t startTime=millisWrapper();
	while((millisWrapper()-startTime) < 30000);		// hang out 

////////////////////////////////SET NETWORK CONNECTION
//SETS the context number associated with a PDP protocal "IP"/"PPP" and APN number.
//	context "2" now has these settings (NOTE don't use "0" it is reserved for SMS)
Serial2.write("CGDCONT");
	GsmMASTER.setApnCGDCONT("2","IP","wap.cingular");
//SETS the TCP/IP stack
//	sockst conection ID 1 is now linked to context ID 2 data, with default timeouts TCP/IP
Serial2.write("SCFG");
	GsmMASTER.setTcpIpStackSCFG("1","2");
//REGISTERS with the network, receives IP address and network resources.
//	connect the specified context ID to the network. 
//	1 gets network resources 0 disconnects from network and frees resources.
Serial2.write("SGACT set");
	if(GsmMASTER.setContextSGACT("2","1","WAP@CINGULARGPRS.COM","CINGULAR1")){
		//AT#SD socket dial opens a socket to remote server.
		//conection ID 1 being opened
		//Serial2.write("SD");
		if( GsmMASTER.socketDialSD("1","0","80","www.johnhenryshammer.com")){
 		//RETURNS: CONNECT
		//////////////////////HERE DO A GET OR POST/////////////////////////////////

		//Constructs and send a get request on open socket
		//	Serial2.write("HTTP");
		//	GsmMASTER.getHTTP(3000,"www.johnhenryshammer.com","/","HTTP/1.1",true);
		//	Serial2.write("\n\n\n\nOUT of HTTP");
		//Constructs and sends a POST
	
		Serial2.write(GsmMASTER.postHTTP(600,"www.johnhenryshammer.com","/cTest/myPing.php",
				"justin", "HTTP/1.1",true,"testPing=helloworld") );
		Serial2.write("POST*****");
		////////////////////////////////////////////////////////////////////////////	
		//Suspends listing to socket, socket can still receive data till
		//a SH command is issued to shut the socket
			GsmMASTER.suspendSocket();
		}else{
			 Serial2.write("SGACT2 unset");
			}

	///////*****OR DO A FTP*****////////
	//////////////////////HERE DO A FTP PUT////////////////////////////////////////////////
	//SEE BELOW FOR FTP STEPS
	//ftp();
	///////////////////////////////////////////////////////////////////////////////////////
	}

//AT#SO you can use resumeSocket to reopen connection
//	resumeSocketSO(const char* const whichSocket);

//AT#SS can be implemented to view the status of a socket
Serial2.write("SS");
	Serial2.write( GsmMASTER.socketStatusSS() );	//then you can check socket status

//AT#SI can be implemented to view the status of a socket
	Serial2.write(GsmMASTER.socketInfoSI("1") );	//see the bytes transfered

//AT#SH closes the socket connection, no data in or out

	GsmMASTER.closeSocketSH("1");

//Serial2.write("SGACT2 unset");
//Give back the IP to the network
//DO THIS AT VERY END OF COMMUNICATION
//	GsmMASTER.setContextSGACT("2","0","WAP@CINGULARGPRS.COM","CINGULAR1");
	startTime=millisWrapper();
	while((millisWrapper()-startTime) < 60000);		// hang out 

	GsmMASTER.sendNoSaveCMGS("3473017780","hello world!");

}


///////////////////////////////////////////////////////////////////////////GPRS CLASS TESTERS
void gsmGPRSTester(){
////////////////////////////////SET NETWORK CONNECTION
//SETS the context number associated with a PDP protocal "IP"/"PPP" and APN number.
//	context "2" now has these settings (NOTE don't use "0" it is reserved for SMS)
Serial2.write("CGDCONT");
	GsmGPRS.setApnCGDCONT("2","IP","wap.cingular");
//SETS the TCP/IP stack
//	sockst conection ID 1 is now linked to context ID 2 data, with default timeouts TCP/IP
Serial2.write("SCFG");
	GsmGPRS.setTcpIpStackSCFG("1","2");
//REGISTERS with the network, receives IP address and network resources.
//	connect the specified context ID to the network. 
//	1 gets network resources 0 disconnects from network and frees resources.
Serial2.write("SGACT");
	if(GsmGPRS.setContextSGACT("2","1","WAP@CINGULARGPRS.COM","CINGULAR1")){
		//AT#SD socket dial opens a socket to remote server.
		//conection ID 1 being opened
		Serial2.write("SD");
		if( GsmGPRS.socketDialSD("1","0","80","www.johnhenryshammer.com")){
 		//RETURNS: CONNECT
		//////////////////////HERE DO A GET OR POST/////////////////////////////////

		//Constructs and send a get request on open socket
		//	Serial2.write("HTTP");
		//	GsmGPRS.getHTTP(3000,"www.johnhenryshammer.com","/","HTTP/1.1",true);
		//	Serial2.write("\n\n\n\nOUT of HTTP");
		//Constructs and sends a POST
			GsmGPRS.postHTTP(600,"www.johnhenryshammer.com","/cTest/myPing.php",
			"justin", "HTTP/1.1",true,"testPing=helloworld"); 
		////////////////////////////////////////////////////////////////////////////	
		//Suspends listing to socket, socket can still receive data till
		//a SH command is issued to shut the socket
			GsmGPRS.suspendSocket();
		}
	
	///////*****OR DO A FTP*****////////
	//////////////////////HERE DO A FTP PUT////////////////////////////////////////////////
	//SEE BELOW FOR FTP STEPS
	//ftp();
	///////////////////////////////////////////////////////////////////////////////////////
	}

//AT#SO you can use resumeSocket to reopen connection
//	resumeSocketSO(const char* const whichSocket);

//AT#SS can be implemented to view the status of a socket
Serial2.write("SS");
	Serial2.write( GsmGPRS.socketStatusSS() );	//then you can check socket status

//AT#SI can be implemented to view the status of a socket
//	Serial2.write(GsmGPRS.socketInfoSI("1") );	//see the bytes transfered

//AT#SH closes the socket connection, no data in or out
	GsmGPRS.closeSocketSH("1");
Serial2.write("SGACT2");
//Give back the IP to the network
	GsmGPRS.setContextSGACT("2","0","WAP@CINGULARGPRS.COM","CINGULAR1");
	

	uint32_t startTime=millisWrapper();
	while((millisWrapper()-startTime) < 20000);		// hang out 
}


//DIDN"T HAVE TIME TO TEST except basic functionality
//somthing with my FTP server is messed up.
void ftp(){
	GsmGPRS.ftpTimeOutFTPO("1000");	//set timeout to twenty seconds
	if (GsmGPRS.FTPOPEN("http://www.johnhenryshammer.com", "ftp@johnhenryshammer.com","","1")){ // set passive with 1
		GsmGPRS.ftpDataTypeFTPTYPE("1"); //get ASCII file
	
		if( GsmGPRS.changeDirFTPCWD("/public_html/cTest" )){
			//GsmGPRS.FTPPUT("putMe.txt", "you put me!");	//filename of data you are putting
			Serial2.write( GsmGPRS.FTPGET("getMe.txt",300) ); //file name and dataSize you are getting 
		}else Serial2.write("bad CD\n");
		GsmGPRS.FTPCLOSE();
	}Serial2.write("bad connection\n");
}
///////////////////////////////////////////////////////////////////////////SMS CLASS TESTERS


void gsmSMSTester(){
//CHECK CMGD gives a numerical list of all messages which are stored seperated by ','
//	Serial2.write( GsmSMS.checkCMGDList() );
//*ERASE CMGD (with arg) erases message at id(EG 1,2,3,4) arg
//	GsmSMS.deletMessagesCMGD("9");
//*READ CMGR read message with specified ID	
//	GsmSMS.readMessageCMGR("4");
//SEND CMGS sends up a SMS with no save;
	GsmSMS.sendNoSaveCMGS("3473017780","hello world!");
	uint32_t startTime=millisWrapper();
	while((millisWrapper()-startTime) < 20000);		// hang out 
//SAVE CMGW saves a message to memory and CMSS sends saved message.
//	GsmSMS.sendSavedMessageCMSS(GsmSMS.saveMessageCMGW("3473017780","I remember!"));	
//	uint32_t startTime=millisWrapper();
//	while((millisWrapper()-startTime) < 20000);		// hang out 
//CHECK CPMS gets number of messages or total space availible in sim memory selected by arg.
//	Serial2.write( GsmSMS.getNumMesInMemCPMS(1) );
//CHECK CMGL gets and displays full info and content of all of specified type of message.
	Serial2.write( GsmSMS.readAllCMGL("REC READ") );
}




////////////////////////////////////////////////////////////////////////////BASE CLASS TESTERS
void turnOnTester(){
//TURN ON
	Serial2.write("start");	
	GSMb.turnOn();
	GSMb.init(3);
	while(!GSMb.checkCREG());  		// hang out till it is registered
	
	uint32_t startTime=millisWrapper();
	while((millisWrapper()-startTime) < 20000);	// hang out for at  least 20 to let the cell tower
							//know you mean it.

	Serial2.write( GSMb.checkCOPS() );	// see who we are linked to
	GSMb.turnOff();				// once we are done turn off
	Serial2.write("out of off");
	startTime=millisWrapper();
	while((millisWrapper()-startTime) < 60000);	// hang out a minute
}


void GSMbaseTester(){
//CHECK CREG (NETWORK REGISTRATION)
//	if (GSMb.checkCREG()) Serial2.write("REGISTERED");
//	else Serial2.write("NOTREGISTERED");
//	
//	

//CHECK COPS (NETWORK INFO)
//	Serial2.write( GSMb.checkCOPS() );

//CHECK GSN (SERIAL NUMBER)
	Serial2.write( GSMb.checkGSN() );

//CHECK CSQ (signal strength)
//	char tempTest[10];
//	Serial2.write(itoa( GSMb.checkCSQ(), tempTest, 10 ));

//CHECK MONI (ALL SURRONDING TOWER INFORMATION)
//	Serial2.write( GSMb.checkMONI() );

}



const char* result=NULL;
void sendCommand(){
	result = GSMb.sendRecATCommandSplit("AT+CSQ","\r",0); 	//send command, parse the reply 
	//result = GSMb.sendRecATCommand("AT+CSQ");
	if (!result) Serial2.write("returned bad pointer\n"); 	//check to see if it is good
	Serial2.write("Return results: ");
	Serial2.write(result);
	Serial2.write("\n");
	Serial2.write("FullData main loop: ");
	Serial2.write(GSMb.getFullData());			//Even though you parsed data you
								//can get the full reply by calling
								//getFullData before the next sendRec
								//call to the Telit
	Serial2.write("END\n");
	while(!Timer0.delay(1000));	//from my timer
		
	result = GSMb.sendRecATCommandSplit("AT+COPS=?","\r",0);
	//result = GSMb.sendRecATCommand("AT+COPS=?");
	if (!result) Serial2.write("returned bad pointer2\n");
	Serial2.write("Return2 results: ");
	Serial2.write(result);
	Serial2.write("\n");
	Serial2.write("FullData2 main loop: ");
	Serial2.write(GSMb.getFullData());
	Serial2.write("END2\n");

}


void talkReply(){
	while (Serial2.available()>0){
		Serial3.write(Serial2.read());
	}
	if (Serial3.available()>0){
		GSMb.catchTelitData(5000,false,600,60);
		Serial2.write(GSMb.getFullData());
	}
}


void talkReplyCatchTest(){
	while (Serial2.available()>0){
		GSMb.catchTelitData(3000,0,60,3000);
		Serial2.write(GSMb.getFullData());
		Serial2.write("parseData:\n");
		GSMb.parseData(GSMb.getFullData(),"**","&&");
		Serial2.write(GSMb.getParsedData());
		Serial2.write("parsesplit:\n");
		GSMb.parseSplit(GSMb.getFullData(),",:",2);
		Serial2.write(GSMb.getParsedData());
	}
}


#define LED PB7
void blink(){
setOutput(DDRB,LED);

outputHigh(PORTB,LED);
	uint32_t startTime=millisWrapper();
	while((millisWrapper()-startTime) < 2000);
outputLow(PORTB,LED);
	startTime=millisWrapper();
	while((millisWrapper()-startTime) < 2000);
}
