import time
import serial
import serial.tools.list_ports
from PyQt5.QtCore import QObject, pyqtSignal
import numpy

class SerialPortConnection(QObject):
    OpenStatus = pyqtSignal()
    DisableStatus = pyqtSignal()
    UpdatedPortList = pyqtSignal()

    def __init__(self, port, baudrate):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.serial = None

    def open_port(self, Ports, Baudrates):
        self.port = Ports
        self.baudrate = int(Baudrates)

        try:
            self.serial = serial.Serial(self.port, self.baudrate)
            self.OpenStatus.emit()
            time.sleep(2)
            print("Serial port opened successfully.")
        except serial.SerialException as error:
            print("Error opening serial port:", str(error))

    def close_port(self):
        if self.serial is not None and self.serial.is_open:
            self.serial.close()
            self.DisableStatus.emit()
            self.serial = None  # Set the serial object to None after closing
            print("Serial port closed.")
        else:
            print('There is no open communication.')
 
    def Refresh_Ports(self):
        # Clear the existing options in the port_combobox
        #Port_Combobox['values'] = ()

        # Get the updated list of available ports
        available_ports = self.get_available_ports()
        self.UpdatedPortList.emit()

        print('Refreshed')
        return self.get_available_ports()

    def SendMessage(self,Text):
        if Text is not None:
            Message = "<"+str(Text)+">"+"\n"

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

    def ReceiveMessagetemp(self):
        if self.serial is not None and self.serial.is_open:
            Message = self.serial.readline().decode().replace('\r\n','')
            print("hola")
            if Message:
                if (Message[0] == "<") and (Message[-1] == ">"):
                    Function = Message[1:3]
                    Parsed_Message = Message[3:-1]
                    return Function,Parsed_Message
            else:
                return None,None
        else:
            return None,None
        
    def ReceiveMessage(self):
        if self.serial is not None and self.serial.is_open:
            try:
                Message = self.serial.readline().decode().replace('\r\n', '')
                if Message:
                    if (Message[0] == "<") and (Message[-1] == ">"):
                        Function = Message[1:3]
                        Parsed_Message = Message[3:-1]
                        return Function, Parsed_Message
            except serial.SerialException as se:
                print(f"Error reading message: {se}")
            except Exception as e:
                print(f"Unexpected error: {e}")
        return None, None

    @staticmethod
    def get_available_ports():
        ports = []
        for port in serial.tools.list_ports.comports():
            ports.append(port.device)
        return ports