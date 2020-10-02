#include<ESP8266WiFi.h>
const int trigPin1=16;
const int echoPin1=5;
const int trigPin2=4;
const int echoPin2=0;
int led=2;

unsigned long timesincelastreset=0;

int counter=0;

int interval_1=3000;
int interval_2=3000;

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void setup(){
  Serial.begin(115200);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(led,OUTPUT);
}

void loop(){
  long duration11, distance11, duration12, distance12;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration11=pulseIn(echoPin1, HIGH);
  distance11=microsecondsToCentimeters(duration11);
  Serial.print("Distance11");
  Serial.print(distance11);
  
  if(distance11<50 && distance11>0)
  {
    int t=0;
    timesincelastreset=millis();
    while(t==0 && (millis()-timesincelastreset)<interval_1)
    {
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);
      duration12=pulseIn(echoPin2, HIGH);
      distance12=microsecondsToCentimeters(duration12);
      if(distance12<50 && distance12>0)
      {
        counter++;
        Serial.println("Counter = " +counter);
        Serial.print("Lights On");
        digitalWrite(led,HIGH);
        t=2000;
        delay(1000);
      }
    }
  }

  int duration21, distance21, duration22, distance22;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration21=pulseIn(echoPin2, HIGH);
  distance21=microsecondsToCentimeters(duration21);
  if(distance21<50 && distance21>0)
  {
    int t=0;
    timesincelastreset=millis();
    while(t==0 && (millis()-timesincelastreset)<interval_2)
    {
      digitalWrite(trigPin1, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration22=pulseIn(echoPin1, HIGH);
      distance22=microsecondsToCentimeters(duration22);
      if(distance22<50 && distance22>0)
      {
        if(counter==0)
        {
          break;
        }
        counter--;
        Serial.println("Counter = " +counter);
        if(counter==0)
        {
          delay(3000);
          Serial.println("Lights Off");
          digitalWrite(led,LOW);
          t=t+1;
        }
        else if(counter!=0 && counter>0)
        {
          break;
        }
      }
    }
  }
  delay(300);
}
