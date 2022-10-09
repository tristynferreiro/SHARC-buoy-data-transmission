"""
    This script is used for cleaning the serial data received from the stm32f0 before decompression and 
    decryption.
"""

f = open("transmission.txt", "r")

substring = ","
output = ""
elements = 0

for x in f:
    if('Accel' in x or 'IMU' in x):     # ignore the header and IMU initialisation.
        continue;
    elif(substring in x):               # if the substring exists in the line
        index = x.find(substring)
        output+=x[:index+1]             # remove the substring
    elements = elements+1               # increment array size 

output = output[:-1]                    # remove trailing , delimiter

f.close()
print(output)
print(elements)
