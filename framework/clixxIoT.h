#ifndef CLIXXIOTHARDWAREBASE_H
#define CLIXXIOTHARDWAREBASE_H


class ClixxIoTHardwareBase
{
public:
    ClixxIoTHardwareBase();

    void Enable();
    void Disable();
};

class ClixxIoTCommunicationsBus : public ClixxIoTHardwareBase
{
public:
    ClixxIoTCommunicationsBus();
};

class ClixxIoTSensorDevice : public ClixxIoTHardwareBase
{
public:
    ClixxIoTSensorDevice();

    int getValue(bool &boolValue);
    int getValue(int &intValue);
    int getValue(double &doubleValue);
};

class ClixxIoTActuatorDevice : public ClixxIoTHardwareBase
{
public:
    ClixxIoTActuatorDevice();

    int setValue(const bool value);
    int setValue(const int value);
    int setValue(const double value);
    int setValue(const QString value);

};

class ClixxIoTProcessorDevice : public ClixxIoTHardwareBase
{
public:
    ClixxIoTProcessorDevice();
};

#endif // CLIXXIOTHARDWAREBASE_H
