##
## This file is part of the clixx.io library for avr project.
##
## Copyright (C) 2014 David Lyon <david.lyon at clixx.io>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
##

LIBRARY = clixxIO_Linux
CC = g++
CFLAGS += -Wall -g -Os -DTARGET_LINUX
LDFLAGS +=
OBJS = $(PROGRAM).o

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

all: ../lib $(LIBRARY).a

# The library gets built and the filename shows the target platform
$(LIBRARY).a: clixxIO_EventQ_Linux.o 
	ar -cr $(LIBRARY).a clixxIO_EventQ_Linux.o
	@printf "  COPY $(LIBRARY).a ../lib\n"
	$(Q)cp $(LIBRARY).a ../lib

%.o: %.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

# The main source file gets compiled to a result showing the target platform
clixxIO_EventQ_Linux.o: clixxIO_EventQ.cpp clixxIO.hpp
	$(CC) -c $(CFLAGS) -o clixxIO_EventQ_Linux.o clixxIO_EventQ.cpp

# A seperate directory for holding libraries is used   
../lib: 
	@printf "  CREATE ../lib\n"
	$(Q)mkdir ../lib
 
clean:
	@printf "  CLEAN   *.o\n"
	$(Q)rm -f *.o
	@printf "  CLEAN   *.a\n"
	$(Q)rm -f *.a
