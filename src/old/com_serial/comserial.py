
import numpy as np
from SerialRead import *


arduino = SerialReader("/dev/ttyACM0",9600)

arduino.conectar()

arduino.leer_cadena_bytes_float()
arduino.limpiar_buffer()




