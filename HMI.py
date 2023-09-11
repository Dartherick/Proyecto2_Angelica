from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtGui import *

from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget

class MainHMI(QMainWindow):
    def __init__(self,file):
        super(MainHMI, self).__init__()
        loadUi(file,self)
        self.TabWidget.setCurrentIndex(1)
        
        self.pushButton.clicked.connect(self.pushButton_action)
        self.pushButton_2.clicked.connect(self.pushButton2_action)
        self.pushButton_3.clicked.connect(self.pushButton3_action)
        self.pushButton_4.clicked.connect(self.pushButton4_action)
        self.pushButton_5.clicked.connect(self.pushButton5_action)
        self.pushButton_6.clicked.connect(self.pushButton6_action)
        self.pushButton_7.clicked.connect(self.pushButton7_action)
        self.ClearButton.clicked.connect(self.ClearB)
        
        BaudRate_List = ("9600","38400","57600","115200")
        self.BaudRate_ComboBox.addItems(BaudRate_List)

        self.Rutas_ComboBox.currentTextChanged.connect(self.TriggerComboBox)

        #delete later
        #self.Map.setStyleSheet("background-color: white")
        self.Map.setStyleSheet("border: 1px solid black; background-color: white")
        self.FactibilidadLabel.setStyleSheet("border: 1px solid black")

        # Load and set the image to the QLabel
        self.ImagePaths = ("Icons\Battery\Empty.png",
                           "Icons\Battery\Bar_1.png",
                           "Icons\Battery\Bar_2.png",
                           "Icons\Battery\Bar_3.png",
                           "Icons\Battery\Bar_4.png",
                           "Icons\Battery\Bar_5.png",
                           "Icons\Battery\Bar_6.png"
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
        elif Percentage <= 5*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[5])
        elif Percentage <= 6*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[6])

        self.Batteries[Battery].setPixmap(pixmap)

    def closeEvent(self, event):
        #serial_connection.close_port()
        print("Closing...")
    
    def pushButton_action(self):
        self.ChangeBatteryImage(1,0)
        self.ChangeBatteryImage(2,0)
        self.ChangeBatteryImage(3,0)
        self.ChangeBatteryImage(4,0)

    def pushButton2_action(self):
        self.ChangeBatteryImage(1,17)
        self.ChangeBatteryImage(2,17)
        self.ChangeBatteryImage(3,17)
        self.ChangeBatteryImage(4,17)

    def pushButton3_action(self):
        self.ChangeBatteryImage(1,34)
        self.ChangeBatteryImage(2,34)
        self.ChangeBatteryImage(3,34)
        self.ChangeBatteryImage(4,34)

    def pushButton4_action(self):
        self.ChangeBatteryImage(1,51)
        self.ChangeBatteryImage(2,51)
        self.ChangeBatteryImage(3,51)
        self.ChangeBatteryImage(4,51)

    def pushButton5_action(self):
        self.ChangeBatteryImage(1,67)
        self.ChangeBatteryImage(2,67)
        self.ChangeBatteryImage(3,67)
        self.ChangeBatteryImage(4,67)
    
    def pushButton6_action(self):
        self.ChangeBatteryImage(1,84)
        self.ChangeBatteryImage(2,84)
        self.ChangeBatteryImage(3,84)
        self.ChangeBatteryImage(4,84)

    def pushButton7_action(self):
        self.ChangeBatteryImage(1,101)
        self.ChangeBatteryImage(2,101)
        self.ChangeBatteryImage(3,101)
        self.ChangeBatteryImage(4,101)
        
    def TriggerComboBox(self):
        #print('klk')
        Distancia_total = (0,40182.6, 44571.4, 42821.5)

        # Load and set the image to the QLabel
        MapPaths = ("Icons\Maps\Map1.png",
                    "Icons\Maps\Map2.png",
                    "Icons\Maps\Map3.png")

        i = self.Rutas_ComboBox.currentIndex() 
        self.DistanceTotal.setText(str(Distancia_total[i])+"m")

        if i > 0:
            pixmap = QtGui.QPixmap(MapPaths[i-1])
            self.Map.setPixmap(pixmap)
        else:
            self.Map.clear()

    def ClearB(self):
        self.TimeRest.setText("")
        self.TimeTotal.setText("")
        self.Map.clear()
        self.TimeTotal.setText("")
        self.FactibilidadLabel.setStyleSheet("border: 1px solid black")
        self.FactibilidadLabel.setText("")
        self.Rutas_ComboBox.setCurrentIndex(0)
        self.DistanciaRecorrida.setValue(0)
