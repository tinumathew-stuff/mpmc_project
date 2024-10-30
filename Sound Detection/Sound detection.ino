// basic sound detection
#define sensorPin 8

// Variable to store the time when last event happened
unsigned long lastEvent = 0;

void setup() {
	pinMode(sensorPin, INPUT);	// Set sensor pin as an INPUT
	Serial.begin(9600);
}

void loop() {
	// Read Sound sensor
	int sensorData = digitalRead(sensorPin);

	// If pin goes LOW, sound is detected
	if (sensorData == LOW) {
		
		// If 25ms have passed since last LOW state, it means that
		// the clap is detected and not due to any spurious sounds
		if (millis() - lastEvent > 25) {
			Serial.println("Clap detected!");
		}
		
		// Remember when last event happened
		lastEvent = millis();
	}
}
unsigned long lastEvent = 0;
pinMode(sensorPin, INPUT);
Serial.begin(9600);
int sensorData = digitalRead(sensorPin);
if (sensorData == LOW) {
	if (millis() - lastEvent > 25) {
		Serial.println("Clap detected!");
	}
	lastEvent = millis();
}
// Controlling Devices With a Clap
#define sensorPin 7
#define relayPin 8

// Variable to store the time when last event happened
unsigned long lastEvent = 0;
boolean relayState = false;    // Variable to store the state of relay

void setup() {
	pinMode(relayPin, OUTPUT);  // Set relay pin as an OUTPUT pin
	pinMode(sensorPin, INPUT);  // Set sensor pin as an INPUT
}

void loop() {
	// Read Sound sensor
	int sensorData = digitalRead(sensorPin);

	// If pin goes LOW, sound is detected
	if (sensorData == LOW) {

	// If 25ms have passed since last LOW state, it means that
	// the clap is detected and not due to any spurious sounds
	if (millis() - lastEvent > 25) {
		//toggle relay and set the output
		relayState = !relayState;
		digitalWrite(relayPin, relayState ? HIGH : LOW);
	}

	// Remember when last event happened
	lastEvent = millis();
	}
}
#define relayPin 7  //relayState to keep track of the relay’s status

boolean relayState = false;
pinMode(relayPin, OUTPUT); // configure the relayPin as an output
relayState = !relayState;
digitalWrite(relayPin, relayState ? HIGH : LOW); //the serial monitor and we simply toggle the state of the relay
