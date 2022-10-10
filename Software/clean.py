"""
    This script is used for cleaning the serial data received from the stm32f0 before decompression and 
    decryption.
"""

f = open("transmission.txt", "r")

substring = ","
output = ""
elements = 0

fileLines = []
numLines=0
startCharIndex = -1

# Read in lines
for lines in f:
    fileLines.append(lines)
    if('#' in lines):
        startCharIndex = numLines
    numLines = numLines+1

#
for x in range(startCharIndex+1,numLines):
    if(substring in fileLines[x]):              # if the substring exists in the line
        index = fileLines[x].find(substring)
        output+=fileLines[x][:index+1]          # remove the substring
    elements = elements+1                       # increment array size 

output = output[:-1]                            # remove trailing , delimiter


f.close()
print(output)
print(elements)
