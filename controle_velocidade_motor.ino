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
unsigned long tempo = 0, tempo_amostra = 1000;
float v = 0, v_ang = 0, v_ref = 0;
int PWM = 0;


void setup()
{
  pinMode(CLK, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), interrupcao, RISING);
  Serial.begin(9600);
}

void motor_control()
{
  v_ref = 5.5;

  PWM = round(4.9469*pow(v_ref, 3) - 69.8478*pow(v_ref, 2) + 324.9863*pow(v_ref, 1) - 440.7973);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, PWM);

  if((millis() - tempo) >= 1000)
  {
    tempo = millis();

    Serial.print(pulses);

    v_ang = (((float)(2*M_PI*pulses))/(reduction*(resolution/4.0)*(tempo_amostra/1000.0)));
    v = v_ang*raio;
    
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

void loop()
{
  motor_control();
}