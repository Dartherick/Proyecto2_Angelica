from SerialCom import SerialPortConnection
import time

serial_connection = SerialPortConnection("COM7", 9600)
serial_connection.open_port("COM7", 9600)

while(True):
    serial_connection.ReceiveMessage()
    time.sleep(0.5)