import struct
import serial,time

class SerialReader:
    def __init__(self, port,_baudrate):
        self.port = port
        self.ser = serial.Serial(port,_baudrate, timeout=1)

    def cambiar_puerto(self, port):
        self.port = port
    
    def conectar(self):
        if self.ser.isOpen():
            print("conectado!")

    def leer_cadena_bytes_float(self):
        while self.ser.inWaiting()==0: pass
        if self.ser.inWaiting()>0:
            res = self.ser.readline()[:-1]
            float_res = struct.unpack('f', res)[0]#convierte el array de bytes a un float
            print("valor:", float_res)


    def limpiar_buffer(self):
        self.ser.flushInput()