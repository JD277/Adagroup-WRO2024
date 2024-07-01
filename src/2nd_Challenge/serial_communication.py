import serial
import time


#TABLA
#  1 == Verde
#  2 == Rojo


class Arduino:
    def __init__(self, port_, baudrate_):
        self.port = port_
        self.baudrate = baudrate_
        
        self.serial = serial.Serial(port_, baudrate_, timeout=.1)

    def write_read_data(self, data_write: str):  
        self.serial.write(bytes(data_write, 'utf-8'))
        
        data_read = self.serial.readline()
        
        return data_read
    
    def write_data(self, data):
        self.serial.write(bytes(data, 'utf-8'))
        
    def read_data(self):
        return self.serial.readline()
        





