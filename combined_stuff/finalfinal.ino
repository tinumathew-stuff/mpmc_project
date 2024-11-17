#include <SoftwareSerial.h>
#define sensorPin1 A0
#define soundpin1 A1
#define sensorPin2 A2
#define flamepin1 A3
#define flamepin2 A4
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define THRESHOLD_VALUE 200
#define LED3 A5
#define LED4 5
#define LED5 12
int parameter1=0;
int parameter2=0;


String incomingData;

int buzzstate = 0;
int fullbuzzstate=1;

const int LED1 = 8;
const int LED2 = 11;
const int rxPin = 2;
const int txPin = 3;
SoftwareSerial espSerial(rxPin, txPin);

float sensorValue;
float outputValue;
const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 4;
const int echoPin2 = 6;
const int buttonPin1 = 15;
const int buttonPin2 = 13;
const int buzzerPin = 7;

long ThresholdDistance1 = 13;
long ThresholdDistance2 = 10;

String s1 = "off,";
String s2 = "off,";
String s3 = "off,";
String s4 = "off,";
String s5 = "off";
int soundvalue;
int melody[] = { NOTE_G3, NOTE_B3 };
int noteDurations[] = { 8, 8 };
bool running = false;

void sounddetection(const int soundpin, const int buttonPin, const int LED) {
    soundvalue = analogRead(soundpin);
    Serial.println(soundvalue);
    if (soundvalue < 440 && soundvalue >420 ) {
        digitalWrite(LED, LOW);
        s2 = "on,";
        playMelody();
        
    }
}

void setup() {
    Serial.begin(9600);
    espSerial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(flamepin1, INPUT);
    pinMode(flamepin2, INPUT);
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(buttonPin1, INPUT_PULLUP);
   pinMode(LED3, OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);

}




void playMelody() {
  if(fullbuzzstate==1){
  fullbuzzstate=0;
  while (true) {
    soundvalue=435;
    delay(1);
    rooms();
    tone(buzzerPin,1000,200);

    if (digitalRead(buttonPin2) == HIGH && buzzstate==1){
        delay(1000);
        stopMelody();
        buzzstate=0;
        s5="off";
        s2="off,";
        break;
        }
    
    if (digitalRead(buttonPin2) == HIGH){
        delay(1000);
        s2="off,";
        stopMelody();
        break;
        }

    if (digitalRead(buttonPin1) == LOW && buzzstate==1){
        delay(1000);
        stopMelody();
        buzzstate=0;
        s5="off";
        s2="off,";
        break;
        }
    
    if (digitalRead(buttonPin1) == LOW){
        delay(1000);
        s2="off,";
        stopMelody();
        break;
        }



    if(incomingData=="ON"){
      break;
    }
  }
}
}

void stopMelody() {

  digitalWrite(buzzerPin, LOW);

}



void measureDistance(const int trigPin, const int echoPin, long ThresholdDistance, const int buttonPin, const int LED) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;

    if (distance < ThresholdDistance) {
        s3 = "on,";
        digitalWrite(LED, HIGH);
        if (ThresholdDistance==13){ //room1
          parameter1=1;
        }
        if (ThresholdDistance==10){ //room2
          parameter2=1;
        }
    }
    else {
        if (ThresholdDistance==13){ //room1
          parameter1=0;
        }
        if (ThresholdDistance==10){ //room2
          parameter2=0;
        }
        digitalWrite(LED, LOW);
        s3 = "off,";
    }
}





void flamedetection(const int flamePin, const int buttonPin, const int LED) {
    int sensorValue = analogRead(flamePin);
    if (sensorValue < THRESHOLD_VALUE) {
        digitalWrite(LED, LOW);
        s4 = "on,";
        playMelody();
    }
    else{
      s4="off,";
    }
}

void readSensor(const int sensorPin, const int buttonPin, const int LED) {
    sensorValue = analogRead(sensorPin);
    outputValue = (sensorValue / 1023) * 255;
    if (int(outputValue) >= 120) {
        digitalWrite(LED, LOW);
        s1 = "on,";
        playMelody();
    }
    else{
      s1="off,";
    }

}

void rooms(){
    fullbuzzstate=0;
    flamedetection(flamepin1, buttonPin1, LED1);
    //sounddetection(soundpin1, buttonPin1, LED1);
    readSensor(sensorPin1, buttonPin1, LED1);
    measureDistance(trigPin1, echoPin1, ThresholdDistance1, buttonPin1, LED1);
    String result1 = s1 + s2 + s3 + s4 + s5;


    flamedetection(flamepin2, buttonPin2, LED2);
    //sounddetection(soundpin1, buttonPin2, LED2);
    readSensor(sensorPin2, buttonPin2, LED2);
    measureDistance(trigPin2, echoPin2, ThresholdDistance2, buttonPin2, LED2);

    if (parameter1==1 && parameter2==1){
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED5,LOW);
    }
    else{
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,HIGH);
      digitalWrite(LED5,HIGH);

    }


    String result2 = s1 + s2 + s3 + s4 + s5;


    String result = result1 + "," + result2;

    Serial.println(result);
    espSerial.println(result);

    //for reading through esp serial brooo
if (espSerial.available()) {               // Check if there's data available from ESP8266
    incomingData = espSerial.readStringUntil('\n');
    Serial.println(incomingData);
    if(incomingData=="ON"){
      s1=s2=s3=s4="off,";
      s5="off";
      stopMelody();
    }
}

}



void loop() {
    fullbuzzstate=1;
    flamedetection(flamepin1, buttonPin1, LED1);
    sounddetection(soundpin1, buttonPin1, LED1);
    readSensor(sensorPin1, buttonPin1, LED1);
    measureDistance(trigPin1, echoPin1, ThresholdDistance1, buttonPin1, LED1);

    //s5 = "off";
    //if (digitalRead(buttonPin1) == HIGH && !running) {
    //    s5 = "on";
    //    playMelody();
   // }
    String result1 = s1 + s2 + s3 + s4 + s5;


    flamedetection(flamepin2, buttonPin2, LED2);
    //sounddetection(soundpin1, buttonPin2, LED2);
    readSensor(sensorPin2, buttonPin2, LED2);
    measureDistance(trigPin2, echoPin2, ThresholdDistance2, buttonPin2, LED2);

    if (parameter1==1 && parameter2==1){
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED5,LOW);
    }
    else{
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,HIGH);
      digitalWrite(LED5,HIGH);

    }

    //if (digitalRead(buttonPin1) == HIGH){
    //      playMelody();
    //      s5="on,";
     //   }
    //if (digitalRead(buttonPin1) == HIGH && s5=="on,"){
    //      stopMelody();
    //      s5="off,";
    //    }
    if (digitalRead(buttonPin2) == HIGH){
          delay(1000);
          buzzstate=1;
          s5="on2";
          playMelody();
        }

      if (digitalRead(buttonPin1) == LOW){
          delay(1000);
          buzzstate=1;
          s5="on1";
          playMelody();
        }
    //if (digitalRead(buttonPin2) == HIGH && s5 == "on,") {
    //  stopMelody();
    //  s5 = "off,";

String result2 = s1 + s2 + s3 + s4 + s5;


    String result = result1 + "," + result2;

    Serial.println(result);
    espSerial.println(result);


//for reading through esp serial brooo
if (espSerial.available()) {               // Check if there's data available from ESP8266
    incomingData = espSerial.readStringUntil('\n');
    Serial.println(incomingData);
    if(incomingData=="ON"){

      s1=s2=s3=s4="off,";
      s5="off";
      stopMelody();
    }
}
}
