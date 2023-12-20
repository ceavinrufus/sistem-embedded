#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp; // I2C

unsigned long delayTime;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bmp.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  Serial.println("-- Default Test --");
  delayTime = 1000;
  Serial.println();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("Temp= ");
  lcd.print(bmp.readTemperature());
  lcd.print(" *C");
  
  lcd.setCursor(0,1);
  lcd.print("Press= ");
  lcd.print(bmp.readPressure() / 100.0F);
  lcd.print(" hPa");
  delay(4000);
  
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Alt= ");
  lcd.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  lcd.print(" m");
  delay(4000);
  
  lcd.clear();
  delay(delayTime);
}
