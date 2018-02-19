// Example using the LM335A library for reading temperatures
// Created by Jonathan Merrill, February 20, 2010.
// http://www.greenrobotics.net
//  Released into the public domain.


LM335A InsideTemp(0); //pass the analog input pin number

void setup() {

    Serial.begin(115200);
    Serial.println("starting");

}

void loop() {

    delay(3000);
    //user must call ReadTemp before any valid temp data is available
    InsideTemp.ReadTemp();  
    Serial.print("Fahrenheit: ");
    //functions to get the temperature in various unitsfs
    Serial.println(InsideTemp.Fahrenheit());
    Serial.print("Celsius: ");
    Serial.println(InsideTemp.Celsius());
    Serial.print("Kelvin: ");
    Serial.println(InsideTemp.Kelvin());

}
