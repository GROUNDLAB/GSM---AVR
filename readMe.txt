Code will be maintained by Justin under new company IEF R&D please see the IEFRD repo for updated code.
GitHub: IEFRD
Website:IEFRD.com


This is a library developed to be used with Atmel AVRs to implement Hayes AT commands to communicate with GSM modules.

for more information and API please see:
http://wiki.groundlab.cc/doku.php?id=gsm_library



main.cpp -> start here used just for testing
gsmbase.cpp -> is the base class and has the basic network functions and parsing.
gsmSMS.cpp -> is the derived class off GSMbase which does SMS
gsmGPRS.cpp -> is for GPRS functionality
serial2.cpp -> does all serial communications for the 1280
timer.cpp -> is just a quick timer I am using (the gsm object needs some function millis to give back milli seconds)
ioHelper.cpp -> just some defs I use for IO PORT stuff 

#The MAKEFILE is included the targets are:

all -> builds and uploads
clean -> cleans
build -> compiles 
upload -> objcopy into elf, then hex then uploads
and 
debugASM.s -> creates debugging files

so you would install avr-gcc tool chain
go to folder with:
makefile
Source
Include

and say:
make build
make upload

Using a ISPMKII programmer and it will upload the code.

*you must keep DTR pin LOW (HIGH RS SERIAL LEVEL) if you are not using a python script*


