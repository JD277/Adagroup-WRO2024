import cv2 as cv
import numpy as np


def col_range(color, lower, upper):
    
    col = np.uint8([[color]])
    
    hsvCol = cv.cvtColor(col, cv.COLOR_BGR2HSV)
    
    
    #si el color a detectar es rojo el rango menor debe ser 0 o un numero muy bajo.
    lower_limit = hsvCol[0][0][0] - lower, 50, 50
    upper_limit = hsvCol[0][0][0] + upper, 255, 255
    
    lower_limit = np.array(lower_limit, dtype=np.uint8)
    upper_limit = np.array(upper_limit, dtype=np.uint8)
    
    return lower_limit, upper_limit
pass


