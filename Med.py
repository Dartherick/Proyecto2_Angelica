class Mediciones():
    def __init__(self):
        #Factibilidad
        self.Velocity = 20
        self.Altitud = 85
        self.Velocity_Asc = 6 
        self.Velocity_Des = 4
        self.T_Charge = 1500
        self.Rutas = ("",
                      "Centro médico Regional del Este - Fundación Los Hospederos de Jesús",
                      "Hospital general Las Colinas - Centro Médico Sabaneta de Yásica",
                      "Hospital Municipal Villa Altagracia - Hospital Bonao")

    def Factibilidad(self):
        Distancia_Recorrida = HMI.DistanciaRecorrida.value() 
        i = HMI.Rutas_ComboBox.currentIndex() 
        print(i)

        self.Distancia_total = (0,40182.6, 44571.4, 42821.5)

        T_asc = self.Altitud / self.Velocity_Asc
        T_des = self.Altitud / self.Velocity_Des
        T_Vuelo = self.Distancia_total[i] / self.Velocity
        T_total = T_Vuelo + T_asc + T_des

        #print(T_total)

        T_Recorrido = Distancia_Recorrida/self.Velocity + T_asc 
        #print(T_Recorrido)

        T_restante = T_total - T_Recorrido
        #print(T_restante)

        T_disponible = self.T_Charge - T_Recorrido
        #print(T_disponible)

        if T_restante < T_disponible:
            print('no factible')
            HMI.FactibilidadLabel.setStyleSheet("background-color: red ; border: 1px solid black")
            HMI.FactibilidadLabel.setText("No factible!")
        else:
            print("es factible")    
            HMI.FactibilidadLabel.setStyleSheet("background-color: green; border: 1px solid black")
            HMI.FactibilidadLabel.setText("Factible!")


        HMI.TimeRest.setText(str(round(T_restante,2))+"s")
        HMI.TimeTotal.setText(str(round(T_total,2))+"s")
