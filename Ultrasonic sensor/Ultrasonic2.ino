const int trigPin = 9;
const int echoPin = 10;
const int outputPin = 13;  //output pin
long ThresholdDistance = 0;// threshold value 
long currentDistance = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(outputPin, OUTPUT);  
}

void loop() {
  currentDistance = measureDistance();
  if (currentDistance<ThresholdDistance) {
    digitalWrite(outputPin, HIGH);
  } else {
    digitalWrite(outputPin, LOW);
  }
    delay(200); //random delay to prevent extra readings 
}

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
