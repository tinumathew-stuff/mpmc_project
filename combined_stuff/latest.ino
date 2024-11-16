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

long ThresholdDistance1 = 16;
long ThresholdDistance2 = 12;

String s1 = "off,";
String s2 = "off,";
String s3 = "off,";
String s4 = "off,";
String s5 = "off";

int melody[] = { NOTE_G3, NOTE_B3 };
int noteDurations[] = { 8, 8 };
bool running = false;

void sounddetection(const int soundpin, const int buttonPin, const int LED) {
    float soundvalue = analogRead(soundpin);
    if (soundvalue > 500) {
        digitalWrite(LED, LOW);
        playMelody(buttonPin);
        s1 = "on,";
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
}

void flamedetection(const int flamePin, const int buttonPin, const int LED) {
    int sensorValue = analogRead(flamePin);
    if (sensorValue < THRESHOLD_VALUE) {
        digitalWrite(LED, LOW);
        s4 = "on,";
        playMelody(buttonPin);
    }
}

void readSensor(const int sensorPin, const int buttonPin, const int LED) {
    sensorValue = analogRead(sensorPin);
    outputValue = (sensorValue / 1023) * 255;
    if (int(outputValue) >= 120) {
        digitalWrite(LED, LOW);
        s2 = "on,";
        playMelody(buttonPin);
    }
}

void loop() {
    flamedetection(flamepin1, buttonPin1, LED1);
    sounddetection(soundpin1, buttonPin1, LED1);
    readSensor(sensorPin1, buttonPin1, LED1);
    measureDistance(trigPin1, echoPin1, ThresholdDistance1, buttonPin1, LED1);

    s5 = "off";
    if (digitalRead(buttonPin1) == HIGH && !running) {
        s5 = "on";
        playMelody(buttonPin1);
    }
    String result1 = s1 + s2 + s3 + s4 + s5;

    flamedetection(flamepin2, buttonPin2, LED2);
    sounddetection(soundpin1, buttonPin2, LED2);
    readSensor(sensorPin2, buttonPin2, LED2);
    measureDistance(trigPin2, echoPin2, ThresholdDistance2, buttonPin2, LED2);

    if (digitalRead(buttonPin2) == HIGH && !running) {
        s5 = "on";
        digitalWrite(LED2, LOW);
        playMelody(buttonPin2);
    }
    String result2 = s1 + s2 + s3 + s4 + s5;
    String result = result1 + "\n" + result2;
    espSerial.println(result);
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
    } else {
        digitalWrite(LED, LOW);
        s3 = "off,";
    }
}

void playMelody(const int buttonPin) {
    running = true;
    int loops[] = { 10, 10, 100 };
    int durations[] = { 5000, 2500, 1000 };

    for (int i = 0; i < 3 && running; i++) {
        for (int x = 0; x < loops[i] && running; x++) {
            for (int thisNote = 0; thisNote < 2 && running; thisNote++) {
                if (digitalRead(buttonPin) == HIGH) {
                    stopMelody();
                    return;
                }
                int noteDuration = durations[i] / noteDurations[thisNote];
                tone(buzzerPin, melody[thisNote], noteDuration);
                delay(noteDuration * 1.30);
                noTone(buzzerPin);
            }
        }
    }
    running = false;
}

void stopMelody() {
    noTone(buzzerPin);
    running = false;
    s5 = "off";
    s4 = "off,";
    s3 = "off,";
    s2 = "off,";
    s1 = "off,";
}
