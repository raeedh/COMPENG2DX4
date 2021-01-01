# Student name: Raeed Hassan
# Student number: 
# Python version: 3.8.2

import serial
import math

ser = serial.Serial('COM4',115200) # Change COM Port #
ser.open

f = open("2dx4data.xyz", "a") # Change name of file 
i = 0
x = 0 # Change initial x-displacement (mm)
increment = 200 # x-displacement icrement (mm)
readingData = False

while 1:  
    s = ser.readline()
    a = s.decode("utf-8") # Decodes byte input from UART into string 
    a = a[0:-2] # Removes carriage return and newline from string
    if (a.isdigit() == True):
        readingData = True
        angle = (i/512)*2*math.pi # Obtain angle based on motor rotation
        b = int(a)
        y = b*math.cos(angle) # Calculate y
        z = b*math.sin(angle) # Calcualte z
        f.write('{} {} {}\n'.format(x,y,z)) # Write data to .xyz file
        i += 1
    if (a.isdigit() == False and readingData == True):
        f.close()
        f = open("filename.xyz", "a")
    if i == 512:
        i = 0
        x = x + increment
    print(a)