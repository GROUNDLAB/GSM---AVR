
main.cpp -> start here used just for testing
gsm.cpp -> is the base class and has the basic network functions and parsing.
gsmSMS.cpp -> is the derived class off GSMbase which does SMS
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


