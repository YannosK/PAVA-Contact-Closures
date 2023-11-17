#include <Arduino.h>

#define IN1 3
#define IN2 2
#define ROUT1 9
#define ROUT2 7
#define ROUT3 5
#define BOUT 11
#define LED 13

volatile int i1;
volatile int i2;
unsigned long previous_time;
unsigned long current_time;

void ISR_scada1closed();
void ISR_scada1opened();
void ISR_scada2closed();
void ISR_scada2opened();

void setup() {
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

  // while(digitalRead(IN1) == HIGH)
  //   {}
  // i1 = LOW;
  
  // while(digitalRead(IN2) == HIGH)
  //   {}
  // i2 = LOW;

  attachInterrupt(digitalPinToInterrupt(3), ISR_scada1closed, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), ISR_scada2closed, HIGH);
  attachInterrupt(digitalPinToInterrupt(3), ISR_scada1opened, LOW);
  attachInterrupt(digitalPinToInterrupt(2), ISR_scada2opened, LOW);

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
  if (i1 == HIGH && i2 == LOW)
    {digitalWrite(ROUT1, LOW);
    digitalWrite(ROUT2, HIGH);
    digitalWrite(ROUT3, HIGH);
    delay(100);}
  else if (i2 == HIGH && i1 == LOW)
    {digitalWrite(ROUT2, LOW);
    digitalWrite(ROUT1, HIGH);
    digitalWrite(ROUT3, HIGH);
    delay(100);}    
  else if (i1 == LOW && i2 == LOW)
    {digitalWrite(ROUT3, LOW);
    digitalWrite(ROUT2, HIGH);
    digitalWrite(ROUT1, HIGH);
    delay(100);}
}

void ISR_scada1closed()
{
  i1 = HIGH;
}

void ISR_scada1opened()
{
  i1 = LOW;
}

void ISR_scada2closed()
{
  i2 = HIGH;
}

void ISR_scada2opened()
{
  i2 = LOW;
}