Clixx.io USB TTL Adaptors running on Linux will need a UDEV rules file.

This means that the serial port traffic (UART) is passed to the host in a USB data stream instead of through a dedicated RS232 serial port.

Every USB device has a Vendor ID and a Product ID as seen for instance in lsusb output:

Copy this file to the /etc/rules.d Directory.