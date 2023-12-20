// Define the pin numbers for the sensors and LED
const int touchPin = 13;
const int ledPin = 12;
const int buzzerPin = 14;
int channel = 0; // PWM channel for the tone
int resolution = 8; // resolution for the PWM signal
int frequency = 1000; // frequency of the tone in Hz

// Define the thresholds for the sensors
const int touchThreshold = 20;
const int hallThreshold = 50;

// Variable for reading the sensors value
int hallEffectValue = 0;
int touchValue = 0;

void setup() {
  Serial.begin(9600);
  // Set the input and output pins
  pinMode(touchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // configure and control the LEDC (LED Control) module on the ESP32
  ledcSetup(channel, frequency, resolution);
  ledcAttachPin(buzzerPin, channel);
}

void loop() {
  int hallEffectValue = hallRead();
  if (abs(hallEffectValue) > hallThreshold) {
    digitalWrite(ledPin, HIGH); // turn on LED
  } else {
    digitalWrite(ledPin, LOW);  // turn off LED
  }

  int touchValue = touchRead(touchPin);
  if (touchValue > touchThreshold) {
    ledcWrite(channel, 0); // turn off buzzer
  } else {
    ledcWrite(channel, 127); // turn on buzzer at 50% duty cycle
  }
  delay(300);
}