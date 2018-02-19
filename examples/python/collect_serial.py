import serial,time

def serial_com(serial_port):
    '''Serial communications: get a response'''

    # read response from serial port
    lines = []
    while True:
        line = serial_port.readline()
        lines.append(line.decode('utf-8').rstrip())

        # wait for new data after each line
        timeout = time.time() + 0.1
        while not serial_port.inWaiting() and timeout > time.time():
            pass
        if not serial_port.inWaiting():
            break 

    return lines
    
if __name__ == "__main__":
    # open serial port
    com_port = '/dev/ttyUSB0'
    
    try:
        serial_port = serial.Serial(com_port, baudrate=19200, timeout=1)
    except serial.SerialException as e:
        print("could not open serial port '{}': {}".format(com_port, e))

    ok = True
    while ok:
        lines = serial_com(serial_port)
        if len(lines):
            print lines
    
    #close the serial port
    serial_port.close()   
