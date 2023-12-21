#include <math.h>
#include <TimerOne.h>

#define CLK 2
#define SDA 3
#define ENA 9
#define IN1 5
#define IN2 6

#define resolution 12
#define reduction 150
#define raio 30
#define ts 700 //em microssegundos
#define Kp 107.3954
#define Ki 1/96.6558


volatile double vel = 0;
const float vel_ref = 3;
volatile double erro = 0;
volatile double integral = 0;
volatile double u = 0;
volatile unsigned long pulsos = 0, t0 = 0, tempo = 0;

void contagem()
{
  unsigned long tat = micros();
  pulsos++;
  if((tat-t0)> ts)
  {
    /*Serial.print("Pulsos: ");
    Serial.println(pulsos, (tat-t0));
    Serial.print("Delta Tempo: ");
    Serial.println((tat-t0));*/
    vel = ((double)(0.00349066*pulsos)/((tat-t0)*1e-6));
    pulsos=0;
    t0 = micros();
  }  
}

void setup() 
{
  Serial.begin(115200);

  pinMode(CLK, 2);
  pinMode(SDA, 3);
  pinMode(ENA, 9);
  pinMode(IN1, 5);
  pinMode(IN2, 6);

  attachInterrupt(digitalPinToInterrupt(CLK), contagem, CHANGE);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void sinalDeControle(){
  erro = vel_ref - vel;
  integral = integral + erro*ts;
  u = Kp*erro + Ki*integral;

  if(u>255){
    u=255;
  }else if(u<0){
    u=0;
  }

  analogWrite(ENA, u);
}

void loop()
{
  Timer1.initialize(1000); //1ms
  Timer1.attachInterrupt(sinalDeControle);

  Serial.println(vel);
  /*while(1)
  {
    Serial.println(vel);
    if((millis()-start)>1500)
    {
      break;
    }
  }

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);*/
}
