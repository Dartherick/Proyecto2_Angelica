from SerialCom import SerialPortConnection
import time

hola = SerialPortConnection("COM3",9600)
hola.open_port("COM3",9600)

while True:
    x,y = hola.ReceiveMessagetemp()
    print(y)
    time.sleep(1)
