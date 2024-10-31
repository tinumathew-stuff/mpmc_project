check the following code snippet for any errors : 
const int trigPin = 9;
const int echoPin = 10;
const int outputPin = 13;  //output pin
const int motionThreshold = 5; 
long previousDistance = 0;

void setup() {
  pinMode(trigPin, OUTPUT);        // Set the Trigger pin as an output
  pinMode(echoPin, INPUT);         // Set the Echo pin as an input
  pinMode(outputPin, OUTPUT);      // Set the output pin
}

void loop() 
{
  if (motionDetected) 
  {
    digitalWrite(outputPin, HIGH);
  } 
  else 
  {
    digitalWrite(outputPin, LOW);
  }

  delay(200); //random delay to avoid excessive readings
}

bool motionDetected() 
{
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); //calc time of echo

  distance = duration * 0.034 / 2; //calc distance using duration of return of echo 

  if (abs(distance - previousDistance) > motionThreshold) 
  {
    previousDistance = distance; //Update previousDistance
    return true;                 //Significant motion detected
  }
  previousDistance = distance;  //Update threshold value 
  return false; 
}
