#include "AFMotor.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#define echopin A0
#define trigpin A5
#define RC_CAR_MODE 0
#define AVOID_CAR_MODE 1
#define BUMP A4

Servo myservo;

const int MOTOR_1 = 1;
const int MOTOR_2 = 2;
const int MOTOR_3 = 3;
const int MOTOR_4 = 4;

AF_DCMotor MOTOR1(MOTOR_1, MOTOR12_64KHZ);
AF_DCMotor MOTOR2(MOTOR_2, MOTOR12_64KHZ);
AF_DCMotor MOTOR3(MOTOR_3, MOTOR12_64KHZ);
AF_DCMotor MOTOR4(MOTOR_4, MOTOR12_64KHZ);

int greenPin = A3;
int redPin = A2;
int bluePin = A1;
int distance_L, distance_F, distance_R;
long distance;
int val = 0;
int set = 25;
int mode = RC_CAR_MODE;

SoftwareSerial BTSerial(2, 3);
#define ActionTime 3000

void setup()
{
 Serial.begin(9600);
 myservo.attach(10);
 myservo.write(90);
 pinMode(BUMP, INPUT);
 pinMode(trigpin, OUTPUT);
 pinMode(echopin, INPUT);
 MOTOR1.setSpeed(255);
 MOTOR2.setSpeed(255);
 MOTOR3.setSpeed(255);
 MOTOR4.setSpeed(255);
 BTSerial.begin(9600);
}

void crash()
{
 val = digitalRead(BUMP);
 Serial.println(val);
 delay(20);
 if (val == 1)
 {
 setColor(255, 0, 255);
 delay(200);
 setColor(0, 0, 0);
 delay(200);
 setColor(255, 0, 255);
 delay(200);
 setColor(0, 0, 0);
 }
}

void setColor(int red, int green, int blue)
{
 analogWrite(redPin, red);
 analogWrite(greenPin, green);
 analogWrite(bluePin, blue);
}

long data()
{
 digitalWrite(trigpin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpin, HIGH);
 delayMicroseconds(10);
 distance = pulseIn(echopin, HIGH);
 return distance / 29 / 2;
}

void compareDistance()
{
 if (distance_L > distance_R)
 {
 MOTOR1.run(BACKWARD);
 MOTOR2.run(BACKWARD);
 MOTOR3.run(FORWARD);
 MOTOR4.run(FORWARD);
 delay(350);
 }
 else if (distance_R > distance_L)
 {
 MOTOR1.run(FORWARD);
 MOTOR2.run(FORWARD);
 MOTOR3.run(BACKWARD);
 MOTOR4.run(BACKWARD);
 delay(350);
 }
 else
 {
 MOTOR1.run(BACKWARD);
 MOTOR2.run(BACKWARD);
 MOTOR3.run(BACKWARD);
 MOTOR4.run(BACKWARD);
 delay(300);
 MOTOR1.run(BACKWARD);
 MOTOR2.run(BACKWARD);
 MOTOR3.run(FORWARD);
 MOTOR4.run(FORWARD);
 delay(500);
 }
}

void hc_sr4()
{
 MOTOR1.run(RELEASE);
 MOTOR2.run(RELEASE);
 MOTOR3.run(RELEASE);
 MOTOR4.run(RELEASE);
 myservo.write(0);
 delay(300);
 distance_R = data();
 delay(100);
 myservo.write(170);
 delay(500);
 distance_L = data();
 delay(100);
 myservo.write(90);
 delay(300);
 compareDistance();
}

void Stop_Release()
{
 MOTOR1.run(RELEASE);
 MOTOR2.run(RELEASE);
 MOTOR3.run(RELEASE);
 MOTOR4.run(RELEASE);
 setColor(255, 0, 0);
 delay(20);
}

void Go_Forward()
{
 MOTOR1.run(FORWARD);
 MOTOR2.run(FORWARD);
 MOTOR3.run(FORWARD);
 MOTOR4.run(FORWARD);
 setColor(0, 255, 0);
 delay(20);
}

void Go_Backward()
{
 MOTOR1.run(BACKWARD);
 MOTOR2.run(BACKWARD);
 MOTOR3.run(BACKWARD);
 MOTOR4.run(BACKWARD);
 crash();
 setColor(255, 255, 0);
 delay(20);
}

void Go_Left()
{
 MOTOR1.run(BACKWARD);
 MOTOR2.run(BACKWARD);
 MOTOR3.run(FORWARD);
 MOTOR4.run(FORWARD);
 setColor(0, 255, 255);
 delay(20);
}

void Go_Right()
{
 MOTOR1.run(FORWARD);
 MOTOR2.run(FORWARD);
 MOTOR3.run(BACKWARD);
 MOTOR4.run(BACKWARD);
 setColor(0, 255, 255);
 delay(20);
}

void RC_CAR()
{
 if (BTSerial.available())
 {
 char cmd = (char)BTSerial.read();
 if (cmd == 'f')
 {
 Go_Forward();
 }
 else if (cmd == 'l')
 {
 Go_Left();
 }
 else if (cmd == 'r')
 {
 Go_Right();
 }
 else if (cmd == 'b')
 {
 Go_Backward();
 }
 else if (cmd == 's')
 {
 Stop_Release();
 }
 else if (cmd == 'a')
 {
 mode = AVOID_CAR_MODE;
 }
 }
}

void AVOID_CAR()
{
 distance_F = data();
 if (distance_F > set)
 {
 MOTOR1.run(FORWARD);
 MOTOR2.run(FORWARD);
 MOTOR3.run(FORWARD);
 MOTOR4.run(FORWARD);
 }
 else
 {
 hc_sr4();
 }
 if (BTSerial.available())
 {
 char cmd = (char)BTSerial.read();
 if (cmd == 'q')
 {
 mode = RC_CAR_MODE;
 }
 }
}

void loop()
{
 if (mode == RC_CAR_MODE)
 {
 RC_CAR();
 }
 else if (mode == AVOID_CAR_MODE)
 {
 AVOID_CAR();
 }
}
