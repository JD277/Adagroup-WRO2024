
import cv2 as cv
import numpy as np
from PIL import Image
from rango_colores import *
from SerialCom import *


#arduino = SerialReader("/dev/ttyACM0",9600)

#arduino.connect()

#morado = [238,39,55]

RED = [231,120,111]
GREEN = [44, 214, 68]
CYAN = [119, 152, 74]

#numero de la camara conectada
CAM = cv.VideoCapture(0)




    

while (True):
    
    #captura cada frame del video y lo convierte a HSV
    _, frame = CAM.read()
    frame_hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    inverted_frame = cv.bitwise_not(frame_hsv.copy())
    
    
    #rango del color verde
    green_lower_lim, green_upper_lim = col_range(GREEN, 10, 25)
    
    red_lower_lim, red_upper_lim = col_range(CYAN, 25, 25)
    
    
    #crea una mascara de la imagen en blanco y negro con el color que se eligio.
    mask_green = cv.inRange(frame_hsv, green_lower_lim, green_upper_lim)
    mask_red = cv.inRange(inverted_frame, red_lower_lim, red_upper_lim)
    
    
    #muestra un cuadrado alrededor del color detectado.
    contour, _ = cv.findContours(mask_green, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    if len(contour) != 0:
        for c in contour:
            if cv.contourArea(c) > 50:
                x,y,w,h = cv.boundingRect(c)
                cv.rectangle(frame_hsv, (x,y), (x+w, y+h), (0,255,0), 2)
        
        
    contour, _ = cv.findContours(mask_red, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    if len(contour) != 0:
        for c in contour:
            if cv.contourArea(c) > 50:
                x,y,w,h = cv.boundingRect(c)
                cv.rectangle(frame_hsv, (x,y), (x+w, y+h), (0,0,255), 2)
        
    
    #muestra la imagen y la mascara
    cv.imshow("frame", frame_hsv)
    cv.imshow("mascara", mask_red)
    
    #cierra el programa si se presiona q
    if cv.waitKey(1) & 0xFF == ord('q'):
        break
    
    #arduino.flush_buffer()


CAM.release()
cv.destroyAllWindows()



