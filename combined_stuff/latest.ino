#include <SoftwareSerial.h>
#define sensorPin1 A0
#define soundpin1 A1
#define sensorPin2 A2
#define flamepin1 A3
#define flamepin2 A4
//#define RELAY_PIN1 8        // Relay connected to pin 8
//#define RELAY_PIN2 5        // Relay connected to pin 8
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define THRESHOLD_VALUE 200
const int LED1 = 8;
const int LED2 = 11;
//esp code
const int rxPin = 2;
const int txPin = 3;
SoftwareSerial espSerial(rxPin, txPin);
//----
float sensorValue;
float outputValue;
const int trigPin1 = 9;
const int echoPin1 = 10;
//const int outputPin1 = 13;  //output pin
const int trigPin2 = 4;
const int echoPin2 = 6;
//unsigned long lastEvent = 0;    // Time of last detected sound
boolean relayState = false;     // Current state of relay
const int buttonPin1 = 15; // Pin connected to the button
const int buttonPin2 = 13; // Pin connected to the button
const int buzzerPin = 7; // Pin connected to the buzzer
long ThresholdDistance1 = 16;// threshold value 
long ThresholdDistance2 = 12;// threshold value
long currentDistance = 0;
String s1 = "off,";
String s2 = "off,";
String s3 = "off,";
String s4 = "off,";
String s5 = "off";
String s6 = "\n";
int melody[] = { NOTE_G3, NOTE_B3 };
int noteDurations[] = { 8, 8 }; // note durations: 4 = quarter note, 8 = eighth note, etc.
bool running = false; // Tracks if melody is playing

void sounddetection(const int soundpin,const int buttonPin,const int LED){
  float soundvalue = analogRead(soundpin);
  Serial.println(soundvalue);
  if(soundvalue>500){
    digitalWrite(LED, LOW);
    playMelody(buttonPin);
     s1 = "on,";
    //digitalWrite(RELAY_PIN,HIGH);// turn relay ON
    delay(3000);// keep it ON for 3 seconds
  }
}


void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  //pinMode(RELAY_PIN1, OUTPUT);
  //pinMode(RELAY_PIN2, OUTPUT);
  // Initialize serial communication for debugging
  Serial.begin(9600);
  // Ensure relay starts in OFF state
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  //digitalWrite(RELAY_PIN1, LOW);
  //digitalWrite(RELAY_PIN2, LOW);
  pinMode(flamepin1, INPUT);
  pinMode(flamepin2, INPUT);
  Serial.println("Clap detection system initialized!");
   pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
 // pinMode(outputPin1, OUTPUT);
 // pinMode(buttonPin1, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
 // pinMode(outputPin2, OUTPUT);
  pinMode(buttonPin2, INPUT);
}
void flamedetection(const int flamePin,const int buttonPin,const int LED){
  int sensorValue = analogRead(flamePin);
  Serial.println(sensorValue);
  if (sensorValue < THRESHOLD_VALUE) {
    digitalWrite(LED, LOW);
    Serial.println("high"); // turn on LED when flame detected
    s4="on,";
    playMelody(buttonPin);
  } else {
    Serial.println("low"); // turn off LED when no flame detected
  }
  delay(100);
}

void readSensor(const int sensorPin,const int buttonPin,const int LED) {
   // Read the analog value from sensor
  sensorValue = analogRead(sensorPin);
  //unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data
  outputValue = (sensorValue/1023)*255;
  Serial.println(outputValue);
  //Serial.println(x);
  if (int(outputValue) >= 120){
      digitalWrite(LED, LOW);
       s2 = "on,"; 
      //digitalWrite(RELAY_PIN,HIGH);// turn relay ON
      //delay(3000);// keep it ON for 3 seconds
      playMelody(buttonPin);
  }
 // return outputValue;

}

void loop() {
  Serial.println("flame detec 1");
  flamedetection(flamepin1,buttonPin1,LED1);
   Serial.println("sound 1");
  sounddetection(soundpin1,buttonPin1,LED1);
 Serial.println("smoke 1");
  readSensor(sensorPin1,buttonPin1,LED1);
  Serial.println("dist detec 1");
  measureDistance(trigPin1,echoPin1,ThresholdDistance1,buttonPin1,LED1);
  
   s5 = "off";
  if (digitalRead(buttonPin1) == HIGH && !running) {
     s5 = "on";
     Serial.println("button 1 on");
     //digitalWrite(LED1,LOW);
    playMelody(buttonPin1); 
  }
  String result1 = s1+s2+s3+s4+s5;
  Serial.println("flame detec 2");
  flamedetection(flamepin2,buttonPin2,LED2);
  Serial.println("sound detec 2");
  sounddetection(soundpin1,buttonPin2,LED2);
  Serial.println("smoke detec 2");
  readSensor(sensorPin2,buttonPin2,LED2);
  Serial.println("dist detec 2");
  measureDistance(trigPin2,echoPin2,ThresholdDistance2,buttonPin2,LED2);
  
  if (digitalRead(buttonPin2) == HIGH && !running) {
    Serial.println("button 2 on");
     s5 = "on";
     digitalWrite(LED2, LOW);
    playMelody(buttonPin2);
  }
   String result2 = s1+s2+s3+s4+s5;
   String result = result1+"\n"+result2;
   Serial.println(result);
   espSerial.println(result); // Send data to the ESP8266
  Serial.println("Data sent to ESP8266: " + result);
 // Serial.println(result);
  delay(500);
  // Call playMelody() whenever needed
}

//  This function returns the analog data to calling function


// Function to handle clap detection and relay control

void measureDistance(const int trigPin,const int echoPin,long ThresholdDistance,const int buttonPin,const int LED) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH); //calc time of echo
  long distance = duration * 0.034 / 2; //calc dist using time
  Serial.println(distance);
  Serial.println(ThresholdDistance);
   if (distance<ThresholdDistance) {
    //digitalWrite(LED, LOW);
    Serial.println("dist of smthn high");
    s3 = "on,";
    digitalWrite(LED,HIGH);
   }
  else{
    Serial.println("dist of smthn low");
    digitalWrite(LED,LOW);
    s3 = "off,";
    //digitalWrite(RELAY_PIN,HIGH);// turn relay ON
   // playMelody(buttonPin,RELAY_PIN);
   } //random delay to prevent extra readings 
}
void playMelody(const int buttonPin) {
  Serial.println("SOund being played");
  //digitalWrite(RELAY_PIN,HIGH);// turn relay ON
  delay(3000);// keep it ON for 3 seconds
  running = true; // Start playing melody
  // Array of loop iterations and note durations to create the melody sequence
  int loops[] = { 10, 10, 100 }; 
  int durations[] = { 5000, 2500, 1000 };
  for (int i = 0; i < 3 && running; i++) {
    for (int x = 0; x < loops[i] && running; x++) {
      for (int thisNote = 0; thisNote < 2 && running; thisNote++) {
        Serial.println("playin loop");
        
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
  Serial.println("buzz stopped");
  noTone(buzzerPin);
  running = false;
   s5 = "off";
   s4 = "off,";
   s3 = "off,";
   s2 = "off,";
   s1 = "off,";
}


