import sys

from PyQt5 import QtGui
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget

import serial
import serial.tools.list_ports

import time

class MainHMI(QMainWindow):
    def __init__(self):
        super(MainHMI, self).__init__()
        loadUi("HMI.ui",self)
        self.TabWidget.setCurrentIndex(0)
        
        self.Button1.clicked.connect(self.Button1_Action)
        
        #PortList = serial_connection.get_available_ports()

        BaudRate_List = ("9600","38400","57600","115200")
        self.BaudRate_ComboBox.addItems(BaudRate_List)


    
    def Button1_Action(self):
        print("Button 1")

    def closeEvent(self, event):
        serial_connection.close_port()
        print("Closing...")

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
    
    def Close_Port_DestroyWindow(self):
        self.close_port()
        #HMI.destroy()

    @staticmethod
    def get_available_ports():
        ports = []
        for port in serial.tools.list_ports.comports():
            ports.append(port.device)
        return ports

if __name__ == "__main__":
    app = QApplication(sys.argv)
    HMI = MainHMI()

    # Create an instance of the SerialPortConnection class
    serial_connection = SerialPortConnection("", 9600)

    HMI.SerialPort_ComboBox.addItems(sorted(serial_connection.Refresh_Ports()))
    HMI.OpenButton.clicked.connect(lambda : serial_connection.open_port(HMI.SerialPort_ComboBox.currentText(),HMI.BaudRate_ComboBox.currentText()))
    HMI.CloseButton.clicked.connect(serial_connection.close_port)
    HMI.RefreshButton.clicked.connect(serial_connection.Refresh_Ports)

    HMI.show()
    app.exec_()