//Bibliotecas:
#include <Arduino.h>
#include <math.h>
#include <string.h>

//Definição dos pinos:
#define encoder_A   2
#define encoder_B   3
#define IN1         5
#define IN2         6
#define ENA         9

//Resolução do encoder:
#define resolution  12

//Razão de redução:
#define reduction   298

//Raio da roda (em metros):
#define raio        0.03

//Definição de variáveis:
bool active = false, direction_motor;
int value = 0, val;
long pulses = 0;
unsigned long tempo_0 = 0, tempo_amostra = 1000;
float v = 0, v_ang = 0;


void count_pulses()
{
  pulses++;
} //end count_pulses

void update_velocity()
{
  v_ang = (((2*M_PI*pulses)/(reduction*(resolution/2)*tempo_amostra)));
}

//Inicializações:
void setup() 
{
  Serial.begin(115200);

  pinMode(encoder_A, INPUT);
  pinMode(encoder_B, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_A), count_pulses, CHANGE);
}

void loop() 
{
  update_velocity();  
  if(active)
  {
    //Retorna a velocidade atual:
    Serial.println(v_ang);
  }

  //Obtenção do comando de controle:
  if(Serial.available())
  {
    value = atoi(Serial.readString().c_str());
    Serial.flush();
    if(value == 48)
    {
      active = !active;
      value = 0;
    }else
    {
      value = round((value-150)*2.55);
    }    
  }  

  //Envio do sinal para o motor:
  if(value > 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(value));
  }
  if(value < 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, abs(value));
  }
  if(value == 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 255);
    v_ang = 0;
  }
}