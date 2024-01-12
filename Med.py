import numpy as np

class Mediciones():
    def __init__(self):
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
        
        self.Disponibilidad = True

        Ts = (0,75,150,225,300,375,450,525,600,675,750,825,900,975,1050,1125,1200,1275,1350,1425,1500)
        BatteryPercentage = (100,90,82,77,75,74.4,73.8,73,72.4,72,71.4,70.8,70.4,70,66,60,56,50,40,32,19.6)
        VoltageList = (5,4.5,4.1,3.85,3.75,3.72,3.69,3.65,3.62,3.6,3.57,3.54,3.52,3.5,3.3,3,2.8,2.5,2,1.6,0.98)

        self.np_array = np.column_stack((Ts, BatteryPercentage, VoltageList))

        self.DatosClima = np.empty([1, 3], dtype=float)

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

        a = round(np.interp(self.T_Recorrido,self.np_array[:,0],self.np_array[:,1]))
        print(a)

        
        if self.T_Recorrido > T_WP:
            self.T_Recorrido = ((Distancia_Puntos[i][j] - Distancia_Puntos[i][2])/self.Velocity) + T_asc
            self.T_restante = self.T_total - T_WP - self.T_Recorrido

        if j == 0:
            self.T_restante = 0

        self.T_disponible = abs(self.T_Charge - self.T_Recorrido)
    
    def EstacionClimatica(self,Temperatura,Humedad,Velocidad_Viento):
        if (Velocidad_Viento >= 21) and (Velocidad_Viento <= 34):
            #Ambiente - Slow
            if ((Temperatura >= 33.1) and (Temperatura <= 48.6)) and ((Humedad >= 64.9) and (Humedad <= 72)):
                self.Disponibilidad = True
            #Caliente 1 - Slow
            elif ((Temperatura >= 37.8) and (Temperatura <= 35.3)) and ((Humedad >= 31.5) and (Humedad <= 61.3)):
                self.Disponibilidad = False

        elif (Velocidad_Viento >= 32) and (Velocidad_Viento <= 39):
            #Ambiente - Fast
            if ((Temperatura >= 34.5) and (Temperatura <= 36.7)) and ((Humedad >= 60.40) and (Humedad <= 66)):
                self.Disponibilidad = True
            #Caliente 1 - Fast
            elif ((Temperatura >= 39.9) and (Temperatura <= 60)) and ((Humedad >= 19.6) and (Humedad <= 48.2)):
                self.Disponibilidad = False