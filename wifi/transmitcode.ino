#include <SoftwareSerial.h>

// Define the SoftwareSerial pins for communication with the ESP8266
const int rxPin = 2; // RX pin on the Arduino (connect to ESP8266's TX)
const int txPin = 3; // TX pin on the Arduino (connect to ESP8266's RX)

SoftwareSerial espSerial(rxPin, txPin); // Create a new SoftwareSerial object

void setup() {
  Serial.begin(9600);     // Initialize the Serial Monitor for debugging
  espSerial.begin(9600);  // Initialize the SoftwareSerial for ESP8266 communication

  delay(2000); // Give time for the ESP8266 to initialize
  Serial.println("Sending data to ESP8266...");
}

void loop() {
  String dataToSend = "Hello from Arduino Uno!"; // Example string to send
  espSerial.println(dataToSend); // Send data to the ESP8266
  Serial.println("Data sent to ESP8266: " + dataToSend); // Print for debugging

  delay(1000); // Delay to avoid flooding the ESP8266 with messages
}
