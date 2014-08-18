# ToDo: Replace with a Makefile
g++ -D TARGET_LINUX -Wall -o iot-subscribe iot-subscribe.cpp -I ../../iot-framework/ -L ../../iot-framework -l clixxIO_Linux -l mosquittopp
