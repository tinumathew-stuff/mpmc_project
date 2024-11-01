/*
 * Clap Detection and Device Control
 * This sketch detects claps using a sound sensor and controls a relay accordingly
 */

// Pin definitions
#define SOUND_SENSOR_PIN 7  // Sound sensor connected to pin 7
#define RELAY_PIN 8        // Relay connected to pin 8

// Global variables
unsigned long lastEvent = 0;    // Time of last detected sound
boolean relayState = false;     // Current state of relay

void setup() {
  // Initialize pins
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Ensure relay starts in OFF state
  digitalWrite(RELAY_PIN, LOW);
  
  Serial.println("Clap detection system initialized!");
}

void loop() {
  // Read sound sensor
  int sensorData = digitalRead(SOUND_SENSOR_PIN);
  
  // Check if sound is detected (LOW indicates sound detection)
  if (sensorData == LOW) {
    // Check if enough time has passed since last detection
    // This helps prevent false triggers from echo or continuous noise
    if (millis() - lastEvent > 25) {
      // Toggle relay state
      relayState = !relayState;
      
      // Update relay output
      digitalWrite(RELAY_PIN, relayState);
      
      // Debug output
      Serial.print("Clap detected! Relay state: ");
      Serial.println(relayState ? "ON" : "OFF");
    }
    
    // Update last event time
    lastEvent = millis();
  }
}
