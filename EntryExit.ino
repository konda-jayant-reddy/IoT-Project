//Bi-Directional Visitor Counter With Controlled Lighting In A Room
//--An IoT Based Project--

#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

const int trigPin1=13;
const int echoPin1=10;
const int trigPin2=8;
const int echoPin2=7;

const int led1=A0;
const int led2=A1;
const int led3=A3;

const int IR=9;

unsigned long timesincelastreset=0;

int counter1=0;
int counter2=0;
int counter=0;

int IRCounter=0;
int IR_old=0;
int IR_new;

int interval_1=3000;
int interval_2=3000;

long microsecondsToCentimeters(long microseconds) 
{
   return microseconds / 29 / 2;
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  
  pinMode(IR,INPUT);
}

void lights()
{
  for(int i=0; i<130; i++)
  {
    analogWrite(A2, i);
  }
}

void loop()
{
  long duration11, distance11, duration12, distance12;
  IR_new=digitalRead(IR);
  if(IR_old==1 && IR_new==0)
  {
    IRCounter=(IRCounter+1)%2;
  }
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration11=pulseIn(echoPin1, HIGH);
  distance11=microsecondsToCentimeters(duration11);

  if(distance11<30 && distance11>0)
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
      if(distance12<30 && distance12>0)
      {
        counter1++;
        Serial.print("Counter1 = ");
        Serial.println(counter1);
        t=2000;
        delay(1000);
      }
    }
    counter=counter1-counter2;
  }

  int duration21, distance21, duration22, distance22;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration21=pulseIn(echoPin2, HIGH);
  distance21=microsecondsToCentimeters(duration21);
  
  if(distance21<30 && distance21>0)
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
      if(distance22<30 && distance22>0)
      {
        if(counter2==counter1)
        {
          counter1=0;
          counter2=0;
          break;
        }
        counter2++;
        Serial.print("Counter2 = ");
        Serial.println(counter2);
        if(counter2==counter1)
        {
          counter1=0;
          counter2=0;
        }
        t=2000;
        delay(1000);
      }
    }
    counter=counter1-counter2;
  }
  if( counter==0 || (counter>0 && IRCounter==1) )
  {
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    Serial.println("Lights Off");
  }
  if(counter==0)
  {
    IRCounter= 0;
  }
  else if(counter>0 && IRCounter==0)
  {
    lights();
    Serial.println("Lights On");
  }
  lcd.setCursor(0,0);
  lcd.print("People In: ");
  lcd.print(counter);
  lcd.setCursor(0,1);
  lcd.print("Light Status:");
  if(counter>0 && IRCounter==0)
  {
    lcd.print("ON");
    lcd.print(" ");
  }
  else if(counter==0 || (counter>0 && IRCounter==1))
  {
    lcd.print("OFF");
  }
  IR_old=IR_new;
  delay(300);
}
