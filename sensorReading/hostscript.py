from threading import Thread
import serial 
import struct
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation 
import collections
import pandas as pd
import os
import csv

class serialPlot:
    def __init__(self, comPort="COM5", baudrate =9600, numBytes = 4, max_entries = 100):
        self.port = comPort
        self.baud = baudrate
        self.numBytes = numBytes
        self.serialData = None           
        self.thread = None
        self.running = True                             # Serial port is running
        self.isReceiving = False                        # Start receiving serial raw data
        self.csvData = pd.DataFrame(columns=["RPM", "Thurst"])
        self.startNullReceive = False
        self.endNullReceive = False
        # self.rpmValues = []
        # self.forceValues = []

        print("Connecting to: " + str(comPort) + " at "+ str(baudrate) + " Baud.")
        try: 
            # reference to the successful serial connection
            self.serialConnection = serial.Serial(comPort, baudrate, timeout=1)
            print("Successful connection!")
        except:
            print("Failed to connect with " + str(comPort)  + " at "+ str(baudrate) + " Baud.")

    def update(self):
        # append value for rpm and thurst force 
        rpm, force = self.serialData
        # print(rpm, force)
        # self.rpmValues.append(rpm)
        # self.forceValues.append(force)

        # a0.set_data(self.rpmValues, self.forceValues)
        # self.csvData.append([{"RPM": self.rpmValues[-1], "Thurst": self.forceValues[-1]}], ignore_index=True)
        new_row = pd.DataFrame([{"RPM": rpm, "Thurst": force}])
        self.csvData = pd.concat([self.csvData, new_row], ignore_index=True)
        # print(self.csvData)
        
    def readSerial(self):
        if self.thread == None:

            self.thread = Thread(target=self.readThread)
            self.thread.start()
            # Wait until isReceiving live 
            while self.isReceiving != True:
                if self.endNullReceive == True:
                    self.close()
                time.sleep(0.1)             

    def readThread(self):
        while (self.running):
            # Trigger Arduino & set start
            while self.startNullReceive == False:
                print('write:',str.encode('s')) 
                self.serialConnection.write(str.encode('s'))
                time.sleep(1)
                if self.serialConnection.read(self.numBytes) == b'\x00\x00':
                    self.startNullReceive = True
            receive = self.serialConnection.read(2)

            # Arduino sending end 
            if receive == b'\x00\x00':
                self.endNullReceive = True
                self.isReceiving = False

            # Receiving data
            if (receive != b'') & (self.endNullReceive == False):
                print('readed:',receive)
                print("unpack value: ",struct.unpack("bb", receive))
                self.serialData = struct.unpack("bb", receive)
                self.isReceiving = True
                self.update()
                
    def close(self):
        self.running = False
        self.thread.join()
        self.serialConnection.flush()
        self.serialConnection.close()
        print("Exited....")
        # print(self.csvData)
        path = os.getcwd() + "\\data.csv"
        if os.path.exists(path):
            # append dataframe to csv file 
            self.csvData.to_csv(path, mode='a', index=False, header=False)
        else:
            # else create csv file from dataframe 
            self.csvData.to_csv(path, index=False)
        # print message
        print("Data appended successfully.")

def main():
    comPort = "COM5"
    baudrate = 9600
    numBytes = 2                
    max_entries = 100
    ser = serialPlot(comPort, baudrate, numBytes,max_entries)
    ser.readSerial()

    
if __name__ == "__main__":
    main()

