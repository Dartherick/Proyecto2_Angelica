import csv
import sys
from time import time

class Export_To_CSV():

    def __init__(self):
        self.Start_Time = time()

    def CreateCsv(self,FileName,i):
        # Creating a CSV file
        with open(FileName+'.csv', 'w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['Name', 'Age'])
            writer.writerow(['Erick', str(i)])

    def ExportData(self):
        pass

if __name__ == "__main__":
    CSVF = Export_To_CSV()

    for i in range(20):
        CSVF.CreateCsv("Test",i)

    print("done")