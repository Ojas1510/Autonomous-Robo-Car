int t1 = A0;
int t2 = A1;
int pin5 = 5;
int pin6 = 6;
int pin7 = 8;
int pin8 = 7;
int irPin = 4;
int flag = 1;
int r1, r2;
unsigned long d = 0;
static int gantryCounter = 0;
unsigned static long StartTime = 0;
unsigned static long CurrentTime = 0;
unsigned long previousMillisG = 0;
unsigned long previousMillisU = 0;
unsigned long intervalU = 500;
unsigned long intervalG = 1000;
#include <NewPing.h>
#define TRIGGER_PIN 13
#define ECHO_PIN 12
#define MAX_DISTANCE 50
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
void setup()
{
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(irPin, INPUT);
  Serial.begin(9600);
  Serial.print("+++");
  delay(1500);
  Serial.println("ATID 3333, CH C, CN");
}

void loop()
{
  if (flag == 0)
  {
    if (Serial.available() > 0)
    {
      char s = Serial.read();
      switch (s) {
        case '^':
          {
            flag = 1;
            StartTime = millis();
          }
      }
    }
  }
  else
  {

    CurrentTime = millis();
    if (CurrentTime - previousMillisU > intervalU)
    {
      detectObstacle();
      previousMillisU = millis();
    }
    CurrentTime = millis();
    if (CurrentTime - previousMillisG > intervalG)
    {
      gantry();
      previousMillisG = millis();
    }
    if (gantryCounter >= 2)
    {
      parkbuggy();
    }
    else
    {
      normalLineFollow();
    }

  }

}
void parkbuggy()
{
  readsensor();
  if (r1 != 0 || r2 != 0)
  {
    normalLineFollow();
  }
  else
  {
    forward();
    delay(400);
    right_rot();
    delay(600);
    left_rot();
    readsensor();
    while (r1 == 1)
    {
      readsensor();
    }
    unsigned long mmtime = millis();
    while (millis() - mmtime < 5000)
    {
      normalLineFollow();
    }
    stopBuggy();
    delay(2000);
  }
}
void gantry()
{
  if (digitalRead(irPin) == HIGH)
  {
    d = pulseIn(irPin, HIGH);
    if (d >= 500 and d < 1500)
    {
      //Serial.println(d);
      Serial.println("Gantry: 1");
      gantryCounter = gantryCounter + 1;
      stopBuggy();
      delay(1000);
      previousMillisG = millis();
    }
    else if (d >= 1500 and d < 2500)
    {
      //  Serial.println(d);
      Serial.println("Gantry: 2");
      gantryCounter = gantryCounter + 1;
      stopBuggy();
      delay(1000);
      previousMillisG = millis();
    }
    else if (d >= 2500 and d < 4000)
    {
      // Serial.println(d);
      Serial.println("Gantry: 3");
      gantryCounter = gantryCounter + 1;
      Serial.print("The gantry Counter is: ");
      Serial.println(gantryCounter);
      stopBuggy();
      delay(1000);
      previousMillisG = millis();
    }
    else
    {
      //Serial.println(d);
      Serial.println("Gantry: Unknown");
    }
  }
}

void stopBuggy()
{
  digitalWrite(pin5, LOW);
  digitalWrite(pin6, LOW);
  digitalWrite(pin7, LOW);
  digitalWrite(pin8, LOW);
}
void normalLineFollow()
{
  readsensor();
  if (r1 == LOW && r2 == LOW)
  {
    right_rot();
    delay(250);

  }
  if (r1 == LOW && r2 == HIGH)
  {
    left_rot();
  }
  if (r1 == HIGH && r2 == LOW)
  {
    right_rot();
  }
  if (r1 == HIGH && r2 == HIGH)
  {
    forward();
  }

}
void detectObstacle()
{
  delay(50);
  unsigned int distanceCm = sonar.ping_cm();
  //Serial.print("Ping: ");
  //Serial.print(distanceCm);
  //Serial.println("cm");
  while ((distanceCm < 15) && (distanceCm > 0))
  {
    stopBuggy();
    delay(1000);
    previousMillisU = millis();
  }
}

void forward()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);

}
void backward()
{
  digitalWrite(6, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
}
void left_rot()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}
void right_rot()
{
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}
void readsensor()
{
  r1 = digitalRead(t1);
  r2 = digitalRead(t2);
  int r1 = analogRead(t1);
  int r2 = analogRead(t2);
  if (r1 > 300)
    r1 = 1;
  else
    r1 = 0;
  if (r2 > 150)
    r2 = 1;
  else
    r2 = 0;
  Serial.print("IRvalue");
  Serial.print(r1);
  Serial.print(",");
  Serial.println("r2");
}
