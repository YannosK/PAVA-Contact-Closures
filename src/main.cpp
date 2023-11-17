#include <Arduino.h>

#define IN1 3
#define IN2 2
#define ROUT1 5
#define ROUT2 7
#define ROUT3 9
#define BOUT 11
#define LED 13

volatile int i1;
volatile int i2;
unsigned long previous_time;
unsigned long current_time;
unsigned short m1 = 1;
unsigned short m2 = 2;
unsigned short m3 = 3;
char answer;
unsigned short incoming;

void ISR_scada1();
void ISR_scada2();
void SerialCom();

void setup() {
  Serial.begin(9600);
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(ROUT1, OUTPUT);
  pinMode(ROUT2, OUTPUT);
  pinMode(ROUT3, OUTPUT);
  pinMode(BOUT, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(ROUT1, HIGH);
  digitalWrite(ROUT2, HIGH);
  digitalWrite(ROUT3, HIGH);
  digitalWrite(BOUT, LOW);

  while(digitalRead(IN1) == LOW)
    {}
  i1 = LOW;
  
  while(digitalRead(IN2) == LOW)
    {}
  i2 = LOW;

  attachInterrupt(digitalPinToInterrupt(IN1), ISR_scada1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN2), ISR_scada2, CHANGE);

  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
}

void loop() {
  while (i1 || i2)
  {
    while(i1)
    {      
      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT1, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m1*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT1, HIGH);
      delay(1000);
      if(!i1) break;
      
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m2*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      delay(1000);
      if(!i1) break;

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m3*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT3, HIGH);
      delay(1000);
      if(!i1) break;
    }

    while(i2)
    {      
      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m3*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT3, HIGH);
      delay(1000);
      if(!i2) break;

      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m2*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      delay(1000);
      if(!i2) break;

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT1, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m1*1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT1, HIGH);
      delay(1000);
      if(!i2) break;
    }
  }
  endLoop:
  digitalWrite(ROUT1, HIGH);
  digitalWrite(ROUT2, HIGH);
  digitalWrite(ROUT3, HIGH);

  while (Serial.available())
  {
    SerialCom();    
  }
}

void ISR_scada1()
{
  if (digitalRead(IN1) == LOW)
    i1 = HIGH;
  else if (digitalRead(IN1) == HIGH)
    i1 = LOW;
}

void ISR_scada2()
{
  if (digitalRead(IN2) == LOW)
    i2 = HIGH;
  else if (digitalRead(IN2) == HIGH)
    i2 = LOW;
}

void SerialCom()
{
  Serial.println("Please input the duration of the EVAC messages, as counted in seconds");
  Serial.print("Message 1: ");
  m1 = Serial.read();
  Serial.print("\r\nMessage 2: ");
  m2 = Serial.read();
  Serial.print("\r\nMessage 3: ");
  m3 = Serial.read();
  Serial.print("\r\nYou inputed the following time durations in seconds:\r\nMessage 1: ");
  Serial.println(m1);
  Serial.print("Message 2: ");
  Serial.println(m2);
  Serial.print("Message 3: ");
  Serial.println(m3);
  Serial.print("Are these values correct? (press 'y' to store them or 'n' to run again): ");
  answer = Serial.read();
  if (answer == 'y')
    return;
  else
    SerialCom();
}