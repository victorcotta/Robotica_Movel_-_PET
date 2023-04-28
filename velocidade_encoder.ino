// NOME: Victor Sidnei Cotta


// ============================ DECLARAÇÃO DE BIBLIOTECAS ==================================================

//#include <RotaryEncoder.h>
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


int valor_pwm;
float v = 0, v_ang = 0, pos[2], time[2];
long pulses = 0, last_pulses = 0;
unsigned long tempo_0 = 1000, tempo_ensaio = 10000, pined_time = 0, i = 0, t_ref = 0, ts = 50;


void setup() 
{
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(CLK), count_pulses, CHANGE);
  Serial.begin(115200);
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
        pos[0] = ((float)pulses*2*M_PI/((resolution/2.0)*reduction));
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
        pos[1] = ((float)pulses*2*M_PI/((resolution/2.0)*reduction));
        time[1] = ((float)millis()/1e3);
        v_ang = ((float)(pos[1]-pos[0])/(time[1]-time[0]));
        v = v_ang*raio;
        i++;
      }
    break;
  }
}

void motor_control() 
{
  if((millis() - tempo_0) <= tempo_ensaio)
  {
    valor_pwm = 255;

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, valor_pwm);

    Serial.print("Pulsos: ");
    Serial.print(pulses);

    update_velocity();

    Serial.print(" | ");
    Serial.print("Velocidade angular (rad/s): ");
    Serial.print(v_ang);
   
    Serial.print(" | ");
    Serial.print("Velocidade linear (mm/s): ");
    Serial.println(v);

    Serial.print("\n");
  }else
  {
    valor_pwm = 0;
  
    analogWrite(ENA, valor_pwm);
  }
}
  
void loop()
{
  motor_control();
}

