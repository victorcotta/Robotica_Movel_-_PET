import serial
import time
import matplotlib.pyplot as plt
import numpy as np

#Parâmetros do motor:
#max_voltage = 6 #tensão máxima, em volts
max_rpm = 100 #rotação máxima, em RPM
reduction_ratio = 1/298 #razão de redução

#Identificação da porta COM:
def check_port():
    for i in range(10): 
        try:
            return serial.Serial(('COM'+str(i)), 115200) 
        except serial.SerialException:
            pass
port = check_port()

#Função para controle do sistema:
def velocity_analysis(duration, prec, u):
    tu = np.arange(0, duration, (10**(-prec)))
    ve = []
    te = []
    try:
        port.open()
    except:
        pass
    time.sleep(2)
    port.write(str(48).encode('utf-8'))
    last_pwm = 48
    t0 = time.time()
    ct = time.time()
    while(True):
        data = port.readline().decode('utf-8')
        if((ct-t0) > 3):
            try:
                ve.append(float(data))
                te.append(ct-t0-3)
            except:
                pass
            try:
                pwm = (int(u[np.where(tu==round((ct-t0-3), prec))][0]*(100/255))+150)   
            except:
                pass             
            if(pwm != last_pwm):
                last_pwm = pwm
                port.write(str(pwm).encode('utf-8'))      
        ct = time.time()
        if((ct-t0) >= (duration+3)):
            port.write(str(48).encode('utf-8'))
            port.close()            
            return tu, te, ve

#Função para plotar resultados:
def plot(tc, vc, te, ve, tu, u):
    plt.figure(1)
    plt.subplot(2, 1, 1)
    #plt.rcParams['xtick.labelsize'] = 20
    #plt.rcParams['ytick.labelsize'] = 20
    plt.plot(tc, vc, 'b', label='$Calculado$')#, linewidth=3)
    plt.plot(te, ve, 'r', label='$Encoder$')#, linewidth=3)
    plt.ylabel('$\omega}$ [rad/s]')#, fontsize=28)
    plt.legend()#fontsize=18)
    plt.title('Comparação das curvas de velocidade e sinal de controle', fontsize=20)
    plt.ylim(-(((max_rpm*2*np.pi)/60)+0), (((max_rpm*2*np.pi)/60)+0))
    plt.grid()
    plt.subplot(2, 1, 2)
    #plt.rcParams['xtick.labelsize'] = 20
    #plt.rcParams['ytick.labelsize'] = 20
    plt.plot(tu, u, 'g', label='$PWM$')#, linewidth=3)
    plt.ylabel('$PWM$')#, fontsize=28)
    #plt.legend(fontsize=18)
    plt.xlabel('Tempo [s]')#, fontsize=28)
    plt.ylim(-270, 270)
    plt.grid()
    plt.show()


#################################################################################################################################

#Definição do sinal de controle:
u1 = 46*np.ones(500)
u2 = 75*np.ones(500)
u3 = 105*np.ones(500)
u4 = 135*np.ones(500)
u5 = 165*np.ones(500)
u6 = 195*np.ones(500)
u7 = 225*np.ones(500)
u8 = 255*np.ones(500)

u = np.hstack((u1, u2, u3, u4, u5, u6, u7, u8))

#Controle e análise do sistema:
tu, te, ve = velocity_analysis(40, 2, u)

#Cálculos e definições:
tc = tu
vc = ((max_rpm*2*np.pi/60)*((u+1e-15)/255))

#Plotagem das respostas temporais:
plot(tc, vc, te, ve, tu, u)

#Velocidade média medida:
vm = np.mean(ve[120:])
print(f'\nVm: \t {round(vm, 4)} rad/s')