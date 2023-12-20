/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Replace with your network credentials
const char* ssid     = "B20";
const char* password = "NAVITAKRISTI";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/bmp280_readings/with/key/cF2NxYS_6JRDE0wChzQ-K5";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 30 minutes = 1800 seconds
uint64_t TIME_TO_SLEEP = 1800;

// Uncomment to use BMP280 SPI
/*#include <SPI.h>
#define BMP_SCK 18
#define BMP_MISO 19
#define BMP_MOSI 23
#define BMP_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp;  // I2C
//Adafruit_BMP280 bmp(BMP_CS);  // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);  // software SPI

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // initialize BMP280 sensor
  bool status = bmp.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring or change I2C address!");
    while (1);
  }

  initWifi();
  makeIFTTTRequest();

  // enable timer deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  Serial.println("Going to sleep now");
  // start deep sleep for 3600 seconds (60 minutes)
  esp_deep_sleep_start();
}

void loop() {
  // sleeping so wont get here 
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + String(bmp.readTemperature()) + "\",\"value2\":\"" + String(bmp.readPressure()/100.0F)
                      + "\",\"value3\":\"" + String(bmp.readAltitude(SEALEVELPRESSURE_HPA)) + "\"}";
                      
  // Comment the previous line and uncomment the next line to publish temperature readings in Fahrenheit                    
  /*String jsonObject = String("{\"value1\":\"") + String(1.8 * bmp.readTemperature() + 32) + "\",\"value2\":\"" 
                      + String(bmp.readPressure()/100.0F) + "\",\"value3\":\"" + String(bmp.readAltitude(SEALEVELPRESSURE_HPA)) + "\"}";*/
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}