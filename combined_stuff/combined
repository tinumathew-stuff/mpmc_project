#define sensorPin A0

void setup() {
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  // Initialize pins
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Ensure relay starts in OFF state
  digitalWrite(RELAY_PIN, LOW);
  
  Serial.println("Clap detection system initialized!");
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  delay(500);
   // Call the clap detection function
  handleClapDetection(SOUND_SENSOR_PIN, RELAY_PIN);
   currentDistance = measureDistance();
  if (abs(currentDistance - previousDistance) > motionThreshold) {
    digitalWrite(outputPin, HIGH);
  } else {
    digitalWrite(outputPin, LOW);
  }
  previousDistance = currentDistance;
  delay(200); //random delay to prevent extra readings
  // Call playMelody() whenever needed
  if (digitalRead(buttonPin) == HIGH && !running) {
    playMelody();
  }
}

//  This function returns the analog data to calling function
int readSensor() {
  unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data
  return outputValue;             //return the analog value we are getting from the sensor
}

#define SOUND_SENSOR_PIN 7  // Sound sensor connected to pin 7
#define RELAY_PIN 8        // Relay connected to pin 8

// Global variables
unsigned long lastEvent = 0;    // Time of last detected sound
boolean relayState = false;     // Current state of relay

// Function to handle clap detection and relay control
boolean handleClapDetection(int sensorPin, int relayPin, unsigned long debounceTime = 25) {
  // Read sound sensor
  int sensorData = digitalRead(sensorPin);
  
  // Check if sound is detected (LOW indicates sound detection)
  if (sensorData == LOW) {
    // Check if enough time has passed since last detection
    if (millis() - lastEvent > debounceTime) {
      // Toggle relay state
      relayState = !relayState;
      
      // Update relay output
      digitalWrite(relayPin, relayState);
      
      // Debug output
      Serial.print("Clap detected! Relay state: ");
      Serial.println(relayState ? "ON" : "OFF");
    }
    
    // Update last event time
    lastEvent = millis();
  }
  
  return relayState;
}
const int trigPin = 9;
const int echoPin = 10;
const int outputPin = 13;  //output pin
const int motionThreshold = 5; 
long previousDistance = 0;
long currentDistance = 0;


long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); //calc time of echo

  long distance = duration * 0.034 / 2; //calc dist using time

  return distance;
}
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

// Pin setup
const int buttonPin = 2; // Pin connected to the button
const int buzzerPin = 8; // Pin connected to the buzzer

// Notes in the melody
int melody[] = { NOTE_G3, NOTE_B3 };
int noteDurations[] = { 8, 8 }; // note durations: 4 = quarter note, 8 = eighth note, etc.
bool running = false; // Tracks if melody is playing

void playMelody() {
  running = true; // Start playing melody
  
  // Array of loop iterations and note durations to create the melody sequence
  int loops[] = { 10, 10, 100 }; 
  int durations[] = { 5000, 2500, 1000 };

  for (int i = 0; i < 3 && running; i++) {
    for (int x = 0; x < loops[i] && running; x++) {
      for (int thisNote = 0; thisNote < 2 && running; thisNote++) {
        
        // Stop melody if button is pressed
        if (digitalRead(buttonPin) == HIGH) {
          stopMelody();
          return;
        }
        
        int noteDuration = durations[i] / noteDurations[thisNote];
        tone(buzzerPin, melody[thisNote], noteDuration);
        
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        
        noTone(buzzerPin);
      }
    }
  }
  running = false; // Stop playing melody
}

void stopMelody() {
  noTone(buzzerPin);
  running = false;
}
