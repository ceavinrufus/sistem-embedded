// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;  // the number of the pushbutton pin
const int ledPin = 12;    // the number of the LED pin
const int ledPin2 = 14;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  Serial.begin(115200); 
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
  }
}
