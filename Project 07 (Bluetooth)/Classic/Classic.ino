#include "BluetoothSerial.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C

const int ledPin = 19;    // the number of the LED pin
int buttonState = 0;  // variable for reading the pushbutton status

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Handle received and sent messages
String message = "";
char incomingChar;
String temperatureString = "";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  while ( !Serial ) delay(100);   // wait for native usb
  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  SerialBT.begin("II2260"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  // Read received messages (LED control command)
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  }

  // Check received message and control output accordingly
  if (message == "led_on"){
    digitalWrite(ledPin, HIGH);
    SerialBT.println("LED turned on!");
  }
  else if (message == "led_off"){
    digitalWrite(ledPin, LOW);
    SerialBT.println("LED turned off!");
  }
  else if (message == "request_temperature"){
    SerialBT.print(F("Temperature = "));
    SerialBT.print(bmp.readTemperature());
    SerialBT.println(" *C");
  }
  else if (message == "request_pressure"){
    SerialBT.print(F("Pressure = "));
    SerialBT.print(bmp.readPressure());
    SerialBT.println(" Pa");
  }
  delay(20);
}