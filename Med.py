import numpy as np
import time
from PyQt5.QtCore import QObject, pyqtSignal

class Mediciones(QObject):
    Clima_Signal = pyqtSignal()
    Resultado_Signal = pyqtSignal()

    def __init__(self):
        super().__init__()
        #Factibilidad
        self.Velocity = 20
        self.Altitud = 85
        self.Velocity_Asc = 6 
        self.Velocity_Des = 4
        self.T_Charge = 1500 #Tiempo que dura la bateria para descargarse (25min)
        self.Rutas = ("",
                      "Centro médico Regional del Este - Fundación Los Hospederos de Jesús",
                      "Hospital general Las Colinas - Centro Médico Sabaneta de Yásica",
                      "Hospital Municipal Villa Altagracia - Hospital Bonao")
        
        self.Disponibilidad = False
        self.Clima = True #clima
        self.Resultado = False
        self.Fact = False
        self.BatteryPercentage = 0

        Ts = (0,75,150,225,300,375,450,525,600,675,750,825,900,975,1050,1125,1200,1275,1350,1425,1500)
        BatteryPercentage = (100,90,82,77,75,74.4,73.8,73,72.4,72,71.4,70.8,70.4,70,66,60,56,50,40,32,19.6)
        VoltageList = (5,4.5,4.1,3.85,3.75,3.72,3.69,3.65,3.62,3.6,3.57,3.54,3.52,3.5,3.3,3,2.8,2.5,2,1.6,0.98)

        self.np_array = np.column_stack((Ts, BatteryPercentage, VoltageList))

        self.Datos_Humedad = np.zeros([1,2], dtype=float)
        self.Datos_Temperatura = np.zeros([1,2], dtype=float)
        self.Datos_Viento = np.zeros([1,2], dtype=float)

    def Factibilidad(self,i,j):
        self.Distancia_total = (0,40182.6, 44571.4, 42821.5)

        Distancia_Puntos = {1:(10361.49, 21714.58, 22347.32, 28022.03, 34393.46, 40182.6),
                            2:(11639.1, 23731.96, 24449.62, 35670.27, 44571.4),
                            3:(10151.61, 18132.21, 19053.29, 25767.08, 31011.84, 36818.03, 42821.5)}

        T_asc = self.Altitud / self.Velocity_Asc
        T_des = self.Altitud / self.Velocity_Des
        T_WP = (Distancia_Puntos[i][2]/self.Velocity) + T_asc
        T_Vuelo = self.Distancia_total[i] / self.Velocity
        self.T_total = T_Vuelo + T_asc + T_des

        self.T_Recorrido = (Distancia_Puntos[i][j]/self.Velocity) + T_asc 
        self.T_restante = self.T_total - self.T_Recorrido

        self.DroneBatteryPercentage = round(np.interp(self.T_Recorrido,self.np_array[:,0],self.np_array[:,1]))
        
        if self.T_Recorrido > T_WP:
            self.T_Recorrido = ((Distancia_Puntos[i][j] - Distancia_Puntos[i][2])/self.Velocity) + T_asc
            self.T_restante = self.T_total - T_WP - self.T_Recorrido

        if j == 0:
            self.T_restante = 0

        self.T_disponible = abs(self.T_Charge - self.T_Recorrido)

        if self.T_restante > self.T_disponible:
            self.Fact = True
        else:
            self.Fact = False
    
    def EstacionClimatica(self):
        if (len(self.Datos_Humedad) != 1) and (len(self.Datos_Temperatura) !=1) and (len(self.Datos_Viento) != 1):
            Humeda              =   self.Datos_Humedad[-1][1]
            Temperatura         =   self.Datos_Temperatura[-1][1]
            Velocidad_Viento    =   self.Datos_Viento[-1][1]

            if Velocidad_Viento == 0:
                self.Clima = True
            
            elif (Velocidad_Viento >= 1) and (Velocidad_Viento <= 33):
                self.Clima = True
            
            elif (Velocidad_Viento >= 34) and (Velocidad_Viento <= 39):
                self.Clima = False
        
        self.Clima_Signal.emit()


    def Aceptabilidad(self):
        if self.Fact and self.Disponibilidad and self.Clima:
            self.Resultado = True
        else:
            self.Resultado = False
        
        self.Resultado_Signal.emit()

        #print(f"Clima resultado = {self.Clima}")
        #print(f"Fact resultado = {self.Fact}")
        #print(f"Disponibilidad resultado = {self.Disponibilidad}")
        #print(f"Aceptabilidad = {self.Resultado}")