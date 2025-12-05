#include <Servo.h>

Servo myServo;

const int trigPin = 8;
const int echoPin = 9;
const int servoPin = 5;
const int ledPin  = 6;

long duration;
int distance;

bool at115 = false;
bool tantrum = false;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(115);

  digitalWrite(ledPin, LOW);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void doTantrum() {
  for (int i = 0; i < 10; i++) {
    myServo.write(115);
    digitalWrite(ledPin, HIGH);
    delay(100);

    myServo.write(55);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  tantrum = false;
  myServo.write(115);
}

void loop() {

  // Check if Processing sent a tantrum command
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'T') {
      tantrum = true;
    }
  }

  if (tantrum) {
    doTantrum();
    return;
  }

  int d = getDistance();
  bool objectDetected = (d > 0 && d < 20);

  // LED control
  digitalWrite(ledPin, objectDetected ? HIGH : LOW);

  // Servo control (SLOW 115 → 55 movement)
  if (objectDetected && !at115) {
    delay(1000);   // wait before lifting

    for (int pos = 115; pos >= 55; pos--) {
      myServo.write(pos);
      delay(15);   // <-- change this to slow it more
    }

    at115 = true;
  }

  if (!objectDetected && at115) {
    myServo.write(115);
    at115 = false;
  }

  // Send state to Processing
  if (objectDetected) Serial.println("DETECTED");
  else Serial.println("NONE");

  delay(100);
}
