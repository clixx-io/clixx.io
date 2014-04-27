CC = gcc

LIBS = -lmosquitto

%.o: %.c 
    $(CC) -c -o $@ $< 

make: Main.cpp
    $(CC) -Wall -o test $^ $(LIBS)

.PHONY: clean
