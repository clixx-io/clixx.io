CC = gcc

LIBS = -lmosquitto

%.o: %.cpp
    $(CC) -c -o $@ $< 

all: iot-subscribe

iot-subscribe: iot-subscribe.cpp
    $(CC) -Wall -o test $^ $(LIBS)

.PHONY: clean
