import sys

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtGui import *

from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget

import serial
import serial.tools.list_ports

import cv2

import time

class MainHMI(QMainWindow):
    def __init__(self,file):
        super(MainHMI, self).__init__()
        loadUi(file,self)
        self.TabWidget.setCurrentIndex(1)
        
        self.Button1.clicked.connect(self.Button1_Action)
        self.pushButton.clicked.connect(self.pushButton_action)
        
        BaudRate_List = ("9600","38400","57600","115200")
        self.BaudRate_ComboBox.addItems(BaudRate_List)

        self.Rutas_ComboBox.currentTextChanged.connect(self.test)


        # Load and set the image to the QLabel
        self.ImagePaths = ("Icons\Battery\Empty.png",
                           "Icons\Battery\Bar_1.png",
                           "Icons\Battery\Bar_2.png",
                           "Icons\Battery\Bar_3.png",
                           "Icons\Battery\Bar_4.png",
                           "Icons\Battery\Bar_5.png",
                           )

        self.Batteries = {1:self.Battery1,
                          2:self.Battery2,
                          3:self.Battery3,
                          4:self.Battery4,
                          }

        print(self.ImagePaths[0])
        pixmap = QtGui.QPixmap(self.ImagePaths[0])
        self.Batteries[1].setPixmap(pixmap)
        self.Batteries[2].setPixmap(pixmap)
        self.Batteries[3].setPixmap(pixmap)
        self.Batteries[4].setPixmap(pixmap)

    def ChangeBatteryImage(self,Battery,Percentage):
        BarPercentage = 100/5

        if Percentage == 0:
            pixmap = QtGui.QPixmap(self.ImagePaths[0])
        elif Percentage <= BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[1])
        elif Percentage <= 2*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[2])
        elif Percentage <= 3*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[3])
        elif Percentage <= 4*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[4])

        self.Batteries[Battery].setPixmap(pixmap)

    def closeEvent(self, event):
        serial_connection.close_port()
        print("Closing...")

    def Button1_Action(self):
        print("Button 1")
    
    def pushButton_action(self):
        self.ChangeBatteryImage(1,33)
        
    def test(self):
        print('klk')
        
class SerialPortConnection:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.serial = None

    def open_port(self,Ports,Baudrates):
        self.port = Ports
        self.baudrate = int(Baudrates)

        try:
            self.serial = serial.Serial(self.port, self.baudrate)
            time.sleep(2)
            print("Serial port opened successfully.")
        except serial.SerialException as error:
            print("Error opening serial port:", str(error))

    def close_port(self):
        if self.serial is not None and self.serial.is_open:
            self.serial.close()
            print("Serial port closed.")
        else:
            print('There is no open communication.')
 
    def Refresh_Ports(self):
        # Clear the existing options in the port_combobox
        #Port_Combobox['values'] = ()

        # Get the updated list of available ports
        available_ports = self.get_available_ports()

        print('Refreshed')
        return self.get_available_ports()

    def SendMessage(self,Text):
        if Text is not None:
            Message = "<"+str(Text)+">"

            if self.serial is not None and self.serial.is_open:
                try:
                    self.serial.write(Message.encode())
                    print(f"Message sent: {Message}")
                except serial.SerialException as e:
                    print("Error sending message:", str(e))
            else:
                print('There is no open communication.')
                print(Message)
        else:
            print('Please select M# first')

    def ReceiveMessage(self):
        if self.serial is not None and self.serial.is_open:
            pass


    @staticmethod
    def get_available_ports():
        ports = []
        for port in serial.tools.list_ports.comports():
            ports.append(port.device)
        return ports

class Camera():
    def __init__(self,index=0):
        self.Cap = cv2.VideoCapture(index)
        self.i = 0

    def TakePic(self):
        if self.Cap.isOpened():
            ret,self.frame = self.Cap.read()
            if ret and self.frame is not None:
                cv2.imwrite('Original.jpg', self.frame)

class Work(QThread):
    def __init__(self,ret,frame):
        Imageupd = pyqtSignal(QImage)
        self.ret = ret
        self.frame = frame

    def run(self):
        self.hilo_corriendo = True
        cap = cv2.VideoCapture(0)
        while self.hilo_corriendo:
            if self.ret:
                Image = cv2.cvtColor(self.frame, cv2.COLOR_BGR2RGB)
                flip = cv2.flip(Image, 1)
                convertir_QT = QImage(flip.data, flip.shape[1], flip.shape[0], QImage.Format_RGB888)
                pic = convertir_QT.scaled(320, 240, Qt.KeepAspectRatio)
                self.Imageupd.emit(pic)

    def stop(self):
        self.hilo_corriendo = False
        self.quit()

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

        Distancia_total = (40182.6, 44571.4, 42821.5)

        T_asc = self.Altitud / self.Velocity_Asc
        T_des = self.Altitud / self.Velocity_Des
        T_Vuelo = Distancia_total[i-1] / self.Velocity
        T_total = T_Vuelo + T_asc + T_des
        print(T_total)

        T_Recorrido = Distancia_Recorrida/self.Velocity + T_asc 
        print(T_Recorrido)

        T_restante = T_total - T_Recorrido
        print(T_restante)

        T_disponible = self.T_Charge - T_Recorrido
        print(T_disponible)

        if T_restante < T_disponible:
            print('no factible')
        else:
            print("es factible")    

if __name__ == "__main__":
    app = QApplication(sys.argv)
    HMI = MainHMI("HMI.ui")

    # Create an instance of the SerialPortConnection class
    serial_connection = SerialPortConnection("", 9600)
    Fact = Mediciones()

    HMI.SerialPort_ComboBox.addItems(sorted(serial_connection.Refresh_Ports()))
    HMI.OpenButton.clicked.connect(lambda : serial_connection.open_port(HMI.SerialPort_ComboBox.currentText(),HMI.BaudRate_ComboBox.currentText()))
    HMI.CloseButton.clicked.connect(serial_connection.close_port)
    HMI.RefreshButton.clicked.connect(serial_connection.Refresh_Ports)

    HMI.Rutas_ComboBox.addItems(Fact.Rutas)
    HMI.pushButton_2.clicked.connect(Fact.Factibilidad)

    HMI.show()
    app.exec_()