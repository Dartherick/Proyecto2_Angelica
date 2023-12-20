import serial
import time

class SerialPortConnection():
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate

    def open_port(self):

        try:
            self.SerialComm = serial.Serial(self.port, self.baudrate)
            time.sleep(2)
            print("Serial port opened successfully.")
        except serial.SerialException as error:
            print("Error opening serial port:", str(error))

    def close_port(self):
        if self.SerialComm is not None and self.SerialComm.is_open:
            self.SerialComm.close()
            print("Serial port closed.")
        else:
            print('There is no open communication.')
 
    def SendMessage(self,Text):
        if Text is not None:
            Message = "<"+str(Text)+">"

            if self.SerialComm is not None and self.SerialComm.is_open:
                try:
                    self.SerialComm.write(Message.encode())
                    print(f"Message sent: {Message}")
                except serial.SerialException as e:
                    print("Error sending message:", str(e))
            else:
                print('There is no open communication.')
                print(Message)
        else:
            print('Please select M# first')

    def ReceiveMessage(self):
        if self.SerialComm is not None and self.SerialComm.is_open:
            try:
                received_data = self.SerialComm.readline().decode()
                #received_data = self.SerialComm.readline().decode('utf-8').strip()

                # Check if the message starts with '<' and ends with '>'
                if received_data.startswith('<') and received_data.endswith('>'):
                    Function = received_data[1:3]
                    Message = received_data[3:-1]
                    return Function,Message

            except serial.SerialException as e:
                print("Error receiving message:", str(e))
        else:
            print('There is no open communication.')

    def Close_Port_DestroyWindow(self):
        self.close_port()

    @staticmethod
    def get_available_ports():
        ports = []
        for port in serial.tools.list_ports.comports():
            ports.append(port.device)
        return ports
