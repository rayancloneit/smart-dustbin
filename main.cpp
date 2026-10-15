#include <Servo.h>

#define TRIG_PIN 6
#define ECHO_PIN 7
#define SERVO_PIN 9
#define LED_PIN 4

// 2 initial to 400 final
const int DETECT_THRESHOLD = 10;
const int AWAY_THRESHOLD = 50;
const int LID_OPEN_ANGLE = 90;
const int LID_CLOSED_ANGLE = 0;

Servo lidServo;
long duration;
int distance;
bool lidIsOpen = false;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  lidServo.attach(SERVO_PIN);
  lidServo.write(LID_CLOSED_ANGLE);
  digitalWrite(LED_PIN, LOW);

  delay(500);
}

void loop() {
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Lid: ");
  Serial.println(lidIsOpen ? "OPEN" : "CLOSED");

  if (!lidIsOpen && distance > 0 && distance < DETECT_THRESHOLD) {
    Serial.println("hand detected, opening lid...");
    lidServo.write(LID_OPEN_ANGLE);
    digitalWrite(LED_PIN, HIGH);
    lidIsOpen = true;
    delay(500);
  }

  if (lidIsOpen && distance > AWAY_THRESHOLD) {
    Serial.println("hand moved away, closing lid...");
    lidServo.write(LID_CLOSED_ANGLE);
    digitalWrite(LED_PIN, LOW);
    lidIsOpen = false;
    delay(1000);
  }

  delay(100);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.0343 / 2;

  return distance;
}
