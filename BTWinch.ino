//#define pSpeed 9
//#define pSensZero 8
//
#define pServo 2 
#define pReverse 5
#define pSpeed_low PORTB &= B11111101
#define pSpeed_High PORTB |=B00000010 
#define pSensZero (PINB & B00000001)
#define pBTrx 3
#define pBTtx 4
#define pUp A5
#define pDown A4
#define pLock A1
#define pUnlock A2
#define pManSpeed A3

#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define vLock 50
#define vUnlock 85

SoftwareSerial mySerial(pBTtx,pBTrx); // RX, TX
Servo clutch;

long PercentPower = 0;

void setup() {
  
  pinMode(pReverse, OUTPUT);
  pinMode(9, OUTPUT); //speed
  
  pinMode(8, INPUT);

  pStop();
  
  SoftwareSerial mySerial(pBTrx,pBTtx); // RX, TX
Serial.begin(9600);
Serial.println("init ok");
  mySerial.begin(9600);
  mySerial.println("winch init ok");

  while(1)
  {
    if (analogRead(pUp)>500 || analogRead(pDown)>500 || analogRead(pLock)>500 || analogRead(pUnlock)>500)
    {
      if (analogRead(pLock)>500)
      {
        LockUnlock(true);
      }
      if (analogRead(pUnlock)>500)
      {
        LockUnlock(false);
      }
      if (analogRead(pUp)>500)
      {
        PercentPower = analogRead(pManSpeed)/100;
        DoBack();
      }
      if (analogRead(pDown)>500)
      {
        PercentPower = analogRead(pManSpeed)/100;
        DoForvard();
      }
    }
    else
    {
      byte ch = mySerial.read();
      if (ch >= 100 && ch <110)
      {
        PercentPower = ch - 99;
        Serial.println(ch);
        DoForvard();
      }
      if (ch >= 110 && ch <120)
      {        
        PercentPower = ch - 109;
        Serial.println(ch);
        DoBack();
      }
      if (ch == 43)  //+ Lock
      {
        LockUnlock(true);
      }
      if (ch == 45) //- Unlok
      {
        LockUnlock(false);
      }
    }
  }
}

// the loop function runs over and over again forever
void loop() {


}

void LockUnlock(bool ToLock)
{
        clutch.attach(pServo);
        if (ToLock) clutch.write(vLock); else clutch.write(vUnlock);
        pSpeed_High;
        if (ToLock) delay(1500); else delay(500);
        pStop();
        analogWrite(pReverse, 110);
        delay(500);
        pSpeed_High;
        delay(500);
        pStop();
        delay(500);
        clutch.detach();
}

void DoForvard()
{
  digitalWrite(pReverse, LOW);
  for (unsigned long i = 0; i < 70000; i++)
  {
      SetPower();
  }
  pStop();
}

void DoBack()
{
  analogWrite(pReverse, 110);
  delay(50); //время на срабатывание реле
  for (unsigned long i = 0; i < 60000; i++)
  {
      SetPower();
  }
  pStop();
}

void pStop()
{
  PercentPower = 0;
  MsTimer2::stop();
  pSpeed_low;
  digitalWrite(pReverse, LOW);
}

// Установка мощности мотора.
void SetPower()
{
 
  if (pSensZero) //Всё взводится только если сейчас 0!
  {
    if (PercentPower > 9){
      pSpeed_High;
    }
    else{
      MsTimer2::stop();
      pSpeed_low;
    
       unsigned long ms = 24 - (PercentPower * 24/10); //А 6мс потому, что датчик 0 не идеален и уже проходит некоторое время до того, как напряжение будет достаточно большое для загорания светодиода в оптоаре.
       
      MsTimer2::set(ms, 2, PowerControl);
      MsTimer2::start();
    }
  }
}

//Метод вызывается из перывания по таймеру и тупо активирует тиристорный ключ.
void PowerControl()
{
      pSpeed_High;
}
