#include <Arduino.h>
#include <EEPROM.h>

#define IN1 3
#define IN2 2
#define ROUT1 4
#define ROUT2 5
#define ROUT3 6
#define BOUT 12
#define LED 13
#define M1A 1
#define M1B 2
#define M1C 3
#define M2A 4
#define M2B 5
#define M2C 6
#define M3A 7
#define M3B 8
#define M3C 9

volatile int i1;
volatile int i2;
unsigned long previous_time;
unsigned long current_time;
unsigned short m1;
unsigned short m2;
unsigned short m3;
char answer;
unsigned short incoming;

// ISRs
void ISR_scada1();
void ISR_scada2();
// utility functions
void SerialCom();
void EEPROMread();
void EEPROMwrite();

void setup()
{
  Serial.begin(9600);

  // SerialCom();

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

  while (digitalRead(IN1) == LOW)
  {
  }
  i1 = LOW;

  while (digitalRead(IN2) == LOW)
  {
  }
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

void loop()
{
  EEPROMread();
  while (i1 || i2)
  {
    while (i1)
    {
      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT1, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m1 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT1, HIGH);
      delay(1000);
      if (!i1)
        break;

      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m2 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      delay(1000);
      if (!i1)
        break;

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m3 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT3, HIGH);
      delay(1000);
      if (!i1)
        break;
    }

    while (i2)
    {
      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m3 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT3, HIGH);
      delay(1000);
      if (!i2)
        break;

      digitalWrite(ROUT1, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT2, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m2 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT2, HIGH);
      delay(1000);
      if (!i2)
        break;

      digitalWrite(ROUT2, HIGH);
      digitalWrite(ROUT3, HIGH);
      digitalWrite(ROUT1, LOW);
      current_time = millis();
      previous_time = current_time;
      while ((current_time - previous_time) <= (m1 * 1000))
      {
        current_time = millis();
        if (!(i1 || i2))
          goto endLoop;
      }

      digitalWrite(ROUT1, HIGH);
      delay(1000);
      if (!i2)
        break;
    }
  }
endLoop:
  digitalWrite(ROUT1, HIGH);
  digitalWrite(ROUT2, HIGH);
  digitalWrite(ROUT3, HIGH);

  if (Serial.available())
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
  return;
}

void ISR_scada2()
{
  if (digitalRead(IN2) == LOW)
    i2 = HIGH;
  else if (digitalRead(IN2) == HIGH)
    i2 = LOW;
  return;
}

void SerialCom()
{
  m1 = 300;
  m2 = 300;
  m3 = 300;

  while (Serial.available())
  {
    answer = Serial.read();
    delay(100);
  }
  answer = Serial.read();

  Serial.println("\r\n\r\n**********************************************************************************");
  Serial.println("Please input the duration of the EVAC messages, as counted in seconds");
  Serial.println("Type the duration and DO NOT PRESS ENTER - wait until the value is shown on screen");

  do
  {
    Serial.print("Message 1: ");
    while (!Serial.available())
      delay(100);
    m1 = Serial.parseInt();
    if (m1 > 255)
      Serial.println("Only values below 255 are allowed");
    else
      Serial.println(m1);
  } while (m1 > 255);

  do
  {
    Serial.print("Message 2: ");
    while (!Serial.available())
      delay(100);
    m2 = Serial.parseInt();
    if (m2 > 255)
      Serial.println("Only values below 255 are allowed");
    else
      Serial.println(m2);
  } while (m2 > 255);

  do
  {
    Serial.print("Message 3: ");
    while (!Serial.available())
      delay(100);
    m3 = Serial.parseInt();
    if (m3 > 255)
      Serial.println("Only values below 255 are allowed");
    else
      Serial.println(m3);
  } while (m3 > 255);

  Serial.println("\r\nThe durations of messages you inserted - as counted in seconds - where:");
  Serial.print("Message 1: ");
  Serial.println(m1);
  Serial.print("Message 2: ");
  Serial.println(m2);
  Serial.print("Message 3: ");
  Serial.println(m3);

  Serial.print("Are these values correct? (press 'y' to store them or 'n' to run again): ");
  while (!Serial.available())
    delay(100);
  answer = Serial.read();
  if (answer == 'y')
  {
    while (Serial.available())
    {
      answer = Serial.read();
      delay(100);
    }
    answer = Serial.read();

    EEPROMwrite();

    Serial.println("\r\n\tBack to the execution");
    Serial.println("**********************************************************************************\r\n");
    return;
  }
  else
  {
    Serial.println("\r\n**********************************************************************************\r\n");
    SerialCom();
  }
}

void EEPROMread()
{
  int msg1[3], msg2[3], msg3[3];

  msg1[0] = (unsigned short)EEPROM.read(M1A);
  msg1[1] = (unsigned short)EEPROM.read(M1B);
  msg1[2] = (unsigned short)EEPROM.read(M1C);

  msg2[0] = (unsigned short)EEPROM.read(M2A);
  msg2[1] = (unsigned short)EEPROM.read(M2B);
  msg2[2] = (unsigned short)EEPROM.read(M2C);

  msg3[0] = (unsigned short)EEPROM.read(M3A);
  msg3[1] = (unsigned short)EEPROM.read(M3B);
  msg3[2] = (unsigned short)EEPROM.read(M3C);

  int i, j;
  int size = 3;
  int count;

  count = 0;
  for (i = 0; i < size; i++)
  {
    int current = msg1[i];
    int freq = 0;
    for (j = 0; j < size; j++)
    {
      if (msg1[j] == current)
      {
        freq++;
      }
    }
    if (freq >= (size + 1) / 2)
    {
      count = 1;
      m1 = current;
      break;
    }
  }
  if (count == 0)
  {
    m1 = 30;
    Serial.println("EEPROM FAULT!\r\n");
    digitalWrite(LED, HIGH);
  }

  count = 0;
  for (i = 0; i < size; i++)
  {
    int current = msg2[i];
    int freq = 0;
    for (j = 0; j < size; j++)
    {
      if (msg2[j] == current)
      {
        freq++;
      }
    }
    if (freq >= (size + 1) / 2)
    {
      count = 1;
      m2 = current;
      break;
    }
  }
  if (count == 0)
  {
    m2 = 30;
    Serial.println("EEPROM FAULT!\r\n");
    digitalWrite(LED, HIGH);
  }

  count = 0;
  for (i = 0; i < size; i++)
  {
    int current = msg3[i];
    int freq = 0;
    for (j = 0; j < size; j++)
    {
      if (msg3[j] == current)
      {
        freq++;
      }
    }
    if (freq >= (size + 1) / 2)
    {
      count = 1;
      m3 = current;
      break;
    }
  }
  if (count == 0)
  {
    m3 = 30;
    Serial.println("EEPROM FAULT!\r\n");
    digitalWrite(LED, HIGH);
  }

  return;
}

void EEPROMwrite()
{
  EEPROM.write(M1A, m1);
  EEPROM.write(M1B, m1);
  EEPROM.write(M1C, m1);
  EEPROM.write(M2A, m2);
  EEPROM.write(M2B, m2);
  EEPROM.write(M2C, m2);
  EEPROM.write(M3A, m3);
  EEPROM.write(M3B, m3);
  EEPROM.write(M3C, m3);
  return;
}