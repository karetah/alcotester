#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

//SCL to A5, SDA to A4
LiquidCrystal_PCF8574 lcd(0x27); 

unsigned long lastSecond = 0;
unsigned long lastSecond10 = 0;
unsigned long lastMinute = 0;
unsigned long lastTen = 0;
unsigned long lastHour = 0;
unsigned long now;

const int analogInPin = A0;
int sensorValue = 0; 

int curSensorValue = 0;
int maxSensorValue = 0;
int barSensorValue = 0;

int pBari = 0;
byte pBar[8] = {
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
};



int getSensorValue()
{
    sensorValue = analogRead(analogInPin);
    return sensorValue;
}

void setup()
{
  int error;
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2); // initialize the lcd

  } else {
    Serial.println(": LCD not found.");
  } // if
  lcd.createChar(0, pBar);
  
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
}
void loop()
{

  now = millis();
  curSensorValue = getSensorValue();
  if (maxSensorValue < curSensorValue) {maxSensorValue = curSensorValue;}
  if (barSensorValue < maxSensorValue) {barSensorValue = maxSensorValue;}

// 1 seconds timer
  if (now - lastSecond > 1000) 
  {
    //Serial.println(maxSensorValue);
    pBari=map(barSensorValue, 100, 500, 0, 15);
    lcd.setCursor(0, 0);
    lcd.print("   ");
    lcd.setCursor(0, 0);
    lcd.print(maxSensorValue);
    lastSecond = now;
    for (int i=0; i<pBari; i++)
      {
      lcd.setCursor(i, 1);
      lcd.write(byte(0));
      }
  }
  // 10 seconds timer
  if (now - lastSecond10 > 10000) 
  {
    maxSensorValue = 0;
    lcd.clear();
    lastSecond10 = now;
  }

  // one minute timer (20 sec)
if (now - lastMinute > 20000) 
  {
    
    barSensorValue = 0;
    lastMinute = now;
  }

  // ten minutes timer
if (now - lastTen > 600000) 
  {
    lastTen = now;
  }

  // 1 hour timer
if (now - lastHour > 3600000) 
  {
    lastHour = now;
  }
}
