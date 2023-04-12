import serial
import time
import os
"""
Note: please click the reset button on the stm dev board when first connecting.
Ignore the first data received as it is runover from the last transmission the
STM attempted to make. Only partial recovery of this data will be achieved.
"""
# Use the code below to find the serial port (should be the last one on the list, for windows it might look more like "COM4")
# import serial.tools.list_ports as port_list
# ports = list(port_list.comports())
# for p in ports:
#     print (p.device)

serialPort = serial.Serial(port = "/dev/cu.usbserial-0001", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
serialPort.flushInput()

serialString = ""                           # Used to hold data coming over UART
file = "./Desktop/enc_comp.txt"
#compileCMD = 'gcc rsa_decrypted.c' 
#runCMD = 'a.exe d test.txt out.txt'

while(1):
    # Wait until there is data waiting in the serial buffer
    if(serialPort.in_waiting > 0):
            # Read data out of the buffer until a carraige return / new line is found
        serialString = serialPort.readline()
            # Print the contents of the serial data
        with open(file,"ab") as f:
            f.write(serialString)
            print(serialString.decode('Ascii'))
            f.close()
       # os.system(compileCMD)
       # os.system(runCMD)
       # print("complete")


