#include "AFMotor.h"
#include <Servo.h>
int echopin=A0; // echo pin
int trigpin =A1; // Trigger pin
int irsensor=A2;
Servo myservo;
const int MOTOR_2 = 2; 
const int MOTOR_3 = 3; 
AF_DCMotor motor2(MOTOR_2, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor3(MOTOR_3, MOTOR34_64KHZ); // create motor object, 64KHz pwm
int distance_F,ir_L,ir_R;

long distance,duration;
int set = 20;
void setup(){
  Serial.begin(9600);           // Initialize serial port
  Serial.println("Start");

  myservo.attach(10);
  myservo.write(90);
  delay(1000);

  pinMode (A1, OUTPUT);
  pinMode (A0, INPUT );
  pinMode (A2,INPUT);
  motor2.setSpeed(250);          // set the motor speed to 0-255
  motor3.setSpeed(250);
}
void loop(){
 forward();
 hc_sr4();
}
void hc_sr4(){
    Serial.println("Stop");
    rlease();
    myservo.write(0);
    delay(300);
    ir_R=irdata();
    delay(100);
    myservo.write(170);
    delay(500);
    ir_L=irdata();
    myservo.write(90);
    delay(300);
    compareObject();
}
void compareObject(){
  if ((ir_L==LOW && ir_R==LOW )||(ir_L==LOW && ir_R==HIGH ))
  {
    Serial.print("VEHICLE are not their either sides");
    Serial.println(ir_L);
    Serial.println(ir_R);
    left();
    rlease();
    forward();
    parking();
  }
  else if(ir_L==HIGH && ir_R==LOW )
  {
    Serial.print("VEHICLE present at left side");
    Serial.println(ir_L);
    right();
    rlease();
    forward();
    parking();
  }
  else
  {
    Serial.print("S=");
    Serial.println(distance_F);
    distance_F==data();
    if(distance_F>set)
    {
      forward();
      hc_sr4();
    }
    else{
      backward();
    }
  }
}
void parking(){
  distance_F==data();
    Serial.print("S=");
    Serial.println(distance_F);
    if (distance_F > set)
    {
    Serial.println("Forward");
    forward();  
    }
    else
    {
      rlease();
    }
}
long data(){
  digitalWrite(A1, LOW);
  delayMicroseconds(2);
  digitalWrite(A1, HIGH);
  delayMicroseconds(10);
  digitalWrite(A1,LOW);
  duration = pulseIn (A0, HIGH);
  distance=(duration/58.2);
  return  distance;
}
void forward(){
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  delay(500);
}
void left(){
  motor2.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(300);
}
void right(){
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  delay(300);
}
void backward(){
  motor2.run(BACKWARD);
  motor2.run(FORWARD);
  delay(1000);
}
void rlease(){
  motor2.run(RELEASE);
  motor2.run(RELEASE);
  delay(200);
}
int irdata(){
   int ir=analogRead(A2);
   return ir;
}

  
