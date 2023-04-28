//Bibliotecas:
#include <Arduino.h>
#include <math.h>
#include <string.h>

//Definição dos pinos:
#define CLK              2
#define SDA              3
#define IN1              5
#define IN2              6
#define ENA              9

//Resolução do encoder:
#define resolution       12

//Razão de redução:
#define reduction_ratio  1/298

//Raio da roda (em milímetros):
#define raio             30

//Definição de variáveis:
bool active = false;
int value = 0;
long pulses = 0, last_pulses = 0;
unsigned long tempo_0 = 1000, tempo_ensaio = 1000, pined_time = 0, i = 0, t_ref = 0, ts = 50;
float v = 0, v_ang = 0, pos[2], time[2];


//Inicializações:
void setup() 
{
  Serial.begin(115200);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(CLK), count_pulses, CHANGE);
}

void count_pulses()
{
  if(digitalRead(SDA))
  {
    pulses--;
  }
  else
  {
    pulses++;
  }  
}

void update_velocity()
{
  switch(i%5)
  {
    case 0:
      last_pulses = pulses;
      i++;
      t_ref = millis();
    break;
    case 1:
      if((millis() - t_ref) >= ts)
      {
        i++;
      }
    break;
    case 2:
      if(pulses != last_pulses)
      {
        pos[0] = ((float)pulses*2*M_PI*reduction_ratio/(resolution/2.0));
        time[0] = ((float)millis()/1e3);
        last_pulses = pulses;
        i++;
        t_ref = millis();
      }      
    break;
    case 3:
      if((millis() - t_ref) >= ts)
      {
        i++;
      }
    break;
    case 4:
      if(pulses != last_pulses)
      {
        pos[1] = ((float)pulses*2*M_PI*reduction_ratio/(resolution/2.0));
        time[1] = ((float)millis()/1e3);
        v_ang = ((float)(pos[1]-pos[0])/(time[1]-time[0]));
        v = v_ang*raio;
        i++;
      }
    break;
  }  
}

void loop() 
{
  update_velocity();  
  if(active)
  {
    //Retorna a velocidade atual:
    Serial.println(v);
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
    v = 0;
  }
}
