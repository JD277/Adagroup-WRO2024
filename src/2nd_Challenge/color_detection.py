import cv2
import numpy as np
from serial_communication import *


def detect_color(mask, border_color, msg: str):
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
    for c in contours:
        area = cv2.contourArea(c)
        
        if area > 2700:
                cv2.drawContours(frame, [cv2.convexHull(c)], 0, border_color, 3)
                arduino.write_data(msg)
        
                
                

            


#-----COLOR RANGES-----#

#green
green_low_range = np.array([42,100,23],np.uint8)
green_high_range = np.array([75,255,255],np.uint8)

#yellowish red
red_low_range1 = np.array([0,100,20],np.uint8)
red_high_range1 = np.array([5,255,255],np.uint8)

#pinkish red
red_low_range2 = np.array([175,100,20],np.uint8)
red_high_range2 = np.array([179,255,255],np.uint8)

#purple
purple_low_range = np.array([148,100,25],np.uint8)
purple_high_range = np.array([164,255,255],np.uint8)


#get video from USB Webcam
video_cap = cv2.VideoCapture(1)

arduino = Arduino('COM21', 9600)

while True:
    #get current frame
    ok, frame = video_cap.read()
    
    if ok:
        #converts frame from RGB color space to HSV
        frame_hsv  = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        #get color masks 
        mask_green = cv2.inRange(frame_hsv, green_low_range, green_high_range)
        mask_purple = cv2.inRange(frame_hsv, purple_low_range, purple_high_range)
        mask_red1  = cv2.inRange(frame_hsv, red_low_range1,red_high_range1)
        mask_red2  = cv2.inRange(frame_hsv, red_low_range2,red_high_range2)
        mask_red_final = cv2.add(mask_red1,mask_red2)
        
        
        #--DEBUG--
        detect_color(mask_green,(0,230,0), '2')
        detect_color(mask_red_final,(0,0,230), '1')
        #detect_color(mask_purple,(230,0,230))
        
        print(arduino.read_data())
        
        cv2.imshow("frame",frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        #--DEBUG--

            
        
            

#end
video_cap.release()
cv2.destroyAllWindows()


