#include <Arduino.h>

#define IN1 2
#define IN2 3
#define ROUT1 9
#define ROUT2 7
#define ROUT3 5
#define BOUT 11

volatile bool i1, i2;
unsigned long previous_time;
unsigned long current_time;

void ISR_input1();
void ISR_input2();

void setup() {
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  pinMode(ROUT1, OUTPUT);
  pinMode(ROUT2, OUTPUT);
  pinMode(ROUT3, OUTPUT);
  pinMode(BOUT, OUTPUT);

  digitalWrite(ROUT1, HIGH);
  digitalWrite(ROUT2, HIGH);
  digitalWrite(ROUT3, HIGH);

  if (digitalRead(IN1) == LOW)
    i1 = true;
  else
    i1 = false;
  if (digitalRead(IN2) == LOW)
    i2 = true;
  else
    i2 = false;

  attachInterrupt(digitalPinToInterrupt(IN1), ISR_input1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN2), ISR_input2, CHANGE);
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
      while ((current_time - previous_time) <= 1000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }
      
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= 2000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= 3000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }
    }
    while(i2)
    {      
      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= 3000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= 2000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT1, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= 1000)
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }
    }
  }
  endLoop:
  digitalWrite(ROUT1, HIGH);
  digitalWrite(ROUT2, HIGH);
  digitalWrite(ROUT3, HIGH);
}

void ISR_input1()
{
  i1 = !i1;
}

void ISR_input2()
{
  i2 = !i2;
}