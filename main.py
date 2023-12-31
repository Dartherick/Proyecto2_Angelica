import sys

from PyQt5.QtCore import *
from PyQt5.QtGui import *

from PyQt5.QtWidgets import QApplication

from HMI import MainHMI
from SerialCom import SerialPortConnection
from Med import Mediciones

from Camera import Camera

def StartFunction(Status,ProgressBar,Start,End,Time):
    HMI.ProgressBar_Progression(ProgressBar,Start,End,Time)
    HMI.EnableTabs(Status)

def Factibilidad():
    Ruta = HMI.Rutas_ComboBox.currentIndex() 
    Punto = HMI.Puntos_ComboBox.currentIndex() 

    Fact.Factibilidad(Ruta,Punto)

    if Fact.T_restante < Fact.T_disponible:
        HMI.FactibilidadLabel.setStyleSheet("background-color: red ; border: 1px solid black")
        HMI.FactibilidadLabel.setText("No factible!")
    else:  
        HMI.FactibilidadLabel.setStyleSheet("background-color: green; border: 1px solid black")
        HMI.FactibilidadLabel.setText("Factible!")

    HMI.TimeRest.setText(str(round(Fact.T_restante,2))+"s")
    HMI.TimeTotal.setText(str(round(Fact.T_total,2))+"s")

# Define a function to run when the UI exits
def on_ui_exit():
    Arducam.stop()
    print("Closing camera port")
    serial_connection.close_port()
    print("Closing Serial Port")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    HMI = MainHMI("HMI.ui")
    Fact = Mediciones()

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

    serial_connection.OpenStatus.connect(lambda : StartFunction(True,HMI.SerialProgressBar,0,100,5))
    serial_connection.DisableStatus.connect(lambda : StartFunction(False,HMI.SerialProgressBar,100,0,5))

    # Connect the aboutToQuit signal to the on_ui_exit function
    app.aboutToQuit.connect(on_ui_exit)

    HMI.show()
    app.exec_()
