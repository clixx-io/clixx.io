# ToDo: Replace with a Makefile
g++ -Wall -g -D TARGET_LINUX -Wall -o gpio-test gpio-test.cpp -I ../../iot-framework/ -L ../../iot-framework -l clixxIO_Linux -l mosquittopp
