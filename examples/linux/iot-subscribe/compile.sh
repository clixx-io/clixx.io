# ToDo: Replace with a Makefile
g++ -D USE_MOSQUITTO -D TARGET_LINUX -Wall -o iot-subscribe iot-subscribe.cpp -I ../../source/iot-framework/ -L ../../source/iot-framework -l clixxIO_Linux -l mosquittopp
