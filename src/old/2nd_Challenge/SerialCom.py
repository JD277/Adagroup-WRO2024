import struct
import serial,time

class SerialReader:
    def __init__(self, port,_baudrate):
        self.port = port
        self.ser = serial.Serial(port,_baudrate, timeout=1)

    def change_port(self, port):
        self.port = port
    
    def connect(self):
        if self.ser.isOpen():
            print("connected!")

    def write(self, msg):
        self.ser.write(msg.encode())
        
    def __debug(self):
        answer=self.ser.readline()
        print(answer)

    def flush_buffer(self):
        self.ser.flushInput()
        
        
