#PI:

#Bibliotecas
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sp
import control as ct
import csv


t_amostra = 0.7

#Convertendo .csv em array numpy
csv_filename = 'curva_velocidade.csv'
with open(csv_filename) as f:
    reader = csv.reader(f)
    list = list(reader)

s = ct.tf('s')

#Normalizando os dados
vel_ = []
for i in range(len(list)):
    vel_.append(float(list[i][0]))

#Vetor de tempo
tf = 3500
t = np.linspace(0, tf, len(vel_))

#Filtro dos dados
vel = sp.savgol_filter(vel_, 101, 2)

#Retas para a obtenção dos parâmetros de Ziegler-Nichols
#Reta tangente:
y1 = 0.01*t + 4.11
#Reta 0,63K:
y2 = ((np.min(vel)+(0.63*(np.max(vel) - np.min(vel))))*np.ones(t.shape))

A = 18.25967622
B = 240.4162

#Plot Parâmetros PI

plt.plot(t, vel, color='red', label = '$\omega}$(t)')
plt.plot(t, y1, color='green', label= 'Tangente') #Reta tangente ao pnto de maior inclinação
plt.axhline(vel[0], color='gray', linestyle='--') #Reta horizontal do valor inicial do gráfico
plt.axhline(vel[-1], color='gray', linestyle='--') #Reta horizontal do valor final do gráfico
#plt.axvline(vel[0], color='gray', linestyle='--') #Reta vertical no ponto início do gráfico
plt.axvline(B, color='purple', linestyle='--') #Reta vertical no ponto início do gráfico
plt.scatter(A, vel[0], color='orange', label='A') #Marcação do ponto A
plt.scatter(B, vel[0], color='purple', label='B') #Marcação do ponto B
plt.xlabel('Tempo [s]')
plt.ylabel('$\omega}$ [rad/s]')
plt.xlim(-5, 300)
plt.ylim(4.2, 6.7)
plt.legend()
plt.title('Parâmetros do Controlador PI')
plt.grid()
plt.show()

#Parâmetros para o controlador
theta = (((A - vel[0])*t_amostra)/tf)
tau = (((B - A)*t_amostra)/tf)

print("\nTheta:", theta, "\t\tTau:", tau)

u_eq = 55
u_step = 125-55
K = (vel[-1]-vel[0])/(u_eq-u_step)


#Cria o sistema de primeira ordem
num = [K]
den = [tau, 1]
G = ct.TransferFunction(num, den)

#Introduz o atraso no sistema
num_atraso, den_atraso = ct.pade(theta, 1)
G_atraso = ct.TransferFunction(num_atraso, den_atraso)*G

print("\n\nG: ", G_atraso)

#Constantes via parâmetros de Ziegler-Nichols - Tabelas 8.6 e 8.7 (Garcia, 2017):
Kc_pzn = (tau)/(K*theta)
print(f'\nKc (Ziegler-Nichols) P: \t {round(abs(Kc_pzn), 4)}')

#Constantes via parâmetros de Ziegler-Nichols - Tabelas 8.6 e 8.7 (Garcia, 2017):
Kc_pizn = ((0.9*tau)/(K*theta))
Ti_pizn = ((10*theta)/3)
print(f'\nKc (Ziegler-Nichols) PI: \t {round(abs(Kc_pizn), 4)}')
print(f'\nTi (Ziegler-Nichols) PI: \t {round(Ti_pizn, 4)}')


#Controlador PI Ziegler-Nichols:
Gpizn = (abs(Kc_pizn) + ((1/(Ti_pizn))*(1/s)))
print("\n\nGpizn: \t\t", Gpizn)

