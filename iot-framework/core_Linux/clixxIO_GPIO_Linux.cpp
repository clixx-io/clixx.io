 /* ===============================================================================
 * 
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 =================================================================================
 */

#include <stdlib.h>
#include <cctype>
#include "clixxIO.hpp"
#include "minIni.h"

using namespace std;

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

clixxIOGPIOPin::clixxIOGPIOPin():_gpionum(),valuefd(-1),directionfd(-1),exportfd(-1),unexportfd(-1)
{
}

clixxIOGPIOPin::clixxIOGPIOPin(short pinnumber,short direction):valuefd(-1),directionfd(direction),exportfd(-1),unexportfd(-1)
{
    // Convert the pin-number to a string
    stringstream pintemp;
    
    pintemp << pinnumber;
    _gpionum = pintemp.str();
    
    // Instatiate clixxIOGPIOPin object for GPIO pin number "gnum"
    this->exportpin();
    
    if (direction == 0)
        this->setdir("in");
    else
        this->setdir("out");

}

/**********************************************************************
 * valuefd
 *
 **********************************************************************/  
clixxIOGPIOPin::clixxIOGPIOPin(string gnum):_gpionum(gnum),valuefd(-1),directionfd(-1),exportfd(-1),unexportfd(-1)
{
    // Instatiate clixxIOGPIOPin object for GPIO pin number "gnum"
    this->exportpin();
}

int clixxIOGPIOPin::configure(const char *logicalname)
{
  char str[100];
  const char inifile[] = "program.ini";
  
  if (logicalname == 0)
      return -1;
      
  if (isdigit(*logicalname))
  {
      // Use the pin number
      _gpionum = logicalname;
      
  } else {

      // Read the pin assignment from the configuration file
      ini_gets("connections-linux",logicalname, "-1", str, sizearray(str), inifile);
      
      // Just print that for now
      perror(str);

  }
  return 0; 
}

/**********************************************************************
 * valuefd
 *
 **********************************************************************/  
clixxIOGPIOPin::~clixxIOGPIOPin()
{
    this->unexportpin();
}

/**********************************************************************
 * export_gpio
 *
 **********************************************************************/  
int clixxIOGPIOPin::exportpin()
{
    int statusVal = -1;
    string exportStr = "/sys/class/gpio/export";
    this->exportfd = statusVal = open(exportStr.c_str(),  O_WRONLY|O_SYNC);
    if (statusVal < 0){
        perror("Error - Could not open SYSFS GPIO export device");
        return(-1);
    }

    stringstream ss;
    ss << this->_gpionum;
    string numStr = ss.str();
    statusVal = write(this->exportfd, numStr.c_str(), numStr.length());
    if (statusVal < 0){
        perror("Error - Could not write to SYSFS GPIO export device");
        return(-1);
    }

    statusVal = close(this->exportfd);
    if (statusVal < 0){
        perror("Error - Could not close SYSFS GPIO export device");
        return(-1);
    }

    return statusVal;
}

/**********************************************************************
 * unexport_gpio
 *
 **********************************************************************/  
int clixxIOGPIOPin::unexportpin()
{
    int statusVal = -1;
    string unexportStr = "/sys/class/gpio/unexport";
    this->unexportfd = statusVal = open(unexportStr.c_str(),  O_WRONLY|O_SYNC);
    if (statusVal < 0){
        perror("Error - Could not open SYSFS GPIO unexport device");
        return(-1);
    }

    stringstream ss;
    ss << this->_gpionum;
    string numStr = ss.str();
    statusVal = write(this->unexportfd, numStr.c_str(), numStr.length());
    if (statusVal < 0){
        perror("Error - Could not write to SYSFS GPIO unexport device");
        return(-1);
    }

    statusVal = close(this->unexportfd);
    if (statusVal < 0){
        perror("Error - Could not close SYSFS GPIO unexport device");
        return(-1);
    }

    return statusVal;
}

/**********************************************************************
 * setdir
 *
 **********************************************************************/  
int clixxIOGPIOPin::setdir(string dir)
{
    int statusVal = -1;
    string setdirStr ="/sys/class/gpio/gpio" + this->_gpionum + "/direction";

    this->directionfd = statusVal = open(setdirStr.c_str(), O_WRONLY|O_SYNC); // open direction file for gpio
    if (statusVal < 0){
        perror("Error - Could not open SYSFS GPIO direction device");
        return(-1);
    }

    if (dir.compare("in") != 0 && dir.compare("out") != 0 ) {
        fprintf(stderr, "Error: Invalid direction value. Should be \"in\" or \"out\". \n");
        return(-1);
    }

    statusVal = write(this->directionfd, dir.c_str(), dir.length());
    if (statusVal < 0){
        perror("Error - Could not write to SYSFS GPIO direction device");
        return(-1);
    }

    statusVal = close(this->directionfd);
    if (statusVal < 0){
        perror("Error - Could not close SYSFS GPIO direction device");
        return(-1);
    }

    return statusVal;
}

/**********************************************************************
 * setval
 *
 **********************************************************************/  
int clixxIOGPIOPin::setval(string val)
{

    int statusVal = -1;
    string setValStr = "/sys/class/gpio/gpio" + this->_gpionum + "/value";

    this->valuefd = statusVal = open(setValStr.c_str(), O_WRONLY|O_SYNC);
    if (statusVal < 0){
        perror("Error - Could not open SYSFS GPIO value device");
        return(-1);
    }

    if (val.compare("1") != 0 && val.compare("0") != 0 ) {
        fprintf(stderr, "Error: Invalid  value. Should be \"1\" or \"0\". \n");
        return(-1);
    }
    
    statusVal = write(this->valuefd, val.c_str(), val.length());
    if (statusVal < 0){
        perror("Error - Could not write to SYSFS GPIO value device");
        return(-1);
    }

    statusVal = close(this->valuefd);
    if (statusVal < 0){
        perror("Error - Could not close SYSFS GPIO value device");
        return(-1);
    }

    return statusVal;
}

/**********************************************************************
 * setval
 *
 **********************************************************************/  
int clixxIOGPIOPin::setval(short val)
{
    string valueStr("0");
    if (val == 0)
    {
        setval(valueStr);
    } else
    {
        valueStr = "1";
    }
    return setval(valueStr);
}

/**********************************************************************
 * getval
 *
 **********************************************************************/  
int clixxIOGPIOPin::getval(){

    string getValStr = "/sys/class/gpio/gpio" + this->_gpionum + "/value";
    char buff[10];
    int statusVal = -1;
    this->valuefd = statusVal = open(getValStr.c_str(), O_RDONLY|O_SYNC);
    if (statusVal < 0){
        perror("Error - Could not open SYSFS GPIO value device");
        return(-1);
    }

    statusVal = read(this->valuefd, &buff, 1);
    if (statusVal < 0)
    {
        perror("Error - Could not read SYSFS GPIO value device");
        return(-1);
    }

    buff[1]='\0';

    string val = string(buff);

    if (val.compare("1") != 0 && val.compare("0") != 0 ) {
        fprintf(stderr, "Error: Invalid  value read. Should be \"1\" or \"0\". \n");
        return(-1);
    }

    statusVal = close(this->valuefd);
    if (statusVal < 0){
        perror("Error - Could not close SYSFS GPIO value device");
        return(-1);
    }

    if (buff[0] == '0')
        statusVal = 0;
    else
        statusVal = 1;
        
    return statusVal;
}

/**********************************************************************
 * getnum
 *
 **********************************************************************/  
string clixxIOGPIOPin::getpinnumber(){

    return this->_gpionum;

}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
bool clixxIO_Button::pressed(){	  

    return false;

}

/** Turns a switch On
*
* This function turns the switch On
* 
*/
void clixxIO_Switch::On()
{
    this->setval(1);
}

/** Turns a switch off
*
* This function turns the switch off
* 
*/
void clixxIO_Switch::Off()
{
    this->setval(0);
}
