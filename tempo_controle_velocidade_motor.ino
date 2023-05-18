#include <math.h>
//#include <time.h>


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
unsigned long tempo = 0, tempo_ensaio = 1000, t, t_0 = 0, t_1 = 0;
float v = 0, v_ang = 0, v_ref = 0;
int pwm = 0, i, j;


void setup()
{
  pinMode(CLK, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), interrupcao, RISING);
  Serial.begin(9600);
}

void loop()
{
  v_ref = 5.5;

  pwm = round(4.9469*pow(v_ref, 3) - 69.8478*pow(v_ref, 2) + 324.9863*pow(v_ref, 1) - 440.7973);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);

  if((millis() - tempo) >= tempo_ensaio)
  {
    tempo = millis();

    t_0 = micros();
    for(i = 0; i < 1000; i++)
    {
      v_ang = (((float)(2*M_PI*pulses))/(reduction*(resolution/4.0)*(tempo_ensaio/1000.0)));
      v = v_ang*raio;
    }
    t = ((micros() - t_0)*1e-3);

    Serial.print("Tempo gasto pela tarefa: ");
    Serial.print(t); 
    Serial.print(" milissegundos.");
    
    Serial.print(" | ");
    Serial.print("Velocidade angular (rad/s): ");
    Serial.print(v_ang);
   
    Serial.print(" | ");
    Serial.print("Velocidade linear (mm/s): ");
    Serial.println(v);
    pulses = 0;
  }
}

void interrupcao()
{
  pulses++;
}

