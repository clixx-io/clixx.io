# ToDo: Replace with a Makefile
g++ -g -D TARGET_LINUX -Wall -o iot-subscribe iot-subscribe.cpp ../../iot-framework/core_Linux/clixxIO_mosquitto.cpp -I ../../iot-framework/ -L ../../iot-framework -l clixxIO_Linux -l mosquittopp
