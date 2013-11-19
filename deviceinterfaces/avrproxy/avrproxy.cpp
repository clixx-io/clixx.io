// AVR proxy
//
// A sketch for downloading that will give total control to all ports
//
// Input Pins
// - Polled or Interrupt
// - Logical, Analog, Serial
 
// Output Pins

// Protocol Binary/Analog/Json

//# Configuration stored in Flash
// one byte per gpio
struct gpioPin {unsigned char pinout:1,
                      poll:1,
                      intr:1,
                      intf:1,
                      intc:1,
       
                      tLog:1,
                      tAng:1,
                      tSer:1;
};

int pollDurations = 100;
#define gpioPinCount 20
#define bufflen 80
       
// Lastvalues, stored in RAM
unsigned char cVals[gpioPinCount];
unsigned int  iVals[gpioPinCount];

char cmdstr[bufflen];
bool cmdRec = false;
void setup()
{
    //Start the connection with the Host
    Serial.begin(115200);

}

void loop()
{
    handleCmd();
}

void serialEvent() {
       
    while(Serial.available() > 0) {
       char inByte = (char)Serial.read();
       if(inByte == ':') {
           cmdRec = true;
           return;
       } else if(inByte == '@') {
           cmdstr[0] = 0;
           cmdRec = false;
           return;
       } else {
           cmdstr[++cmdstr[0]] = inByte;
           if (cmdstr[0] >= bufflen)
               cmdstr[0] = 0;
           return;
       }
    }
}

void handleCmd() {
       
    if(!cmdRec) return;
        
    // If you have problems try changing this value,
    // my MEGA2560 has a lot of space
    int data[80];
    int numArgs = 0;

    int beginIdx = 0;
    int idx = cmd.indexOf(",");

    char charBuffer[20];

    while (idx != -1) {
           
       arg = cmd.substring(beginIdx, idx);
       arg.toCharArray(charBuffer, 16);

       data[numArgs++] = atoi(charBuffer);
       beginIdx = idx + 1;
       idx = cmd.indexOf(",", beginIdx);
    }
    
    // And also fetch the last command
    arg = cmd.substring(beginIdx);
    arg.toCharArray(charBuffer, 16);
    data[numArgs++] = atoi(charBuffer);
        // Now execute the command
    execCmd(data);

    cmdRec = false;
}

// For advanced function like switch all the leds in RGB
void execCmd(int* data) {
       
    switch(data[0]) {
        case 101:
                {
                     for(int i = 2; i < (data[1]*2)+1; i+=2) {
                         pinMode(data[i], OUTPUT);
                         analogWrite(data[i], data[i+1]);
                     }
                }
                break;

        case 102:
                {
                     pinMode(data[1], INPUT);
                     int sensor = analogRead(data[1]);
                     Serial1.println(sensor);
                }
                break;

        case 103:
                {
                     String result = "";
                     int sensor = 0;
                     for(int j = 2; j < data[1]+2; j++) {
                            pinMode(data[j], INPUT);
                            sensor = analogRead(data[j]);
                            result += String(sensor)+",";
                     }
                     Serial1.println(result);
                }
                break;
        default:
                {
                     pinMode(data[0], OUTPUT);
                     analogWrite(data[0], data[1]);
                }
                break;
        }
}
