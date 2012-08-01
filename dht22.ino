#include <SoftwareSerial.h>
#include <DHT22.h>

#define tx_PIN 2
#define DHT22_PIN 4

SoftwareSerial LCD = SoftwareSerial(0, tx_PIN);
DHT22 sensor(DHT22_PIN);

// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay=10;  // conservative, 2 actually works
int flag = 0;
int cnt = 0;

void clearLCD() {
  LCD.write(0xFE);   //command flag
  LCD.write(0x58);   //clear command.
  delay(LCDdelay);
}

void contrastLCD(char c) {
  LCD.write(0xFE);
  LCD.write(0x91);
  LCD.write(c);
  delay(LCDdelay);
}

void cursorOff() {
  LCD.write(0xFE);
  LCD.write(0x54);
  delay(LCDdelay);
}

void backlightOn() {       //turns on the backlight
  LCD.write(0xFE);   //command flag for backlight stuff
  LCD.write(0x42);   //light level.
  delay(LCDdelay);
}

void backlightOff() {      //turns off the backlight
  LCD.write(0xFE);   //command flag for backlight stuff
  LCD.write(0x46);   //light level for off.
  delay(LCDdelay);
}

void lineOne() {           //puts the cursor at line 0 char 0.
  LCD.write(0xFE);   //command flag
  LCD.write(128);    //position
}

void newLine() {
  LCD.write(10); 
}

void cursorHome() {
  LCD.write(0xFE);
  LCD.write(72);
}

void cursorSet(int xpos, int ypos){  
  LCD.write(017);
  LCD.write(xpos); //Column position   
  LCD.write(ypos); //Row position 
} 

void setup() {
  pinMode(tx_PIN, OUTPUT);
  LCD.begin(9600);
  delay(1000);  // the LCD display take some time to initialize
  clearLCD();
  contrastLCD(255);
  backlightOff();
  LCD.print("Starting sensor");
}


void loop()
{
  DHT22_ERROR_t errorCode;
  errorCode = sensor.readData();
  clearLCD();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      char buf[128];
      sprintf(buf, "    Temp: %hi.%01hiC\nHumidity: %i.%01i%%",
              sensor.getTemperatureCInt()/10, abs(sensor.getTemperatureCInt()%10),
              sensor.getHumidityInt()/10, sensor.getHumidityInt()%10);
      LCD.print(buf);
      break;
    case DHT_ERROR_CHECKSUM:
      LCD.print("check sum error ");
      break;
    case DHT_BUS_HUNG:
      LCD.print("BUS Hung");
      break;
    case DHT_ERROR_NOT_PRESENT:
      LCD.print("Not Present");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      LCD.print("ACK time out");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      LCD.print("Sync Timeout");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      LCD.print("Data Timeout");
      break;
    case DHT_ERROR_TOOQUICK:
      LCD.print("Polled to quick ");
      break;
  }
  delay(10000); // We update poll the sensor every 10 sec
}
