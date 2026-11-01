// Pins
const int trigPin = 9;
const int echoPin = 10;
const int irLeftPin = 2;
const int irRightPin = 3;

const int enaPin = 5;  // left motor speed (PWM)
const int in1Pin = 7;
const int in2Pin = 8;
const int enbPin = 6;  // right motor speed (PWM)
const int in3Pin = 11;
const int in4Pin = 12;

const int stopDistanceCm = 20;  // stop and turn if something is closer than this
const int motorSpeed = 150;     // 0 to 255

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irLeftPin, INPUT);
  pinMode(irRightPin, INPUT);

  pinMode(enaPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enbPin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  stopCar();
}

float readDistanceCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // 30 ms timeout ≈ 5 m
  if (duration == 0) return -1;                    // no echo
  return duration * 0.0343 / 2.0;
}

void forward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enaPin, motorSpeed);
  analogWrite(enbPin, motorSpeed);
}

void turnLeft() {
  // left motor backward, right motor forward
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enaPin, motorSpeed);
  analogWrite(enbPin, motorSpeed);
}

void stopCar() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enaPin, 0);
  analogWrite(enbPin, 0);
}

void loop() {
  float distance = readDistanceCm();
  bool irLeftBlocked = (digitalRead(irLeftPin) == LOW);
  bool irRightBlocked = (digitalRead(irRightPin) == LOW);
  bool obstacleClose = (distance > 0 && distance < stopDistanceCm);

  if (obstacleClose || irLeftBlocked || irRightBlocked) {
    stopCar();
    delay(300);
    turnLeft();
    delay(500);   // adjust to turn about 90 degrees on your chassis
    stopCar();
    delay(200);
  } else {
    forward();
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | IR L: ");
  Serial.print(irLeftBlocked ? "blocked" : "clear");
  Serial.print(" | IR R: ");
  Serial.println(irRightBlocked ? "blocked" : "clear");

  delay(50);
}
