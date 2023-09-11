import sys

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtGui import *

from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget

from HMI import MainHMI
from SerialCom import SerialPortConnection
#from Med import Mediciones

# Define a function to run when the UI exits
def on_ui_exit():
    serial_connection.close_port()
    print("Closing Serial Port")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    HMI = MainHMI("HMI.ui")

    # Create an instance of the SerialPortConnection class
    serial_connection = SerialPortConnection("", 9600)
    #Fact = Mediciones()

    HMI.SerialPort_ComboBox.addItems(sorted(serial_connection.Refresh_Ports()))
    HMI.OpenButton.clicked.connect(lambda : serial_connection.open_port(HMI.SerialPort_ComboBox.currentText(),HMI.BaudRate_ComboBox.currentText()))
    HMI.CloseButton.clicked.connect(serial_connection.close_port)
    HMI.RefreshButton.clicked.connect(serial_connection.Refresh_Ports)

    # Connect the aboutToQuit signal to the on_ui_exit function
    app.aboutToQuit.connect(on_ui_exit)

    HMI.show()
    app.exec_()

'''
    HMI.Rutas_ComboBox.addItems(Fact.Rutas)
    HMI.EnterButton.clicked.connect(Fact.Factibilidad)

    HMI.VelocityText.setText(str(Fact.Velocity)+"m/s")
    HMI.AltitudeText.setText(str(Fact.Altitud)+"m")
    HMI.VelocityAsc_Text.setText(str(Fact.Velocity_Asc)+"m/s")
    HMI.VelocityDesc_Text.setText(str(Fact.Velocity_Des)+"m/s")'''