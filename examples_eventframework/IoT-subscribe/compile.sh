# ToDo: Replace with a Makefile
g++ -D TARGET_LINUX -Wall -o iot-subscribe Main.cpp -I ../../eventframework/ -L ../../eventframework -l clixxIO_Linux -l mosquittopp
