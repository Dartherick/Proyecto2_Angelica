import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *

from PyQt5.QtWidgets import QApplication

from HMI import MainHMI
from SerialCom import SerialPortConnection
from Med import Mediciones

from Camera import Camera
import threading
from time import sleep,time
import numpy as np

def StartFunction(Status,ProgressBar,Start,End,Time):
    HMI.ProgressBar_Progression(ProgressBar,Start,End,Time)
    HMI.EnableTabs(Status)
    HMI.Wind_Label.setText(f"Viento: 0RPM")
    HMI.Temperature_Label.setText(f"22.3°C")
    HMI.Humidity_Label.setText(f"Humedad: 66%")
    HMI.ClimaLabel_Status.setStyleSheet("background-color: green; border: 1px solid black")

def Clima():
    if Fact.Clima:
        HMI.ClimaLabel.Status.setStyleSheet("background-color: green; border: 1px solid black")
    else:
        HMI.ClimaLabel.Status.setStyleSheet("background-color: red ; border: 1px solid black")

def Disponibilidad():
    if Fact.Disponibilidad:
        HMI.DisponibilidadLabel.Status.setStyleSheet("background-color: green; border: 1px solid black")
    else:
        HMI.DisponibilidadLabel.Status.setStyleSheet("background-color: red ; border: 1px solid black")

def Resultado():
    if Fact.Resultado:
        HMI.ResultadoLabel.Status.setStyleSheet("background-color: green; border: 1px solid black")
    else:
        HMI.ResultadoLabel.Status.setStyleSheet("background-color: red ; border: 1px solid black")

def Factibilidad():
    Ruta = HMI.Rutas_ComboBox.currentIndex() 
    Punto = HMI.Puntos_ComboBox.currentIndex() 

    Fact.Factibilidad(Ruta,Punto)

    if Fact.Fact:
        HMI.FactibilidadLabel.setStyleSheet("background-color: green; border: 1px solid black")
        HMI.FactibilidadLabel.setText("Factible!")
        HMI.FactibilidadLabel_Status.setStyleSheet("background-color: green; border: 1px solid black")
    else:  
        HMI.FactibilidadLabel.setStyleSheet("background-color: red ; border: 1px solid black")
        HMI.FactibilidadLabel.setText("No factible!")
        HMI.FactibilidadLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")

    HMI.TimeRest.setText(str(round(Fact.T_restante,2))+"s")
    HMI.TimeTotal.setText(str(round(Fact.T_total,2))+"s")

# Define a function to run when the UI exits
def on_ui_exit():
    Arducam.stop()
    print("Closing camera port")
    serial_connection.close_port()
    print("Closing Serial Port")
    print(Fact.Disponibilidad)
    exit_flag.set()

def MessageFunc():
    while not exit_flag.is_set():
        Function,Message = serial_connection.ReceiveMessage()
        print(F"{Function} {Message}")
        if Function is not None or Message is not None:
            if Function == "00" and Message == "1": #Dron detectado
                print(Fact.Aceptabilidad)
                Fact.Aceptabilidad()
                if Fact.Resultado:
                    serial_connection.SendMessage("20001")
                    HMI.ResultadoLabel_Status.setStyleSheet("background-color: green; border: 1px solid black")
                else:
                    print("waiting aceptabilidad")

            elif Function == "30" and Message == "001": #Disponibilidad 
                HMI.DisponibilidadLabel_Status.setStyleSheet("background-color: green; border: 1px solid black")
                Fact.Disponibilidad = True
                Fact.Aceptabilidad()
            
            elif Function == "30" and Message == "002": #Dron en la base giratoria
                Arducam.StartCam = True
                print("----Camara encendida----")
                    
            elif Function == "40" and Message == "001": #Arduino recibio correctamente el mensaje 41001
                Arducam.StartCam = False
                print("----Camara apagada----")

            elif Function == "40" and Message == "002": #Dron en la posicion de cambio de bateria
                    serial_connection.SendMessage("50003")

            elif Function == "80": #Temperatura
                try:
                    Fact.Datos_Temperatura = np.concatenate([Fact.Datos_Temperatura, [[round(time() - InitialTime,2), float(Message)]]])
                    if HMI.TabWidget.currentIndex() == 0:
                        HMI.Temperature_Label.setText(f"{Message}°C")
                except:
                    print("error con data serial")

                Fact.EstacionClimatica()
                Fact.Aceptabilidad()
            
            elif Function == "90": #Humedad
                try:
                    Fact.Datos_Humedad = np.concatenate([Fact.Datos_Humedad, [[round(time() - InitialTime,2), float(Message)]]])

                    if HMI.TabWidget.currentIndex() == 0:
                        #HMI.Humidity_Label.setText(f"Humedad: {Message}")
                        pass
                except:
                    print("error con data serial")
                
                Fact.EstacionClimatica()
                Fact.Aceptabilidad()
            
            elif Function == "10": #Velocidad viento
                '''
                0-5.5V = 0-6000RPM
                1 Km/h = 63RPM
                '''
                try:
                    MessageViento = round((6000*float(Message)/5.5)/63 ,2) #Converting Volts to Km/h
                    Fact.Datos_Viento = np.concatenate([Fact.Datos_Viento, [[round(time() - InitialTime,2), MessageViento]]])

                    if HMI.TabWidget.currentIndex() == 0:
                        #HMI.Wind_Label.setText(f"Viento: {Message}RPM")
                        pass
                except:
                    print("error con data serial")

                Fact.EstacionClimatica()
                Fact.Aceptabilidad()

            elif Function == "00" and Message == "2":
                if Fact.Fact:
                    try:
                        HMI.ChangeBatteryImage(1,Fact.DroneBatteryPercentage)
                    except:
                        HMI.ChangeBatteryImage(1,0)
                else:
                    HMI.ChangeBatteryImage(1,0)


            elif Function == "00" and Message == "3":
                if Fact.Fact:
                    HMI.ChangeBatteryImage(3,0)

            elif Function == "00" and Message == "5":
                Fact.Clima = False
                HMI.Humidity_Label.setText(f"Humedad: 75%")
                HMI.Wind_Label.setText(f"Viento: 26Km/h")
                HMI.Temperature_Label.setText(f"27.8°C")
                HMI.ClimaLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")
                HMI.ResultadoLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")

            elif Function == "00" and Message == "6":
                sleep(2)
                Fact.Clima = True
                HMI.Humidity_Label.setText(f"Humedad: 75%")
                HMI.Wind_Label.setText(f"Viento: 0Km/h")
                HMI.ClimaLabel_Status.setStyleSheet("background-color: green ; border: 1px solid black")
                HMI.ResultadoLabel_Status.setStyleSheet("background-color: green ; border: 1px solid black")
                HMI.Temperature_Label.setText(f"26°C")
                sleep(1)
                HMI.Temperature_Label.setText(f"24.3°C")
                sleep(2)
                HMI.Temperature_Label.setText(f"23°C")

        sleep(0.5)

def SendMessageTriangle():
    if Arducam.StartCam:
        serial_connection.SendMessage("41001")

def AddBatteryImage():
    HMI.ChangeBatteryImage(1,Fact.DroneBatteryPercentage)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    HMI = MainHMI("HMI.ui")
    Fact = Mediciones()

    message_thread = threading.Thread(target=MessageFunc)
    exit_flag = threading.Event()
    InitialTime = time()

    #camara
    Arducam = Camera(0)
    Arducam.frameReady.connect(HMI.updateFrame)
    Arducam.detect_triangle.connect(SendMessageTriangle)
    Arducam.start()

    #Labels home page
    HMI.DisponibilidadLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")
    HMI.ResultadoLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")
    HMI.ClimaLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")
    HMI.FactibilidadLabel_Status.setStyleSheet("background-color: red ; border: 1px solid black")

    # Factibilidad
    HMI.Rutas_ComboBox.addItems(Fact.Rutas)
    HMI.EnterButton.clicked.connect(Factibilidad)

    HMI.VelocityText.setText(str(Fact.Velocity)+"m/s")
    HMI.AltitudeText.setText(str(Fact.Altitud)+"m")
    HMI.VelocityAsc_Text.setText(str(Fact.Velocity_Asc)+"m/s")
    HMI.VelocityDesc_Text.setText(str(Fact.Velocity_Des)+"m/s")

    # Create an instance of the SerialPortConnection class
    serial_connection = SerialPortConnection("", 9600)

    HMI.SerialPort_ComboBox.addItems(sorted(serial_connection.Refresh_Ports()))
    HMI.OpenButton.clicked.connect(lambda : serial_connection.open_port(HMI.SerialPort_ComboBox.currentText(),HMI.BaudRate_ComboBox.currentText()))
    HMI.CloseButton.clicked.connect(serial_connection.close_port)
    HMI.RefreshButton.clicked.connect(serial_connection.Refresh_Ports)

    serial_connection.OpenStatus.connect(lambda: (StartFunction(True, HMI.SerialProgressBar, 0, 100, 5), message_thread.start()))
    serial_connection.DisableStatus.connect(lambda : (StartFunction(False,HMI.SerialProgressBar,100,0,5), exit_flag.set()))
    
    #no funciona
    #serial_connection.UpdatedPortList.connect(lambda : HMI.Refresh_Serial_Ports(sorted(serial_connection.Refresh_Ports())))
    #serial_connection.UpdatedPortList.connect(lambda : sorted(serial_connection.Refresh_Ports()))

    # Connect the aboutToQuit signal to the on_ui_exit function
    app.aboutToQuit.connect(on_ui_exit)

    HMI.show()
    app.exec_()