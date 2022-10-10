"""
    This script is used for cleaning the serial data received from the stm32f0 before decompression and 
    decryption.
"""

# Global Variables
substring = ","

#Functions

def clean_transmittedCompressioData(fileLines,startCharIndex, numLines):
    cleaned = ""
    elements = 0
    # removes unwanted newlines and blank spaces caused by transmission formatting
    for x in range(startCharIndex+1,numLines):
        if(substring in fileLines[x]):              # if the substring exists in the line
            index = fileLines[x].find(substring)
            cleaned+=fileLines[x][:index+1]         # remove the substring
            elements = elements+1
    return cleaned[:-1], elements                   # remove trailing , delimiter
    
def clean_sensorData(sensorData):
    index = sensorData.find('}')                    # only save data before the terminating character
    sensorData = sensorData[:index]
    return sensorData

def main():
    numLines=0                      # number lines in file
    fileLines = []                  # lines in file

    originalSensorData = ""
    cleanedSensorData = ""          # real sensor data

    startCharIndex = -1             # START sequence line number
    cleanedCompressedData = ""      # real compressed bits
    elements = -1                   # number of compressed bits
    
    f = open("transmission.txt", "r")
    # Read in lines
    for line in f:
        fileLines.append(line)
        if('}' in line):
            originalSensorData = line
        if('#' in line):
            startCharIndex = numLines
        numLines = numLines+1

    f.close()
    cleanedSensorData = clean_sensorData(originalSensorData)
    cleanedCompressedData, elements = clean_transmittedCompressioData(fileLines, startCharIndex,numLines)

    print(cleanedSensorData)
    print(cleanedCompressedData)
    print(elements)
    

if __name__ == "__main__":
    main() 


