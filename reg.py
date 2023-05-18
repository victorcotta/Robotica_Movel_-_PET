import numpy as np
import matplotlib.pyplot as plt

pwm = np.array([35, 45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215, 225, 235, 245, 255])
v_ang = np.array([2.8, 4.03, 5.05, 5.69, 6.11, 6.47, 6.73, 6.88, 7.12, 7.24, 7.37, 7.46, 7.54, 7.64, 7.75, 7.84, 7.88, 7.92, 7.96, 7.98, 8.03, 8.07, 8.12])

p = np.polyfit(v_ang, pwm, 3)
res = np.polyval(p, v_ang)

x = np.linspace(0, 255, 1000)

plt.plot(v_ang, pwm, 'r')
plt.plot(v_ang, res, 'g')

plt.xlabel('Velocidade Angular ($\omega}$) [rad/s]', fontsize = 11)
plt.ylabel('Sinal PWM [$2^8$]', fontsize = 11)

plt.legend()
plt.show()
