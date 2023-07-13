#include <math.h>

#define    CLK              2                     //Conexão A do encoder
#define    SDA              3                     //Conexão B do encoder
#define    ENA              9                     //Saída PWM para controle de velocidade do motor
#define    IN1              5                     //Controle IN1 do driver L298N
#define    IN2              6                     //Controle IN2 do driver L298N

//#define    max_rpm          100                   //Rotação máxima, em RPM
#define    resolution       12                    //Resolução do encoder
#define    reduction        298                   //Redução
#define    raio             30                    //Raio da roda em milímetros


volatile unsigned long pulses = 0;
unsigned long tempo = 0, tempo_amostra = 1000, t0 = 0;
float v = 0, v_ang = 0, v_ref;
int pwm = 0;


void setup()
{
  pinMode(CLK, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), interrupcao, RISING);
  Serial.begin(2000000);
  v_ref = 0;
}

void loop()
{
  if(Serial.available())
  {
    String reading = Serial.readString();
    v_ref = reading.toFloat();

    if(((fabs(v_ref) < 2.8) || (fabs(v_ref) > 8.12)) && (v_ref != 0))
    {
      v_ref = 0;
      Serial.println("\n Valor inválido!");
    }
    if(v_ref > 0)
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }else
    {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    if(v_ref)
    {
      pwm = round(4.9469*pow(fabs(v_ref), 3) - 69.8478*pow(fabs(v_ref), 2) + 324.9863*pow(fabs(v_ref), 1) - 440.7973);
    }
    else
    {
      pwm = 0;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
    analogWrite(ENA, pwm);

    if(v_ref)
    {
      t0 = micros();
      do
      {
        if((micros() - tempo) >= tempo_amostra)
        {
          v_ang = (((float)(2*M_PI*pulses))/(reduction*(resolution/4.0)*((float)tempo_amostra/100000.0)));
          v = v_ang*raio;
          
          Serial.print("Velocidade angular (rad/s): ");
          Serial.print(v_ang);
        
          Serial.print(" | ");
          Serial.print("Velocidade linear (mm/s): ");
          Serial.println(v);
          pulses = 0;

          tempo = micros();
        }
      }while((micros() - t0) < (0.2*1e6)); //duração da amostragem
    }
  }
  v_ref = 0;
  analogWrite(ENA, 0);
}

void interrupcao()
{
  pulses++;
}


void interrupcao()
{
  pulses++;
}
