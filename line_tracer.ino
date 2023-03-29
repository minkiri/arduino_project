#define RightMotorA_forward 5
#define RightMotorA_backward 6
#define LeftMotorB_forward 10
#define LeftMotorB_backward 11
#define IR_right 12
#define IR_left 8
#define TRIG 13
#define ECHO 3
#define LED_R 7
#define LED_G 9
#define LED_B 4
#define cds A2

int flag_right = 0;
int flag_left = 0;
long duration;
long distance;
int val = 0;
int light;
int Sub_msg_l, Sub_msg_r;
long motorSpeed = 0;

void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  pinMode(RightMotorA_forward, OUTPUT);
  pinMode(RightMotorA_backward, OUTPUT);
  pinMode(LeftMotorB_forward, OUTPUT);
  pinMode(LeftMotorB_backward, OUTPUT);
  
  pinMode(IR_right, INPUT);
  pinMode(IR_left, INPUT);
  
  pinMode(cds, INPUT);
  Serial.begin(9600);
}

void sensor_control()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 17 / 1000;

  if (distance <= 10)
  {
    motorSpeed = 0;
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  } 
  else if (distance <= 20)
  {
    motorSpeed = 80;
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
  } 
  else
  {
    motorSpeed = 100;
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
    sensor_light();
  }
}

void sensor_light()
{
  int light = map(analogRead(cds), 0, 1023, 0, 255);
  
  if (light <= 120)
  {
    motorSpeed -= 30;
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
  }
}

void motor_control()
{
  sensor_control();
  
  Sub_msg_l = digitalRead(IR_left);
  Sub_msg_r = digitalRead(IR_right);

  analogWrite(RightMotorA_backward, 0);
  analogWrite(LeftMotorB_backward, 0);
  
  if (Sub_msg_l && Sub_msg_r)
  {
    analogWrite(RightMotorA_forward, motorSpeed + 40);
    analogWrite(LeftMotorB_forward, motorSpeed - 10);
  }
  else if (!Sub_msg_l && Sub_msg_r)
  {
    analogWrite(RightMotorA_forward, motorSpeed / 2 );
    analogWrite(LeftMotorB_forward, motorSpeed);
  }
  else if (!Sub_msg_r && Sub_msg_l)
  {
    analogWrite(RightMotorA_forward, motorSpeed + 60);
    analogWrite(LeftMotorB_forward, motorSpeed / 2);
  }
}

void Left_control()
{
  if (!Sub_msg_r && Sub_msg_l && distance <= 15)
  {
    delay(300);
    go1();
  }
  else if (Sub_msg_r && !Sub_msg_l && distance <= 15)
  {
    delay(300);
    go2();
  }
}

void go1()
{
  analogWrite(LeftMotorB_forward, 170);
  analogWrite(RightMotorA_forward, 0);
}

void go2()
{
  analogWrite(LeftMotorB_forward, 170);
  analogWrite(RightMotorA_forward, 0);
}

void loop()
{
  motor_control();
  Left_control();
  delay(5);
}
