#AVR SOURCE FILES FOR GSM,SERIAL FUNCTIONALITY#\
#                                                                   1%  #
# Copyright (C) 2010  Justin Downs of GRounND LAB                       #
# www.GroundLab.cc                                                      #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# at your option) any later version.                                    #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# with this program.  If not, see <http://www.gnu.org/licenses/>.       #
##################################################/









###GCC COMPILER########
CXX=avr-g++
MMCU = -mmcu=atmega1280 
CPPFLAGS = -I Include $(MMCU) -Wall -DF_CPU=8000000 # -Wstrict-prototypes
ALLCPPFLAGS = -Os $(CPPFLAGS)
#use to link in libs -L tells directory -l tells lib name
LINKFLAG = 
Source=Source
#VPATH = Source Include
vpath %.h Include #specify directory to look for .h files
vpath %.cpp Source #same .cpp
# get all cpp files from Source directory
SRCFiles=  $(wildcard $(Source)/*.cpp)
# create object file name targets
OBJ= $(SRCFiles:.cpp=.o)

####AVRDUDE###########i#
#attiny t2313 atmeg644p m644p atmega1280 m1280
CHIP= m1280
#######################

#all in one
all: build upload

upload: outMain.hex
	avrdude -B10 -p $(CHIP) -c avrispmkII -P usb -U flash:w:$< 

#Turn elf file into hex strip text and data
outMain.hex:outMain.elf 
	avr-objcopy -j .text -j .data -O ihex outMain.elf outMain.hex


#-S creates assembly file with C calls for debug
#-h list header files
debugASM.s : outMain.elf
	avr-objdump -S -h $< > $@ 
	avr-size outMain.elf

# Uses elf file as target for a basic compile,
# all cpp files get turned to o files and linked as dependents
build: outMain.elf

#Compile and link all o files into the main elf file
outMain.elf: $(notdir $(OBJ))
	$(CXX) $(MMCU) -o outMain.elf  $^ $(LINKFLAG)

#Compile with outlinking all cpp files
.cpp.o:
	$(CXX) $(ALLCPPFLAGS) -c  $< -o $(notdir $@)

# Rule how to process a cpp file into a make.d target
# if it is out of date or not found

# For same as source dir
#%.d: %.cpp
#	$(SHELL) -ec '$(CXX) -M $(ALLCPPFLAGS) $< | sed "s?$(notdir $*).o?$*.o $*.d?" > $@ '
	
# For main dir
.cpp.d:
	$(CXX) -M $(ALLCPPFLAGS) $< \
| sed "s?$(notdir $*).o?$(notdir $*).o $(notdir $*).d?" | tee $(notdir $@) 

# -M option to g++ makes it look at the #include calls in the .cppfiles
# and lists them as dependencies, so if the .h files change it recompiles.
# $< is the name of the first dependincy "%.cpp". % is a wild card.
# It outputs target.o:target.c target.h <-.h is grabed from #include
# Output is sent to sed which, makes the content of the make.d file by replacing
# $* <-which is the same name as % from target, minus the directory (notdir) in the
# g++ output. 
# To create the .d file the output from sed is > with tee into $@ 
# file which stands for the name of the target %.d.

# asks for all source files as make.d make files, 
# specifies them as targets, triggers above line
MAKEDFILES= $(notdir $(SRCFiles:.cpp=.d)) #takes away dir name to use current dir
include $(MAKEDFILES)
#to look in Source dir
#include $(SRCFiles:.cpp=.d)

clean:
	rm  *.hex *.elf *.o *.d *.s
.PHONY: all clean build upload
.SUFFIXES:.o .c .cpp .d .hex .elf .s

