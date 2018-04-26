#!/usr/bin/python2.7
# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
# import Thrusters_control
import os     #importing os library so as to communicate with the system
import time   #importing time library to make Rpi wait because its too impatient 
#os.system ("sudo pigpiod") #Launching GPIO library
time.sleep(1) # This delay is needed for initialization to avoid error
import pigpio #importing GPIO library
import serial
import ms5837


ser = serial.Serial('/dev/ttyACM0', 9600)
#ser1 = serial.Serial('/dev/ttyUSB0', 9600)
#pi = pigpio.pi().set_mode (1, pigpio.OUTPUT)
time.sleep(6)

#pressure sensor initialization and pressure reading
#the pressure reading is then written to the arduino
#pressure = ser1.read(ms5837.OSR_256)
ser.write('5')
        

# 1500 is the stop position, values from 1530 to 1700 is the forward thruster increment in speed
# Values from 1470 to 1100 is the reverse increment in speed

def forward():    # Thrusters Forward       
        ser.write('1')

def stop():       #Thrusters Stop
        ser.write('2')

def turn_right():  #Thrusters right turn
        ser.write('3')

def turn_left():  #Thrusters left turn
        ser.write('4')

        

#initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (480, 320)
camera.framerate = 30
camera.hflip = True


rawCapture = PiRGBArray(camera, size=(480, 320))
 
# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        # grab the raw NumPy array representing the image, then initialize the timestamp
        # and occupied/unoccupied text
        image = frame.array
        blur = cv2.blur(image, (3,3))
        
        
        
        #thresh = cv2.inRange(hsv,np.array((0, 200, 200)), np.array((20, 255, 255)))
                
        lower = np.array([45,45,200],dtype="uint8")
                    
        upper = np.array([125,125,255], dtype="uint8")      
        
        thresh = cv2.inRange(blur, lower, upper)
        #thresh = np.resize(thresh, (3,3))
        thresh2 = thresh.copy()
        
        # find contours in the threshold image
        image,contours,hierarchy = cv2.findContours(thresh,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)

        # finding contour with maximum area and store it as best_cnt
        max_area = 0
        best_cnt = 1
        area = 0
        for cnt in contours:
                area = cv2.contourArea(cnt)
                if area > max_area:
                        max_area = area
                        best_cnt = cnt

        # finding centroids of best_cnt and draw a circle there
        M = cv2.moments(best_cnt)
        cx,cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
        #if best_cnt>1:
        cv2.circle(blur,(cx,cy),10,(85,85,255),-1)
        cv2.putText(blur, "Orange Detected", (cx - 20, cy - 20),
            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
        # show the frame
        cv2.imshow("Frame", blur)
        #cv2.imshow('thresh',thresh2)
        key = cv2.waitKey(1) & 0xFF
 
        # clear the stream in preparation for the next frame
        rawCapture.truncate(0)

        if area > 0:
                forward()
        else:
                stop()
                turn_right()


                
        # if the `q` key was pressed, break from the loop
        if key == ord("q"):
                break

ser.close()
#gpio.cleanup() #clear all GPIO

