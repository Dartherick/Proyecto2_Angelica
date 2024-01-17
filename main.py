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

def Factibilidad():
    Ruta = HMI.Rutas_ComboBox.currentIndex() 
    Punto = HMI.Puntos_ComboBox.currentIndex() 

    Fact.Factibilidad(Ruta,Punto)

    if Fact.Fact:
        HMI.FactibilidadLabel.setStyleSheet("background-color: green; border: 1px solid black")
        HMI.FactibilidadLabel.setText("Factible!")
    else:  
        HMI.FactibilidadLabel.setStyleSheet("background-color: red ; border: 1px solid black")
        HMI.FactibilidadLabel.setText("No factible!")

    HMI.TimeRest.setText(str(round(Fact.T_restante,2))+"s")
    HMI.TimeTotal.setText(str(round(Fact.T_total,2))+"s")

# Define a function to run when the UI exits
def on_ui_exit():
    Arducam.stop()
    print("Closing camera port")
    serial_connection.close_port()
    print("Closing Serial Port")
    exit_flag.set()

def MessageFunc():
    while not exit_flag.is_set():
        Function,Message = serial_connection.ReceiveMessage()
        print(F"{Function} {Message}")
        if Function is not None or Message is not None:
            if Function == "00" and Message == "1": #Dron detectado
                sleep(0.2)
                serial_connection.SendMessage("20001")

            elif Function == "30" and Message == "001": #Disponibilidad 
                Fact.Disponibilidad = True
            
            elif Function == "30" and Message == "002": #Dron en la base giratoria
                Arducam.StartCam = True
                print("-----------------------------------")
                    
            elif Function == "40" and Message == "001": #Arduino recibio correctamente el mensaje 41001
                Arducam.StartCam = False
                print("===================================")

            elif Function == "40" and Message == "002": #Dron en la posicion de cambio de bateria
                for i in range(10):
                    serial_connection.SendMessage("50003")
                    sleep(1)

            elif Function == "80": #Temperatura
                Fact.Datos_Temperatura = np.concatenate([Fact.Datos_Temperatura, [[time.time() - InitialTime, float(Message)]]])
                if HMI.TabWidget.currentIndex() == 0:
                    HMI.Temperature_Label.setText(f"Temperatura {Message}C")
            
            elif Function == "90": #Humedad
                Fact.Datos_Humedad = np.concatenate([Fact.Datos_Humedad, [[time.time() - InitialTime, float(Message)]]])
                if HMI.TabWidget.currentIndex() == 0:
                    HMI.Humidity_Label.setText(f"Humedad {Message}")
            
            elif Function == "10": #Velocidad viento
                '''
                0-5.5V = 0-6000RPM
                1 Km/h = 63RPM
                '''
                Message = (6000*Message/5.5)/63 #Converting Volts to Km/h
                Fact.Datos_Viento = np.concatenate([Fact.Datos_Viento, [[time.time() - InitialTime, float(Message)]]])
                if HMI.TabWidget.currentIndex() == 0:
                    HMI.Wind_Label.setText(f"Viento {Message}RPM")

        sleep(0.5)

def SendMessageTriangle():
    if Arducam.StartCam:
        serial_connection.SendMessage("41001")

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
    Arducam.detect_triangle.connect(lambda : serial_connection.SendMessage('41001'))
    Arducam.start()

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
    serial_connection.UpdatedPortList.connect(lambda : sorted(serial_connection.Refresh_Ports()))

    # Connect the aboutToQuit signal to the on_ui_exit function
    app.aboutToQuit.connect(on_ui_exit)

    HMI.show()
    app.exec_()